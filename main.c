// 前照灯LED制御およびドーザーブレード制御 dsPIC 版
#define FCY 69784687UL
#include <libpic30.h>
#include "mcc_generated_files/mcc.h"
#include <stdio.h>
#include <string.h>
#include "soft_i2c.h"
#include "lcd_i2c.h"
#include "ina226.h"
#include "ltc2630.h"


typedef union tagHL16 {
    uint16_t HL;
    struct {
        uint8_t L;
        uint8_t H;
    };
    struct {
        unsigned :8;
        unsigned :7;
        unsigned T:1;
    };
} HL16;


typedef union tagHL32 {
    unsigned long HL;
    struct {
        uint8_t L;
        uint16_t M;
        uint8_t H;
    };
} HL32;

// 前照灯明るさ（電流値0.1mV単位）
#define LED_LOW 0
#define LED_MID 1500
#define LED_HIGH 7350
uint8_t led_level = 0;

#define SPI_BYTES 8  /* SPI受信するデーターのバイト数 */ 
uint8_t data[SPI_BYTES]; // SPI受信格納先

char buf[32];



#define N_I_PAST 16
signed short i_past[N_I_PAST]; // 電流値
uint8_t i_past_p = 0; // 格納位置

signed short MAX_CHANGE1; // 最大変化量


signed short i_target; // 目標電流値(0.1mA単位)
signed short i_raw; // 電流ナマ値
signed short da_set; // DA出力値


// SPI受信
void int_strb(void) {
    if (SPI_STRB_GetValue() == 0) return;

    uint8_t idx, b, d, *dp = data;
    for (idx=0; idx<SPI_BYTES; idx++) {
        d = 0;
        for (b=0; b<8; b++) {
            while (SPI_CLOCK_GetValue() == 0) {} // CLOCK立ち上がりをソフトループで待つ
            d <<= 1;
            while (SPI_CLOCK_GetValue() == 1) {} // CLOCK立ち下がりをソフトループで待つ
            d |= SPI_DATA_GetValue();
        }
        (*(dp++)) = d;
    }
}


int main(void)
{
    uint8_t i, left, right;
    uint16_t t;
    da_set = 4095;

    // initialize the device
    SYSTEM_Initialize();
    CN_SetInterruptHandler(int_strb);

    DCDC_SetLow();

    MAX_CHANGE1 = 32; // 変化量初期値＝最大値
    led_level = 0;
    for (i=0; i<N_I_PAST; i++) i_past[i] = 0;
    
    char buf[32];
    __delay_ms(100);    
//    LCD_i2c_init(8);
    INA226_init(INA226_MODE_CONT_I, INA226_VSHCT_140US, INA226_VSHCT_140US, INA226_AVG_4);
    INA226_setadr(INA226_SHUNTV);    
    LTC2630_write((uint16_t)da_set);
    
    data[0] = data[1] = data[2] = data[3] = 0;
    data[4] = data[5] = data[6] = data[7] = 0x80; // 停止
    DCDC_SetLow();

    uint8_t led_low0 = 0;
    uint8_t led_low = 0;
    uint8_t led_high0 = 0;
    uint8_t led_high = 0;
    
    while (1)
    {
        WATCHDOG_TimerClear();

        if ((data[0] & 1) == 0) { // 通信エラーもしくはノーコン
            DCDC_SetLow();
            STANDBY_SetLow();
            INA1_SetLow();
            INA2_SetLow();
            continue;
        }
        
        // 前照灯明るさ
        led_low = (data[0] & 64); // ←が押された
        if (led_low) {
            if (led_low0 == 0) {
                if (led_level) {
                    led_level --;
                }
            }
        }
        led_high = (data[0] & 128); // →が押された
        if (led_high) {
            if (led_high0 == 0) {
                if (led_level < 3) {
                    led_level ++;
                }
            }
        }
        if (led_level == 0) {
            DCDC_SetLow();
            i_target = 0;
        }
        if (led_level == 1) {
            i_target = LED_LOW;
            DCDC_SetHigh();
        }
        if (led_level == 2) {
            i_target = LED_MID;
            DCDC_SetHigh();
        }
        if (led_level == 3) {
            i_target = LED_HIGH;
            DCDC_SetHigh();
        }
        
        i_raw = INA226_read_current(); // 電流ナマ値
        if (i_raw < 0) {
            i_raw = 0;
        }

        // 最大変化量の決定
        if (i_raw > i_target) { // ナマ値が目標を超過
            MAX_CHANGE1 = 1;
        }
        else {
            signed short mc = ((i_target - i_raw) >> 3); // 差の８分の１
            if (mc < MAX_CHANGE1) { // 最大変化量が現在値より小さい場合のみ
                if (mc > 0) {
                    MAX_CHANGE1 = mc; // 最大変化量を更新
                }
            }
        }
        
        // 電流履歴
        signed short i_past1 = i_past[i_past_p]; // 過去値取り出し
        i_past[i_past_p++] = i_raw; // 最新値格納
        // 格納位置更新
        if (i_past_p >= N_I_PAST) {
            i_past_p = 0;
        }
        
        // 未来値予想
        signed short i_future = (i_raw + i_raw - i_past1);
        if (i_future < 0) {
            i_future = 0;
        }

        // フィードバック
        if (i_target > i_future) { // 電流不足
            da_set -= MAX_CHANGE1; // DA減らす＝電流増える
            if (da_set < 2048) {
                da_set = 2048;
            }
        }
        if (i_target < i_future) { // 電流超過
            da_set += MAX_CHANGE1; // DA増やす＝電流減る
            if (da_set > 4095) {
                da_set = 4095;
            }
        }
        
        LTC2630_write((uint16_t)da_set);
        
        // ドーザーブレード
        if (data[0] & 16) { // ↑が押された
            PHASEA_SetHigh();
            INA2_SetHigh();
            INA1_SetHigh();
            STANDBY_SetHigh();
        }
        else if (data[0] & 32) { // ↓が押された
            PHASEA_SetLow();
            INA2_SetHigh();
            INA1_SetHigh();
            STANDBY_SetHigh();
        }
        else {
            STANDBY_SetLow();
            INA1_SetLow();
            INA2_SetLow();
        }

//        LCD_i2C_cmd(0x80);
//        sprintf(buf, "%d %2d %5d", led_level, MAX_CHANGE1, i_raw);
//        LCD_i2C_data(buf);

        led_low0 = led_low;
        led_high0 = led_high;
    }
    return 1; 
}


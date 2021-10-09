#include <xc.h>
#include <stdio.h>
#include <string.h>

#define FCY 69784687UL
#include <libpic30.h>
#include "soft_i2c.h"
#include "ina226.h"
#include "mcc_generated_files/mcc.h"


typedef union tagHL16 {
    signed short SL;
    uint16_t HL;
    struct {
        uint8_t L;
        uint8_t H;
    };
} HL16;


// レジスター regset に、regdata を書き込む
void INA226_write(uint8_t regset, uint16_t regdata) {
	I2C_start();
	I2C_send(INA226_dev_addr);
	if  (I2C_ackchk()) {
	}
	I2C_send(regset);
	if  (I2C_ackchk()) {
	}
    I2C_send(regdata >> 8);
	if  (I2C_ackchk()) {
	}
    I2C_send(regdata & 255);
	if  (I2C_ackchk()) {
	}
	I2C_stop();
}        


// 読み出しレジスターアドレスの設定
void INA226_setadr(uint8_t regset) {
	I2C_start();
	I2C_send(INA226_dev_addr);
	if  (I2C_ackchk()) {
	}
	I2C_send(regset);
	if  (I2C_ackchk()) {
	}
    I2C_stop();
}


// INA226_setadr したアドレスから読み出す
uint16_t INA226_read(void) {
    HL16 v;

	I2C_start();
	I2C_send(INA226_dev_addr | 1);
	if  (I2C_ackchk()) {
	}
    v.H = I2C_rcv();
    I2C_acksnd();
    v.L = I2C_rcv();
    I2C_nacksnd();
    I2C_stop();
    
    return v.HL;    
}


// 初期化する
void INA226_init(uint16_t mode, uint16_t vshct, uint16_t vbusct, uint16_t avg) {
    INA226_write(INA226_CONFIG, 0x4000 + mode + vshct + vbusct + avg);
}

// 電流値取得 0.1mA 単位
// INA226_setadr(INA226_SHUNTV) 済みであること
signed short INA226_read_current() {
    HL16 v;
    v.HL = INA226_read();
    return v.SL;
}

#ifndef INA226_H
#define INA226_H

/**
  Section: Included Files
*/

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
        
        
// I2C アドレス
#define INA226_dev_addr 0x88	// INA226アドレス

//INA226のレジスターセット
#define INA226_CONFIG  0
#define INA226_SHUNTV  1
#define INA226_BUSV    2
#define INA226_POWER   3
#define INA226_CURRENT 4
#define INA226_CALIB   5
#define INA226_MASK    6
#define INA226_ALERT   7        
        
// コンフィグ設定値
// Operating Mode 動作モード
#define INA226_MODE_OFF     0
#define INA226_MODE_TRIG_I  1
#define INA226_MODE_TRIG_V  2
#define INA226_MODE_TRIG_IV 3 
#define INA226_MODE_CONT_I  5  
#define INA226_MODE_CONT_V  6  
#define INA226_MODE_CONT_IV 7 
#define INA226_MODE_DEF     7 
// Shunt Voltage Conversion Time 電流変換時間        
#define INA226_VSHCT_140US   0        
#define INA226_VSHCT_204US   8        
#define INA226_VSHCT_332US  16      
#define INA226_VSHCT_588US  24      
#define INA226_VSHCT_1100US 32       
#define INA226_VSHCT_2116US 40       
#define INA226_VSHCT_4156US 48       
#define INA226_VSHCT_8244US 56       
#define INA226_VSHCT_DEF 32       
// Bus Voltage Conversion Time 電圧変換時間       
#define INA226_VBUSCT_140US    0        
#define INA226_VBUSCT_204US   64        
#define INA226_VBUSCT_332US  128      
#define INA226_VBUSCT_588US  192      
#define INA226_VBUSCT_1100US 256       
#define INA226_VBUSCT_2116US 320       
#define INA226_VBUSCT_4156US 384       
#define INA226_VBUSCT_8244US 448       
#define INA226_VBUSCT_DEF    256       
// 平均を取る回数
#define INA226_AVG_1       0 
#define INA226_AVG_4     512 
#define INA226_AVG_16   1024 
#define INA226_AVG_64   1536  
#define INA226_AVG_128  2048   
#define INA226_AVG_256  2560   
#define INA226_AVG_512  3072   
#define INA226_AVG_1024 3584     
        
void INA226_write(uint8_t regset, uint16_t regdata);        
uint16_t INA226_read();
void INA226_setadr(uint8_t regset); 
void INA226_init(uint16_t mode, uint16_t vshct, uint16_t vbusct, uint16_t avg);
signed short INA226_read_current();

#endif	//INA226_H
/**
 End of File
*/


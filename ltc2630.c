#include <xc.h>
#include <stdio.h>
#include <string.h>

#define FCY 69784687UL
#include <libpic30.h>
#include "ltc2630.h"
#include "mcc_generated_files/mcc.h"


void LTC2630_write(uint16_t volt) {
    CSLD_SetHigh();
    SCK_SetHigh();
    CSLD_SetLow();
    SCK_SetLow();

    // コマンド出力 0011xxxx
    SDI_SetLow();
    SCK_SetHigh();
    SCK_SetLow();
    SCK_SetHigh();
    SCK_SetLow();
    SDI_SetHigh();
    SCK_SetHigh();
    SCK_SetLow();
    SCK_SetHigh();
    SCK_SetLow();
    SCK_SetHigh();
    SCK_SetLow();
    SCK_SetHigh();
    SCK_SetLow();
    SCK_SetHigh();
    SCK_SetLow();
    SCK_SetHigh();
    SCK_SetLow();

    // データー送信
    if (volt & 2048) {
        SDI_SetHigh();
    }
    else {
        SDI_SetLow();
    }
    SCK_SetHigh();
    SCK_SetLow();

    if (volt & 1024) {
        SDI_SetHigh();
    }
    else {
        SDI_SetLow();
    }
    SCK_SetHigh();
    SCK_SetLow();

    if (volt & 512) {
        SDI_SetHigh();
    }
    else {
        SDI_SetLow();
    }
    SCK_SetHigh();
    SCK_SetLow();

    if (volt & 256) {
        SDI_SetHigh();
    }
    else {
        SDI_SetLow();
    }
    SCK_SetHigh();
    SCK_SetLow();

    if (volt & 128) {
        SDI_SetHigh();
    }
    else {
        SDI_SetLow();
    }
    SCK_SetHigh();
    SCK_SetLow();
    
    if (volt & 64) {
        SDI_SetHigh();
    }
    else {
        SDI_SetLow();
    }
    SCK_SetHigh();
    SCK_SetLow();
    
    if (volt & 32) {
        SDI_SetHigh();
    }
    else {
        SDI_SetLow();
    }
    SCK_SetHigh();
    SCK_SetLow();
    
    if (volt & 16) {
        SDI_SetHigh();
    }
    else {
        SDI_SetLow();
    }
    SCK_SetHigh();
    SCK_SetLow();
    
    if (volt & 8) {
        SDI_SetHigh();
    }
    else {
        SDI_SetLow();
    }
    SCK_SetHigh();
    SCK_SetLow();
    
    if (volt & 4) {
        SDI_SetHigh();
    }
    else {
        SDI_SetLow();
    }
    SCK_SetHigh();
    SCK_SetLow();
    
    if (volt & 2) {
        SDI_SetHigh();
    }
    else {
        SDI_SetLow();
    }
    SCK_SetHigh();
    SCK_SetLow();
    
    if (volt & 1) {
        SDI_SetHigh();
    }
    else {
        SDI_SetLow();
    }
    SCK_SetHigh();
    SCK_SetLow();
    
    SCK_SetHigh();
    SCK_SetLow();
    SCK_SetHigh();
    SCK_SetLow();
    SCK_SetHigh();
    SCK_SetLow();
    SCK_SetHigh();
    SCK_SetLow();
    
    CSLD_SetHigh();
}

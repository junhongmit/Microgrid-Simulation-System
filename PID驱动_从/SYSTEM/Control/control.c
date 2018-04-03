#include "control.h"
#include "lcd.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "pwm.h"
#include "pid.h"

extern HTRACK *Freqtrack;
extern u8 choose,Error,SetK,SetPro;
extern HBUTTON *but[3][5],*button1,*button2,*button3,*button4,*button5;
extern HEDIT *edit1,*edit2,*edit3,*edit4,*edit5,*edit6,*edit7;
extern HTAB *tab1;
extern float setValue;
extern float integral1,integral2,integral3;
extern float SetFreq, pwm,W;
extern u16 delay,delay_left;
extern float K;
extern float MI,pwm,pwmA,pwmB,pwmC,PhaseShift,FreqWord;
const unsigned char gImage_window[2528] = {  0X10,0X10,0X00,0X1E,0X00,0X2A,0X01,0X1B, 
0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,
0XF7,0X9E,0XF7,0X9E,0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,
0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,0XEF,0XBF,0XEF,0XBF,0XEF,0XBF,0XEF,0XBF,
0XEF,0XBF,0XEF,0XBF,0XEF,0XBF,0XEF,0XBF,0XEF,0XBF,0XEF,0XBF,0XF7,0X9E,0XF7,0X9E,
0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,
0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,
0XE7,0XBF,0XE7,0XBF,0XEF,0XBF,0XEF,0XBF,0XEF,0XBF,0XEF,0XBF,0XEF,0XBF,0XEF,0XBF,
0XEF,0XBF,0XEF,0XBF,0XEF,0XBF,0XEF,0XBF,0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,
0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,0XE7,0X9F,0XE7,0X9F,
0XE7,0X9F,0XE7,0X9F,0XE7,0X9F,0XE7,0X9F,0XE7,0X9F,0XE7,0X9F,0XE7,0X9F,0XE7,0X9F,
0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,
0XE7,0XBF,0XE7,0XBF,0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,
0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,0XE7,0X9F,0XE7,0X9F,0XE7,0X9F,0XE7,0X9F,
0XE7,0X9F,0XE7,0X9F,0XE7,0X9F,0XE7,0X9F,0XE7,0X9F,0XE7,0X9F,0XE7,0XBF,0XE7,0XBF,
0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,
0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,
0XF7,0X9E,0XF7,0X9E,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,
0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,
0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,0XF7,0X9E,0XF7,0X9E,
0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,
0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,
0XDF,0X9F,0XDF,0X9F,0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,
0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,
0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,0XDF,0X7F,0XDF,0X7F,
0XDF,0X7F,0XDF,0X7F,0XDF,0X7F,0XDF,0X7F,0XDF,0X7F,0XDF,0X7F,0XDF,0X7F,0XDF,0X7F,
0XE7,0X9F,0XE7,0X9F,0XE7,0X9F,0XE7,0X9F,0XE7,0X9F,0XE7,0X9F,0XE7,0X9F,0XE7,0X9F,
0XE7,0X9F,0XE7,0X9F,0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,
0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,0XDF,0X7F,0XDF,0X7F,0XDF,0X7F,0XDF,0X7F,
0XDF,0X7F,0XDF,0X7F,0XDF,0X7F,0XDF,0X7F,0XDF,0X7F,0XDF,0X7F,0XE7,0X9F,0XE7,0X9F,
0XE7,0X9F,0XE7,0X9F,0XE7,0X9F,0XE7,0X9F,0XE7,0X9F,0XE7,0X9F,0XE7,0X9F,0XE7,0X9F,
0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,
0XEF,0X7D,0XEF,0X7D,0XD7,0X7F,0XD7,0X7F,0XD7,0X7F,0XD7,0X7F,0XD7,0X7F,0XD7,0X7F,
0XD7,0X7F,0XD7,0X7F,0XD7,0X7F,0XD7,0X7F,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,
0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XEF,0X7D,0XEF,0X7D,
0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,
0XCF,0X5F,0XCF,0X5F,0XCF,0X5F,0XCF,0X5F,0XCF,0X5F,0XCF,0X5F,0XCF,0X5F,0XCF,0X5F,
0XCF,0X5F,0XCF,0X5F,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,
0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,
0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,0XCF,0X5F,0XCF,0X5F,
0XCF,0X5F,0XCF,0X5F,0XCF,0X5F,0XCF,0X5F,0XCF,0X5F,0XCF,0X5F,0XCF,0X5F,0XCF,0X5F,
0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,
0XDF,0X9F,0XDF,0X9F,0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,
0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,0XCF,0X3E,0XCF,0X3E,0XCF,0X3E,0XCF,0X3E,
0XCF,0X3E,0XCF,0X3E,0XCF,0X3E,0XCF,0X3E,0XCF,0X3E,0XCF,0X3E,0XDF,0X9F,0XDF,0X9F,
0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,
0XEF,0X5D,0XEF,0X5D,0XEF,0X5D,0XEF,0X5D,0XEF,0X5D,0XEF,0X5D,0XEF,0X5D,0XEF,0X5D,
0XEF,0X5D,0XEF,0X5D,0XC7,0X3E,0XC7,0X3E,0XC7,0X3E,0XC7,0X3E,0XC7,0X3E,0XC7,0X3E,
0XC7,0X3E,0XC7,0X3E,0XC7,0X3E,0XC7,0X3E,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,
0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XDE,0XFB,0XDE,0XFB,
0XDE,0XFB,0XDE,0XFB,0XDE,0XFB,0XDE,0XFB,0XDE,0XFB,0XDE,0XFB,0XDE,0XFB,0XDE,0XFB,
0XC7,0X3E,0XC7,0X3E,0XC7,0X3E,0XC7,0X3E,0XC7,0X3E,0XC7,0X3E,0XC7,0X3E,0XC7,0X3E,
0XC7,0X3E,0XC7,0X3E,0XBF,0X3F,0XBF,0X3F,0XBF,0X3F,0XBF,0X3F,0XBF,0X3F,0XBF,0X3F,
0XBF,0X3F,0XBF,0X3F,0XBF,0X3F,0XBF,0X3F,0XDE,0XFB,0XDE,0XFB,0XDE,0XFB,0XDE,0XFB,
0XDE,0XFB,0XDE,0XFB,0XDE,0XFB,0XDE,0XFB,0XDE,0XFB,0XDE,0XFB,0XC7,0X3E,0XC7,0X3E,
0XC7,0X3E,0XC7,0X3E,0XC7,0X3E,0XC7,0X3E,0XC7,0X3E,0XC7,0X3E,0XC7,0X3E,0XC7,0X3E,
0XBF,0X3F,0XBF,0X3F,0XBF,0X3F,0XBF,0X3F,0XBF,0X3F,0XBF,0X3F,0XBF,0X3F,0XBF,0X3F,
0XBF,0X3F,0XBF,0X3F,0XDE,0XDB,0XDE,0XDB,0XDE,0XDB,0XDE,0XDB,0XDE,0XDB,0XDE,0XDB,
0XDE,0XDB,0XDE,0XDB,0XDE,0XDB,0XDE,0XDB,0X9E,0X9D,0X9E,0X9D,0X9E,0X9D,0X9E,0X9D,
0X9E,0X9D,0X9E,0X9D,0X9E,0X9D,0X9E,0X9D,0X9E,0X9D,0X9E,0X9D,0XBF,0X3F,0XBF,0X3F,
0XBF,0X3F,0XBF,0X3F,0XBF,0X3F,0XBF,0X3F,0XBF,0X3F,0XBF,0X3F,0XBF,0X3F,0XBF,0X3F,
0XDE,0XDB,0XDE,0XDB,0XDE,0XDB,0XDE,0XDB,0XDE,0XDB,0XDE,0XDB,0XDE,0XDB,0XDE,0XDB,
0XDE,0XDB,0XDE,0XDB,0X96,0X7D,0X96,0X7D,0X96,0X7D,0X96,0X7D,0X96,0X7D,0X96,0X7D,
0X96,0X7D,0X96,0X7D,0X96,0X7D,0X96,0X7D,0XBF,0X1F,0XBF,0X1F,0XBF,0X1F,0XBF,0X1F,
0XBF,0X1F,0XBF,0X1F,0XBF,0X1F,0XBF,0X1F,0XBF,0X1F,0XBF,0X1F,0XDE,0XDB,0XDE,0XDB,
0XDE,0XDB,0XDE,0XDB,0XDE,0XDB,0XDE,0XDB,0XDE,0XDB,0XDE,0XDB,0XDE,0XDB,0XDE,0XDB,
0X96,0X7D,0X96,0X7D,0X96,0X7D,0X96,0X7D,0X96,0X7D,0X96,0X7D,0X96,0X7D,0X96,0X7D,
0X96,0X7D,0X96,0X7D,0XBF,0X1F,0XBF,0X1F,0XBF,0X1F,0XBF,0X1F,0XBF,0X1F,0XBF,0X1F,
0XBF,0X1F,0XBF,0X1F,0XBF,0X1F,0XBF,0X1F,0XDE,0XDB,0XDE,0XDB,0XDE,0XDB,0XDE,0XDB,
0XDE,0XDB,0XDE,0XDB,0XDE,0XDB,0XDE,0XDB,0XDE,0XDB,0XDE,0XDB,0X8E,0X5D,0X8E,0X5D,
0X8E,0X5D,0X8E,0X5D,0X8E,0X5D,0X8E,0X5D,0X8E,0X5D,0X8E,0X5D,0X8E,0X5D,0X8E,0X5D,
0XBF,0X1F,0XBF,0X1F,0XBF,0X1F,0XBF,0X1F,0XBF,0X1F,0XBF,0X1F,0XBF,0X1F,0XBF,0X1F,
0XBF,0X1F,0XBF,0X1F,0XDE,0XDB,0XDE,0XDB,0XDE,0XDB,0XDE,0XDB,0XDE,0XDB,0XDE,0XDB,
0XDE,0XDB,0XDE,0XDB,0XDE,0XDB,0XDE,0XDB,0X86,0X3D,0X86,0X3D,0X86,0X3D,0X86,0X3D,
0X86,0X3D,0X86,0X3D,0X86,0X3D,0X86,0X3D,0X86,0X3D,0X86,0X3D,0XB7,0X1F,0XB7,0X1F,
0XB7,0X1F,0XB7,0X1F,0XB7,0X1F,0XB7,0X1F,0XB7,0X1F,0XB7,0X1F,0XB7,0X1F,0XB7,0X1F,
0XD6,0XBA,0XD6,0XBA,0XD6,0XBA,0XD6,0XBA,0XD6,0XBA,0XD6,0XBA,0XD6,0XBA,0XD6,0XBA,
0XD6,0XBA,0XD6,0XBA,0X86,0X3D,0X86,0X3D,0X86,0X3D,0X86,0X3D,0X86,0X3D,0X86,0X3D,
0X86,0X3D,0X86,0X3D,0X86,0X3D,0X86,0X3D,0XB7,0X1F,0XB7,0X1F,0XB7,0X1F,0XB7,0X1F,
0XB7,0X1F,0XB7,0X1F,0XB7,0X1F,0XB7,0X1F,0XB7,0X1F,0XB7,0X1F,0XD6,0XBA,0XD6,0XBA,
0XD6,0XBA,0XD6,0XBA,0XD6,0XBA,0XD6,0XBA,0XD6,0XBA,0XD6,0XBA,0XD6,0XBA,0XD6,0XBA,
0X7E,0X1C,0X7E,0X1C,0X7E,0X1C,0X7E,0X1C,0X7E,0X1C,0X7E,0X1C,0X7E,0X1C,0X7E,0X1C,
0X7E,0X1C,0X7E,0X1C,0XB7,0X1F,0XB7,0X1F,0XB7,0X1F,0XB7,0X1F,0XB7,0X1F,0XB7,0X1F,
0XB7,0X1F,0XB7,0X1F,0XB7,0X1F,0XB7,0X1F,0XD6,0XBA,0XD6,0XBA,0XD6,0XBA,0XD6,0XBA,
0XD6,0XBA,0XD6,0XBA,0XD6,0XBA,0XD6,0XBA,0XD6,0XBA,0XD6,0XBA,0X7D,0XFC,0X7D,0XFC,
0X7D,0XFC,0X7D,0XFC,0X7D,0XFC,0X7D,0XFC,0X7D,0XFC,0X7D,0XFC,0X7D,0XFC,0X7D,0XFC,
0XAE,0XFF,0XAE,0XFF,0XAE,0XFF,0XAE,0XFF,0XAE,0XFF,0XAE,0XFF,0XAE,0XFF,0XAE,0XFF,
0XAE,0XFF,0XAE,0XFF,0XD6,0X9A,0XD6,0X9A,0XD6,0X9A,0XD6,0X9A,0XD6,0X9A,0XD6,0X9A,
0XD6,0X9A,0XD6,0X9A,0XD6,0X9A,0XD6,0X9A,0X7D,0XFC,0X7D,0XFC,0X7D,0XFC,0X7D,0XFC,
0X7D,0XFC,0X7D,0XFC,0X7D,0XFC,0X7D,0XFC,0X7D,0XFC,0X7D,0XFC,0XAE,0XFE,0XAE,0XFE,
0XAE,0XFE,0XAE,0XFE,0XAE,0XFE,0XAE,0XFE,0XAE,0XFE,0XAE,0XFE,0XAE,0XFE,0XAE,0XFE,
0XD6,0X9A,0XD6,0X9A,0XD6,0X9A,0XD6,0X9A,0XD6,0X9A,0XD6,0X9A,0XD6,0X9A,0XD6,0X9A,
0XD6,0X9A,0XD6,0X9A,0X7D,0XFC,0X7D,0XFC,0X7D,0XFC,0X7D,0XFC,0X7D,0XFC,0X7D,0XFC,
0X7D,0XFC,0X7D,0XFC,0X7D,0XFC,0X7D,0XFC,0XAE,0XFE,0XAE,0XFE,0XAE,0XFE,0XAE,0XFE,
0XAE,0XFE,0XAE,0XFE,0XAE,0XFE,0XAE,0XFE,0XAE,0XFE,0XAE,0XFE,0XD6,0X9A,0XD6,0X9A,
0XD6,0X9A,0XD6,0X9A,0XD6,0X9A,0XD6,0X9A,0XD6,0X9A,0XD6,0X9A,0XD6,0X9A,0XD6,0X9A,
0X75,0XDB,0X75,0XDB,0X75,0XDB,0X75,0XDB,0X75,0XDB,0X75,0XDB,0X75,0XDB,0X75,0XDB,
0X75,0XDB,0X75,0XDB,0XAE,0XFE,0XAE,0XFE,0XAE,0XFE,0XAE,0XFE,0XAE,0XFE,0XAE,0XFE,
0XAE,0XFE,0XAE,0XFE,0XAE,0XFE,0XAE,0XFE,0XD6,0X9A,0XD6,0X9A,0XD6,0X9A,0XD6,0X9A,
0XD6,0X9A,0XD6,0X9A,0XD6,0X9A,0XD6,0X9A,0XD6,0X9A,0XD6,0X9A,0X6D,0XBB,0X6D,0XBB,
0X6D,0XBB,0X6D,0XBB,0X6D,0XBB,0X6D,0XBB,0X6D,0XBB,0X6D,0XBB,0X6D,0XBB,0X6D,0XBB,
0XAE,0XDE,0XAE,0XDE,0XAE,0XDE,0XAE,0XDE,0XAE,0XDE,0XAE,0XDE,0XAE,0XDE,0XAE,0XDE,
0XAE,0XDE,0XAE,0XDE,0XCE,0X79,0XCE,0X79,0XCE,0X79,0XCE,0X79,0XCE,0X79,0XCE,0X79,
0XCE,0X79,0XCE,0X79,0XCE,0X79,0XCE,0X79,0X6D,0X9B,0X6D,0X9B,0X6D,0X9B,0X6D,0X9B,
0X6D,0X9B,0X6D,0X9B,0X6D,0X9B,0X6D,0X9B,0X6D,0X9B,0X6D,0X9B,0XA6,0XDE,0XA6,0XDE,
0XA6,0XDE,0XA6,0XDE,0XA6,0XDE,0XA6,0XDE,0XA6,0XDE,0XA6,0XDE,0XA6,0XDE,0XA6,0XDE,
0XF7,0X9E,0X94,0X92,0X73,0XAE,0X73,0X8E,0X73,0X8E,0X73,0X8E,0X73,0X8E,0X73,0XAE,
0X94,0X92,0XF7,0X9E,0XFF,0XFF,0X3A,0XCE,0X2B,0X10,0X2B,0X11,0X2B,0X11,0X2B,0X11,
0X2B,0X11,0X2B,0X10,0X3A,0XCE,0XCE,0X59,0XF7,0X9E,0X64,0X96,0X44,0X16,0X3B,0XF6,
0X3B,0XF6,0X3B,0XF6,0X3B,0XF6,0X44,0X16,0X64,0X96,0XFF,0XFF,0X84,0X51,0X94,0X92,
0XEF,0X5D,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XEF,0X5D,0X94,0X92,0X84,0X51,
0X32,0XAE,0X3B,0X51,0X84,0XF5,0X9D,0X97,0X9D,0X97,0X9D,0X97,0X9D,0X97,0X95,0X57,
0X3B,0X51,0X32,0XAE,0X5C,0X55,0X6C,0XF8,0XDF,0X5E,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XDF,0X5E,0X6C,0XF8,0X5C,0X55,0X73,0XAE,0XEF,0X5D,0XFF,0XDF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XDF,0XEF,0X5D,0X73,0XAE,0X2A,0XF0,0X84,0XF5,
0XBE,0X5A,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XBE,0X5A,0X95,0X57,0X2A,0XF0,
0X3C,0X16,0XE7,0X7E,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XE7,0X7E,0X3C,0X16,0X73,0X8E,0XFF,0XDF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XDF,0X73,0X8E,0X2B,0X11,0XB6,0X19,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XB6,0X19,0X2B,0X11,0X3B,0XF6,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X3B,0XF6,
0X73,0X8E,0XFF,0XDF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XDF,0X73,0X8E,0X2B,0X11,0XAE,0X19,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XAE,0X19,0X2B,0X11,0X3B,0XF6,0XF7,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF7,0XFF,0X3B,0XF6,0X73,0X8E,0XF7,0XBE,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF7,0XBE,0X73,0X8E,
0X2B,0X11,0XA5,0XF9,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XA5,0XF9,0X2B,0X11,0X3B,0XF6,0XF7,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XF7,0XFF,0X3B,0XF6,0X73,0X8E,0XF7,0XBE,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF7,0XBE,0X73,0X8E,0X2B,0X11,0X9D,0XD9,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X9D,0XD9,0X2B,0X11,
0X3B,0XF6,0XF7,0XDF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XF7,0XDF,0X3B,0XF6,0X73,0X8E,0XF7,0XBE,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XF7,0XBE,0X73,0X8E,0X2B,0X11,0X7D,0X58,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X7D,0X58,0X2B,0X11,0X3B,0XF6,0XEF,0XDF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XEF,0XDF,0X3B,0XF6,
0X73,0X8E,0XFF,0XDF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XDF,0X73,0X8E,0X2B,0X11,0X6D,0X38,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0X6D,0X38,0X2B,0X11,0X3B,0XF6,0XEF,0XDF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XEF,0XDF,0X3B,0XF6,0X73,0X8E,0XFF,0XDF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XDF,0X73,0X8E,
0X2B,0X11,0X6D,0X17,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0X6D,0X17,0X2B,0X11,0X3B,0XF6,0XEF,0XBF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XEF,0XBF,0X3B,0XF6,0X73,0X8E,0XF7,0XBE,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF7,0XBE,0X73,0X8E,0X2B,0X11,0X5C,0XD7,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5C,0XD7,0X2B,0X11,
0X3B,0XF6,0XEF,0XBF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XEF,0XBF,0X3B,0XF6,0X73,0XAE,0XE7,0X1C,0XEF,0X7D,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XEF,0X7D,0XE7,0X1C,0X73,0XAE,0X2B,0X11,0X54,0X96,0X6D,0XBB,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X65,0X7A,0X54,0X96,0X2B,0X11,0X3C,0X16,0XD7,0X3E,
0XDF,0X9F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XDF,0X9F,0XD7,0X3E,0X3C,0X16,
0X8C,0X51,0X8C,0X71,0XE7,0X3C,0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,0XE7,0X3C,
0X8C,0X71,0X8C,0X51,0X43,0X71,0X2B,0X31,0X65,0X7A,0X6D,0X9B,0X6D,0X9B,0X6D,0X9B,
0X6D,0X9B,0X65,0X7A,0X2B,0X31,0X43,0X71,0X5C,0X56,0X64,0XD8,0XD7,0X3E,0XEF,0XBF,
0XEF,0XBF,0XEF,0XBF,0XEF,0XBF,0XD7,0X3E,0X64,0XD8,0X5C,0X56,0XF7,0X9E,0X84,0X30,
0X73,0XAE,0X73,0X8E,0X73,0X8E,0X73,0X8E,0X73,0X8E,0X73,0XAE,0X84,0X30,0XF7,0X9E,
0XF7,0X9E,0X4B,0XB2,0X2B,0X31,0X2B,0X11,0X2B,0X11,0X2B,0X11,0X2B,0X11,0X2B,0X31,
0X4B,0XB2,0XF7,0X9E,0XF7,0X9E,0X54,0X55,0X3C,0X16,0X3B,0XF6,0X3B,0XF6,0X3B,0XF6,
0X3B,0XF6,0X3C,0X16,0X54,0X55,0XF7,0X9E,};


HBUTTON button[40];
HFONT font[40];
u8 zpos[200];
HTRACK track[10];
HEDIT  edit[10];
HTAB tab[5];
u8 tempst[25];
u8 Kst[10],Prost1[10],Prost2[10];
void checknumber1()
{
	float temp;
	temp=StrToReal((char*)Kst);
	if (temp>2.5) {temp=2.5;strcpy((char*)Kst,(char *)RealToStr(temp));} 
	if (temp<0.4) {temp=0.4;strcpy((char*)Kst,(char *)RealToStr(temp));} 
}
void checknumber2()
{
	float temp;
	temp=StrToReal((char*)Prost1);
	if (temp<0) {temp=0;strcpy((char*)Prost1,(char *)RealToStr(temp));} 
	if (temp>1000) {temp=1000;strcpy((char*)Prost1,(char *)RealToStr(temp));} 
}
void checknumber3()
{
	float temp;
	temp=StrToReal((char*)Prost2);
	if (temp<0) {temp=0;strcpy((char*)Prost2,(char *)RealToStr(temp));} 
	if (temp>1000) {temp=1000;strcpy((char*)Prost2,(char *)RealToStr(temp));} 
}
void CalculatePro(float K,u16 *fenzi,u16 *fenmu)
{
	u16 gcd=0,a,b,min,i;
	a=Round(K*1000); b=1000;
	if (b==0) return;
	while (gcd!=1) {
		min=(a>b)?b:a;
		for (i=min; i>=1; i--)
			if (a%i==0&&b%i==0) {
				gcd=i; break;
			}
		a/=gcd; b/=gcd;
	}
	(*fenzi)=a;
	(*fenmu)=b;
}
u16 SendMessage(void *window,u16 Amessage,u16 Wparam,u16 Lparam)
{
	u16 i;
	char temps[20];
	u16 fenzi,fenmu;
	if (Amessage&WM_TOUCHMESSAGE) {
		for (i=0; i<40; i++) if (button[i].flag&&button[i].show) {
			switch(Amessage) {
				case WM_TOUCHDOWN:
					ButtonDown(&button[i],Wparam,Lparam);
				break;
				case WM_TOUCHMOVE:
					ButtonMouseMove(&button[i],Wparam,Lparam);
				break;
				case WM_TOUCHUP:
					ButtonUp(&button[i],Wparam,Lparam);
				break;
			}
		}
		for (i=0; i<10; i++) if (track[i].flag&&track[i].show) {
			switch(Amessage) {
				case WM_TOUCHDOWN:
					TrackButtonDown(&track[i],Wparam,Lparam);
				break;
				case WM_TOUCHMOVE:
					TrackMouseMove(&track[i],Wparam,Lparam);
				break;
				case WM_TOUCHUP:
					TrackButtonUp(&track[i],Wparam,Lparam);
				break;
			}
		}
		for (i=0; i<5; i++) if (tab[i].flag&&tab[i].show) {
			switch(Amessage) {
				case WM_TOUCHDOWN:
					TabButtonDown(&tab[i],Wparam,Lparam);
				break;
			}
		}
	} else if (Amessage==WM_COMMAND){
		switch (Wparam) {
			case 000:
				SetButtonState(button1,1);
				choose=0; delay=1; delay_left=10000; PID_Clear(&integral2);PID_Clear(&integral3);
				UnSetButtonState(button2);
			break;
			case 001:
				SetButtonState(button2,1);
				choose=1; delay=1; delay_left=10000; PID_Clear(&integral2);PID_Clear(&integral3);
				UnSetButtonState(button1);
			break;
			case 003:
			break;
			case 103:
				if (!SetK) {
					SetButtonState(but[0][3],1);
					SetButtonState(but[0][4],3);
					UnSetButtonState(but[2][4]);
					SetK=1;
					LCD_Fill(62,165,120,30,WHITE);
					LCD_ShowBigNum(62,165,1/StrToReal((char*)Kst));
					sprintf(temps,"%s : %s",Prost1,Prost2);
					LCD_Fill(78,196,120,16,WHITE);
					LCD_ShowString(78,196,100,25,16,(u8*)temps);
				} else {
					UnSetButtonState(but[0][3]);
					UnSetButtonState(but[0][4]);
					SetButtonState(but[2][4],3);
					SetK=0;
					LCD_Fill(62,165,120,30,WHITE);
					LCD_ShowBigNum(62,165,StrToReal((char*)Kst));
					sprintf(temps,"%s : %s",Prost1,Prost2);
					LCD_Fill(78,196,120,16,WHITE);
					LCD_ShowString(78,196,100,25,16,(u8*)temps);
				}
			break;
			case 104:
				if (!SetK) {
					if (!SetPro) {
						SetButtonState(but[0][4],1);
						SetPro=1;
						LCD_Fill(62,165,120,30,WHITE);
						LCD_ShowBigNum(62,165,StrToReal((char*)Kst));
						sprintf(temps,"%s : %s",Prost1,Prost2);
						LCD_Fill(78,196,120,16,WHITE);
						LCD_ShowString(78,196,100,25,16,(u8*)temps);
					} else {
						SetButtonState(but[0][4],0);
						SetPro=0;
						LCD_Fill(62,165,120,30,WHITE);
						LCD_ShowBigNum(62,165,StrToReal((char*)Kst));
						sprintf(temps,"%s : %s",Prost1,Prost2);
						LCD_Fill(78,196,120,16,WHITE);
						LCD_ShowString(78,196,100,25,16,(u8*)temps);
					}
				}
			break;
			case 100:
				if (SetK) {
					strcat((char*)Kst,"7"); LCD_Fill(62,165,120,30,WHITE); LCD_ShowBigNum(62,165,StrToReal((char*)Kst));
				} else if (!SetPro) {
					strcat((char*)Prost1,"7");POINT_COLOR=BLACK; sprintf(temps,"%s : %s",Prost1,Prost2); LCD_Fill(78,196,120,16,WHITE); LCD_ShowString(78,196,100,25,16,(u8*)temps);
				} else if (SetPro) {
					strcat((char*)Prost2,"7");POINT_COLOR=BLACK;  sprintf(temps,"%s : %s",Prost1,Prost2); LCD_Fill(78,196,120,16,WHITE); LCD_ShowString(78,196,100,25,16,(u8*)temps);
				}
			break;
			case 101:
				if (SetK) {
					strcat((char*)Kst,"8"); LCD_Fill(62,165,120,30,WHITE); LCD_ShowBigNum(62,165,StrToReal((char*)Kst));
				} else if (!SetPro) {
					strcat((char*)Prost1,"8");POINT_COLOR=BLACK;  sprintf(temps,"%s : %s",Prost1,Prost2); LCD_Fill(78,196,120,16,WHITE); LCD_ShowString(78,196,100,25,16,(u8*)temps);
				} else if (SetPro) {
					strcat((char*)Prost2,"8");POINT_COLOR=BLACK;  sprintf(temps,"%s : %s",Prost1,Prost2); LCD_Fill(78,196,120,16,WHITE); LCD_ShowString(78,196,100,25,16,(u8*)temps);
				}
			break;
			case 102:
				if (SetK) {
					strcat((char*)Kst,"9"); LCD_Fill(62,165,120,30,WHITE); LCD_ShowBigNum(62,165,StrToReal((char*)Kst));
				} else if (!SetPro) {
					strcat((char*)Prost1,"9");POINT_COLOR=BLACK;  sprintf(temps,"%s : %s",Prost1,Prost2); LCD_Fill(78,196,120,16,WHITE); LCD_ShowString(78,196,100,25,16,(u8*)temps);
				} else if (SetPro) {
					strcat((char*)Prost2,"9");POINT_COLOR=BLACK;  sprintf(temps,"%s : %s",Prost1,Prost2); LCD_Fill(78,196,120,16,WHITE); LCD_ShowString(78,196,100,25,16,(u8*)temps);
				}
			break;
			case 105:
				if (SetK) {
					strcat((char*)Kst,"4"); LCD_Fill(62,165,120,30,WHITE); LCD_ShowBigNum(62,165,StrToReal((char*)Kst));
				} else if (!SetPro) {
					strcat((char*)Prost1,"4");POINT_COLOR=BLACK;  sprintf(temps,"%s : %s",Prost1,Prost2); LCD_Fill(78,196,120,16,WHITE); LCD_ShowString(78,196,100,25,16,(u8*)temps);
				} else if (SetPro) {
					strcat((char*)Prost2,"4");POINT_COLOR=BLACK;  sprintf(temps,"%s : %s",Prost1,Prost2); LCD_Fill(78,196,120,16,WHITE); LCD_ShowString(78,196,100,25,16,(u8*)temps);
				}
			break;
			case 106:
				if (SetK) {
					strcat((char*)Kst,"5"); LCD_Fill(62,165,120,30,WHITE); LCD_ShowBigNum(62,165,StrToReal((char*)Kst));
				} else if (!SetPro) {
					strcat((char*)Prost1,"5");POINT_COLOR=BLACK;  sprintf(temps,"%s : %s",Prost1,Prost2); LCD_Fill(78,196,120,16,WHITE); LCD_ShowString(78,196,100,25,16,(u8*)temps);
				} else if (SetPro) {
					strcat((char*)Prost2,"5");POINT_COLOR=BLACK;  sprintf(temps,"%s : %s",Prost1,Prost2); LCD_Fill(78,196,120,16,WHITE); LCD_ShowString(78,196,100,25,16,(u8*)temps);
				}
			break;
			case 107:
				if (SetK) {
					strcat((char*)Kst,"6"); LCD_Fill(62,165,120,30,WHITE); LCD_ShowBigNum(62,165,StrToReal((char*)Kst));
				} else if (!SetPro) {
					strcat((char*)Prost1,"6");POINT_COLOR=BLACK;  sprintf(temps,"%s : %s",Prost1,Prost2); LCD_Fill(78,196,120,16,WHITE); LCD_ShowString(78,196,100,25,16,(u8*)temps);
				} else if (SetPro) {
					strcat((char*)Prost2,"6");POINT_COLOR=BLACK;  sprintf(temps,"%s : %s",Prost1,Prost2); LCD_Fill(78,196,120,16,WHITE); LCD_ShowString(78,196,100,25,16,(u8*)temps);
				}
			break;
			case 110:
				if (SetK) {
					strcat((char*)Kst,"1"); LCD_Fill(62,165,120,30,WHITE); LCD_ShowBigNum(62,165,StrToReal((char*)Kst));
				} else if (!SetPro) {
					strcat((char*)Prost1,"1");POINT_COLOR=BLACK;  sprintf(temps,"%s : %s",Prost1,Prost2); LCD_Fill(78,196,120,16,WHITE); LCD_ShowString(78,196,100,25,16,(u8*)temps);
				} else if (SetPro) {
					strcat((char*)Prost2,"1");POINT_COLOR=BLACK;  sprintf(temps,"%s : %s",Prost1,Prost2); LCD_Fill(78,196,120,16,WHITE); LCD_ShowString(78,196,100,25,16,(u8*)temps);
				}
			break;
			case 111:
				if (SetK) {
					strcat((char*)Kst,"2"); LCD_Fill(62,165,120,30,WHITE); LCD_ShowBigNum(62,165,StrToReal((char*)Kst));
				} else if (!SetPro) {
					strcat((char*)Prost1,"2");POINT_COLOR=BLACK;  sprintf(temps,"%s : %s",Prost1,Prost2); LCD_Fill(78,196,120,16,WHITE); LCD_ShowString(78,196,100,25,16,(u8*)temps);
				} else if (SetPro) {
					strcat((char*)Prost2,"2");POINT_COLOR=BLACK;  sprintf(temps,"%s : %s",Prost1,Prost2); LCD_Fill(78,196,120,16,WHITE); LCD_ShowString(78,196,100,25,16,(u8*)temps);
				}
			break;
			case 112:
				if (SetK) {
					strcat((char*)Kst,"3"); LCD_Fill(62,165,120,30,WHITE); LCD_ShowBigNum(62,165,StrToReal((char*)Kst));
				} else if (!SetPro) {
					strcat((char*)Prost1,"3");POINT_COLOR=BLACK;  sprintf(temps,"%s : %s",Prost1,Prost2); LCD_Fill(78,196,120,16,WHITE); LCD_ShowString(78,196,100,25,16,(u8*)temps);
				} else if (SetPro) {
					strcat((char*)Prost2,"3");POINT_COLOR=BLACK;  sprintf(temps,"%s : %s",Prost1,Prost2); LCD_Fill(78,196,120,16,WHITE); LCD_ShowString(78,196,100,25,16,(u8*)temps);
				}
			break;
			case 113:
				if (SetK) {
					strcat((char*)Kst,"0"); LCD_Fill(62,165,120,30,WHITE); LCD_ShowBigNum(62,165,StrToReal((char*)Kst));
				} else if (!SetPro) {
					strcat((char*)Prost1,"0");POINT_COLOR=BLACK;  sprintf(temps,"%s : %s",Prost1,Prost2); LCD_Fill(78,196,120,16,WHITE); LCD_ShowString(78,196,100,25,16,(u8*)temps);
				} else if (SetPro) {
					strcat((char*)Prost2,"0");POINT_COLOR=BLACK;  sprintf(temps,"%s : %s",Prost1,Prost2); LCD_Fill(78,196,120,16,WHITE); LCD_ShowString(78,196,100,25,16,(u8*)temps);
				}
			break;
			case 114:
				if (SetK) {
					if (strchr((char*)Kst,'.')==NULL)strcat((char*)Kst,"."); 
					LCD_Fill(62,165,120,30,WHITE); LCD_ShowBigNum(62,165,StrToReal((char*)Kst));
				} else if (!SetPro) {
					if (strchr((char*)Prost1,'.')==NULL)strcat((char*)Prost1,"."); 
					POINT_COLOR=BLACK; sprintf(temps,"%s : %s",Prost1,Prost2); LCD_Fill(78,196,120,16,WHITE); LCD_ShowString(78,196,100,25,16,(u8*)temps);
				} else if (SetPro) {
					if (strchr((char*)Prost2,'.')==NULL)strcat((char*)Prost2,"."); 
					POINT_COLOR=BLACK; sprintf(temps,"%s : %s",Prost1,Prost2); LCD_Fill(78,196,120,16,WHITE); LCD_ShowString(78,196,100,25,16,(u8*)temps);
				}
			break;
			case 108:
				if (SetK) {
					memset(Kst,0,sizeof(Kst));  
					LCD_Fill(62,165,120,30,WHITE); LCD_ShowBigNum(62,165,StrToReal((char*)Kst));
				} else if (!SetPro) {
					memset(Prost1,0,sizeof(Prost1));  
					POINT_COLOR=BLACK; sprintf(temps,"%s : %s",Prost1,Prost2); LCD_Fill(78,196,120,16,WHITE); LCD_ShowString(78,196,100,25,16,(u8*)temps);
				} else if (SetPro) {
					memset(Prost2,0,sizeof(Prost2));  
					POINT_COLOR=BLACK; sprintf(temps,"%s : %s",Prost1,Prost2); LCD_Fill(78,196,120,16,WHITE); LCD_ShowString(78,196,100,25,16,(u8*)temps);
				}
			break;
			case 109://Enter
				if (SetK) {
					checknumber1();
					K=StrToReal((char*)Kst);
					LCD_Fill(62,165,120,30,WHITE);
					CalculatePro(K,&fenzi,&fenmu);
					LCD_ShowBigNum(62,165,K);
					POINT_COLOR=BLACK; 
					strcpy((char*)Kst,(char*)RealToStr(K));
					strcpy((char*)Prost1,(char*)IntToStr(fenzi));
					strcpy((char*)Prost2,(char*)IntToStr(fenmu));
					sprintf(temps,"%s : %s",Prost1,Prost2);
					LCD_Fill(78,196,120,16,WHITE);
					LCD_ShowString(78,196,100,25,16,(u8*)temps);
					//Freqtrack->pos=log10(K)*1256.47F+500;
					//SetTrackPos(Freqtrack,Freqtrack->pos);
				} else if (!SetPro) {
					checknumber2();
					fenzi=atoi((char*)Prost1);
					fenmu=atoi((char*)Prost2);
					K=(float)fenzi/fenmu;
					if (K>2.5) K=2.5; else
					if (K<0.4) K=0.4;
					CalculatePro(K,&fenzi,&fenmu);
					strcpy((char*)Prost1,(char*)IntToStr(fenzi));
					strcpy((char*)Prost2,(char*)IntToStr(fenmu));
					LCD_Fill(62,165,120,30,WHITE);
					LCD_ShowBigNum(62,165,K);
					POINT_COLOR=BLACK; 
					strcpy((char*)Kst,(char*)RealToStr(K));
					sprintf(temps,"%s : %s",Prost1,Prost2);
					LCD_Fill(78,196,120,16,WHITE);
					LCD_ShowString(78,196,100,25,16,(u8*)temps);
					//Freqtrack->pos=log10(K)*1256.47F+500;
					//SetTrackPos(Freqtrack,Freqtrack->pos);
				} else if (SetPro) {
					checknumber3();
					fenzi=atoi((char*)Prost1);
					fenmu=atoi((char*)Prost2);
					K=(float)fenzi/fenmu;
					if (K>2.5) K=2.5; else
					if (K<0.4) K=0.4;
					CalculatePro(K,&fenzi,&fenmu);
					strcpy((char*)Prost1,(char*)IntToStr(fenzi));
					strcpy((char*)Prost2,(char*)IntToStr(fenmu));
					LCD_Fill(62,165,120,30,WHITE);
					LCD_ShowBigNum(62,165,K);
					POINT_COLOR=BLACK; 
					strcpy((char*)Kst,(char*)RealToStr(K));
					sprintf(temps,"%s : %s",Prost1,Prost2);
					LCD_Fill(78,196,120,16,WHITE);
					LCD_ShowString(78,196,100,25,16,(u8*)temps);
					//Freqtrack->pos=log10(K)*1256.47F+500;
					//SetTrackPos(Freqtrack,Freqtrack->pos);
				}
			break;
			case 201:

			break;
			case 202:

			break;
			case 1001:
				
			break;
			default:
		}
	} else if (Amessage==WM_TRACKPOSMOVE) {
		switch (Lparam) {
			case 201:
				K=pow(10,(Wparam-500)/1256.47F);
				LCD_Fill(62,165,120,30,WHITE);
				CalculatePro(K,&fenzi,&fenmu);
				LCD_ShowBigNum(62,165,K);
				strcpy((char*)Kst,(char*)RealToStr(K));
				strcpy((char*)Prost1,(char*)IntToStr(fenzi));
				strcpy((char*)Prost2,(char*)IntToStr(fenmu));
				sprintf(temps,"%s : %s",Prost1,Prost2);
				LCD_Fill(78,196,120,16,WHITE);
				LCD_ShowString(78,196,100,25,16,(u8*)temps);
			break;
			case 301:

			break;
			case 401:

			break;
			case 501:

			break;
			case 502:

			break;
			case 503:
				
			break;
			case 504:

			break;
			case 202:
				/*//TIM1_Mode_Config(Wparam);
				dutytrack->pos=(u32)dutytrack->pos*Wparam/dutytrack->range;
				dutytrack->range=Wparam;
				//TIM_SetCompare1(TIM1,dutytrack->pos);
				//TIM_SetCompare2(TIM1,dutytrack->pos);
				LCD_Fill(78,50+16+4,100,30,WHITE);
				POINT_COLOR=BLACK;
				LCD_ShowString(38,80,100,20,16,"Period:");
				LCD_ShowBigNum(78,50+16+4,Wparam);
				strcpy(tempst,"Frequency:");
				strcat(tempst,RealToStr(84000000.0/Wparam/1000));
				strcat(tempst,"kHz");
				LCD_Fill(30,140,200,20,WHITE);
				LCD_ShowString(30,140,200,20,16,tempst);*/
			break;
		}
	} else if (Amessage==WM_TIMER) {
		for (i=0; i<10; i++) if (edit[i].flag) {
			edit[i].cursor=~edit[i].cursor;
			UpdateWindow(&edit[i]);
		}
	}
	return 1;
}
void SetWindowPos(void *window,u16 x,u16 y,u16 width,u16 height,u16 uFlag)
{
	u16 i;
	bool refresh;
	for (i=0; i<40; i++)
		if (&button[i]==window&&button[i].flag) {
			if (!(uFlag&SWP_NOPOS)) {
				if (button[i].x!=x||button[i].y!=y) refresh=true;
				button[i].x=x;
				button[i].y=y;
			}
			if (!(uFlag&SWP_NOSIZE)) {
				if (button[i].width!=width||button[i].height!=height) refresh=true;
				button[i].width=width;
				button[i].height=height;
			}
			if (refresh) UpdateWindow(window);
		}
}
void ShowWindow(void *window, u16 show)
{
	u16 i;
	for (i=0; i<40; i++)
		if (window==&button[i]&&button[i].flag) {
			button[i].show=show;
			if (show==SW_SHOW) UpdateWindow(window);
			return;
		}
	for (i=0; i<10; i++) 
		if (window==&track[i]&&track[i].flag) {
			track[i].show=show;
			track[i].button->show=show;
			if (show==SW_SHOW) {UpdateWindow(window);UpdateWindow(track[i].button);}
			return;
		}
	for (i=0; i<10; i++) 
		if (window==&edit[i]&&edit[i].flag) {
			edit[i].show=show;
			if (show==SW_SHOW) UpdateWindow(window);
			return;
		}
	for (i=0; i<5; i++)
		if (window==&tab[i]&&tab[i].flag) {
			tab[i].show=show;
			if (show==SW_SHOW) UpdateWindow(window);
			return;
		}
}
HFONT *CreateFont(char *name,u16 size)
{
	return NULL;
}


//按钮部分
HBUTTON *CreateButton(u16 x,u16 y,u16 width,u16 height,char *name,HFONT *font,u16 cmd)
{
	u16 now=60,i;
	for (i=0; i<40; i++)
		if (!button[i].flag) {
			button[i].flag=true; now=i;
			break;
		}
	if (now==60) return NULL;
	button[now].font=font;
	button[now].x=x; button[now].y=y;
	button[now].width=width;
	button[now].height=height;
	button[now].edgewidth=2;
	button[now].edgeheight=2;
	button[now].cwidth=3;
	button[now].cheight=3;
	button[now].num=cmd;
	button[now].show=true;
	strcpy((char*)button[now].text,(char*)name);
	
	UpdateWindow(&button[now]);
	return &button[now];
}
void ButtonDown(HBUTTON *now,int x,int y)
{
	if (x>=now->x&&y>=now->y&&x<=now->x+now->width-1&&y<=now->y+now->height-1) {
		if (!now->setstate) now->state=1;
		now->havepressed=true;
		UpdateWindow(now);
	}
}
void ButtonMouseMove(HBUTTON *now,int x,int y)
{
	u8 laststate=now->state;
	if (x>=now->x&&y>=now->y&&x<=now->x+now->width-1&&y<=now->y+now->height-1) {
		if (now->havepressed) {
			if (!now->setstate) now->state=1;
		}
	} else {
		if (now->state==1&&now->havepressed) {
			if (!now->setstate) now->state=2;
		}
	}
	if (laststate!=now->state) UpdateWindow(now);
}
void ButtonUp(HBUTTON *now,int x,int y)
{
	u8 laststate=now->state;
	if (x>=now->x&&y>=now->y&&x<=now->x+now->width-1&&y<=now->y+now->height-1) {
		if (now->havepressed&&now->state!=3) SendMessage(now,WM_COMMAND,now->num,0);
		now->havepressed=false;
	}
	if (!now->setstate) now->state=0;
	if (laststate) UpdateWindow(now);
}
void SetButtonState(HBUTTON *now,int state)
{
	now->setstate=1;
	now->state=state;
	UpdateWindow(now);
}
void UnSetButtonState(HBUTTON *now)
{
	now->setstate=0;
	now->state=0;
	UpdateWindow(now);
}


//拖动条部分
HTRACK *CreateTrackBar(u16 x,u16 y,u16 width,u16 height,u16 range,u16 cmd)
{
	u16 now=60,i;
	for (i=0; i<10; i++)
		if (!track[i].flag) {
			track[i].flag=true; now=i;
			break;
		}
	if (now==60) return NULL;
	track[now].x=x; track[now].y=y;
	track[now].width=width;
	track[now].height=height;
	track[now].pos=0;
	track[now].range=range;
	track[now].num=cmd;
	track[now].button=CreateButton(x+10,y+2,12,height-4,"",NULL,10000);
	track[now].show=true;
	
	UpdateWindow(&track[now]);
	UpdateWindow(track[now].button);
	return &track[now];
}
void TrackButtonDown(HTRACK *now,int x,int y)
{
	if (x>=now->x&&y>=now->y&&x<=now->x+now->width-1&&y<=now->y+now->height-1) {
		if (now->button->state==1) {
			now->x0=x-now->button->x;
			now->y0=y-now->button->y;
			now->move=true;
		}
	}
}
void TrackMouseMove(HTRACK *now,int x,int y)
{
	if (now->move) {
		if (x-now->x0<now->x+10) x=now->x+10+now->x0;
		if (x-now->x0>now->x+now->width-22) x=now->x+now->width-22+now->x0;
		if (x-now->x0!=now->button->x) UpdateWindow(now);
		SetWindowPos(now->button,x-now->x0,now->y+2,0,0,SWP_NOSIZE);
		now->pos=(x-now->x0-now->x-10)*now->range/(now->width-32);
		SendMessage(now,WM_TRACKPOSMOVE,now->pos,now->num);
	}
}
void TrackButtonUp(HTRACK *now,int x,int y)
{
	now->x0=0;
	now->y0=0;
	now->move=false;
}
void SetTrackPos(HTRACK *now,u16 x)
{
	now->pos=x;
	UpdateWindow(now);
	SetWindowPos(now->button,(u32)(now->width-32)*now->pos/now->range+now->x+10,now->y+2,0,0,SWP_NOSIZE);
	UpdateWindow(now->button);
	SendMessage(now,WM_TRACKPOSMOVE,now->pos,now->num);
}



//编辑框部分
HEDIT *CreateEdit(u16 x,u16 y,u16 width,u16 height,u8 *text)
{
	u16 now=60,i;
	for (i=0; i<10; i++)
		if (!edit[i].flag) {
			edit[i].flag=true; now=i;
			break;
		}
	if (now==60) return NULL;
	edit[now].x=x; edit[now].y=y;
	edit[now].width=width;
	edit[now].height=height;
	edit[now].show=true;
	strcpy((char*)edit[now].text,(char*)text);
	
	UpdateWindow(&edit[now]);
	return &edit[now];
}



//分页部分
HTAB *CreateTab(u16 x,u16 y,u16 width,u16 height,u16 num)
{
	u16 now=60,i;
	for (i=0; i<5; i++)
		if (!tab[i].flag) {
			tab[i].flag=true; now=i;
			break;
		}
	if (now==60) return NULL;
	tab[now].x=x; tab[now].y=y;
	tab[now].width=width;
	tab[now].height=height;
	tab[now].TabNum=0;
	tab[now].num=num;
	tab[now].show=true;
	memset(tab[now].text,0,sizeof(tab[now].text));
	
	UpdateWindow(&tab[now]);
	return &tab[now];
}
void AddTabText(HTAB *now, u8 *text)
{
	if (now->TabNum<10) {
		strcpy((char*)now->text[now->TabNum++],(char*)text);
		now->TabLastShow=now->TabShow;
		now->TabShow=now->TabNum-1;
		UpdateWindow(now);
	}
}
void SetTabShow(HTAB *now, u16 show)
{
	now->TabLastShow=now->TabShow;
	now->TabShow=show;
	UpdateWindow(now);
}
void TabButtonDown(HTAB *now,int x,int y)
{
	u8 i;
	u16 nowx,nextx;
	if (x>=now->x&&y>=now->y&&x<=now->x+now->width-1&&y<=now->y+20) {
		nowx=now->x+2;
		for (i=0; i<now->TabNum; i++) {
			nextx=nowx+12+8*strlen((char*)now->text[i]);
			if (x>=nowx&&x<=nextx) {
				now->TabLastShow=now->TabShow;
				now->TabShow=i; break;
			}
			nowx=nextx;
		}
		UpdateWindow(now);
		SendMessage(now,WM_COMMAND,now->num,0);
	}
}

void UpdateWindow(void *window)
{
	u16 i,j;
	u16 x,y,width,height,cwidth,cheight,edgewidth,edgeheight,nowx,nextx,snowx,snextx;
	u8 state;
	for (i=0; i<40; i++)
		if (window==&button[i]&&button[i].flag&&button[i].show) {
			x=button[i].x; y=button[i].y;
			width=button[i].width;
			height=button[i].height;
			cwidth=button[i].cwidth;
			cheight=button[i].cheight;
			edgewidth=button[i].edgewidth;
			edgeheight=button[i].edgeheight;
			state=button[i].state;
			if (state==3) {
				state=0; 
				LCD_Fill(x,y,width,height,RGB(196,196,196));
			} else StretchBlt(x,y,width,height,gImage_window,0+10*state,0,10,28);
			BitBlt(x,y,cwidth,cheight,gImage_window,0+10*state,28);
			BitBlt(x+width-cwidth,y,cwidth,cheight,gImage_window,7+10*state,28);
			BitBlt(x,y+height-cheight,cwidth,cheight,gImage_window,0+10*state,39);
			BitBlt(x+width-cwidth,y+height-cheight,cwidth,cheight,gImage_window,7+10*state,39);
			StretchBlt(x+cwidth,y,width-2*cwidth,edgeheight,gImage_window,3+10*state,28,4,2);
			StretchBlt(x,y+cheight,edgewidth,height-2*cheight,gImage_window,0+10*state,31,2,8);
			StretchBlt(x+width-edgewidth,y+cheight,edgewidth,height-2*cheight,gImage_window,8+10*state,31,2,8);
			StretchBlt(x+cwidth,y+height-edgeheight,width-2*cwidth,edgeheight,gImage_window,3+10*state,40,4,2);
			if (button[i].state==0) POINT_COLOR=BLACK; else 
			if (button[i].state==3) POINT_COLOR=RGB(96,96,96); else
			POINT_COLOR=RED;
			LCD_ShowString(x+width/2-strlen((char*)button[i].text)*3,y+height/2-6,strlen((char*)button[i].text)*5+20,30,12,button[i].text);
			return;
		}
	for (i=0; i<10; i++) 
		if (window==&track[i]&&track[i].flag&&track[i].show) {
			x=track[i].x; y=track[i].y;
			width=track[i].width;
			height=track[i].height;
			LCD_Fill(x,y,width,height,WHITE);
			LCD_Fast_DrawPoint(x+2,y+(height-4)/2,RGB(156,156,156));
			POINT_COLOR=RGB(176,176,176); LCD_DrawLine(x+3,y+(height-4)/2,x+width-3,y+(height-4)/2);
			LCD_Fast_DrawPoint(x+2,y+(height-4)/2+1,RGB(176,176,176));
			LCD_Fast_DrawPoint(x+2,y+(height-4)/2+2,RGB(176,176,176));
			POINT_COLOR=RGB(252,252,252); LCD_DrawLine(x+3,y+(height-4)/2+3,x+width-3,y+(height-4)/2+3);
			LCD_Fast_DrawPoint(x+width-3,y+(height-4)/2+1,RGB(231,234,234));
			LCD_Fast_DrawPoint(x+width-3,y+(height-4)/2+2,RGB(231,234,234));
			LCD_Fill(x+3,y+(height-4)/2+1,width-6,2,RGB(231,234,234));
			return;
		}
	for (i=0; i<10; i++) 
		if (window==&edit[i]&&edit[i].flag&&edit[i].show) {
			POINT_COLOR=0x738E;
			x=edit[i].x; y=edit[i].y;
			width=edit[i].width;
			height=edit[i].height;
			LCD_Fill(x,y,width,height,WHITE);
			LCD_DrawLine(x,y,x+width-1,y);
			LCD_DrawLine(x,y+height-1,x+width-1,y+height-1);
			LCD_DrawLine(x,y,x,y+height-1);
			LCD_DrawLine(x+width-1,y,x+width-1,y+height-1);
			LCD_ShowBigNum(x,y+(height-30)/2,StrToReal((char *)edit[i].text));
			if (edit[i].cursor) {
				POINT_COLOR=BLACK;
				LCD_DrawLine(x+7+15*strlen((char *)edit[i].text),y+5,x+7+15*strlen((char *)edit[i].text),y+35);
			}
			/*width+=x;
			height+=y;
			p=edit[i].text;
			while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
			{       
				if(x>=width){x=x0;y+=30;}
				if(y>=height)break;//退出
				LCD_ShowBigNum(x,y,*p,size,0);
				x+=15;
				p++;
			} */
			
		}
	for (i=0; i<5; i++)
		if (window==&tab[i]&&tab[i].flag&&tab[i].show) {
			x=tab[i].x; y=tab[i].y;
			width=tab[i].width;
			height=tab[i].height;
			LCD_Fill(x,y,width,20,RGB(240,240,240));
			nowx=x+2;
			if (tab[i].TabNum==0) break;
			for (j=0; j<tab[i].TabNum; j++) {
				nextx=nowx+12+8*strlen((char*)tab[i].text[j]);
				if (j==tab[i].TabShow) {
					snowx=nowx-1; snextx=nextx+1;
				}
				StretchBlt(nowx,y+2,nextx-nowx,18,gImage_window,0,0,10,28);
				POINT_COLOR=RGB(137,140,149);
				LCD_DrawLine(nowx,y+2,nextx,y+2);
				LCD_DrawLine(nowx,y+2,nowx,y+20);
				LCD_DrawLine(nextx,y+2,nextx,y+20);
				POINT_COLOR=BLACK;
				LCD_ShowString(nowx+6,y+5,strlen((char*)tab[i].text[j])*8+20,20,16,tab[i].text[j]);
				nowx=nextx+1;
			}
			LCD_Fill(x,y+20,width,height-20,WHITE);
			POINT_COLOR=RGB(137,140,149);
			LCD_DrawLine(x,y+20,x+width-1,y+20);
			LCD_DrawLine(x,y+20,x,y+height-1);
			LCD_DrawLine(x+width-1,y+20,x+width-1,y+height-1);
			LCD_DrawLine(x,y+height-1,x+width-1,y+height-1);
			
			LCD_Fill(snowx,y,snextx-snowx,21,WHITE);
			POINT_COLOR=RGB(137,140,149);
			LCD_DrawLine(snowx,y,snextx,y);
			LCD_DrawLine(snowx,y,snowx,y+20);
			LCD_DrawLine(snextx,y,snextx,y+20);
			POINT_COLOR=BLACK;
			LCD_ShowString(snowx+7,y+3,strlen((char*)tab[i].text[tab[i].TabShow])*8+20,20,16,tab[i].text[tab[i].TabShow]);
		}
}

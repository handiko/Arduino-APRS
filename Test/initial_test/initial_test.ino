#include <math.h>


// APRS DATA FIELD CONSTANTS
const unsigned char flag=0x7e;
const unsigned char dest_aprs[7]={('A'<<1),('P'<<1),('A'<<1),('R'<<1),('D'<<1),('1'<<1),('0'<<1)};
const unsigned char dest_beacon[7]={('B'<<1),('E'<<1),('A'<<1),('C'<<1),('O'<<1),('N'<<1),('0'<<1)};
const unsigned char mycall[7]={('Y'<<1),('D'<<1),('1'<<1),('S'<<1),('D'<<1),('L'<<1),((11+'0')<<1)};
const unsigned char digi[8]={"WIDE2 2"};
const unsigned char ctrl_field=0x03;
const unsigned char pid=0xf0;
const unsigned char info_status='>';

// WAVE DURATION CONSTANTS
const unsigned int wd_1200=(unsigned int)1/1200;
const unsigned int wd_2400=(unsigned int)1/2400;
const float wd_adj=1.0;

void gen_1200(int duration_us)
{
  
}

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}

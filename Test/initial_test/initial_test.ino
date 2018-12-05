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

// WAVE TIMING CONSTANTS
const unsigned int wd_1200us = (1000000/1200);
const unsigned int wd_2400us = (1000000/2400);
const float wd_adj=1.009;

void gen_pulse(unsigned int time_const, char pin_out)
{
  //digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(pin_out, HIGH);
  for(int i=0;i<10;i++)
    delayMicroseconds(0.5*wd_adj*time_const/10);
  //digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(pin_out, LOW);
  for(int i=0;i<10;i++)
    delayMicroseconds(0.5*wd_adj*time_const/10);
}

void setup() {
  // put your setup code here, to run once:
  //pinMode(LED_BUILTIN, OUTPUT);
  pinMode(7, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  gen_pulse(wd_1200us,7);
  gen_pulse(wd_2400us,7);
  gen_pulse(wd_2400us,7);
}

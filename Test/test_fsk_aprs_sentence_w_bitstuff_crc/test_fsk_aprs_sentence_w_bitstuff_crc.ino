#include <math.h>

#define WAV_PIN 7
#define _1200 1
#define _2400 0

// APRS DATA FIELD CONSTANTS
const unsigned char flag=0x7e;
const unsigned char dest_aprs[7]={('A'<<1),('P'<<1),('A'<<1),('R'<<1),('D'<<1),('1'<<1),('0'<<1)};
const unsigned char dest_beacon[7]={('B'<<1),('E'<<1),('A'<<1),('C'<<1),('O'<<1),('N'<<1),('0'<<1)};
const unsigned char mycall[7]={('Y'<<1),('D'<<1),('1'<<1),('S'<<1),('D'<<1),('L'<<1),((11+'0')<<1)};
const unsigned char digi[]={"WIDE2 2"};
const unsigned char ctrl_field=0x03;
const unsigned char pid=0xf0;
const unsigned char info_status='>';
const unsigned char status_str[]={"Hello World !"};

const char lipsum[]=
{
"Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum"
};

// WAVE TIMING CONSTANTS
const unsigned int wd_1200us = (1000000/1200);
const unsigned int wd_2400us = (1000000/2400);
const float wd_adj=1.009;

// GLOBAL VARIABLES
char nada=_1200;
int baudrate=1200;
char bit_stuff=0;
unsigned short crc;

// FUNCTION DECLARATIONS
void gen_pulse(unsigned int time_const, char pin_out);
void gen_tone(char note);
void ubah_nada(void);
void kirim_karakter(unsigned char input);
void kirim_aprs_sentence(void);
void hitung_crc(char in);
void kirim_crc(void);

// FUNCTIONS
void gen_pulse(unsigned int time_const, char pin_out)
{
  digitalWrite(pin_out, HIGH);
  for(int i=0;i<10;i++)
    delayMicroseconds(0.5*wd_adj*time_const/10);
  digitalWrite(pin_out, LOW);
  for(int i=0;i<10;i++)
    delayMicroseconds(0.5*wd_adj*time_const/10);
}

void gen_tone(char note)
{
  if(baudrate>1200)
    baudrate=1200;
    
  if(note==_1200)
  {
    for(int i=0;i<(1200/baudrate);i++)
      gen_pulse(wd_1200us,WAV_PIN);
  }
  else
  {
    for(int i=0;i<(2*1200/baudrate);i++)
      gen_pulse(wd_2400us,WAV_PIN);
  }
}

void ubah_nada(void)
{
  nada = ~nada;
  gen_tone(nada);
}

void hitung_crc(char in)
{
  static unsigned short xor_in;
  xor_in=crc^in;
  crc>>=1;
  if(xor_in & 0x01) crc^=0x8408;
}

void kirim_crc(void)
{
  static unsigned char crc_lo;
  static unsigned char crc_hi;

  crc_lo=crc^0xff;
  crc_hi=(crc>>8)^0xff;
  kirim_karakter(crc_lo);
  kirim_karakter(crc_hi);
}

void kirim_karakter(unsigned char input)
{
  char in_bit;
  for(int i=0;i<8;i++)
  {
    in_bit = (input>>i) & 0x01;

    if(input!=0x7e)
      hitung_crc(in_bit);

    if(!in_bit)
    {
      ubah_nada();
      bit_stuff=0;
    }
    else
    {
      gen_tone(nada);
      bit_stuff++;
      if(bit_stuff==5)
      {
        if(input==0x7e)
        {
          bit_stuff=0;
        }
        else
        {
          ubah_nada();
          bit_stuff=0;
        }
      }
    }
  }
}

void kirim_aprs_sentence(void)
{
  // kirim phasing symbols
  for(int i=0;i<120;i++)
    kirim_karakter(0x00);

  // krim flag symbols
  for(int i=0;i<10;i++)
    kirim_karakter(flag);

  // kirim dest address
  for(int i=0;i<7;i++)
    kirim_karakter(dest_aprs[i]);
    
  // kirim my callsign
  for(int i=0;i<7;i++)
    kirim_karakter(mycall[i]);

  // kirim digi path
  for(int i=0;i<6;i++)
    kirim_karakter(digi[i]<<1);
  kirim_karakter((digi[6]<<1)+1);

  // kirim control field & protocol id
  kirim_karakter(ctrl_field);
  kirim_karakter(pid);

  // kirim status info type
  kirim_karakter(info_status);

  // kirim status string
  for(int i=0;i<sizeof(status_str);i++)
    kirim_karakter(status_str[i]);

  // kirim crc16
  kirim_crc();

  // krim flag symbols
  for(int i=0;i<3;i++)
    kirim_karakter(flag);
}

// MAIN
void setup() {
  // put your setup code here, to run once:
  pinMode(WAV_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  kirim_aprs_sentence();
  digitalWrite(LED_BUILTIN, LOW);
  delay(5000);
}

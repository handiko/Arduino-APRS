#include <math.h>

unsigned short crc=0xffff;
unsigned char crc_lo;
unsigned char crc_hi;
unsigned char SendData[27] = 
{
  0x86, 0xA2, 0x40, 0x40, 0x40, 0x40, 0x60, 0xAE, 0x64, 0x8C, 0xA6,
  0x40, 0x40, 0x68, 0xA4, 0x8A, 0x98, 0x82, 0xB2, 0x40, 0x61, 0x03,
  0xF0, 0x54, 0x65, 0x73, 0x74
};

void hitung_crc(char in_crc)
{
  static unsigned short xor_in;
  xor_in = crc ^ in_crc;
  crc >>= 1;
  if(xor_in & 0x01) crc ^= 0x8408;
}

void send_character(unsigned char in_char)
{
  bool bit_buff;
  for(int i=0;i<8;i++)
  {
    bit_buff=(in_char >> i) & 0x01;
    hitung_crc(bit_buff);
    Serial.print(bit_buff);
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  for(char i=0;i<sizeof(SendData);i++)
    send_character(SendData[i]);
  Serial.println(" ");
  delay(10000);
}

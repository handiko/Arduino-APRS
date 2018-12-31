/*
 *  Copyright (C) 2018 - Handiko Gesang - www.github.com/handiko
 *  
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *  
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include <math.h>
#include <stdio.h>

// Defines the Square Wave Output Pin
#define OUT_PIN 2

#define _1200   1
#define _2400   0

#define _FLAG     0x7e
#define _CTRL_ID  0x03
#define _PID      0xf0
#define _DT       ','

bool nada = _2400;

/*
 * SQUARE WAVE SIGNAL GENERATION
 * 
 * baud_adj lets you to adjust or fine tune overall baud rate
 * by simultaneously adjust the 1200 Hz and 2400 Hz tone,
 * so that both tone would scales synchronously.
 * adj_1200 determined the 1200 hz tone adjustment.
 * tc1200 is the half of the 1200 Hz signal periods.
 * 
 *      -------------------------                           -------
 *     |                         |                         |
 *     |                         |                         |
 *     |                         |                         |
 * ----                           -------------------------
 * 
 *     |<------ tc1200 --------->|<------ tc1200 --------->|
 *     
 * adj_2400 determined the 2400 hz tone adjustment.
 * tc2400 is the half of the 2400 Hz signal periods.
 * 
 *      ------------              ------------              -------
 *     |            |            |            |            |
 *     |            |            |            |            |            
 *     |            |            |            |            |
 * ----              ------------              ------------
 * 
 *     |<--tc2400-->|<--tc2400-->|<--tc2400-->|<--tc2400-->|
 *     
 */
const float baud_adj = 0.975;
const float adj_1200 = 1.0 * baud_adj;
const float adj_2400 = 1.0 * baud_adj;
unsigned int tc1200 = (unsigned int)(0.5 * adj_1200 * 1000000.0 / 1200.0);
unsigned int tc2400 = (unsigned int)(0.5 * adj_2400 * 1000000.0 / 2400.0);

/*
 * This strings will be used to generate AFSK signals, over and over again.
 */
const char *mycall = "MYCALL";
char myssid = 1;

const char *dest = "ARPS";

const char *digi = "WIDE2";
char digissid = 2;

const char strings[860]={
"b0NWtAXLKj0Sn8WRsakzQS8JN25zAAf3md5ILaYty6jvZHrq1QU1CWfC6tKOMY7cFCopla9sn0b\
n26zcd9qRHFWflqMcmMwx9ZDmzxrs4cfjiMox4R0pNCB0fm26gDVcdMCZcVOnovLDWUlFHL0m2UL\
j3SVJonE4swIlemv2miVFJ3hjETh54cubpJhefhHtOGlwwtd64PigxsjzB3oXI6tJR3sCd84sheQ\
is2DrnBZPd4pYdZvv6nx01hDeQNiUYGilAHb7cdqlEIMwhHVaqIgn43JOwQzSMGOWvAbFdSxLyoU\
d8rYeyVWHxW3tyJS7wjWjsr1UV3RCkPBL4XhMpceV3z0zu6y9rQGWxBwVAbBliOo630lkdmwRkuM\
B0INNcS4CjELYzsVQnEnX5OMCryDdbFEGwCpDEiFPETlP4EeqsYI6ACIRsM9A8buf1eecrwBKgkT\
3Ty0mHlOjc4ibBiJCJB5vTzvEbQdfgsLGubfPL1Y8Vb5PAzwCGVotWxUPUPamGgBezXZ4JbOAbUf\
XGEM1ppuRtam8zk4ePExs1ccD4qumNt0pvfEWyCiIrVuLAK1TGoOG9rE0U0wCaLILlmLiTu1UtPM\
STm1sZzEAdunENMmMrHH4bO5W3dL36Njoq7fCVyFGiIurYBcmamYRWHFas3f6DCN7IpOiKo0PM1E\
If7eeVegEB4lQZ5EVSXJ4HpGodk4h903bu4KIfm2VilJUUtjiy9lMqTXGliafDss5zBGpL8S7yh1\
z2NdgD8TrRGXR4EJ9gSiJTCBiGoSe1uzoeqPNV1pMM7ld7bKbTriOlBNyTCm7lx7cM8J5IsO4ieg\
CSjG0OzwiQEhed7hvS2b78Qu"
};

unsigned int tx_delay = 5000;
unsigned int str_len = 400;

char bit_stuff = 0;
unsigned short crc=0xffff;

/*
 * 
 */
void set_nada_1200(void);
void set_nada_2400(void);
void set_nada(bool nada);

void calc_crc(bool in_bit);
void send_crc(void);
void send_header(void);
void send_payload(void);

void send_char_NRZI(unsigned char in_byte, bool enBitStuff);
void send_string_len(const char *in_string, int len);

void send_flag(unsigned char flag_len);

void set_io(void);
void print_code_version(void);
void print_debug(void);

/*
 * 
 */
void set_nada_1200(void)
{
  digitalWrite(OUT_PIN, HIGH);
  delayMicroseconds(tc1200);
  digitalWrite(OUT_PIN, LOW);
  delayMicroseconds(tc1200);
}

void set_nada_2400(void)
{
  digitalWrite(OUT_PIN, HIGH);
  delayMicroseconds(tc2400);
  digitalWrite(OUT_PIN, LOW);
  delayMicroseconds(tc2400);
  
  digitalWrite(OUT_PIN, HIGH);
  delayMicroseconds(tc2400);
  digitalWrite(OUT_PIN, LOW);
  delayMicroseconds(tc2400);
}

void set_nada(bool nada)
{
  if(nada)
    set_nada_1200();
  else
    set_nada_2400();
}

/*
 * This function will calculate CRC-16 CCITT for the FCS (Frame Check Sequence)
 * as required for the HDLC frame validity check.
 * 
 * Using 0x1021 as polynomial generator. The CRC registers are initialized with
 * 0xFFFF
 */
void calc_crc(bool in_bit)
{
  unsigned short xor_in;
  
  xor_in = crc ^ in_bit;
  crc >>= 1;

  if(xor_in & 0x01)
    crc ^= 0x8408;
}

void send_crc(void)
{
  unsigned char crc_lo = crc ^ 0xff;
  unsigned char crc_hi = (crc >> 8) ^ 0xff;

  send_char_NRZI(crc_lo, HIGH);
  send_char_NRZI(crc_hi, HIGH);
}

void send_header(void)
{
  char temp;

  /*
   * APRS AX.25 Header 
   * ........................................................
   * |   DEST   |  SOURCE  |   DIGI   | CTRL FLD |    PID   |
   * --------------------------------------------------------
   * |  7 bytes |  7 bytes |  7 bytes |   0x03   |   0xf0   |
   * --------------------------------------------------------
   * 
   * DEST   : 6 byte "callsign" + 1 byte ssid
   * SOURCE : 6 byte your callsign + 1 byte ssid
   * DIGI   : 6 byte "digi callsign" + 1 byte ssid
   * 
   * ALL DEST, SOURCE, & DIGI are left shifted 1 bit, ASCII format.
   * DIGI ssid is left shifted 1 bit + 1
   * 
   * CTRL FLD is 0x03 and not shifted.
   * PID is 0xf0 and not shifted.
   */

  /********* DEST ***********/
  temp = strlen(dest);
  for(int j=0; j<temp; j++)
    send_char_NRZI(dest[j] << 1, HIGH);
  if(temp < 6)
  {
    for(int j=0; j<(6 - temp); j++)
      send_char_NRZI(' ' << 1, HIGH);
  }
  send_char_NRZI('0' << 1, HIGH);

  
  /********* SOURCE *********/
  temp = strlen(mycall);
  for(int j=0; j<temp; j++)
    send_char_NRZI(mycall[j] << 1, HIGH);
  if(temp < 6)
  {
    for(int j=0; j<(6 - temp); j++)
      send_char_NRZI(' ' << 1, HIGH);
  }
  send_char_NRZI((myssid + '0') << 1, HIGH);

  
  /********* DIGI ***********/
  temp = strlen(digi);
  for(int j=0; j<temp; j++)
    send_char_NRZI(digi[j] << 1, HIGH);
  if(temp < 6)
  {
    for(int j=0; j<(6 - temp); j++)
      send_char_NRZI(' ' << 1, HIGH);
  }
  send_char_NRZI(((digissid + '0') << 1) + 1, HIGH);

  /***** CTRL FLD & PID *****/
  send_char_NRZI(_CTRL_ID, HIGH);
  send_char_NRZI(_PID, HIGH);
}

void send_payload(void)
{
  send_char_NRZI(_DT, HIGH);
  send_string_len(strings, str_len);
}

/*
 * This function will send one byte input and convert it
 * into AFSK signal one bit at a time LSB first.
 * 
 * The encode which used is NRZI (Non Return to Zero, Inverted)
 * bit 1 : transmitted as no change in tone
 * bit 0 : transmitted as change in tone
 */
void send_char_NRZI(unsigned char in_byte, bool enBitStuff)
{
  bool bits;
  
  for(int i = 0; i < 8; i++)
  {
    bits = in_byte & 0x01;

    calc_crc(bits);

    if(bits)
    {
      set_nada(nada);
      bit_stuff++;

      if((enBitStuff) && (bit_stuff == 5))
      {
        nada ^= 1;
        set_nada(nada);
        
        bit_stuff = 0;
      }
    }
    else
    {
      nada ^= 1;
      set_nada(nada);

      bit_stuff = 0;
    }

    in_byte >>= 1;
  }
}

void send_string_len(const char *in_string, int len)
{
  for(int j=0; j<len; j++)
  {
    send_char_NRZI(in_string[j + len], HIGH);
  }
}

void send_flag(unsigned char flag_len)
{
  for(int j=0; j<flag_len; j++)
    send_char_NRZI(_FLAG, LOW); 
}

/*
 * In this preliminary test, a packet is consists of FLAG(s) and PAYLOAD(s).
 * Standard APRS FLAG is 0x7e character sent over and over again as a packet
 * delimiter. In this example, 100 flags is used the preamble and 3 flags as
 * the postamble.
 */
void send_packet(void)
{
  print_debug();

  digitalWrite(LED_BUILTIN, 1);

  /*
   * AX25 FRAME
   * 
   * ........................................................
   * |  FLAG(s) |  HEADER  | PAYLOAD  | FCS(CRC) |  FLAG(s) |
   * --------------------------------------------------------
   * |  N bytes | 22 bytes |  N bytes | 2 bytes  |  N bytes |
   * --------------------------------------------------------
   * 
   * FLAG(s)  : 0x7e
   * HEADER   : see header
   * PAYLOAD  : 1 byte data type + N byte info
   * FCS      : 2 bytes calculated from HEADER + PAYLOAD
   */
  
  send_flag(100);
  crc = 0xffff;
  send_header();
  send_payload();
  send_crc();
  send_flag(3);

  digitalWrite(LED_BUILTIN, 0);
}

/*
 * Function to randomized the value of a variable with defined low and hi limit value.
 * Used to create random AFSK pulse length.
 */
void randomize(unsigned int &var, unsigned int low, unsigned int high)
{
  var = random(low, high);
}

/*
 * 
 */
void set_io(void)
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(OUT_PIN, OUTPUT);

  Serial.begin(115200);
}

void print_code_version(void)
{
  Serial.println(" ");
  Serial.print("Sketch:   ");   Serial.println(__FILE__);
  Serial.print("Uploaded: ");   Serial.println(__DATE__);
  Serial.println(" ");
  
  Serial.println("Random String Pulsed AFSK Generator - Started \n");
}

void print_debug(void)
{
  int temp;
  
  Serial.println("Sending flag : 100 characters");
  Serial.print("Sending payload : ");Serial.print(str_len);Serial.println(" characters");
  Serial.println("Sending flag : 3 characters \n");

  /*
   * PROTOCOL DEBUG
   * 
   * MYCALL-N>APRS,DIGIn-N:<PAYLOAD STRING> <CR><LF>
   */

  temp = strlen(mycall);
  for(int j=0; j<temp; j++)
    Serial.print(mycall[j]);
  Serial.print('-');
  Serial.print(myssid, DEC);
  Serial.print('>');

  temp = strlen(dest);
  for(int j=0; j<temp; j++)
    Serial.print(dest[j]);
  Serial.print(',');

  temp = strlen(digi);
  for(int j=0; j<temp; j++)
    Serial.print(digi[j]);
  Serial.print('-');
  Serial.print(digissid, DEC);
  Serial.print(':');

  Serial.print(_DT);

  for(int j=0; j<str_len; j++)
    Serial.print(strings[j + str_len]);
  
  Serial.println('\n');
}

/*
 * 
 */

void setup()
{
  set_io();
  print_code_version();
}

void loop()
{
  send_packet();
  
  delay(tx_delay);
  randomize(tx_delay, 10, 5000);
  randomize(str_len, 10, 420);
}

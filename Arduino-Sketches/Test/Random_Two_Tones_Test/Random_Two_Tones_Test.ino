/*
 *  Random Two Tones Test - Test code to make your Arduino UNO to outputs
 *  1200 Hz and 2400 Hz square wave signal switching from one into another
 *  randomly
 *  
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

// Defines the Square Wave Output Pin
#define OUT_PIN 2

#define _1200   1
#define _2400   0

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
 *     |<--tc1200-->|<--tc1200-->|<--tc1200-->|<--tc1200-->|
 *     
 */
const float baud_adj = 0.86;
const float adj_1200 = 1.0 * baud_adj;
const float adj_2400 = 1.0 * baud_adj;
unsigned int tc1200 = (unsigned int)(0.5 * adj_1200 * 1000000.0 / 1200.0);
unsigned int tc2400 = (unsigned int)(0.5 * adj_2400 * 1000000.0 / 2400.0);

/*
 * 
 */
void set_nada_1200(void);
void set_nada_2400(void);
void set_nada(bool nada);
void switch_random(void);

void set_io(void);
void print_code_version(void);

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
 * Make a random 1200baud AFSK Signal by randomly
 * alternates between 1200Hz and 2400Hz. Random numbers
 * between 1 and 50 are generated. Everytime the numbers
 * are divisible by two, generate 1200Hz tone and 2400hz
 * tone otherwise. Adjust the generated baudrate by
 * tuning the baud_adj variable and observe the generated
 * tone in either GRC or Audacity. Adjust baud_adj until
 * the frequencies are 1200hz and 2400Hz
 */
void switch_random(void)
{
  char i = random(1, 50);

  if((i % 2) == 0)
    set_nada(_1200);
  else
    set_nada(_2400);
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
  
  Serial.println("Random Two Tones Test 1200 Hz & 2400 Hz - Started");
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
  switch_random();
}

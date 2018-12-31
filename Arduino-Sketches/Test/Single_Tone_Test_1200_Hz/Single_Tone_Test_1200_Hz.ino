/*
 *  Single Tone Test 1200Hz - Test code to make your Arduino UNO to outputs
 *  1200 Hz square wave signal.
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

#define OUT_PIN 2

#define _1200   1

bool nada = _1200;
const float baud_adj = 0.97;
const float adj_1200 = 1.0 * baud_adj;
unsigned int tc1200 = (unsigned int)(0.5 * adj_1200 * 1000000.0 / 1200.0);

/*
 * 
 */
void set_nada_1200(void);
void set_nada(bool nada);

void set_io();
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

void set_nada(bool nada)
{
  if(nada)
    set_nada_1200();
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
  
  Serial.println("Single Tone Test 1200 Hz - Started");
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
  set_nada(_1200);
}

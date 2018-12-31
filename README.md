# Arduino-APRS
Create simple 1200baud APRS / AFSK modulator using Arduino UNO.
Examples and Test files are included.

## Requirements
* **Arduino UNO / Compatible Board**. Even works on ESP8266 / ESP12 board by changing the #define OUT_PIN 2 into the desired GPIO.
* **Arduino IDE**
* **GNU Radio Companion** (not a mandatory, but recommended for testing purposes). You can also use Audacity instead.
* **1 Resistor between 270 Ohms - 470 Ohms** (GPIO current limitter).
* **1 mini speaker** (8 Ohms - 32 Ohms and below 0.5 watt works well).
* **Jumper wires and bread board**

## Test
Here are some preliminary test to show you how to generate APRS (or AFSK in general) signal on Arduino UNO.

### Single Tone (1200 Hz / 2400 Hz) Test 

...

#include <math.h>

const char samplePoints=20;
int dacLookTable[samplePoints];
const float pi=3.141592;
const int dacBias=2048;
const int dacAmpl=4000;
char idx=1;

int calcDACLookTable(int point)
{
  dacLookTable[0] = (int)(dacBias + (0.5 * dacAmpl * sin(2*pi*point/(samplePoints+1))));
}

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);

  for(int i=0;i<samplePoints;i++)
    dacLookTable[i]=calcDACLookTable(i+1);
    
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_BUILTIN, HIGH);
  
  Serial.println(dacLookTable[idx-1]);
  idx++;
  if(idx==21)
    idx=1;

  delay(75);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
}

#include <SPI.h>

uint32_t startTime, elapsedTime, old_res;
#define SCALE_FACTOR 0.000152587890625

// D14,D15 = GND
// RANGE = GND to set the rage +/- 5V

//define pins
#define BUSY 2               // you can use any digital pin   
#define RESET 3              // you can use any digital pin  
#define START_CONVERSION 4   // you can use any digital pin    
#define CHIP_SELECT 5        // SPI CS     
#define D7_out 6             // SPI MISO there is no need to use MOSI port with the ad7606 
#define RD 7                 // SPI SCLK 
//#define RANGE 36           // you can use any digital pin -> to GND
#define TOTAL_RAW_BYTES 16
// OS0,OS1,0S2 -> GND
SPISettings _spiSettings;

int bytesToRead = TOTAL_RAW_BYTES;
byte raw[TOTAL_RAW_BYTES];
uint16_t parsed[8];

void setup() {
  // put your setup code here, to run once:
  pinMode(BUSY, INPUT);
  pinMode(RESET, OUTPUT);
  pinMode(START_CONVERSION, OUTPUT);
  pinMode(CHIP_SELECT, OUTPUT);
  pinMode(D7_out, OUTPUT);
  pinMode(RD, OUTPUT);
  digitalWrite(START_CONVERSION, HIGH);
  digitalWrite(CHIP_SELECT, HIGH);
  reset(RESET);

  Serial.begin(2000000);
  while(!Serial){} // wait for usb connection
  SPI.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  startTime = micros();  
  readData();
  elapsedTime = micros() - startTime; 
  if (elapsedTime != old_res) 
  {
    Serial.print("Conversion time:\t");
    Serial.print(elapsedTime);
    Serial.println(" microseconds");
    old_res = elapsedTime;
  }  
  parseRawBytes();

  Serial.print("Dati in arrivo:");
  for (int i = 0; i < 8; i++) {
    Serial.print((float)parsed[i] * SCALE_FACTOR, 5);
    Serial.print(",");
  }
 
  Serial.print("\r\n");
  delay(1000);
}

//-----------------------------------------
void parseRawBytes() {
  for(int i = 0; i<(sizeof(parsed) / sizeof(int));i++)
    {
      parsed[i] = (raw[i*2] << 8) + raw[(i*2)+1];
    }
}

//-----------------------------------------
/*reset signal*/
void reset(uint8_t port)
{
  digitalWrite(port, HIGH);
 // delayMicroseconds(1);
  digitalWrite(port, LOW);
//  delayMicroseconds(1);
}
//-----------------------------------------
void conversionPulse(uint8_t port)
{
  digitalWrite(port, LOW);
//  delayMicroseconds(1);
  digitalWrite(port, HIGH);  
}

//-----------------------------------------
/*
1- start conversion START_CONVERSION HIGH
2- wait until the busy is LOW again 
3- put the CS to LOW
4- Read a byte from the SPI 
*/

void readData()
{
  conversionPulse(START_CONVERSION);
  while (digitalRead(BUSY) == HIGH) {
  //  delayMicroseconds(1);
    Serial.println("Busy high");
  }
  SPI.beginTransaction(_spiSettings);
  digitalWrite(CHIP_SELECT, LOW);
  while (bytesToRead > 0) {
    raw[TOTAL_RAW_BYTES - bytesToRead] = SPI.transfer(0x00);
    bytesToRead--;
  }
  digitalWrite(CHIP_SELECT, HIGH);
  SPI.endTransaction();

  bytesToRead = TOTAL_RAW_BYTES;
}

//-----------------------------------------
/*
  0 +/-  5V
  1 +/- 10V
*/
/*void setRange(bool range)
{
  pinMode(RANGE, OUTPUT);
  digitalWrite(RANGE,range);
}*/

//-----------------------------------------
/*OS2 OS1 OS0
oversampling
000 No oversampling Maximum sampling rate is 200KSPS
001  2 times        100 KSPS
010  4 times        50 KSPS
011  8 times        25 KSPS
100 16 times        12.5 KSPS
101 32 times        6.25 KSPS
110 64 times        3.125 KSPS
*/
/*void setOversampling(uint8_t OS0,uint8_t OS1,uint8_t OS2){
  pinMode(_OS0,OUTPUT);
  pinMode(_OS1,OUTPUT);
  pinMode(_OS2,OUTPUT);
  digitalWrite(_OS0,bitRead(B001,OS0));
  digitalWrite(_OS1,bitRead(B010,OS1));
  digitalWrite(_OS2,bitRead(B100,OS2));
}*/
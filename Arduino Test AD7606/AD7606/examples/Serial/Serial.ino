#include "C:\Users\Giulia\Downloads\AD7606-main\AD7606-main\AD7606.cpp"

#define DB7 6
#define DB8 8
#define CVA_CVB 12
#define CS 5
#define RD 13
#define RESET 3
#define BUSY 2

AD7606_Serial AD(DB7, DB8, RD, CS, CVA_CVB, CVA_CVB, BUSY, RESET);

void setup()
{
	Serial.begin(115200);
}

void loop()
{
	int16_t Data[8];
	AD.read(Data);
  float convertedData[8];
	for (uint8_t i = 0; i < 8; i++)
	{
		//Serial.println(Data[i]);
    convertedData[i] = Data[i]*(5*2.5)/(32768*2);
	}

  //Serial.println("Dati convertiti: ");
  for(uint8_t i=0; i<8; i++){
    Serial.print(i+1);
    Serial.print(",");
    Serial.println(convertedData[i],4);
  }
  //Serial.println("Fine dati");
  delay(3000);
}

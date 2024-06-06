void setup() {
  // put your setup code here, to run once:
  Serial.begin(2000000);
  while(!Serial){}
}

void loop() {
  // put your main code here, to run repeatedly:
  //compute random floating point number
  float randomNumber = random(-200, 200) / 100.0;
  Serial.print(randomNumber);

  delay(2000); //wait for 2 seconds
}

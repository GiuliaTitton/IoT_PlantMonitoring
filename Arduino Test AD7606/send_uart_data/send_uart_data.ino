void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while(!Serial){}
}

void loop() {
  // put your main code here, to run repeatedly:
  //compute random floating point number
  int randomNumber = random(0, 100);
  Serial.print(randomNumber);

  delay(2000); //wait for 2 seconds
}

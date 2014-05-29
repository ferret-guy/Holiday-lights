unsigned long StartTime = millis();

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly: 
 int StartTime = micros();
  for( int p=0; p<9999; p++ ) {

  }
int CurrentTime = micros();
int ElapsedTime = CurrentTime - StartTime;
Serial.println(ElapsedTime);
Serial.write("\n");
}

#include <SoftwareSerial.h>

int rxPin = 2; //GSM module RX pin to arduino 2
int txPin = 3; //GSM module TX pin to arduino 3
SoftwareSerial sim800(rxPin,txPin);

int trigPin = 8;
int echoPin = 9;

void setup() {
  //pinMode(ledPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.begin(9600); //initialize serial

  sim800.begin(9600);
  Serial.println("SIM800L software serial initialize");

  sim800.println("AT");
  delay(1000);
}

void loop() {
    ultrasonic();
}

void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    sim800.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(sim800.available()) 
  {
    Serial.write(sim800.read());//Forward what Software Serial received to Serial Port
  }
}

void ultrasonic(){
  long time_duration, distance_in_cm;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  time_duration = pulseIn(echoPin, HIGH);
  distance_in_cm = time_duration / 29 / 2;

  Serial.print(distance_in_cm);
  Serial.println(" cm");

  //ranging distance - 2cm - 400 cm
  //if distance is equal or less than 20 cm
  if(distance_in_cm <= 20){
  sim800.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();
  sim800.println("AT+CMGS=\"+639303842799\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
  updateSerial();
  sim800.print("Last Minute Engineers | lastminuteengineers.com"); //text content
  updateSerial();
  sim800.write(26);
  }
  delay(500);
}

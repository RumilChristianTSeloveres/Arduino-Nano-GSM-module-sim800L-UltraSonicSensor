#include <SoftwareSerial.h>

int rxPin = 2;
int txPin = 3;
SoftwareSerial sim800L(rxPin, txPin);

char receiveSMS;
short stat = -1; //means wala pa niya mareceived ang text nga ge define ani
String dataSMS;

void setup() {
  // put your setup code here, to run once:
  // initialized serial gamiton for send text to GSM module
  Serial.begin(9600);

  // initialized the GSM module baud rate at 9600
  sim800L.begin(9600);
  
  // print in the serial monitor to check if it okay
  Serial.println("SIM800L software serial initializing...");

  receive();
}

void loop() {
  // put your main code here, to run repeatedly:
  String RSMS;
  
  while(sim800L.available() > 0){
    receiveSMS = sim800L.read();
    Serial.println(receiveSMS); // print to serial monitor ug unsay ge send text sa CP
    RSMS.concat(receiveSMS); // ge concatenation ang char to string

    stat = RSMS.indexOf("Hi"); // mao ni pangitaon nga text sa textmessage para mo respond ang GSM module
  }

  if(stat != -1) {
    Serial.println("FOUND!");

    dataSMS = "Hello World! I am GSM module 800L";
    
    senddata(); // GSM -> CP

    receive();

    stat = -1;
  }
}

// Update Serial function
void updateSerial() {
  delay(500);
  while (Serial.available())
  {
    sim800L.write(Serial.read());//Forward what Serial received to Software Serial Port
  }

  while(sim800L.available()) 
  {  
    Serial.write(sim800L.read());//Forward what Software Serial received to Serial Port
  }
}

// function for receive a message form CP
void receive() { // CP -> GSM
  sim800L.println("AT"); // AT command means for receiving text
  updateSerial(); // always update the serial
  sim800L.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();
  sim800L.print("AT+CNMI=2,2,0,0,0");
  updateSerial();
}

// function for GSM send data to CP
void senddata(){ //GSM -> CP
  Serial.println("Sending data...");
  sim800L.println("AT+CMFG=1\r");
  delay(100);
  sim800L.print("AT+CMGS=\"+639xxxxxxxxx\"\r"); // change the +639xxxxxxxxx into your CP number
  delay(500);
  sim800L.print(dataSMS);
  delay(500);
  sim800L.print((char)26);
  delay(500);
  sim800L.println();
  Serial.println("data sent...");
  delay(500);
}

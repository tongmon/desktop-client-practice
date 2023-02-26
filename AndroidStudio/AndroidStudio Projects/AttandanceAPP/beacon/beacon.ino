#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); // RX, TX

void setup()  
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  Serial.println("Goodnight moon!");

  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
}

void loop() // run over and over
{
  // Receive from bluetooth and send it to PC
  if (mySerial.available())
    Serial.write(mySerial.read());

  // Receive from PC and send it to bluetooth
  if (Serial.available())
    mySerial.write(Serial.read());
}

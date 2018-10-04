// Sketch for reading from a Maxbotix 7092 ultrasonic rangefinder
//   and writing to an attached SD card

// SD card libraries and variables
#include <SPI.h>
#include <SD.h>

Sd2Card card;
SdVolume volume;
SdFile root;
String filename = "log2.csv";

File myFile;

// Sensor variables
int sensorPin = A1;
int controlPin = 1;
int sensorValue = 0;
float distance = 0.0;
float cmToFt = 0.0328084;
unsigned long time;

void setup() {
  Serial.print("\nInitializing SD card...");

  // Using pin 7 for CS
  if (!SD.begin(7)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");

  myFile = SD.open(filename, FILE_WRITE);
  if (myFile) {
    myFile.println("millisecs_since_start,distance_ft");
    myFile.close();
  } else {
    Serial.println("Error opening file on SD card.");
  }
  
}

void loop() {
  digitalWrite(controlPin, HIGH);
  delay(10);
  sensorValue = analogRead(sensorPin);
  distance = cmToFt * sensorValue;
  digitalWrite(controlPin, LOW);

  myFile = SD.open(filename, FILE_WRITE);
  if (myFile) {
    time = millis();
    myFile.print(time);
    myFile.print(",");
    myFile.print(distance);
    myFile.print("\r\n");
    myFile.close();
  } else {
    Serial.println("Error opening file on SD card.");
  }
  
  Serial.print("Voltage is: ");
  Serial.print(sensorValue * (3.3/1024));
  Serial.print("; distance is: ");
  Serial.print(sensorValue);
  Serial.print(" cm, or ");
  Serial.print(distance);
  Serial.print(" feet.\n");
  delay(2000);
}

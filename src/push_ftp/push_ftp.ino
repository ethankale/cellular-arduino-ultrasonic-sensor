/*
   SerialGSMPassthrough sketch
   This sketch allows you to send AT commands from the USB CDC serial port
   of the MKR GSM 1400 board to the onboard ublox SARA-U201 celluar module.
   For a list of supported AT commands see:
   https://www.u-blox.com/sites/default/files/u-blox-CEL_ATCommands_%28UBX-13002752%29.pdf
   Circuit:
   - MKR GSM 1400 board
   - Antenna
   - 1500 mAh or higher lipo battery connected
   - SIM card
   Make sure the Serial Monitor's line ending is set to "Both NL and CR"
   create 11 December 2017
   Sandeep Mistry
*/

#include "ftp_secrets.h"

// baud rate used for both Serial ports
unsigned long baud = 115200;

// network vars
String apn = "soracom.io";
String usr = "sora";
String pwd = "sora";

// ftp vars
String ftp_url = SECRET_FTP_URL;
String ftp_dir = SECRET_FTP_DIR;
String ftp_usr = SECRET_FTP_USR;
String ftp_pwd = SECRET_FTP_PWD;

// generic vars
String cmd = "";
String result = "";

void setup() {
  // reset the ublox module
  pinMode(GSM_RESETN, OUTPUT);
  digitalWrite(GSM_RESETN, HIGH);
  delay(100);
  digitalWrite(GSM_RESETN, LOW);

  Serial.begin(baud);
  while (!Serial) {
   ; // wait for serial port to connect. Needed for native USB port only
  }
  SerialGSM.begin(baud);

  cmd = "AT";
  result = sendModemCmd(cmd);

  cmd = "AT+CREG?";
  result = sendModemCmd(cmd);
  
  cmd = "AT+UPSD=0,1,\"" + apn + "\"";
  result = sendModemCmd(cmd);
    
  cmd = "AT+UPSD=0,2,\"" + usr + "\"";
  result = sendModemCmd(cmd);
    
  cmd = "AT+UPSD=0,3,\"" + pwd + "\"";
  result = sendModemCmd(cmd);
      
  cmd = "AT+UPSDA=2,1";
  result = sendModemCmd(cmd);
        
  cmd = "AT+UPSDA=2,3";
  result = sendModemCmd(cmd);

  delay(2000);

  cmd = "AT+UPSDA=2,4";
  result = sendModemCmd(cmd);

  
}

void loop() {
  ;
}

String sendModemCmd(String cmd){
  String response;
  SerialGSM.println(cmd);
  while (!SerialGSM.available()) { ; }
  delay(10);
  response = SerialGSM.readString();
  Serial.println(response);
  return response;
}

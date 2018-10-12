/*
  Use AT commands to connect to the cell network and to FTP stuff.
*/

// include the SD library:
#include <SPI.h>
#include <SD.h>

#include "ftp_secrets.h"

String sendModemCmd(String cmd, int ms_delay=10, int is_verbose=1);

// baud rate used for both Serial ports
unsigned long baud = 115200;

// network vars
String apn = "soracom.io";
String usr = "sora";
String pwd = "sora";

// create your ftp_secrets.h file in this directory, and #define the following params.
String ftp_url = SECRET_FTP_URL;
String ftp_dir = SECRET_FTP_DIR;
String ftp_usr = SECRET_FTP_USR;
String ftp_pwd = SECRET_FTP_PWD;

// sd vars
Sd2Card card;
SdVolume volume;
SdFile root;
int file_size_sd;
String filename = "LOG.TXT";

File myFile;


// generic vars
String cmd = "";
String result = "";
String substr = "";
String connection = "";
int signal_strength = 99;
int i = 0;

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

  // write file from SD card to modem
  if (!SD.begin(7)) {
    Serial.println("SD initialization failed!");
    while (1);
  }
  Serial.println("SD initialization success.");
  
  myFile = SD.open(filename, FILE_READ);

  if (myFile) {
     int i = 0;
     file_size_sd = myFile.size();
     Serial.println("The file to be transfered is " + String(file_size_sd) + " bytes.");

     sendModemCmd("AT+ULSTFILE=");
     sendModemCmd("AT+ULSTFILE=1");
     sendModemCmd("AT+UDELFILE=\"" + filename + "\"");

     Serial.println("Copy file data");   
     result = sendModemCmd("AT+UDWNFILE=\"" + filename + "\"," + file_size_sd);
     
     while (myFile.available()>0) {
        SerialGSM.write(myFile.read());
     }
     
     delay(1000);
     Serial.println("Done Writing");
  }else {
    Serial.println("Couldn't open " + filename + " for copying.");
  }

  myFile.close();
  
  // connect to the network
  cmd = "AT+CREG?";
  result = sendModemCmd(cmd);

  connection = connectGSM(apn, usr, pwd, 15);
  Serial.println(connection);

  // Unconvinced that this delay is necessary
  delay(2000);

  // What's the time?
  sendModemCmd("AT+CCLK?");

  connection = connectFTP(ftp_url, ftp_usr, ftp_pwd, 15);
  Serial.println(connection);
  
  // Configure FTP
  //cmd = "AT+UFTP=1,\"" + ftp_url + "\"";
  //result = sendModemCmd(cmd);

  //cmd = "AT+UFTP=2,\"" + ftp_usr + "\"";
  //result = sendModemCmd(cmd);

  //cmd = "AT+UFTP=3,\"" + ftp_pwd + "\"";
  //result = sendModemCmd(cmd);

  //cmd = "AT+UFTP=6,1";
  //result = sendModemCmd(cmd);
  
  //cmd = "AT+UFTP=5,30";
  //result = sendModemCmd(cmd);

  // Connect to FTP and manipulate files
  //cmd = "AT+UFTPC=1";
  //SerialGSM.println(cmd);
  //i = 0;
  //while (i < 15) {
  //  delay(500);
  //  String response = SerialGSM.readString();
  //  response.trim();
  //  if (response.length() > 0) {
  //    Serial.println(response);
  //    if (response.substring(0,8) == "+UUFTPCR") { break; }
  //  }
  //  i +=1;
  //  Serial.println(i);
  //}

  //cmd = "AT+UFTPC=14,\"" + ftp_dir + "\"";
  //result = sendModemCmd(cmd);

  cmd = "AT+UFTPC=8,\"" + ftp_dir + "\"";
  result = sendModemCmd(cmd);

  cmd = "AT+UFTPC=5,\"" + filename + "\",\"" + ftp_dir + "/" + filename + "\"";
  result = sendModemCmd(cmd, 5000);
  
  cmd= "AT+UFTPER";
  result = sendModemCmd(cmd);

  //cmd = "AT+UFTPC=2,\"/waterlevel/test/test.log\"";
  //result = sendModemCmd(cmd);

  //cmd= "AT+UFTPER";
  //result = sendModemCmd(cmd);
  
  // disconnect from FTP
  cmd = "AT+UFTPC=0";
  result = sendModemCmd(cmd);


  cmd = "AT+UPSDA=2,4";
  result = sendModemCmd(cmd);

}

void loop() {
  ;
}

String connectGSM(String apn, String usr, String pwd, int timeout_seconds){
  SerialGSM.println("AT+UPSD=0,1,\"" + apn + "\"");
  SerialGSM.println("AT+UPSD=0,2,\"" + usr + "\"");
  SerialGSM.println("AT+UPSD=0,3,\"" + pwd + "\"");
  SerialGSM.println("AT+UPSDA=2,1");

  delay(10);

  // Very important; have to empty the SerialGSM buffer before
  //  the loop listening for the ERROR response.
  String result = SerialGSM.readString();
  //Serial.println(result);
  Serial.print("Connecting to GSM...");
  i = 0;
  String connection = "ERROR";
  while (connection == "ERROR") {
    SerialGSM.println("AT+UPSDA=2,3");
    while (!SerialGSM.available()) { ; }
    delay(10);
    result = SerialGSM.readString();
  
    connection = result.substring(12);
    connection.trim();
    //Serial.println(connection);
    delay(500);
    i += 1;
    if (i > (timeout_seconds*2)) {
      //Serial.println("Could not connect in the time given (" + String(timeout_seconds) + ").");
      return "Could not connect in the time given (" + String(timeout_seconds) + " seconds).";
    }
    Serial.print(".");
  }
  return "Connection established!";
}

String connectFTP(String url, String usr, String pwd, int timeout_seconds){
  SerialGSM.println("AT+UFTP=1,\"" + url + "\"");
  delay(10);
  SerialGSM.println("AT+UFTP=2,\"" + usr + "\"");
  delay(10);
  SerialGSM.println("AT+UFTP=3,\"" + pwd + "\"");
  delay(10);
  SerialGSM.println("AT+UFTP=6,1");
  delay(10);
  SerialGSM.println("AT+UFTP=5,30");
  delay(10);
  SerialGSM.println("AT+UFTPC=1");
  delay(10);
  // Very important; have to empty the SerialGSM buffer before
  //  the loop.
  String result = SerialGSM.readString();
  //Serial.println(result);
  
  Serial.print("Connecting...");
  
  i = 0;
  while (i < (timeout_seconds*2)) {
    delay(500);
    String response = SerialGSM.readString();
    response.trim();
    if (response.length() > 0) {
      //Serial.println(response);
      if (response.substring(0,8) == "+UUFTPCR") { 
        return "Connected to FTP Server!";
      }
    }
    i +=1;
    Serial.print(".");
  }
  return "Could not connect to FTP server in " + String(timeout_seconds) + " seconds.";  
}

String sendModemCmd(String cmd, int ms_delay, int is_verbose){
  String response;
  SerialGSM.println(cmd);
  while (!SerialGSM.available()) { ; }
  delay(ms_delay);
  response = SerialGSM.readString();
  if (is_verbose) { Serial.println(response); }
  return response;
}

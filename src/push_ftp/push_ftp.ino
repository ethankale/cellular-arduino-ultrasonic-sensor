/*
  Use AT commands to connect to the cell network and to FTP stuff.
*/

#include "ftp_secrets.h"

String sendModemCmd(String cmd, int ms_delay=10);

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

// generic vars
String cmd = "";
String result = "";
String substr = "";
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

  cmd = "AT";
  result = sendModemCmd(cmd);

  cmd = "AT+CREG?";
  result = sendModemCmd(cmd);

  // wait to try & connect until there's a signal.
  while (signal_strength == 99) {
    cmd = "AT+CSQ";  
    result = sendModemCmd(cmd);
  
    substr = result.substring(result.indexOf(',')-2,result.indexOf(','));
    signal_strength = substr.toInt();
    delay(1000);
  }
  
  Serial.println(signal_strength);

  
  cmd = "AT+UPSD=0,1,\"" + apn + "\"";
  result = sendModemCmd(cmd);
    
  cmd = "AT+UPSD=0,2,\"" + usr + "\"";
  result = sendModemCmd(cmd);
    
  cmd = "AT+UPSD=0,3,\"" + pwd + "\"";
  result = sendModemCmd(cmd);
      
  cmd = "AT+UPSDA=2,1";
  result = sendModemCmd(cmd);

  // Make the connection
  cmd = "AT+UPSDA=2,3";
  result = sendModemCmd(cmd);

  delay(2000);

  // Configure FTP
  cmd = "AT+UFTP=1,\"" + ftp_url + "\"";
  result = sendModemCmd(cmd);

  cmd = "AT+UFTP=2,\"" + ftp_usr + "\"";
  result = sendModemCmd(cmd);

  cmd = "AT+UFTP=3,\"" + ftp_pwd + "\"";
  result = sendModemCmd(cmd);

  cmd = "AT+UFTP=6,1";
  result = sendModemCmd(cmd);
  
  cmd = "AT+UFTP=5,30";
  result = sendModemCmd(cmd);

  // Connect to FTP and manipulate files
  cmd = "AT+UFTPC=1";
  SerialGSM.println(cmd);
  i = 0;
  while (i < 30) {
    delay(500);
    String response = SerialGSM.readString();
    response.trim();
    if (response.length() > 0) {
      Serial.println(response);
      if (response.substring(0,8) == "+UUFTPCR") { break; }
    }
    i +=1;
    Serial.println(i);
  }

  cmd = "AT+UFTPC=14,\"" + ftp_dir + "\"";
  result = sendModemCmd(cmd);

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

String sendModemCmd(String cmd, int ms_delay){
  String response;
  SerialGSM.println(cmd);
  while (!SerialGSM.available()) { ; }
  delay(ms_delay);
  response = SerialGSM.readString();
  Serial.println(response);
  return response;
}

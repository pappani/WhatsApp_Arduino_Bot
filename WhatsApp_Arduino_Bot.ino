//Control remote WhatsApp bot via GPRS Module
//Pappani Federico - federicopappani.it

#include <NeoSWSerial.h>
NeoSWSerial gprs(10, 11);
String ipAddress = ""; //your IP address
String APN = "TM";
int resetPin = 2;
int txPin = 11;
int rxPin = 10;
boolean gprsConnectionSetup = false;
String response = "";
static long maxResponseTime = 30000;
unsigned long lastTime;

//The frequency of http requests (seconds)
int refreshRate = 15; //SECONDS
//variables for a well-scheduled loop - in which the sendData() gets called every 15 secs (refresh rate)
unsigned long last;
unsigned long current;
unsigned long elapsed;

//if there is an error in sendLocation() function after the GPRS Connection is setup - and the number of errors exceeds 3 - the system reboots. (with the help of the reboot pin) 
int maxNumberOfErrors = 3;
boolean reboot = false;
int errorsEncountered = 0; //number of errors encountered after gprsConnection is set up - if exceeds the max number of errors the system reboots


//if any error occurs with the gsm module or gps module, the corresponding LED will light up - until they don't get resolved
int sim800Pin = 2; //error pin
int gpsPin = 6; //error pin



//a helper function which converts a float to a string with a given precision
String floatToString(float x, byte precision = 2) {
  char tmp[50];
  dtostrf(x, 0, precision, tmp);
  return String(tmp);
}

void setupGPRSConnection(){
  gprs.write("AT\n");
  Serial.println(".AT");
  waitUntilResponse("OK");
  gprs.write("AT+CSQ\n");
  Serial.println(".AT+CSQ");
  waitUntilResponse("OK");
  gprs.write("AT+SAPBR=0,1\n");
  Serial.println(".AT+SAPBR=0,1");
  waitUntilResponse("OK");
  gprs.write("AT+CREG=?\n");
  Serial.println(".AT+CREG=?");
  waitUntilResponse("OK");
  gprs.write("AT+SAPBR=3,1,\"Contype\",\"GPRS\"\n");
  Serial.println(".AT+SAPBR=3,1,Contype...");
  waitUntilResponse("OK");
  gprs.write("AT+SAPBR=3,1,\"APN\",\"TM\"\n");
  Serial.println(".AT+SAPBR=3,1,APN...");
  waitUntilResponse("OK");
  gprs.write("AT+SAPBR=1,1\n");
  Serial.println(".AT+SAPBR=1,1");
  waitUntilResponse("OK");
  gprs.write("AT+HTTPINIT\n");
  Serial.println(".AT+HTTPINIT");
  waitUntilResponse("OK");
  gprs.write("AT+HTTPPARA=\"CID\",1\n");
  Serial.println(".AT+HTTPARA=CID...");
  waitUntilResponse("OK");
  gprs.write("AT+HTTPPARA=\"URL\",\"YOURURL/begin"\n");
  Serial.println(".AT+HTTPPARA=URL...");
  waitUntilResponse("OK");
  gprs.write("AT+HTTPSSL=0\n");
  Serial.println(".AT+HTTPSSL=0");
  waitUntilResponse("OK");
  gprs.write("AT+HTTPACTION=0\n");
  Serial.println(".AT+HTTPACTION=0");
  waitUntilResponse("OK");
  delay(50000);
  gprs.write("AT+HTTPREAD\n");
  Serial.println(".AT+HTTPREAD");
  waitUntilResponse("OK");
  gprsConnectionSetup = true;
  
}

void readResponse(){
  response = "";
  while(response.length() <= 0 || !response.endsWith("\n"))
  {
    tryToRead();
    if(millis() - lastTime > maxResponseTime)
    {
      return;
    }
  }
}

void tryToRead(){ 
  while(gprs.available()){
    char c = gprs.read();  //gets one byte from serial buffer
    response += c; //makes the string readString
  }
}

void waitUntilResponse(String resp){ 
  lastTime = millis();
  response="";
  String totalResponse = "";
  while(response.indexOf(resp) < 0 && millis() - lastTime < maxResponseTime)
  { 
    readResponse();
    totalResponse = totalResponse + response;
    Serial.println(response);
  }
  
  if(totalResponse.length() <= 0)
  { 
    Serial.println("NO RESPONSE");
    digitalWrite(sim800Pin, HIGH);
    if (gprsConnectionSetup == true){
      Serial.println("error");
      errorsEncountered++;
    }
  }
  else if (response.indexOf(resp) < 0)
  { 
    if (gprsConnectionSetup == true){
      Serial.println("error");
      errorsEncountered++;
    }
    Serial.println("UNEXPECTED RESPONSE");
    Serial.println(totalResponse);
    digitalWrite(sim800Pin, HIGH);
  }else{
    Serial.println("SUCCESSFUL");
    digitalWrite(sim800Pin, LOW);
    errorsEncountered = 0;
  }

  //if there are more errors or equal than previously set ==> reboot!
  if (errorsEncountered>= maxNumberOfErrors){
    reboot = true;
  }
}
void setup() {
    digitalWrite(resetPin, 1);
    delay(200);
    pinMode(resetPin, OUTPUT);
    Serial.begin(9600);
    gprs.begin(9600);
    gprs.write(27);
    Serial.println("Beginning...");
    delay(5000);
    Serial.println("SIM800L should have booted up");
    gprs.listen();
    setupGPRSConnection();
    Serial.println("Connection set up");
    


}

void loop() {
  delay(1000);
  gprs.listen();
 //The line below sets the URL we want to connect to
 gprs.println("AT+HTTPPARA=\"URL\", \"http://" + ipAddress +  "/begin");
 waitUntilResponse("OK");
 //GO
 gprs.println("AT+HTTPACTION=0");
 waitUntilResponse("200");
 Serial.println("Message sent");
delay(30000);
}

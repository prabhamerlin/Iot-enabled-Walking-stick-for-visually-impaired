
#include <SoftwareSerial.h>   
SoftwareSerial espSerial(2, 3);   //Pin 2 and 3 act as RX and TX. Connect them to TX and RX of ESP8266      
#define DEBUG true
String mySSID = "huwaei nova 3e";    
String myPWD = "prabhaqwerty";
String myAPI = "________";  // fill with ThingSpeak write API key
String myHOST = "api.thingspeak.com";
String myPORT = "80";
String myFIELD = "field1"; 
int sendVal;
//-----------------------------------------//
#define IN4  8
const int trigPin = 5;
const int echoPin = 6;

long duration;
int distance = 0;

void setup()
{
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(IN4, OUTPUT);
  Serial.begin(9600); // Starts the serial communication
  //-----------------------------------------//
  Serial.begin(9600);
  espSerial.begin(115200);
  
  espData("AT+RST", 1000, DEBUG);                      //Reset the ESP8266 module
  espData("AT+CWMODE=1", 1000, DEBUG);                 //Set the ESP mode as station mode
  espData("AT+CWJAP=\""+ mySSID +"\",\""+ myPWD +"\"", 1000, DEBUG);   //Connect to WiFi network
  
  delay(1000);
  
}

  void loop()
  {
    
    //----------------------------------------//
      
     digitalWrite(trigPin,LOW);
     delayMicroseconds(2);
     digitalWrite(trigPin,HIGH);
     delayMicroseconds(10);  //transmit for 10us
     digitalWrite(trigPin,LOW);
     duration = pulseIn(echoPin,HIGH); //in micro seconds
     distance = (0.034 * duration)/2;  //speed of a sound wave is 340m/s
                                     //speed of sound in cm/us is 0.034cm/us

    Serial.print("Distance: ");
    Serial.println(distance); //distance in cm
    
    if ((distance <20)&&(distance > 0))
    {
        digitalWrite(IN4, HIGH);   // turn the Buzzer on (HIGH is the voltage level)
         delay(1000);  
        digitalWrite(IN4, LOW);   // turn the buzzer on (HIGH is the voltage level)
       
    }
    else if ((distance > 21)&&(distance < 30))
    {
      digitalWrite(IN4, HIGH);   // buzzer pattern 1
      delay(100);  
      digitalWrite(IN4, LOW);   
      delay(5);// wait for a set time
       digitalWrite(IN4, HIGH);  
      delay(100);
      digitalWrite(IN4, LOW);   
      delay(5);// wait for a set time
    }
    else if ((distance > 31)&&(distance < 50))
    {
      digitalWrite(IN4, HIGH);   //buzzer pattern 2
      delay(100);  
      digitalWrite(IN4, LOW);   
      delay(5);
      digitalWrite(IN4, HIGH);  
      delay(100);  
      digitalWrite(IN4, LOW);  
      delay(5);
      digitalWrite(IN4, HIGH);  
      delay(100);  
      digitalWrite(IN4, LOW);   
      delay(5);
      digitalWrite(IN4, HIGH);   
      delay(100);  
      digitalWrite(IN4, LOW);  
      delay(5);
      digitalWrite(IN4, HIGH);  
      delay(100);  
      digitalWrite(IN4, LOW);   
      delay(5);
    }
    else if (distance > 50)
    {
      digitalWrite(IN4, LOW);
    }
    //delayMicroseconds(10);
                              
    //----------------------------------------//
    
    sendVal = distance;
    String sendData = "GET /update?api_key="+ myAPI +"&"+ myFIELD +"="+String(sendVal);
    espData("AT+CIPMUX=1", 1000, DEBUG);       //Allow multiple TCP connections
    espData("AT+CIPSTART=0,\"TCP\",\""+ myHOST +"\","+ myPORT, 1000, DEBUG);
    espData("AT+CIPSEND=0," +String(sendData.length()+4),1000,DEBUG);  
    espSerial.find(">"); 
    espSerial.println(sendData);
    Serial.print("Value to be sent: ");
    Serial.println(sendVal);
     delay(1000);
    espData("AT+CIPCLOSE=0",1000,DEBUG);
//    delay(5000);
 
  }

  String espData(String command, const int timeout, boolean debug)
{
  Serial.print("AT Command ==> ");
  Serial.print(command);
  Serial.println("     ");
  
  String response = "";
  espSerial.println(command);
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (espSerial.available())
    {
      char c = espSerial.read();
      response += c;
    }
  }
  if (debug)
  {
    //Serial.print(response);
  }
  return response;
}
  

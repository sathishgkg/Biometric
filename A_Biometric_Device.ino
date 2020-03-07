/*************Header File Deceleration ****************/
#include <SD.h>                          /* SD Card Library */
#include <SPI.h>                         /* SPI Library   */
#include <Wire.h>                        /* Wire Library   */
#include <EEPROM.h>                      /* Eeprom Library */
#include <RtcDS1307.h>                   /* DS1307 RTC Library */      
#include <WiFiClient.h>                  /* Esp8266 Client Library */
#include <ESP8266WiFi.h>                 /* Esp8266 WiFi Library */
#include "FPS_GT511C3.h"                 /* GT511C3 finger print library*/
#include <ArduinoJson.h>                 /* Json Library */
#include <SoftwareSerial.h>              /* Software Serial Library */
#include <ESP8266WebServer.h>            /* Esp8266 WebServer Library */
#include <ESP8266HTTPClient.h>           /* Esp8266 Http_Client Library */
#include <ESP8266httpUpdate.h>           /* ESP8266 Http_Update Library */
//#include <Adafruit_Fingerprint.h>        /* R307 Finger Print Library */

//#if defined(ESP8266)                     
//#include <pgmspace.h>                    /* This tells compiler to put information to flash insted of sram */
//#else
//#include <avr/pgmspace.h>
//#endif

FPS_GT511C3 fps;
#define Debugs            0
#define Server_Port_No    80

/******* Esp8266 Pin Mapping ********/
#define Rtc_Scl                    D4
#define Rtc_Sck                    D1
#define Buzzer_Gpio                D0
#define Red_Led_Gpio               D10
#define Green_Led_Gpio             D9
#define SoftwareSerial_Receiver    D2
#define SoftwareSerial_Transmitter D3

#define Attendance_Data      1
#define Enroll_Data          2
#define Server_Post          3
#define countof(a) (sizeof(a) / sizeof(a[0]))

/******** Biometric StateMachine ********/
enum BioMetric_StateMachine 
{
  Enroll_Finger = 1,
  Delete_Finger,
  Read_Finger,
  Delete_Finger_DB
};




RtcDS1307<TwoWire> Rtc(Wire);            /* Construct a Rtc object using the provided WIRE method */

//SoftwareSerial FingeSensor_Serial(SoftwareSerial_Receiver, SoftwareSerial_Transmitter);         /* Software Serial for fingerprint sensor, Pin no - D2 & D3, D2=receiver(rx), D3=Transmitter(tx),  
//                                                                                         FingerPrint Senor Rx to esp8266 Tx, FingerPrint Senor Tx to esp8266 Rx */  
//Adafruit_Fingerprint finger = Adafruit_Fingerprint(&FingeSensor_Serial);   /* Construct a finger object using the provided Serial method */

ESP8266WebServer server(Server_Port_No);             /* Config the esp8266 server port Http server use port no 80 */



/****SD Card object initilazion****/
File myFile;
File myFile1;
File myFile2;
File myFile3;

/******* Location Address ********/
uint8_t Five_Minutes = 5;
//uint8_t URL_Length  = 105;
uint8_t SSID_Length = 100;
uint8_t OAT_Update_Time   = 23;
uint8_t Wifissid_Address  = 0;
uint8_t Wifipass_Address  = 15;
//uint8_t ServerUrl_Address = 50;
uint8_t SsidLength_Address = 100;
uint8_t PassLength_Address = 102;
uint8_t BioMetric_SWversion = 0;
uint8_t Login_Name = 50;
uint8_t Login_Pass = 70;
uint8_t Login_Name_length =150;
uint8_t Login_Pass_length =170;

/******* String Variables ********/
String User_Data[500];
String JsonData     ="";
String Fetch_Ids    ="";
String Essid        ="";                  
String Epass        ="";                 
String sssid        ="";                 
String passs        ="";              
String Date         ="";
String Time         ="";
String Enroll       ="";
String Delete       ="";
String URL          ="";
String User_Name    ="";
String PassWord     ="";
String date         ="";
String Set_Date     ="";
String times        ="";
String mont         ="";
String Post_Data    = "";
String Server_Url   ="";
String inputString  =""; 
String Login_UserName ="admin";
String Login_Password ="admin";
String MonthName[13]={"dom","Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};

String Device_IN             ="{\"deviceInfo\":";
String Data_Array_Open       = ",\"dataSet\":[";
String Data_Array_Close      = "]";


/*****Char Variables*******/
char datestring           [20]; 
char JSONmessageBuffer    [300];
char JSONmessageBuffer1   [300];
char Biometric_Information[150];

/*******Station Class*********/
const char* ssid;                        /* Access Point SSID */
const char* password;                    /* Access Point Password */


/********ESP8266 Access point*********/
const char* ssidAP     = "BioMetric";        /* ESP8266 SSID */
const char* passwordAP = "12345678";      /* ESP8266 PASSWORD */


uint8_t inChar       = 0;
uint8_t deviceid     = 1;
uint8_t case_type    = 3;
uint8_t String_Count = 0;
uint16_t dataSet     = 0;
uint16_t Finger_Id   = 0;
uint32_t TimeStamp   = 0;

bool OAT_Flag         = false;
bool Enroll_Flag      = false;
bool TimeSet_Flag     = false;
bool DateSet_Flag     = false;
bool Username_Flag    = false;
bool Network_Status   = true;
bool Data_Available   = false;
bool Five_Minute_Flag = false;
bool Server_Post_Flag = true;

void setup() 
{
  Biometric_System_Setup();
}

void loop() 
{
   OTA_Update();
   server.handleClient();
   Biometric_State_Machine(case_type);  
   Data_Bundling();
}

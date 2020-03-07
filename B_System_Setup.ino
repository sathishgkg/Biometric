void Biometric_System_Setup(void)
{
  Esp8266_Gpio_Config();
  #if Debugs  
  Esp8266_Serial_Config();
  #endif  
  Esp8266_FingerPrint_Config();
  Esp8266_Eeprom_Config();
  Get_Wifi_Config();
  Esp8266_WiFi_Config();  
  Esp8266_Login_Config();
  Esp8266_Rtc_Config();
  Esp8266_SdCard_Config();
  BioMetric_Startup_Sequence();
}

void Get_Wifi_Config()
{
  char UserSsid_Lenth=char(EEPROM.read(SsidLength_Address));
  char UserPass_Lenth=char(EEPROM.read(PassLength_Address));
   for(int wifi=0;wifi<UserSsid_Lenth;wifi++)
      {
        char data =char(EEPROM.read(Wifissid_Address));
        User_Name+=data;
        Wifissid_Address+=1;
      }
   for(int wifi=0;wifi<UserPass_Lenth;wifi++)
      {
        char data =char(EEPROM.read(Wifipass_Address));
        PassWord+=data;
        Wifipass_Address+=1;
      }
      Username_Flag=false;
      ssid=User_Name.c_str();
      password=PassWord.c_str();
      WiFi.begin(ssid,password);
      #if Debugs       
      Serial.print("SSID :");
      Serial.print(ssid);
      Serial.println("/");
      Serial.print("password :");
      Serial.print(password);     
      Serial.println("/"); 
      #endif           
      inputString=" ";  
      User_Name="";
      PassWord="";              
      Wifissid_Address=0;
      Wifipass_Address=15;
      String_Count=0;
 }

void Esp8266_Login_Config(void)
{
  int loginname_Lenth=int(EEPROM.read(Login_Name_length));
  int loginPass_Lenth=int(EEPROM.read(Login_Pass_length));

  if(loginname_Lenth >=255 && loginPass_Lenth >=255 ){
    Login_UserName ="admin";
    Login_Password ="admin";
      #if Debugs       
      Serial.print("Login_UserName :");
      Serial.print(Login_UserName);
      Serial.println();
      Serial.print("Login_Password :");
      Serial.print(Login_Password);     
      Serial.println(); 
      #endif    
    } 
  else if(loginname_Lenth !=255 && loginPass_Lenth !=255 ){    
          #if Debugs       
      Serial.print("loginname_Lenth :");
      Serial.print(loginname_Lenth);
      Serial.println();
      Serial.print("loginPass_Lenth :");
      Serial.print(loginPass_Lenth);     
      Serial.println(); 
      #endif      
   for(int wifi=0;wifi<loginname_Lenth;wifi++)
      {
        char data =char(EEPROM.read(Login_Name));
        User_Name+=data;
        Login_Name+=1;
      }
   for(int wifi=0;wifi<loginPass_Lenth;wifi++)
      {
        char data =char(EEPROM.read(Login_Pass));
        PassWord+=data;
        Login_Pass+=1;
      }
      
      Login_UserName=User_Name.c_str();
      Login_Password=PassWord.c_str();  
            #if Debugs       
      Serial.print("Login_UserName :");
      Serial.print(Login_UserName);
      Serial.println();
      Serial.print("Login_Password :");
      Serial.print(Login_Password);     
      Serial.println(); 
      #endif    
      Login_Name=50;
      Login_Pass=70;
      User_Name="";
      PassWord="";
  }
}

void Esp8266_WiFi_Config(void)
{
    #if Debugs    
        Serial.print("Connecting to ");
        Serial.println(ssid);
        Serial.println(password);
    #endif  
        WiFi.mode(WIFI_AP_STA);
        WiFi.begin(ssid,password);
        WiFi.softAP(ssidAP,passwordAP);

        server.on("/",Login);
        server.on("/b",Biometric_AP_GUI);      
        server.on("/a",Biometric_Parameter_Setup);
        server.begin();
}

void Esp8266_Rtc_Config(void)
{
       //--------RTC SETUP ------------
       Rtc.Begin(Rtc_Sck, Rtc_Scl); //D1,D4
       delay(10);      
}

void Esp8266_Gpio_Config(void)
{
       pinMode(Buzzer_Gpio,OUTPUT);
       pinMode(Green_Led_Gpio,OUTPUT);
       pinMode(Red_Led_Gpio,OUTPUT);       
}

void Esp8266_SdCard_Config(void)
{
if (!SD.begin(15)) 
{
  #if Debugs      
      Serial.println("initialization failed!");
  #endif    
    return;
}
  #if Debugs    
      Serial.println("initialization done.");
  #endif  
if (SD.exists("fuser.txt")) 
{
  #if Debugs      
      Serial.println("fuser.txt exists.");
  #endif    
}
else
{
  #if Debugs      
      Serial.println("fuser.txt doesn't exist.");
      Serial.println("Creating fuser.txt...");
  #endif    
      myFile1 = SD.open("fuser.txt", FILE_WRITE);
      myFile1.close();
}
if (SD.exists("fenroll.txt")) 
{
  #if Debugs        
      Serial.println("fenroll.txt exists.");
  #endif    
}
else
{
  #if Debugs      
      Serial.println("fenroll.txt doesn't exist.");
      Serial.println("Creating fenroll.txt...");
  #endif    
      myFile = SD.open("fenroll.txt", FILE_WRITE);
      myFile.close();
}
if (SD.exists("fpost.txt")) 
{
  #if Debugs        
      Serial.println("fpost.txt exists.");
  #endif    
}
else
{
  #if Debugs      
      Serial.println("fpost.txt doesn't exist.");
      Serial.println("Creating fpost.txt...");
  #endif    
      myFile = SD.open("fpost.txt", FILE_WRITE);
      myFile.close();
}
if (SD.exists("log.txt")) 
{
  #if Debugs        
      Serial.println("log.txt exists.");
  #endif    
}
else
  {
    #if Debugs      
      Serial.println("log.txt doesn't exist.");
      Serial.println("Creating log.txt...");
  #endif    
      myFile = SD.open("log.txt", FILE_WRITE);
      myFile.close();
  }
}

void Esp8266_Eeprom_Config(void){
      EEPROM.begin(512);
      delay(10);
  #if Debugs      
      Serial.println("EEPROM Done");
  #endif        
}      

void Esp8266_FingerPrint_Config(void){
      fps.Open();
      fps.SetLED(true);
      delay(10);
  #if Debugs      
      Serial.println("fingerprint Done");
  #endif         
}

void Esp8266_Serial_Config(void){
      Serial.begin(9600);
      delay(10);
      Serial.println(" ");
  #if Debugs      
      Serial.println("Serial Done");
  #endif        
}

void BioMetric_Startup_Sequence(void){
    digitalWrite(Buzzer_Gpio,HIGH);
    delay(500);
    digitalWrite(Buzzer_Gpio,LOW);
    digitalWrite(Green_Led_Gpio,HIGH);
    digitalWrite(Red_Led_Gpio,HIGH);
    digitalWrite(Green_Led_Gpio,LOW); 
  #if Debugs      
      Serial.println("Startup Done");
  #endif
}

void Enable_Buzzer(uint16_t Buzzer_Time)
{
    digitalWrite(Buzzer_Gpio,HIGH);
    delay(Buzzer_Time);
    digitalWrite(Buzzer_Gpio,LOW);
}

int Real_Time_Clock() 
{
  RtcDateTime now = Rtc.GetDateTime();
  TimeStamp = now.Epoch32Time();
  printDateTime(now);
  if((now.Minute() % Five_Minutes == 0) && (now.Second() <= 1))
  {
   Five_Minute_Flag = true;
  }
  if(now.Hour() % OAT_Update_Time == 0 && (now.Minute() <= 1) && (now.Second() <= 1))
  {
    OAT_Flag = true;
  }

}

void printDateTime(const RtcDateTime& dt)
{
  snprintf_P(datestring,
             countof(datestring),
             PSTR("%04u-%02u-%02u %02u:%02u:%02u"),
             dt.Year(),
             dt.Month(),
             dt.Day(),
             dt.Hour(),
             dt.Minute(),
             dt.Second() );
#if Debugs  
  Serial.println(datestring);
#endif  
}

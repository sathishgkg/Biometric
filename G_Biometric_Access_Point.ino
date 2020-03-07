void Biometric_Parameter_Setup(){
   if (server.hasArg("user_name") && server.hasArg("password"))
  {  
       Login_UserName = server.arg("user_name");//Get login username
       Login_Password = server.arg("password");//Get login Password
   #if Debugs  
       Serial.println(Login_UserName);
       Serial.println(Login_Password);
   #endif
     
     EEPROM.put(Login_Name_length,Login_UserName.length());
     EEPROM.put(Login_Pass_length,Login_Password.length());
   for (int wifi = 0; wifi < Login_UserName.length(); wifi++) 
   {
      EEPROM.put(Login_Name,Login_UserName[wifi]);
      Login_Name+=1;
   }
   for (int wifi = 0; wifi < Login_Password.length(); wifi++) 
   {
      EEPROM.put(Login_Pass,Login_Password[wifi]);
      Login_Pass+=1;
   }
   EEPROM.commit();
  }
  if (server.hasArg("ssid") && server.hasArg("pass"))
  {  
       sssid = server.arg("ssid");//Get SSID
       passs = server.arg("pass");//Get Password
   #if Debugs  
       Serial.println(sssid);
       Serial.println(passs);
   #endif
     
     EEPROM.put(SsidLength_Address,sssid.length());
     EEPROM.put(PassLength_Address,passs.length());
   for (int wifi = 0; wifi < sssid.length(); wifi++) 
   {
      EEPROM.put(Wifissid_Address,sssid[wifi]);
      Wifissid_Address+=1;
   }
   for (int wifi = 0; wifi < passs.length(); wifi++) 
   {
      EEPROM.put(Wifipass_Address,passs[wifi]);
      Wifipass_Address+=1;
   }
   EEPROM.commit();
  }
  if(server.hasArg("Date") && server.hasArg("time"))
  {  
       Date=server.arg("Date");
       Time=server.arg("time");
   #if Debugs  
       Serial.println(Date);
       Serial.println(Time);
   #endif
       RtcDateTime compiled = RtcDateTime(Date.c_str(),Time.c_str());
       printDateTime(compiled);
       Rtc.SetDateTime(compiled);
  }
  if(server.hasArg("enroll"))
  {  
       Enroll=server.arg("enroll");//Get SSID
   #if Debugs  
       Serial.println(Enroll);
   #endif
       case_type = Enroll_Finger;
       Finger_Id=Enroll.toInt();
  }
  if(server.hasArg("url"))
  {  
       URL=server.arg("url");//Get SSID
   #if Debugs  
       Serial.println(URL);
   #endif
  }
  if(server.hasArg("delete"))
  {  
       Delete=server.arg("delete");//Get SSID
   #if Debugs  
       Serial.println(Delete);
   #endif
   if(Delete != "DB")
   {
      case_type = Delete_Finger;
      Finger_Id=Delete.toInt();
   }
   else if(Delete == "DB")
   {
    case_type = Delete_Finger_DB;
   }
  }  
        
    String s = "\r\n\r\n<!DOCTYPE HTML>\r\n<html><h1>Biometric</h1> ";
           s += "<p>Successfull</html>\r\n\r\n";
    server.send(200,"text/html",s);   

}

  void Biometric_AP_GUI() {
  int Tnetwork=0,i=0,len=0;
  String st="",s="",Login_us="",Login_pass="";                                    //String array to store the SSID's of available networks
 if (server.hasArg("user_name") && server.hasArg("password"))
  {  
       Login_us = server.arg("user_name");//Get login username
       Login_pass = server.arg("password");//Get login Password
   #if Debugs  
       Serial.println(Login_us);
       Serial.println(Login_pass);
   #endif
  }
  if(Login_us == Login_UserName  && Login_pass == Login_Password)
  {
  Tnetwork = WiFi.scanNetworks();            //Scan for total networks available
  uint8_t fps_count = fps.GetEnrollCount();
  st = "<ul>";
  for (int i = 0; i < Tnetwork; ++i)
    {
      // Print SSID and RSSI for each network found
      st += "<li>";
      st +=i + 1;
      st += ": ";
      st += WiFi.SSID(i);
      st += " (";
      st += WiFi.RSSI(i);
      st += ")";
      st += (WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*";
      st += "</li>";
    }
      st += "</ul>";
   IPAddress ip = WiFi.softAPIP();                  //Get ESP8266 IP Adress
        //String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
        s = "\n\r\n<!DOCTYPE HTML><html><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><head><style>div.container {width: 100%;border: 1px solid gray;}header,footer{padding: 1em;color: white;background-color: NAVY;clear:left;text-align: center;}nav {float: left;max-width: 160px;margin: 0;padding: 1em;}nav ul {list-style-type: none;padding: 0;}nav ul a {text-decoration: none;}article {margin-left: 170px;border-left:1pxsolid gray;padding: 1em;overflow: hidden;}</style></head><body><div class=\"container\"><header><h1>Biometric</h1></header>";
        //s += ipStr;
        s += "<p>";
        s += st;
		s += "<center>";
        s += "<form method='get' action='a'><label>Enroll ID:<br></label><input name='enroll' length=32><br><input type='submit' value=\"Submit\"></form><br>";
        s += "<form method='get' action='a'><label>Delete ID:<br> </label><input name='delete' length=32><br><input type='submit' value=\"Submit\"></form><br>";
        s += "<form method='get' action='a'><label>Wifi Ssid: </label><br><input name='ssid' length=32><br><label>Password: </label><br><input name='pass' length=64><br><input type='submit' value=\"Submit\"></form><br>";
        s += "<form method='get' action='a'><label>Set Date: </label><br><input name='Date' length=32><br><label>Set Time: </label><br><input name='time' length=64><br><input type='submit' value=\"Submit\"></form>";
        s +=  "<p>Device LogIn SetUp :</p>";
        s += "<form method='get' action='a'><label>User_Name :</label><br><input name='user_name' length=32><br><label>Password: </label><br><input name='password' length=64><br><input type='submit' value=\"Submit\"></form><br>";
        s +=  "<p>No Of Regisration :</p>"+(String)fps_count+".";
        s += "<center>";       
	    s += "<footer>Copyright &copy; Biometric</footer></div></body></html>\r\n\r\n";
      
    server.send( 200 , "text/html", s);
  }
  else
  {
    String s = "\r\n\r\n<!DOCTYPE HTML>\r\n<html><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><h1>Biometric</h1> ";
           s += "<p>Login fail</html>\r\n\r\n";
    server.send(200,"text/html",s);   
  }
} 

void Login(void)
{
   String s="",st="";  
  s = "\n\r\n<!DOCTYPE HTML>\r\n<html><body><h1> BioMetric Login</h1> ";
        //s += ipStr;
        s += "<p>";
        s += st;
        s += "<form method='get' action='b'><label>User Name  : &nbsp </label><input name='user_name' length=32><br><label>&nbsp Password: </label><input name='password' length=64><br><input type='submit'></form><br>"; 
        s += "</body></html>\r\n\r\n";
      
    server.send( 200 , "text/html", s);
}

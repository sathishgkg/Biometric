int Post_Finger_Data(int fid)
{
  const size_t bufferSize = JSON_OBJECT_SIZE(4);
  DynamicJsonBuffer jsonBuffer(bufferSize);
  JsonObject& root = jsonBuffer.createObject();
  root["UserId"] = fid;
  root["timeStamp"] = (String)TimeStamp;
  root["dateTime"] = datestring;
  root.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
#if Debugs
  Serial.print("Json atten post");
  Serial.println(JSONmessageBuffer);
#endif
  Save_SdCard(Attendance_Data, JSONmessageBuffer);
}

// Enroll url http://coderays.com/biometric/apps/api/user_enroll.php
//http://twobin.000webhostapp.com/temptest/fingerprint.php
int Post_Enroll_Data(int fid)
{
  const size_t bufferSize2 = 2 * JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3);
  DynamicJsonBuffer jsonBuffer(bufferSize2);
  JsonObject& root2 = jsonBuffer.createObject();
  JsonObject& deviceInfo = root2.createNestedObject("deviceInfo");
  deviceInfo["deviceId"] = deviceid;
  deviceInfo["swVersion"] = 1;
  JsonObject& data = root2.createNestedObject("data");
  data["UserId"] = fid;
  data["timeStamp"] = (String)TimeStamp;
  data["dateTime"] = datestring;
  root2.prettyPrintTo(JSONmessageBuffer1, sizeof(JSONmessageBuffer1));
#if Debugs
  Serial.print("Json Enroll post");
  Serial.println(JSONmessageBuffer1);
#endif
  if (WiFi.status() == WL_CONNECTED)
  {
    HTTPClient http;
    http.begin("http://coderays.com/biometric_v1/apps/api/user_enroll.php");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    long httpCode = http.POST(JSONmessageBuffer1); //Send the request
    String payload = http.getString(); //Get the response payload
#if Debugs
    Serial.print("HttpCode :");
    Serial.println(httpCode); //Print HTTP return code
    Serial.print("Json :");
    Serial.println(payload); //Print request response payload
#endif
    const size_t bufferSize4 = JSON_OBJECT_SIZE(3) + 40;
    DynamicJsonBuffer jsonBuffer(bufferSize4);

    JsonObject& root4 = jsonBuffer.parseObject(payload);
    int deviceId = root4["deviceId"]; // 1
    int userId = root4["userId"]; // 1
    int post_status = root4["status"]; // 1
   // root4.printTo(Serial);
    http.end();
    if (post_status == 0)
    {
      Save_SdCard(Enroll_Data, JSONmessageBuffer1);
    }
#if Debugs
    Serial.println("Posted");
#endif
  } else if (WiFi.status() != WL_CONNECTED)
  {
    Save_SdCard(Enroll_Data, JSONmessageBuffer1);
  }
  delay(3000);
}

void Post_Biometric_Data(void)
{
  String User_Fail_Data="";
  if (WiFi.status() == WL_CONNECTED)
  {
    HTTPClient http;
    // http.begin("http://coderays.com/biometric/apps/api/user_dataEntry.php");
    http.begin("http://coderays.com/biometric_v1/apps/api/user_dataEntry.php");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    long httpCode = http.POST(Post_Data); //Send the request
    String payload = http.getString(); //Get the response payload
#if Debugs
    Serial.print("HttpCode :");
    Serial.println(httpCode); //Print HTTP return code
    Serial.print("Json :");
    Serial.println(payload); //Print request response payload
#endif
    //   const size_t bufferSize4 = JSON_OBJECT_SIZE(3) + 40;
    DynamicJsonBuffer jsonBuffer(600);
    
    JsonObject& root = jsonBuffer.parseObject(payload);
  //  root.printTo(Serial);
    http.end();
    int post_status = root["status"]; // 1
    for (uint16_t data_Lenght = 0; data_Lenght < dataSet ; data_Lenght++)
    {
      uint8_t User_Data_Status = root["data"][data_Lenght]["status"];
#if Debugs
    Serial.println(User_Data_Status);
#endif      
      if ((User_Data_Status != 1) && (post_status == 1))
      {
        Server_Post_Flag = false;
              User_Fail_Data+=User_Data[data_Lenght];
        Save_SdCard(Server_Post, User_Fail_Data);
        User_Fail_Data="";
      }
    }


#if Debugs
    Serial.println("Posted");
#endif
    if (post_status == 1)
    {
      SD.remove("fuser.txt");
      SD.remove("fenroll.txt");
      JsonData="";
    if (!Server_Post_Flag)
    {
      myFile2 = SD.open("fpost.txt");
      while (myFile2.available())
      {
        char Fat_Data = myFile2.read();
        if (Fat_Data == '/')
        {
          Save_SdCard(Attendance_Data, JsonData);
#if Debugs
          Serial.println(JsonData);
#endif
          JsonData = "";
        }
        else
        {
          JsonData += Fat_Data;
        }
#if Debugs
        // Serial.println("Reading SD Card");
#endif
      }
      myFile2.close();
      Server_Post_Flag = true;
      SD.remove("fpost.txt");
    }
    }
  }
  else
  {
#if Debugs
    Serial.println("WiFi Not Connected");
#endif
  }
}



void Data_Bundling()
{
  if (Five_Minute_Flag)
  {
    JsonData="";
#if Debugs
    Serial.println("FiveMinutes");
#endif
    Real_Time_Clock();
    myFile = SD.open("fuser.txt");
    while (myFile.available())
    {
      char Fat_Data = myFile.read();
      if (Fat_Data == '/')
      {
        User_Data[dataSet++] = JsonData;
        Data_Available = true;
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
    myFile.close();

    if (Data_Available)
    {
      const size_t Bundle_bufferSize = JSON_OBJECT_SIZE(4);
      DynamicJsonBuffer jsonBuffer(Bundle_bufferSize);
      JsonObject& header = jsonBuffer.createObject();
      header["deviceId"] = deviceid;
      header["swVerechosion"] = 1;
      header["timeStamp"] = (String)TimeStamp;
      header["dateTime"] = (String)datestring;
      header.prettyPrintTo(Biometric_Information, sizeof(Biometric_Information));

      //   Biometric_Information = "{\"deviceInfo\":{\"deviceId\":"+(String)deviceid+",\"swVersion\":1,\"timeStamp\":"+(String)TimeStamp+",\"dateTime\":"+datestring+"}";
      Post_Data += Device_IN;
      Post_Data += Biometric_Information;
      Post_Data += Data_Array_Open;
      for (int Fill_Buffer = 0; Fill_Buffer < dataSet; Fill_Buffer++)
      {
        Post_Data += User_Data[Fill_Buffer];
        if (Fill_Buffer < (dataSet - 1)) {
          Post_Data += ",";
        }
      }
      Post_Data += Data_Array_Close;
      Post_Data += "}";
      Data_Available = false;
#if Debugs
      Serial.println(Post_Data);
#endif
      Post_Biometric_Data();
    }
    dataSet = 0;
    Post_Data = "";

    Sd_Card_Data();
    Five_Minute_Flag = false;
  }
}



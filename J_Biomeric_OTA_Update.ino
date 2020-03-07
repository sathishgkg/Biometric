
void OTA_Update()
{
  if (OAT_Flag)
  {
    OAT_Flag = false;
    if (WiFi.status() == WL_CONNECTED)
    {
      HTTPClient http;
             http.begin("http://twobin.000webhostapp.com/esp8266updated/sw_version.txt");
      int httpcode = http.GET();
      String vno = http.getString();
      int version_no = vno.toInt();
#if Debugs
      Serial.print("s/w version_no");
      Serial.println(version_no);
#endif
      if (version_no > BioMetric_SWversion)
      {
        t_httpUpdate_return ret = ESPhttpUpdate.update("http://twobin.000webhostapp.com/esp8266updated/Biometric_System.bin");///Biometric_System.bin
        switch (ret)
        {
          case HTTP_UPDATE_FAILED:
#if Debugs
            Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
#endif
            break;

          case HTTP_UPDATE_NO_UPDATES:
#if Debugs
            Serial.println("HTTP_UPDATE_NO_UPDATES");
#endif
            break;

          case HTTP_UPDATE_OK:
#if Debugs
            Serial.println("HTTP_UPDATE_OK");
#endif
            break;
        }
      }
    }
  }
}

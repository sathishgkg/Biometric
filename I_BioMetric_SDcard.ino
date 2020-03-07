void Sd_Card_Data() {
  String JsonData = "";
#if Debugs
  Serial.println("fEnroll data");
#endif
  myFile = SD.open("fenroll.txt");
  while (myFile.available()) {
    char Fat_Data = myFile.read();
    if (Fat_Data == '/') {
      HTTPClient http;
      http.begin("http://coderays.com/biometric_v1/apps/api/user_enroll.php");
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      http.POST(JsonData); //Send the request
      http.end();
#if Debugs
      Serial.println("Posted enroll");
      Serial.print(JsonData);
#endif
      JsonData = " ";
      delay(5000);
      Enroll_Flag = true;
    } else {
      JsonData += Fat_Data;
    }
  }
  myFile.close();
  if (Enroll_Flag == true)
  {
    Enroll_Flag = false;
    SD.remove("fenroll.txt");
  }
}


void Save_SdCard(int data_type , String attendance)
{
  myFile3 = SD.open("log.txt", FILE_WRITE);
  if ( data_type == Attendance_Data )
  {
#if Debugs
    Serial.print( "Attendance" );
#endif
    myFile1 = SD.open( "fuser.txt" , FILE_WRITE);
  }
  else if ( data_type == Enroll_Data )
  {
#if Debugs
    Serial.print( "enroll" );
#endif
    myFile = SD.open( "fenroll.txt" , FILE_WRITE );
  }
  else if ( data_type == Server_Post)
  {
#if Debugs
    Serial.print( "fpost" );
#endif
    myFile2 = SD.open( "fpost.txt" , FILE_WRITE );
  }
  if ( myFile )
  {
#if Debugs
    Serial.println( "Write enroll to SD card" );
#endif
    myFile.print(attendance);
    myFile.println('/');
    myFile.close();
    myFile3.print("enroll");
    myFile3.println(attendance);
    myFile3.close();

  }
  else if ( myFile1 )
  {
#if Debugs
    Serial.println( "Write attendance to SD card" );
#endif
    myFile1.print(attendance);
    myFile1.println('/');
    myFile1.close();
    myFile3.print("attendance");
    myFile3.println(attendance);
    myFile3.close();
  }
  else if (myFile2)
  {
#if Debugs
    Serial.println( "Write server data to SD card" );
#endif
    myFile2.print(attendance);
    myFile2.println('/');
    myFile2.close();
  }
  else
  {
#if Debugs
    Serial.println( "error opening file.txt" );
#endif
  }
  attendance = "";

}

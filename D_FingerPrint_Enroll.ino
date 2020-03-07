uint8_t getFingerprintEnroll(int enrollid) 
{
   if(enrollid<200)
  {
   if(fps.CheckEnrolled(enrollid) == false)
   {
    Serial.print("ID not in use");
    fps.EnrollStart(enrollid);
    while(fps.IsPressFinger() == false) 
    {
    delay(100);
    }
    bool Capture = fps.CaptureFinger(true);
    if(Capture == true)
    {
        Serial.println("Remove finger");
        fps.Enroll1(); 
        while(fps.IsPressFinger() == true) 
        {
          delay(100);
        }
        Serial.println("Press same finger again");
        while(fps.IsPressFinger() == false) 
        {
          delay(100);
        }
        Capture = fps.CaptureFinger(true);
        if(Capture == true)
        {
          Serial.println("Remove finger");
          fps.Enroll2();
          while(fps.IsPressFinger() == true) 
          {
           delay(100);
          }
          Serial.println("Press same finger again");
          while(fps.IsPressFinger() == false) 
          {
           delay(100);
          } 
          Capture = fps.CaptureFinger(true); 
          if(Capture == true)
          {
            Serial.println("Remove finger");
            int enroll_data = fps.Enroll3();
            if (enroll_data == 0)
            {
             Serial.println("Enrolling Successfull");
            }
            else
            {
             Serial.print("Enrolling Failed with error code:");
             Serial.println(enroll_data);
            }                 
          }
        }
    }
   }
   else
   {
    Serial.print("ID already used");    
   }
  }
  else
  {
    Serial.print("ID should be less than 200");   
  }
}

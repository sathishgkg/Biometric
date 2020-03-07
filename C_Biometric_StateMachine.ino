void Biometric_State_Machine(char Biometric_State)
{
  switch(Biometric_State)
  {
    case Enroll_Finger:
           #if Debugs       
               Serial.println("Enroll");
           #endif 
               digitalWrite(Green_Led_Gpio,HIGH);
               digitalWrite(Red_Led_Gpio,HIGH);
               Enable_Buzzer(500);
               getFingerprintEnroll(Finger_Id);
               case_type = Read_Finger;
               inputString = " ";
         break;
    case Delete_Finger:
           #if Debugs   
               Serial.println("delete");   
           #endif 
               digitalWrite(Green_Led_Gpio,LOW);
               digitalWrite(Red_Led_Gpio,LOW);
               Enable_Buzzer(2000);
               deleteFingerprint(Finger_Id);
               case_type = Read_Finger;
               inputString = "";
         break;     
    case Read_Finger:
            //   digitalWrite(Red_Led_Gpio,HIGH);  
               Real_Time_Clock();
               getFingerprintID();
               inputString = "";
         break;
    case Delete_Finger_DB:
           #if Debugs   
               Serial.println("delete");   
           #endif
               digitalWrite(Green_Led_Gpio,LOW);
               digitalWrite(Red_Led_Gpio,LOW);
               Enable_Buzzer(4000);
               SD.remove("fuser.txt");
               SD.remove("fenroll.txt");
               SD.remove("fpost.txt");
               SD.remove("log.txt");
               Delete_DataBase();
               case_type = Read_Finger;
               inputString = "";
         break;          
  }
}

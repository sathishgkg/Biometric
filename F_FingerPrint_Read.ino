uint8_t getFingerprintID() 
{
  if (fps.IsPressFinger())
  {
    fps.CaptureFinger(false);
    int id = fps.Identify1_N();
    if (id <200)
    {
      Serial.print("Verified ID:");
      Serial.println(id);
      return id;
    }
    else
    {
      Serial.println("Finger not found");
    }
  }   
}

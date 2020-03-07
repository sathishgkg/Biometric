uint8_t deleteFingerprint(int fingerid) 
{
  fps.DeleteID(fingerid);
}

void Delete_DataBase() 
{
  fps.DeleteAll();
}

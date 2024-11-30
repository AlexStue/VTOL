float Angle ( ) {

//__Variablen________________________________________________________

float AcXp = AcX; float AcYp = AcY; float AcZp = AcZ;
float GyXp = GyX; float GyYp = GyY; float GyZp = GyZ;
float Wxfp = Wxf; float Wyfp = Wyf; float Wzfp = Wzf;

//__Sensor________________________________________________________

  Wire.beginTransmission(0x68);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(0x68,14,true);   // Wertebereiche Standart +-2g, +-500°/s von-32767 bis 32767  1g=16.383
  AcX = Wire.read()<<8|Wire.read();
  AcY = Wire.read()<<8|Wire.read();
  AcZ = Wire.read()<<8|Wire.read();
  Tmp = Wire.read()<<8|Wire.read();
  GyX = Wire.read()<<8|Wire.read();// x Roll d
  GyY = Wire.read()<<8|Wire.read();// y Pitch c
  GyZ = Wire.read()<<8|Wire.read();// z Yaw b

  GyX = GyX * 0.0170 + 0.60;  // Offset & Gain
  GyY = GyY * 0.0170 - 0.94;  // 500°/s = 32767
  GyZ = GyZ * 0.01526 + 0.32; // 

//__Winkel__________________________________________________________

  AcV = sqrt( pow(AcX,2) + pow(AcY,2) + pow(AcZ,2) );
  if( AcV == 0 ) AcV = 1;

  float Wx_Ac= asin(AcY/AcV) *  57.29578 - 1.6;  // Rad in Winkel
  float Wy_Ac= asin(AcX/AcV) * -57.29578 + 3.21; // Kalibrierung
  float Wz_Ac= asin(AcZ/AcV) *  57.29578 + 0;

  Wx += GyX * Ts_s; // Winkel aus Gyro
  Wy += GyY * Ts_s;
  Wz += GyZ * Ts_s;

  Wx += Wy * sin(GyZ * 0.01745 * Ts_s); // Korrektur Transformation aus Gyro
  Wy += Wx * sin(GyZ * 0.01745 * Ts_s);

  Wx = Wx * Comb_W + Wx_Ac * (1 - Comb_W); // Fusion in Grad
  Wy = Wy * Comb_W + Wy_Ac * (1 - Comb_W); 

//__Filter__________________________________________________________

  float alpha_Gy =  (Lpf_Gy * Ts_s) / (1 + Lpf_Gy * Ts_s);
  GyX = GyXp + alpha_Gy * (GyX - GyXp);
  GyY = GyYp + alpha_Gy * (GyY - GyYp);
  GyZ = GyZp + alpha_Gy * (GyZ - GyZp);

  float alpha_Ac =  (Lpf_Ac * Ts_s) / (1 + Lpf_Ac * Ts_s);
  AcX = AcXp + alpha_Ac * (AcX - AcXp);
  AcY = AcYp + alpha_Ac * (AcY - AcYp);
  AcZ = AcZp + alpha_Ac * (AcZ - AcZp);

  float alpha_W =  (Lpf_W * Ts_s) / (1 + Lpf_W * Ts_s);
  Wxf = Wxfp + alpha_W * (Wx - Wxfp);
  Wyf = Wyfp + alpha_W * (Wy - Wyfp);
  //Wzf = Wzfp + alpha_W * (Wz - Wzfp);

//__Cutoff____________________________________________________________

  if ( Wxf > Cutoff_Angle || Wxf < - Cutoff_Angle ) error = 1;
  if ( Wyf > Cutoff_Angle || Wyf < - Cutoff_Angle ) error = 1;

//__Out_______________________________________________________________

  // Serial.print(map(AcV, 0, 32200, -2000, 2000));
  // Serial.print('\t');
  // Serial.print("Angle");
  // Serial.print('\t');

}

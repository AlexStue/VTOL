void Angle ( ) {

//__Variablen_____________________________________________________

  float AcXp = AcX; float AcYp = AcY; float AcZp = AcZ;
  float GyXp = GyX; float GyYp = GyY; float GyZp = GyZ;
  float Wxfp = Wxf; float Wyfp = Wyf; float Wzfp = Wzf;

//__Sensor________________________________________________________

  IMU.update();
  IMU.getAccel(&accelData);
  AcX = accelData.accelX * 1000 + 8; // milli g
  AcY = accelData.accelY * 1000 - 92;
  AcZ = accelData.accelZ * 1000 + 6;
  IMU.getGyro(&gyroData);
  GyX = - gyroData.gyroX + 0.4;
  GyY = - gyroData.gyroY - 0.3;
  GyZ =   gyroData.gyroZ + 0.4;

  if( GyX < 1 && GyX > -1  ) GyX = 0;
  if( GyY < 1 && GyY > -1  ) GyY = 0;
  if( GyZ < 1 && GyZ > -1  ) GyZ = 0;

//__Winkel_______________________________________________________

  AcV = sqrt( pow(AcX,2) + pow(AcY,2) + pow(AcZ,2) );
  if( AcV == 0 ) AcV = 1;

  float Wx_Ac= asin(AcY/AcV) * -57.29578 + 0;  // Rad in Winkel
  float Wy_Ac= asin(AcX/AcV) *  57.29578 + 0; // Kalibrierung -57.29578
  float Wz_Ac= asin(AcZ/AcV) *  57.29578 + 0;

  Wx += GyX * Ts_s; // Winkel aus Gyro
  Wy += GyY * Ts_s;
  Wz += GyZ * Ts_s;

  Wx += Wy * sin(GyZ * 0.01745 * Ts_s); // Korrektur Transformation aus Gyro
  Wy += Wx * sin(GyZ * 0.01745 * Ts_s);

  Wx = Wx * Comb_W + Wx_Ac * (1 - Comb_W); // Fusion in Grad
  Wy = Wy * Comb_W + Wy_Ac * (1 - Comb_W); 

//__Filter_______________________________________________________

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

//__Cutoff_______________________________________________________

  if ( Wxf > Cutoff_Angle || Wxf < - Cutoff_Angle ) error = 2;
  if ( Wyf > Cutoff_Angle || Wyf < - Cutoff_Angle ) error = 2;

//__Out__________________________________________________________

  // Serial.print(Wxf);
  // Serial.print("\t");
  // Serial.print(Wyf);
  // Serial.println("\t");

}

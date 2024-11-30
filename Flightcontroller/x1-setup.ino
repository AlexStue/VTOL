void Setup ( ) {

  Serial.begin(19200);
  Wire.begin();
  hc12.begin(9600);
  hc12.setTimeout(3);

//_Servos______________________________________________________

  myServos.begin();
  myServos.setPWMFreq(50);    // Max: 24-1500

  myServos.setPWM( 0, 0, ESC_min + 3); // MotA
  myServos.setPWM( 1, 0, ESC_min + 3); // MotB
  myServos.setPWM( 2, 0, ESC_min + 3); // MotC
  myServos.setPWM( 3, 0, ESC_min + 3); // MotD

  delay(200); // Bis Fahrtenregler hochgefahren sind

//_Gyro________________________________________________________

  Wire.beginTransmission(0x68);         // Sensor starten
  Wire.write(0x6B); 
  Wire.write(0);    
  Wire.endTransmission(true);

  Wire.beginTransmission(0b1101000);    //Auflösung Gyro
  Wire.write(0x1B);                     //Accessing the register 1B - Gyroscope Configuration (Sec. 4.4)
  Wire.write(0b00001000);               //Setting 1 the gyro to full scale +-500°/s
  Wire.endTransmission(true);

  Wire.beginTransmission(0b1101000);    // Filter
  Wire.write(0x1A);                     // Accessing the register 1A (Sec. 4.3)
  Wire.write(0b00000110);               // Option 6
  Wire.endTransmission(true);

  Serial.print("Setup");
  Serial.print('\t');

}

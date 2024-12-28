void Setup ( ) {

  Serial.begin(38400);
  Wire.begin();
  hc12.begin(38400);
  pixels.begin();

  IMU.init(calib, 0x69);

  myServos.begin();
  myServos.setPWMFreq(50);    // Max: 24-1500

  myServos.setPWM( 0, 0, ESC_min + 3); // MotA
  myServos.setPWM( 1, 0, ESC_min + 3); // MotB
  myServos.setPWM( 2, 0, ESC_min + 3); // MotC
  myServos.setPWM( 3, 0, ESC_min + 3); // MotD

  delay(200); // Bis Fahrtenregler hochgefahren sind

}

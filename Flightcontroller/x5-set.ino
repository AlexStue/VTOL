void Stellen( ) {

// A vorne links gUS
// B hinten gUS
// C rechts  UZ
// D links   UZ

//_Calib___________________________________________________________________

//_PreScale________________________________________________________________

  int R_sat_a = constrain( R_TrZ_sat - R_Pitch_sat - R_Yaw_sat , 0 , 100 )
  int R_sat_b = constrain( R_TrZ_sat + R_Pitch_sat - R_Yaw_sat , 0 , 100 )
  int R_sat_c = constrain( R_TrZ_sat - R_Roll_sat  + R_Yaw_sat , 0 , 100 )
  int R_sat_d = constrain( R_TrZ_sat + R_Roll_sat  + R_Yaw_sat , 0 , 100 )

  int ESCa = map(R_sat_a, 0, 100, ESC_min, ESC_max);
  int ESCb = map(R_sat_b, 0, 100, ESC_min, ESC_max);
  int ESCc = map(R_sat_c, 0, 100, ESC_min, ESC_max);
  int ESCd = map(R_sat_d, 0, 100, ESC_min, ESC_max);

// Ctrl 50 Hz = 20ms; Range: 0 - 4095; Step: 0.004884ms
// ESC: 1.08ms - 1.96ms; Set: 221 - 401

  myServos.setPWM(  0, 0, ESCa);
  myServos.setPWM( 14, 0, ESCb);
  myServos.setPWM(  1, 0, ESCc);
  myServos.setPWM( 15, 0, ESCd);

//_Stellen_________________________________________________________________

  int Servo_a = map(e, 0, 200, 90, 500); // calib
  int Servo_b = map(e, 0, 200, 90, 500);
  int Servo_c = map(e, 0, 200, 90, 500);
  int Servo_d = map(e, 0, 200, 90, 500);

  myServos.setPWM(  2, 0, Servo_a );
  myServos.setPWM( 12, 0, Servo_b );
  myServos.setPWM(  3, 0, Servo_c );
  myServos.setPWM( 13, 0, Servo_d );

//__Out_______________________________________________________________

  // Serial.print("Stellen");
  // Serial.print('\t');

}

void Stellen( ) {

// A vorne  gUS
// B hinten gUS
// C rechts  UZ
// D links   UZ

//_Calib___________________________________________________________________

// int ESC_min_Cal = ESC_min +- 0; // wofür?
// int ESC_min_Cal = ESC_min +- 0;

//_PreScale________________________________________________________________

  int Astell = constrain( ESC_min + R_TrZ_sat - R_Pitch_sat - R_Yaw_sat , ESC_min , ESC_max );
  int Bstell = constrain( ESC_min + R_TrZ_sat + R_Pitch_sat - R_Yaw_sat , ESC_min , ESC_max );
  int Cstell = constrain( ESC_min + R_TrZ_sat - R_Roll_sat  + R_Yaw_sat , ESC_min , ESC_max );
  int Dstell = constrain( ESC_min + R_TrZ_sat + R_Roll_sat  + R_Yaw_sat , ESC_min , ESC_max );

//_Stellen_________________________________________________________________

  myServos.setPWM( 0, 0, Astell); // MotA
  myServos.setPWM( 1, 0, Bstell); // MotB
  myServos.setPWM( 2, 0, Cstell); // MotC
  myServos.setPWM( 3, 0, Dstell); // MotD

// Ctrl 50 Hz = 20ms; Range: 0 - 4095; Step: 0.004884ms
// ESC: 1.08ms - 1.96ms; Set: 221 - 401

//__Out_______________________________________________________________

  // Serial.print("Stellen");
  // Serial.print('\t');

}

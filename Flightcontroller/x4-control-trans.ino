void Regler_Trans( ) {

//__Pre________________________________________________________

  int aV = aE;
  float alpha_Empf =  (Lpf_Empf * Ts_s) / (1 + Lpf_Empf * Ts_s);
  a = ap + alpha_Empf * (aV - ap);

  int aM = 0; int AcVM = 0;
  float R_TrZ_u = 0;

 //__Startsprung__________________________________________________

  AcVM = map(AcV, 0, 32200, -2000, 2000) + 3; // ADC in mg (1g = 1000mg) - Calib; Ref im Setup?

  if ( levelUP == 0 & a > 70 ) { // Abheben starten
    R_TrZ_u = f_levelUP_Pow;
    if ( AcVM >= f_levelUP_AcZ | countUP >= f_levelUP_cnt ) { // Abheben erfolgreich, feste niedrigere Drehzahl
      R_TrZ_i = f_levelUP_Hov;
      levelUP = 1; 
      countUP = 0;
      aM = 0;
    }
    countUP++;
  }

  if( levelUP == 0 & a <= 10) { // Abheben abgebrochen
    R_TrZ_sat = 0;
    countUP = 0;
  }

 //__Übergabe_________________________________________________________

  if ( levelUP == 1 & a < 10 ) { // Wenn Abheben erfolgreich, warten auf Reset Steuerung
    levelUP = 2;
  }

  if ( levelUP == 2 ) { // Übergabe der Steuerung
    aM = map(a, -a_max, a_max, -500, 500);
  }

  byte schief = 0;
  if ( abs(Wxf) >= f_schief_W | abs(Wyf) >= f_schief_W ) {
    schief = 1;
  }

 //__Höhen-Regler____________________________________________________

  if ( levelUP == 2 ) { // ( levelUP != 0 & schief == 1 ) ( levelUP == 2 )
    R_TrZ_i += a * Ts_s * f_aI;
    R_TrZ_i  = constrain(R_TrZ_i, 0, R_TrZ_Ksat);
    R_TrZ_u = R_TrZ_i;
  }

  if ( levelUP != 0 & schief == 0 ) {
    float R_TrZ_ep = R_TrZ_e;
          R_TrZ_e  = ( aM - AcVM );
    float R_TrZ_p  = R_TrZ_e * R_TrZ_Kp;
          R_TrZ_i += R_TrZ_e * R_TrZ_Ki  * Ts_s;
          R_TrZ_i  = constrain(R_TrZ_i, 0, R_TrZ_Ksat);
    float R_TrZ_d  = ( R_TrZ_e - R_TrZ_ep ) * R_TrZ_Kd;
          R_TrZ_u  = R_TrZ_p + R_TrZ_i + R_TrZ_d;
  }

//__Out______________________________________________________________

  R_TrZ_sat  = constrain(R_TrZ_u, -R_TrZ_Ksat, R_TrZ_Ksat);

  // Serial.print(a);
  // Serial.print('\t');
  // Serial.print(aM);
  // Serial.print('\t');
  // Serial.print(AcVM);
  // Serial.print('\t');
  // Serial.println(R_TrZ_sat);
  // Serial.print(R_TrZ_sat);
  // Serial.print('\t');

}

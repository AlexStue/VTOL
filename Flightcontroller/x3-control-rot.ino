float Regler_Rot( ) {

// ep
  float R_Roll_ep     = R_Roll_e;
  float R_Roll_Ag_ep  = R_Roll_Ag_e;
  float R_Pitch_ep    = R_Pitch_e;
  float R_Pitch_Ag_ep = R_Pitch_Ag_e;
  float R_Yaw_ep      = R_Yaw_e;

//__Regler_Winkel___________________________________________________________

// e
  R_Roll_Ag_e  = ( d - Wxf );
  R_Pitch_Ag_e = ( c - Wyf );

// p
  float R_Roll_Ag_p  = R_Roll_Ag_e  * R_Roll_Ag_Kp;
  float R_Pitch_Ag_p = R_Pitch_Ag_e * R_Pitch_Ag_Kp;

// i
  R_Roll_Ag_i  += R_Roll_Ag_e  * R_Roll_Ag_Ki  * Ts_s;
  R_Pitch_Ag_i += R_Pitch_Ag_e * R_Pitch_Ag_Ki * Ts_s;

// d
  float R_Roll_Ag_d  = ( R_Roll_Ag_e   - R_Roll_Ag_ep )  * R_Roll_Ag_Kd;
  float R_Pitch_Ag_d = ( R_Pitch_Ag_e  - R_Pitch_Ag_ep ) * R_Pitch_Ag_Kd;

// u
  float R_Roll_Ag_u  = R_Roll_Ag_p  + R_Roll_Ag_i  + R_Roll_Ag_d;
  float R_Pitch_Ag_u = R_Pitch_Ag_p + R_Pitch_Ag_i + R_Pitch_Ag_d;

// sat
  R_Roll_Ag_sat  = constrain(R_Roll_Ag_u,  -R_Roll_Ag_Ksat,  R_Roll_Ag_Ksat);
  R_Pitch_Ag_sat = constrain(R_Pitch_Ag_u, -R_Pitch_Ag_Ksat, R_Pitch_Ag_Ksat);

//__Regler_Geschwindigkeit__________________________________________________

// e
  R_Roll_e  = (  R_Roll_Ag_u  - GyX );
  R_Pitch_e = (  R_Pitch_Ag_u - GyY );
  R_Yaw_e   = ( -b            - GyZ );

// p
  float R_Roll_p  = R_Roll_e  * R_Roll_Kp;
  float R_Pitch_p = R_Pitch_e * R_Pitch_Kp;
  float R_Yaw_p   = R_Yaw_e   * R_Yaw_Kp;

// i
  R_Roll_i  += R_Roll_e  * R_Roll_Ki  * Ts_s;
  R_Pitch_i += R_Pitch_e * R_Pitch_Ki * Ts_s;
  R_Yaw_i   += R_Yaw_e   * R_Yaw_Ki   * Ts_s;
  R_Roll_i   = constrain( R_Roll_i,  -R_Roll_Ksat,  R_Roll_Ksat);
  R_Pitch_i  = constrain( R_Pitch_i, -R_Pitch_Ksat, R_Pitch_Ksat);
  R_Yaw_i    = constrain( R_Yaw_i,   -R_Yaw_Ksat,   R_Yaw_Ksat);

// d
  float R_Roll_d  = ( R_Roll_e  - R_Roll_ep )  * R_Roll_Kd;
  float R_Pitch_d = ( R_Pitch_e - R_Pitch_ep ) * R_Pitch_Kd;
  float R_Yaw_d   = ( R_Yaw_e   - R_Yaw_ep )   * R_Yaw_Kd;

// u
  float R_Roll_u  = R_Roll_p  + R_Roll_i  + R_Roll_d;
  float R_Pitch_u = R_Pitch_p + R_Pitch_i + R_Pitch_d;
  float R_Yaw_u   = R_Yaw_p   + R_Yaw_i   + R_Yaw_d;

// sat
  R_Roll_sat  = constrain( R_Roll_u,  -R_Roll_Ksat,  R_Roll_Ksat);
  R_Pitch_sat = constrain( R_Pitch_u, -R_Pitch_Ksat, R_Pitch_Ksat);
  R_Yaw_sat   = constrain( R_Yaw_u,   -R_Yaw_Ksat,   R_Yaw_Ksat);

  // Wie Integral Limitieren?

//__Out_______________________________________________________________

  // Serial.print("Rot");
  // Serial.print('\t');

}

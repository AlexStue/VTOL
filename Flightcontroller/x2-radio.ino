void Radio( ) {

  ap = a; bp = b; cp = c; dp = d;
  aEp = aE; bEp = bE; cEp = cE; dEp = dE;

  if (hc12.available() > 4) {
    mod = hc12.read();
    a   = hc12.read();
    b   = hc12.read();
    c   = hc12.read();
    d   = hc12.read();
    while (hc12.available()) hc12.read();

    // Thrust a | x Roll d | y Pitch c | z Yaw b
    aE = map(a, 0, 200, -a_max, a_max); 
    bE = map(b, 0, 200, -b_max, b_max);
    cE = map(c, 0, 200, -c_max, c_max);
    dE = map(d, 0, 200, -d_max, d_max);

    count = 0; radio_send = 0;
    if ( error == 1 ) error = 0;
  }

  // int aV = aE;
  // int bV = bE + ( bE - bEp ) * f_Ctrl_d;
  // int cV = cE + ( cE - cEp ) * f_Ctrl_d;
  // int dV = dE + ( dE - dEp ) * f_Ctrl_d;

  // float alpha_Empf =  (Lpf_Empf * Ts_s) / (1 + Lpf_Empf * Ts_s);
  // a = ap + alpha_Empf * (aV - ap);
  // b = bp + alpha_Empf * (bV - bp);
  // c = cp + alpha_Empf * (cV - cp);
  // d = dp + alpha_Empf * (dV - dp);  

//__Cutoff____________________________________________________________

  count++;
  if ( count == Cutoff_Read ) error = 1;
  if ( count  > Cutoff_Read + 5 ) {
    radio_send = 0; count = Cutoff_Read;
  }

}

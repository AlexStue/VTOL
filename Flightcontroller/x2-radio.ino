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
    aE = map(aE, 0, 200, -a_max, a_max); 
    bE = map(bE, 0, 200, -b_max, b_max);
    cE = map(cE, 0, 200, -c_max, c_max);
    dE = map(dE, 0, 200, -d_max, d_max);

    count = 0; radio_send = 0;
    if ( error == 1 ) error = 0;
  }

//__Cutoff____________________________________________________________

  count++;
  if ( count == Cutoff_Read ) error = 1;
  if ( count  > Cutoff_Read + 5 ) {
    radio_send = 0; count = Cutoff_Read;
  }

}

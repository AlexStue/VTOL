#include <Wire.h>
#include <SoftwareSerial.h>
SoftwareSerial hc12(11,10);

//__Variablen__________________

  byte led = 0;
  byte count = 0;
  int a,b,c,d;
  int aref,bref,cref,dref;

  String data = "";

//__Setup____________________

void setup(){
  
    Serial.begin(9600);
    hc12.begin(9600);
    
    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
    pinMode(A2, INPUT);
    pinMode(A3, INPUT);

    aref = analogRead(A0);
    bref = analogRead(A1);
    cref = analogRead(A3);
    dref = analogRead(A2);

}

//__Loop___________________________

void loop() {
  
    if (led == 0) digitalWrite(LED_BUILTIN, HIGH);
    if (led == 1) digitalWrite(LED_BUILTIN, LOW);
    led++;
    if (led == 2) led = 0;
  
//__Read___________________________

    if ( count == 0){

    a = -1 * analogRead(A0);
    b = -1 * analogRead(A1);
    c = -1 * analogRead(A3);
    d = -1 * analogRead(A2);  

    a = a + aref + 1; // nullen + kalib.
    b = b + bref + 0;
    c = c + cref + 0;
    d = d + dref + 0;
    
    a = map(a, -510, 510, -100, 100); //
    b = map(b, -516, 507, -100, 100); //
    c = map(c, -500, 523, -100, 100); //
    d = map(d, -510, 510, -100, 100); //

    data = "" + String(a) + "" + String(b) + "" + String(c) + "" + String(d);

    //Serial.println(data);

    }

//__Send___________________________

    // Erst senden wenn "IAmReady"-Paket vom Flieger kommt
    if ( hc12.available() ) {
    hc12.print(data);
    count = 0;
    }

//__ReRead___________________________

    if ( count > 10 ) {
    count = 0;
    }

    delay(1); // nicht so hastig

}
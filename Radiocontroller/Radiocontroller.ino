#include <Wire.h>
#include <SoftwareSerial.h>
SoftwareSerial hc12(1, 0); // Rx, Tx
#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel pixels( 1, 16, NEO_GRB + NEO_KHZ800);
#include <Adafruit_SSD1306.h>
Adafruit_SSD1306 oled(128, 64, &Wire, -1);

//__Variablen__________________

byte mod = 0;
byte read = 0;
byte count_io = 0;
int count_error = 0;
unsigned long Tr1p = 0; float Tr1 = 0;

int aref,bref,cref,dref;
int a = 0;
int b = 0;
int c = 0;
int d = 0;

byte aS = 0;
byte bS = 0;
byte cS = 0;
byte dS = 0;

void setup(){

  //Serial.begin(38400);
  hc12.begin(38400);
  Wire.begin();
  pixels.begin();

  pixels.setPixelColor(0, pixels.Color(0, 0, 50)); // grb
  pixels.show();

  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  oled.setTextSize(1);
  oled.setTextColor(WHITE);

  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);

  aref = analogRead(A0);
  bref = analogRead(A1);
  cref = analogRead(A2);
  dref = analogRead(A3);

}

void loop() {

  if ( hc12.available() ) { 

    Tr1p = millis();

    mod = 1; // digitalRead(D0)
    a = 123;
    b = 132;
    c = analogRead(A2);
    d = 231;  

    //a = a + aref + 1; // offst + cal‚
    //b = b + bref + 0;
    //c = c + cref + 0;
    //d = d + dref + 0;
    
    aS = map(a, 10, 1023, 0, 200);
    bS = map(b, 10, 1023, 0, 200);
    cS = map(c, 10, 1023, 0, 200); // cS = map(c, -500, 523, 0, 200);
    dS = map(d, 10, 1023, 0, 200);

    read = hc12.read();
    hc12.write(mod);
    hc12.write(aS);
    hc12.write(bS);
    hc12.write(cS);
    hc12.write(dS);

    while (hc12.available()) hc12.read();
    count_io++;
    count_error = 0;

    if ( read == 0 ) {
      pixels.setPixelColor(0, pixels.Color(10, 0, 0)); // grb
      pixels.show(); }

    if ( read > 0 ) {
      pixels.setPixelColor(0, pixels.Color(0, 10, 0));
      pixels.show(); }

  }

  if ( count_io  > 10 ) {

    oled.clearDisplay();
    oled.setCursor(10, 20);
    oled.println(Tr1);
    oled.setCursor(10, 35);
    oled.println(cS);
    oled.setCursor(60, 35);
    oled.println(read);
    oled.display();
    count_io = 0;
  }

  Tr1 = ( millis() - Tr1p );
  if ( Tr1 > 1000 && Tr1 < 1003 ) {
    pixels.setPixelColor(0, pixels.Color(0, 100, 0)); // grb
    pixels.show();
    Serial.println(Tr1);
  }
}


#include <Wire.h>

#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel pixels(1, 16, NEO_GRB + NEO_KHZ800);

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
Adafruit_SSD1306 oled(128, 65, &Wire, -1);


int aref = 0;
float AcX = 0; float AcY = 0; float AcZ = 0; float Tmp = 0; float AcV = 0;
float GyX = 0; float GyY = 0; float GyZ = 0;

void setup() {
  
  Serial.begin(9600);
  Wire.begin();
  
  pixels.begin();
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  oled.setTextSize(1);          // text size
  oled.setTextColor(WHITE);     // text color

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

void loop() {

  Wire.beginTransmission(0x68);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(0x68,14,true);   // Wertebereiche Standart +-2g, +-500°/s von-32767 bis 32767  1g=16.383
  AcX = Wire.read()<<8|Wire.read();
  AcY = Wire.read()<<8|Wire.read();
  AcZ = Wire.read()<<8|Wire.read();
  Tmp = Wire.read()<<8|Wire.read();
  GyX = Wire.read()<<8|Wire.read();// x Roll d
  GyY = Wire.read()<<8|Wire.read();// y Pitch c
  GyZ = Wire.read()<<8|Wire.read();// z Yaw b

  Serial.print(AcX);
  Serial.println('\t');

  oled.clearDisplay(); // clear display
  oled.setCursor(0, 10);        // position to display
  oled.println(AcX); // text to display
  oled.display();               // show on OLED
  
  pixels.setPixelColor(0, pixels.Color(20, 20, 0)); // green, red, blue
  pixels.show();
  delay(50);
  
  pixels.setPixelColor(0, pixels.Color(0, 0, 0));
  pixels.show();
  delay(50);
  
}

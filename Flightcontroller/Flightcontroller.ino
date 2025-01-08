

  #include <Wire.h>
  #include <math.h>

  #include <Adafruit_PWMServoDriver.h>
  Adafruit_PWMServoDriver myServos = Adafruit_PWMServoDriver();

  #include <SoftwareSerial.h>
  SoftwareSerial hc12(1, 0); // Rx, Tx

  #include <Adafruit_NeoPixel.h>
  Adafruit_NeoPixel pixels( 1, 16, NEO_GRB + NEO_KHZ800);

  #include "FastIMU.h"
  BMI160 IMU; 
  calData calib = { 0 };  //Calibration data
  AccelData accelData;    //Sensor data
  GyroData gyroData;

  //___Filter_________________________________

  #define Comb_W               0.8     // 
  #define Lpf_Gy              30        // 
  #define Lpf_Ac              80        // 
  #define Lpf_W              100        // 
  #define Lpf_Empf            20        // 
  #define Ts                  20        // 
  #define f_aI                 0.4      //
  #define f_Ctrl_d             3

  #define f_levelUP_cnt       60
  #define f_levelUP_AcZ      600
  #define f_levelUP_Pow       30
  #define f_levelUP_Hov       15
  #define f_schief_W          10

  //___Limits_________________________________

  #define ESC_min            223        // 223
  #define ESC_max            400        // 400
  #define SERVO_min          100
  #define SERVO_max          500

  #define a_max              100        // 80
  #define b_max               30        // 30
  #define c_max               90        // 30
  #define d_max               30        // 30

  #define AsollMax           100        // 
  #define BsollMax           100        // 
  #define CsollMax           100        // 
  #define DsollMax           100        // 

  #define R_Roll_Ksat     100000        // 
  #define R_Roll_Ag_Ksat  100000        // 
  #define R_Pitch_Ksat    100000        // 
  #define R_Pitch_Ag_Ksat 100000        // 
  #define R_Yaw_Ksat      100000        // 
  #define R_TrZ_Ksat         100        //

  #define Cutoff_Angle        40        // 
  #define Cutoff_Read         30        // 10

  //___Parameter_Rot________________________________

  #define R_Roll_Kp      0.14           // 0.05  0.2 0.18
  #define R_Roll_Ki      0.00           // 0.05 0
  #define R_Roll_Kd      0.15           // 0.1  0.1 0.11

  #define R_Roll_Ag_Kp   0.5            // 0.2 0.4 0.6
  #define R_Roll_Ag_Ki   0              // 
  #define R_Roll_Ag_Kd   0.4            // 0.2

  #define R_Pitch_Kp     0.14           // 0.05  0.2 0.18
  #define R_Pitch_Ki     0.00           // 0.05 0
  #define R_Pitch_Kd     0.15           // 0.1  0.1 0.11

  #define R_Pitch_Ag_Kp  0.5            // 0.2 0.4 0.6
  #define R_Pitch_Ag_Ki  0              // 
  #define R_Pitch_Ag_Kd  0.4            // 0.2

  #define R_Yaw_Kp       0.2            // 0.2
  #define R_Yaw_Ki       0.1            // 0.1
  #define R_Yaw_Kd       0.1            // 0.1

  //___Parameter_Trans________________________________

  #define R_TrZ_Kp       0.005          // 0
  #define R_TrZ_Ki       0.00           // 0
  #define R_TrZ_Kd       0.00           // 0

//__Variablen_Angle____________________________________

  float AcX = 0; float AcY = 0; float AcZ = 0; float Tmp = 0; float AcV = 0;
  float GyX = 0; float GyY = 0; float GyZ = 0;
  float Wx = 0;  float Wy = 0;  float Wz = 0;
  float Wxf = 0; float Wyf = 0; float Wzf = 0;

//__Variablen_Ctrl_____________________________________

  int mod  = 0;
  int a  = 0; int b  = 0; int c  = 0; int d  = 0;
  int aE = 0; int bE = 0; int cE = 0; int dE = 0;
  int ap = 0; int bp = 0; int cp = 0; int dp = 0;
  int aEp = 0; int bEp = bE; int cEp = cE; int dEp = dE;

//__Variablen_Regler_Rot_______________________________

  float R_Roll_e = 0;  float R_Roll_Ag_e = 0;
  float R_Pitch_e = 0; float R_Pitch_Ag_e = 0;
  float R_Yaw_e = 0;

  float R_Roll_i = 0;  float R_Roll_Ag_i = 0;
  float R_Pitch_i = 0; float R_Pitch_Ag_i = 0;
  float R_Yaw_i = 0;

  float R_Roll_sat = 0;  float R_Roll_Ag_sat = 0;
  float R_Pitch_sat = 0; float R_Pitch_Ag_sat = 0;
  float R_Yaw_sat = 0;

//__Variablen_Regler_Trans_____________________________

  byte levelUP = 0; byte countUP = 0;
  float R_TrZ_e = 0;
  float R_TrZ_i = 0;
  float R_TrZ_sat = 0;

//__Variablen_Weitere__________________________________

  byte led = 0; 
  byte count = 0; byte error = 0; 
  byte radio_send = 0; 
  unsigned long Tr1p = 0; float Tr1 = 0;
  unsigned long Tr2p = 0; float Tr2 = 0;
  float Ts_s = Ts * 0.001;

//___Run_________________________________________________________

void setup() { Setup(); }

void loop() {

  if ( error == 0 ) {
      pixels.setPixelColor(0, pixels.Color(1, 0, 0));
      pixels.show(); }

  if ( error == 1 ) {
      pixels.setPixelColor(0, pixels.Color(1, 1, 0));
      pixels.show(); }

  if ( error == 2 ) {
      pixels.setPixelColor(0, pixels.Color(0, 255, 0)); // grb
      pixels.show(); }

//___Routine_______________________________________________________

  if ( radio_send == 0 ) {
    hc12.write(error); radio_send = 1;
  }

  Radio ();
  Angle ();

  if ( error == 0 ) {
    Regler_Rot(); 
    Regler_Trans(); 
    Stellen();
  }
  if ( error == 1 ) {
    aE = -200; bE = 0; cE = 0; dE = 0;
    Regler_Rot(); 
    Regler_Trans(); 
    Stellen();
  }
  if ( error == 2 ) {
    Off();
  }
  
  Tr1 = ( millis() - Tr1p );

  for ( 0 ; Tr1 < Ts ; 0 ) {
    Tr1 = ( millis() - Tr1p );
  }

  Tr2 = ( millis() - Tr2p );

  Serial.print(map(c, 0, 200, 90, 500));
  Serial.print('\t');
  Serial.print(c);
  Serial.print('\t');
  Serial.println(count);

  Tr1p = millis();
  Tr2p = millis();

} 

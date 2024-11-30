
#include <Wire.h>
#include <math.h>
#include <Adafruit_PWMServoDriver.h>
#include <SoftwareSerial.h>
Adafruit_PWMServoDriver myServos = Adafruit_PWMServoDriver();
SoftwareSerial hc12(10, 11);

//___Filter_________________________________

#define Comb_W               0.95     // 
#define Lpf_Gy              30        // 
#define Lpf_Ac              80        // 
#define Lpf_W               30        // 
#define Lpf_Empf            20        // 
#define Ts                  20        // 
#define f_aI                 0.4      //
#define f_Ctrl_d             3

#define f_levelUP_cnt       60
#define f_levelUP_AcZ      600
#define f_levelUP_Pow      30
#define f_levelUP_Hov       15
#define f_schief_W          10

//___Limits_________________________________

#define ESC_min            223        // 223
#define ESC_max            400        // 400

#define a_max              100        // 80
#define b_max               30        // 30
#define c_max               30        // 30
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

  float a  = 0; float b  = 0; float c  = 0; float d  = 0;
  int aE = 0; int bE = 0; int cE = 0; int dE = 0;

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
  byte radio = 0; 
  unsigned long Tr1p = 0; float Tr1 = 0;
  float Ts_s = Ts * 0.001;

//___Run_________________________________________________________

void setup() { Setup(); }

void loop() {

  if (led == 0) digitalWrite(LED_BUILTIN, HIGH);
  if (led == 1) digitalWrite(LED_BUILTIN, LOW);
  led++;
  if (led == 2) led = 0; // next: Blinkwerte nach Modi

//___Routine_______________________________________________________

  if ( radio == 0 ) {
    hc12.write(led); radio = 1;
  }

// Werte t-1 speichern
  float ap = a; float bp = b; float cp = c; float dp = d;
  int aEp = aE; int bEp = bE; int cEp = cE; int dEp = dE;

// Wenn neues Radio-Datenpacket im Buffer abliegt plus Limitierung der Sollgrößen
  if ( hc12.available() ) {

    aE = hc12.parseInt(); bE = hc12.parseInt(); cE = hc12.parseInt(); dE = hc12.parseInt();
    aE = map(aE, 0, 200, -a_max, a_max); // Thrust a | x Roll d | y Pitch c | z Yaw b
    bE = map(bE, 0, 200, -b_max, b_max);
    cE = map(cE, 0, 200, -c_max, c_max);
    dE = map(dE, 0, 200, -d_max, d_max);

    count = 0; error = 0; radio = 0;
  }

// Einfluss der Änderung der Sollgrößen als Vorsteuerung
  int aV = aE;
  int bV = bE + ( bE - bEp ) * f_Ctrl_d;
  int cV = cE + ( cE - cEp ) * f_Ctrl_d;
  int dV = dE + ( dE - dEp ) * f_Ctrl_d;

// Glätten der Sollgrößen
  float alpha_Empf =  (Lpf_Empf * Ts_s) / (1 + Lpf_Empf * Ts_s);
  a = ap + alpha_Empf * (aV - ap);
  b = bp + alpha_Empf * (bV - bp);
  c = cp + alpha_Empf * (cV - cp);
  d = dp + alpha_Empf * (dV - dp);

// Ausführen von Lageberechnung und Lageregler
  Angle(); Regler_Rot(); Regler_Trans(); count++;

// Fehlerzähler wenn kein neues Radio-Datenpacket empfangen wurde
  if ( count == Cutoff_Read ) error = 1;
  if ( count  > Cutoff_Read + 5 ) {
    radio = 0; count = Cutoff_Read;
  }

// Abschaltung der Motoren bei Fehler
  if ( error == 0 ) Stellen();
  if ( error == 1 ) Off();

// Gewährleistung einer konstanten Berechnungsdauer bzw. Zyklusdauer
  Tr1 = ( millis() - Tr1p ); // Aktuelle Zyklusdauer
  
  for ( 0 ; Tr1 < Ts ; 0 ) { // Warten bis konstant definierte Zyklusdauer erreicht ist
    Tr1 = ( millis() - Tr1p );
  }

  Tr1p = millis(); // Startwert neuer Zyklus

} 

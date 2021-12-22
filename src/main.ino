#include <Arduino.h>

#define THERMISTOR A0
#define THERMOPILE A1

float V2 = 5, Vth, Vtp, Vout, Rth, Tamb, Tobj;
const float Rref = 100e3;     // Thermistor voltage divider reference resistance (ohms)
const float R1 = 1e3, // Ohms
            R2 = 330e3, 
            R3 = 120e3, 
            R4 = 120;


const double c = 1.349e-12;    // Thermopile constant (s*a*\omega*\epsilon)
const double A = 7.42031e-4, B = 2.12125e-4, C = 1.11286e-7;    // Steinhart-Hart coefficients (for ambient temperature)

void setup() {
  pinMode(THERMISTOR, INPUT);
  pinMode(THERMOPILE, INPUT);

  Serial.begin(9600);
}

void loop() {

  // Read inputs and convert ADC data into an analogue value (0-5 Volts)
  Vth = analogRead(THERMISTOR) * (5.0 / 1023);
  Vout = analogRead(THERMOPILE) * (5.0 / 1023);

  Rth = Rref * (Vth / (V2 - Vth));  // Determine thermistor resistance from thermistor voltage

  // Use Steinhart-Hart equation to determine ambient temperature using Rth
  Tamb = (1 / (A + B * log(Rth) + C * pow(log(Rth), 3))) - 273.15;

  // Undo offset and scaling to get actual thermopile voltage (mV range)
  Vtp = (R1 / (R2 + R1)) * ((Vout - 142e-3) - (V2 * R4 / (R3 + R4));

  // Determine object temperature (in degrees C) using Stefan-Boltzman law
  Tobj = pow((Vtp / c) + pow(Tamb + 273.15, 4), 0.25) - 273.15;


  delay(200); // 200ms delay
}

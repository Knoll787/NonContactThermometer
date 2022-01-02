#include <Arduino.h>

#define THERMISTOR		A0
#define THERMOPILE 		A1
#define BTN		   		D7
#define DIGIT1	   		D4
#define DIGIT2	   		D5
#define DIGIT3	   		D6

float Vcc = 5.0,		// Voltage (V): Main Power Supply Voltage
      Vrt,              // Voltage (V): Thermistor
      Vtp,				// Voltage (V): Thermopile
      Vtc,				// Voltage (V): Output of thermocouple circuit
      Tamb,				// Celcius (°C): Ambient Air Temperature
      Tobj,				// Celcius (°C): Temperature of the object being measured
      Rth = 1e3,		// Resistance (Ω): Thermistor Resistance
      R1 = 1e3,			// Resistance (Ω): Resistor Value
      R2 = 330e3,		// Resistance (Ω): Resistor Value
      R3 = 500e3,		// Resistance (Ω): Resistor Value
      R4 = 500;			// Resistance (Ω): Resistor Value
      R5 = 100e3,		// Resistance (Ω): Resistor Value


// Steinhart-Hart coefficients (for ambient temperature)
const double A = 0.6339663380e-3;
const double B = 2.2192271060e-4;
const double C = 1.0817136530e-7;

//const double c = 1.349e-12;    // Thermopile constant (s*a*\omega*\epsilon)

float calc(void) {
    // Read inputs and convert ADC data into an analogue value (0-5 Volts)
    Vrt = analogRead(THERMISTOR) * (5.0 / 1023);
    Vtc = analogRead(THERMOPILE) * (5.0 / 1023);

    Rth = R5 * (Vth / (Vcc - Vth));  // Determine thermistor resistance from thermistor voltage

    // Use Steinhart-Hart equation to determine ambient temperature using Rth
    Tamb = (1 / (A + B * log(Rth) + C * pow(log(Rth), 3))) - 273.15;

    // Undo offset and scaling to get actual thermopile voltage (mV range)
    Vtp = (R1 / (R2 + R1)) * ((Vout - 142e-3) - (Vcc * R4 / (R3 + R4));

    // Determine object temperature (°C) using Stefan-Boltzman Law
    Tobj = pow((Vtp / c) + pow(Tamb + 273.15, 4), 0.25) - 273.15;
    return Tobj
}

void display(float temp) {
	int digit1 = ((int)temp / 10) % 10;
	int digit2 = (int)temp % 10;
	int digit3 = (int)((temp * 10.0)) % 10;

	digitalWrite(DIGIT1, LOW);
	digitalWrite(D0, (digit1 & 0b0001));
	digitalWrite(D1, (digit1 & 0b0010) >> 1);
	digitalWrite(D2, (digit1 & 0b0100) >> 2 );
	digitalWrite(D3, (digit1 & 0b1000) >> 3);


	digitalWrite(DIGIT2, LOW);
	digitalWrite(D0, (digit2 & 0b0001));
	digitalWrite(D1, (digit2 & 0b0010) >> 1);
	digitalWrite(D2, (digit2 & 0b0100) >> 2);
	digitalWrite(D3, (digit2 & 0b1000) >> 3);

	digitalWrite(DIGIT3, LOW);
	digitalWrite(D0, (digit3 & 0b0001));
	digitalWrite(D1, (digit3 & 0b0010) >> 1);
	digitalWrite(D2, (digit3 & 0b0100) >> 2);
	digitalWrite(D3, (digit3 & 0b1000) >> 3);


	return
}


void setup() {
	pinMode(THERMISTOR, INPUT);
	pinMode(THERMOPILE, INPUT);
	pinMode(BTN, INPUT);			// Button: Trigger to get Temp
	pinMode(DIGIT1, OUTPUT);		// Latch Enable: Left
	pinMode(DIGIT2, OUTPUT);		// Latch Enable: Middle
	pinMode(DIGIT3, OUTPUT);		// Latch Enable: Right
	pinMode(D0, OUTPUT);			// Hex Seg Input
	pinMode(D1, OUTPUT);			// Hex Seg Input
	pinMode(D2, OUTPUT);			// Hex Seg Input
	pinMode(D3, OUTPUT);			// Hex Seg Input

	Serial.begin(9600);
}

void loop() {
	if(digitalRead(BTN) == HIGH)
		display(calc());
}

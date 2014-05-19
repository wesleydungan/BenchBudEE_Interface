#include <Arduino.h>
#include "BenchBudEE.h"


// analog
const int kPOSITIVE_VOLTAGE_READING = A0;
const int kNEGATIVE_VOLTAGE_READING = A1;
const int kFAN_CURRENT_READING = A2;
const int kFAN_TACHOMETER_READING = A3;

// digital
const int kFAN_CURRENT_MEASUREMENT_STATE = 0;
const int kDAC__SHDN = 2;
const int kDAC__LDAC = 3;
const int kADC__DR = 4;
const int kADC__RESET = 5;
const int kRELAY_STATE = 6;
const int kDAC__CS = 7;
const int kADC__CS = 8;
const int kLED_PWM = 9;
const int kFAN_CURRENT_PWM = 10;
const int kMISO = 11;
const int kMOSI = 12;
const int kSCK = 13;


void setup_io_pins()
{
  pinMode(kFAN_CURRENT_MEASUREMENT_STATE, OUTPUT);
  set_fan_current_measurement_state(kFanCurrentMeasurementOff);

  pinMode(kDAC__SHDN, OUTPUT);
  digitalWrite(kDAC__SHDN, HIGH);

  pinMode(kDAC__LDAC, OUTPUT);
  digitalWrite(kDAC__LDAC, HIGH);

  pinMode(kADC__DR, OUTPUT);
  digitalWrite(kADC__DR, HIGH);

  pinMode(kADC__RESET, OUTPUT);
  digitalWrite(kADC__RESET, HIGH);

  pinMode(kRELAY_STATE, OUTPUT);
  set_relay_state(kRelayOff);

  pinMode(kDAC__CS, OUTPUT);
  digitalWrite(kDAC__CS, HIGH);

  pinMode(kADC__CS, OUTPUT);
  digitalWrite(kADC__CS, HIGH);

  pinMode(kLED_PWM, OUTPUT);
  digitalWrite(kLED_PWM, LOW);

  pinMode(kFAN_CURRENT_PWM, OUTPUT);
  digitalWrite(kFAN_CURRENT_PWM, LOW);

  pinMode(kMOSI, OUTPUT);
  digitalWrite(kMOSI, LOW);

  pinMode(kMISO, INPUT);

  pinMode(kSCK, OUTPUT);
  digitalWrite(kSCK, LOW);
}


void cycle_clock()
{
  digitalWrite(kSCK, HIGH);
  delay(1);
  digitalWrite(kSCK, LOW);
  delay(1);
}


//
// Voltage Monitoring
//

uint16_t get_positive_voltage_reading()
{
  return 0;
}


uint16_t get_negative_voltage_reading()
{
  return 0;
}


//
// Fan
//

void set_fan_current_measurement_state(FanCurrentMeasurementState state)
{
  if (state == kFanCurrentMeasurementOn)
  {
    digitalWrite(kFAN_CURRENT_MEASUREMENT_STATE, HIGH);
  }
  else
  {
    digitalWrite(kFAN_CURRENT_MEASUREMENT_STATE, LOW);
  }
}


void set_fan_current_pwm_value(uint8_t value)
{
}


void set_fan_current_limit_value(uint8_t value)
{
  /*
    WRITE COMMAND REGISTER FOR MCP4801 (8-BIT DAC)
    0 - _GA _SHDN D7 D6 D5 D4 D3 D2 D1 D0 X X X X

    bit 15    0 = Write to DAC register
              1 = Ignore this command

    bit 14    - Don’t Care

    bit 13    _GA: Output Gain Selection bit
               1 = 1x (VOUT = VREF * D/4096)
               0 = 2x (VOUT = 2 * VREF * D/4096), where internal VREF = 2.048V.

    bit 12    _SHDN: Output Shutdown Control bit
                1 = Active mode operation.
                0 = Shutdown the device.

    bit 11-0  D11:D0: DAC Input Data bits. Bit X is ignored.
  */

  digitalWrite(kDAC__CS, LOW);
  digitalWrite(kDAC__LDAC, HIGH);
  delay(1);

  // 15, 14, 13
  digitalWrite(kMOSI, LOW);
  cycle_clock();
  cycle_clock();
  cycle_clock();

  // 12
  digitalWrite(kMOSI, HIGH);
  cycle_clock();

  // msb first (11 - 4)
  for (int i = 7; i >= 0; i--)
  {
    int bit = (value >> i) & 0x01;
    digitalWrite(kMOSI, (bit == 0) ? LOW : HIGH);
    cycle_clock();
  }

  // don't care padding (3 - 0)
  digitalWrite(kMOSI, 0);
  cycle_clock();
  cycle_clock();
  cycle_clock();
  cycle_clock();

  digitalWrite(kDAC__CS, HIGH);
  delay(1);

  digitalWrite(kDAC__LDAC, LOW);
  delay(1);
  digitalWrite(kDAC__LDAC, HIGH);
  delay(1);
}


uint16_t get_fan_current_reading()
{
  return 0;
}


uint16_t get_fan_tachometer_reading()
{
  return 0;
}


//
// LED
//

void set_led_pwm_value(uint8_t value)
{
}


//
// Instrumentation Amp
//

uint16_t get_instrumentation_amp_reading()
{
  return 0;
}


uint16_t get_temperature_reading()
{
  return 0;
}


//
// Relay
//

void set_relay_state(RelayState state)
{
  if (state == kRelayOn)
  {
    digitalWrite(kRELAY_STATE, HIGH);
  }
  else
  {
    digitalWrite(kRELAY_STATE, LOW);
  }
}
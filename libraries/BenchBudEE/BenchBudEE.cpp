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


//
// Utilities
//

void setup_io_pins()
{
  pinMode(kFAN_CURRENT_MEASUREMENT_STATE, OUTPUT);
  set_fan_current_measurement_state(kFanCurrentMeasurementOff);

  pinMode(kDAC__SHDN, OUTPUT);
  digitalWrite(kDAC__SHDN, HIGH);

  pinMode(kDAC__LDAC, OUTPUT);
  digitalWrite(kDAC__LDAC, HIGH);

  pinMode(kADC__DR, INPUT);

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


void spi_write(uint8_t value)
{
  for (int i = 7; i >= 0; i--)
  {
    int bit = (value >> i) & 0x01;
    digitalWrite(kMOSI, (bit == 0) ? LOW : HIGH);
    cycle_clock();
  }
}


uint8_t spi_read()
{
  uint8_t value = 0;

  for (int i = 7; i >= 0; i--)
  {
    int bit = digitalRead(kMISO) & 0x01;
    value |= (bit << i);
    cycle_clock();
  }

  return value;
}


//
// Voltage Monitoring
//

uint16_t get_positive_voltage_reading()
{
  return analogRead(kPOSITIVE_VOLTAGE_READING);
}


uint16_t get_negative_voltage_reading()
{
  return analogRead(kNEGATIVE_VOLTAGE_READING);
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
  analogWrite(kFAN_CURRENT_PWM, value);
}


// DAC - MPC4801
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

  uint16_t command = 0;

  // 15, 14 are zero

  command |= 1 << 13;
  command |= 1 << 12;

  command |= static_cast<uint16_t>(value) << 4;

  // 3:0 are zero

  spi_write((command & 0xff00) >> 8);
  spi_write((command & 0x00ff) >> 0);

  digitalWrite(kDAC__CS, HIGH);
  delay(1);

  digitalWrite(kDAC__LDAC, LOW);
  delay(1);
  digitalWrite(kDAC__LDAC, HIGH);
  delay(1);
}


uint16_t get_fan_current_reading()
{
  return analogRead(kFAN_CURRENT_READING);
}


uint16_t get_fan_tachometer_reading()
{
  return analogRead(kFAN_TACHOMETER_READING);
}


//
// LED
//

void set_led_pwm_value(uint8_t value)
{
  analogWrite(kLED_PWM, value);
}


// ADC - MPC3901
void configure_adc()
{
#if 0
  // only configure the adc once; this didn't work too well because turning the
  // BenchBudEE off and back on would not cause the ADC to be configured again
  static bool configure_adc_complete = false;

  if (configure_adc_complete)
  {
    return;
  }
  else
  {
    configure_adc_complete = true;
  }
#endif

  // reset
  digitalWrite(kADC__RESET, LOW);
  delay(1);
  digitalWrite(kADC__RESET, HIGH);
  delay(1);

  digitalWrite(kADC__CS, LOW);

  uint8_t command = 0;

  //
  // enable the ADC's external clock mode
  //

  // control byte structure
  // 7, 8 - device address bits
  // 6:1 - register address bits
  // 0 - read / ~write

  command = 0;

  // 0x0b is Config2
  command |= 0x0b << 1;

  // read / ~write
  command |= 0 << 0;

  spi_write(command);

  command = 0;

  // RESET_CH<1:0>
  command |= 0 << 7;
  command |= 0 << 6;

  // SHUTDOWN_CH<1:0>
  command |= 0 << 5;
  command |= 0 << 4;

  // DITHER_CH<1:0>
  command |= 1 << 3;
  command |= 1 << 2;

  // VREFEXT
  command |= 0 << 1;

  // CLKEXT
  command |= 0 << 0;

  spi_write(command);

  // a chip select toggle is needed to end the current command
  digitalWrite(kADC__CS, HIGH);

  // this delay makes debugging easier
  delay(5);
}


//
// Instrumentation Amp
//

uint16_t get_instrumentation_amp_reading()
{
  uint16_t value = 0;

  configure_adc();

  // read from ADC
  //

  digitalWrite(kADC__CS, LOW);

  uint8_t command = 0;

  // 0x00-0x02 is CH0
  command |= 0x00 << 1;

  // read / ~write
  command |= 1 << 0;

  spi_write(command);

  // this delay makes debugging easier
  delay(1);

  value |= static_cast<uint16_t>(spi_read()) << 8;
  value |= static_cast<uint16_t>(spi_read()) << 0;

  digitalWrite(kADC__CS, HIGH);

  return value;
}


uint16_t get_temperature_reading()
{
  uint16_t value = 0;

  configure_adc();

  // read from ADC
  //

  digitalWrite(kADC__CS, LOW);

  uint8_t command = 0;

  // 0x03-0x05 is CH1
  command |= 0x03 << 1;

  // read / ~write
  command |= 1 << 0;

  spi_write(command);

  // this delay makes debugging easier
  delay(1);

  value |= static_cast<uint16_t>(spi_read()) << 8;
  value |= static_cast<uint16_t>(spi_read()) << 0;

  digitalWrite(kADC__CS, HIGH);

  return value;
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

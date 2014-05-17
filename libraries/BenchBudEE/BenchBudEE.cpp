#include "BenchBudEE.h"


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

void set_fan_current_measurement_state(CurrentMeasurementState state)
{
}


void set_fan_current_pwm_value(uint8_t value)
{
}


void set_fan_current_limit_value(uint16_t value)
{
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
}

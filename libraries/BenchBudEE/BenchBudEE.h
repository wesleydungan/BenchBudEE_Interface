#include <stdint.h>


enum CurrentMeasurementState
{
  kCurrentMeasurementDisable,
  kCurrentMeasurementEnable
};


enum RelayState
{
  kRelayOff,
  kRelayOn
};


// Voltage Monitoring
uint16_t get_positive_voltage_reading();
uint16_t get_negative_voltage_reading();


// Fan
void set_fan_current_measurement_state(CurrentMeasurementState state);
void set_fan_current_pwm_value(uint8_t value);
void set_fan_current_limit_value(uint16_t value);
uint16_t get_fan_current_reading();
uint16_t get_fan_tachometer_reading();


// LED
void set_led_pwm_value(uint8_t value);


// Instrumentation Amp
uint16_t get_instrumentation_amp_reading();
uint16_t get_temperature_reading();


// Relay
void set_relay_state(RelayState state);

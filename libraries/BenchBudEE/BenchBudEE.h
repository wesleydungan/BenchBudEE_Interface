#include <stdint.h>


enum FanCurrentMeasurementState
{
  kFanCurrentMeasurementOff,
  kFanCurrentMeasurementOn
};


enum RelayState
{
  kRelayOff,
  kRelayOn
};


void setup_io_pins();


// Voltage Monitoring
uint16_t get_positive_voltage_reading();
uint16_t get_negative_voltage_reading();


// Fan
void set_fan_current_measurement_state(FanCurrentMeasurementState state);
void set_fan_current_pwm_value(uint8_t value);
void set_fan_current_limit_value(uint8_t value);
uint16_t get_fan_current_reading();
uint16_t get_fan_tachometer_reading();


// LED
void set_led_pwm_value(uint8_t value);


// Instrumentation Amp
int16_t get_channel_reading(int channel_number);
double get_channel_voltage(int channel_number);
double get_temperature();


// Relay
void set_relay_state(RelayState state);

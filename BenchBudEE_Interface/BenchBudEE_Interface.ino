#include <BenchBudEE.h>
#include <stdlib.h>
#include <string.h>


void setup()
{
  Serial.begin(9600);

  while (!Serial) {
    // wait until the serial port is communicating
  }

  Serial.print("BenchBudEE Version 1.0\n");
}


void unknown_command(char* command_buffer)
{
  Serial.print("Error: '");
  Serial.print(command_buffer);
  Serial.print("' is an unknown BenchBudEE command\n");
}


void return_value(uint16_t value)
{
  char value_string[8];
  sprintf(value_string, "0x%04x\n", value);
  Serial.print(value_string);
}


void process_set_command(char* command_buffer)
{
  uint16_t value = strtol(&command_buffer[3], NULL, 16);

  if (strncmp(command_buffer, "scm", 3) == 0)
  {
    // 
    CurrentMeasurementState state = kCurrentMeasurementDisable;

    if (value > 0)
    {
      state = kCurrentMeasurementEnable;
    }

    set_fan_current_measurement_state(state);
  }
  else if (strncmp(command_buffer, "scp", 3) == 0)
  {
    set_fan_current_pwm_value(value);
  }
  else if (strncmp(command_buffer, "scl", 3) == 0)
  {
    set_fan_current_limit_value(value);
  }
  else if (strncmp(command_buffer, "slp", 3) == 0)
  {
    set_led_pwm_value(value);
  }
  else if (strncmp(command_buffer, "s_r", 3) == 0)
  {
    RelayState state = kRelayOff;

    if (value > 0)
    {
      state = kRelayOn;
    }

    set_relay_state(state);
  }
  else
  {
    unknown_command(command_buffer);
    // unknown commands should not reach the return_value() call
    return;
  }
  
  return_value(value);
}


void process_get_command(char* command_buffer)
{
  uint16_t value = 0;

  if (strcmp(command_buffer, "gpv") == 0)
  {
    value = get_positive_voltage_reading();
  }
  else if (strcmp(command_buffer, "gnv") == 0)
  {
    value = get_negative_voltage_reading();
  }
  else if (strcmp(command_buffer, "g_c") == 0)
  {
    value = get_fan_current_reading();
  }
  else if (strcmp(command_buffer, "gft") == 0)
  {
    value = get_fan_tachometer_reading();
  }
  else if (strcmp(command_buffer, "gia") == 0)
  {
    value = get_instrumentation_amp_reading();
  }
  else if (strcmp(command_buffer, "g_t") == 0)
  {
    value = get_temperature_reading();
  }
  else
  {
    unknown_command(command_buffer);
    // unknown commands should not reach the return_value() call
    return;
  }

  return_value(value);
}


void loop()
{
  if (Serial.available() > 0)
  {
    char command_buffer[256];
    int command_buffer_index;

    while (Serial.available() > 0)
    {
      char c = Serial.read();
      command_buffer[command_buffer_index] = tolower(c);
      command_buffer_index++;
      delay(100);
    }

    // terminate the command string
    command_buffer[command_buffer_index] = '\0';

    Serial.print("[BenchBudEE]> ");
    Serial.print(command_buffer);
    Serial.print("\n");

    if ((command_buffer_index == 3) && (command_buffer[0] == 'g'))
    {
      process_get_command(command_buffer);
    }
    else if ((command_buffer_index == 5) && (command_buffer[0] == 's'))
    {
      process_set_command(command_buffer);
    }
    else
    {
      unknown_command(command_buffer);
    }
  }
}


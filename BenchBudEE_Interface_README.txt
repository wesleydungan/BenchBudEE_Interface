BenchBudEE Interface Serial Commands

These commands are designed around the idea that the BenchBudEE will
be a slave device controlled (at least for now) by a master program
running on a PC. Initially, I anticipate this will be the Arduino SDK
"Serial Monitor" interface.

SET / WRITE COMMANDS
Cmd    Description                               Value Size  Value Range
----   -----------                               ----------  -----------
scmXX  Set Fan Current Measurement State            8-bit      0, 1
scpXX  Set Fan Current PWM                          8-bit      0x00 - 0xff
sclXX  Set Fan Current Limit                        8-bit      0x00 - 0xff
slpXX  Set LED PWM                                  8-bit      0x00 - 0xff
s_rXX  Set Relay State                              8-bit      0, 1


GET / READ COMMANDS
Cmd    Description                               Value Size  Value Range
----   -----------                               ----------  -----------
gpv    Get Positive Adjustable Voltage Reading     16-bit      0x000 - 0x3ff (10-bits usable)
gnv    Get Negative Adjustable Voltage Reading     16-bit      0x000 - 0x3ff (10-bits usable)
g_c    Get Fan Current Reading                     16-bit      0x0000 - 0xffff
gft    Get Fan Tachometer Reading                  16-bit      0x000 - 0x3ff (10-bits usable)
g_t    Get Temperature Reading                     16-bit      0x0000 - 0xffff
gia    Get Instrumentation Amp Reading             16-but      0x0000 - 0xffff

When a serial link is estabilished, the BenchBudEE will respond with a
version message.

All received commands will be echoed back. A second response line will
also be sent, depending on the result of the command, either...

  OK: <16-bit hex value>

or...

  ERROR: <error message>

Both the set and get commands will return 16-bit hex values on
success. In the case of the set commands, the hex value will be the
value that was passed as part of the issued command. In the case of
the get commands, the hex value will be the return value for the
command that was issued.

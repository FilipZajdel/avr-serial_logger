## avr-serial_logger

Project of device that can sense temperature and voltage on its ADC. Device sends data through serial port (8 bits, no parity, 1 stop bit, 9600 baud) when requested by another device. 

Possible commands that can be sent to device are specified in [message decoder](https://github.com/FilipZajdel/avr-components/tree/master/system/message_decoder) and can be changed for the need. Also handling of each command can be added/changed in [main](https://github.com/FilipZajdel/avr-serial_logger/blob/master/main/main.c)

Following commands are handled now:
* "temperature" - gets reading from ds18b20
* "adc" - gets reading from ADC


[Building the project](#building-the-project)

[Flashing](#flashing)

[interfacing-with-pc](#interfacing-with-pc)

## Building the project

In order to build the project, avr toolchain and Cmake are required.

In avr-serial_logger directory: 
```
git clone --recursive https://github.com/FilipZajdel/avr-serial_logger
cd avr-serial_logger
mkdir build
cd build
cmake ..
make -j8
```

## Flashing 

.hex file can be flashed using avrdude
```
avrdude -p m328p -c usbasp -U flash:w:YOUR_FILE.hex
```
 
In case of any other ATmega mcu, specify compiler options in main 
   CMakeLists.txt

## Interfacing with PC

Device can communicate with PC using misceallenous applications like putty, screen, grabserial. Here's one written by me for purpose of saving data coming from device.

[Relog](https://github.com/FilipZajdel/AVRLoggerReader)

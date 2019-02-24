# avr-serial_logger

To clone repo with submodules: git clone --recursive (/avr_serial_logger)

# Building the project

To build the project Cmake is requred.

In avr-serial_logger directory: 
```
mkdir build
cd build
cmake ..
make -j8
```

# Flashing 

.hex file can be flashed using avrdude
```
avrdude -p m328p -c usbasp -U flash:w:YOUR_FILE.hex
```
 
In case of any other ATmega mcu, specify compiler options in main 
   CMakeLists.txt

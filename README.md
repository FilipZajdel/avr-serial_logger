# avr-serial_logger

To clone repo with submodules: git clone --recurse [REPO URL]

Following instructions show how build the project:

! To build the project Cmake is requred.

1. In avr-serial_logger directory: 
	mkdir build
	cd build
	cmake ..
	make -j8

2. The result is .hex file which can be used to flash ATmega 328p. 
3. In case of any other ATmega mcu, specify compiler options in main 
   CMakeLists.txt

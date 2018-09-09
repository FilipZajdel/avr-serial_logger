cmake_minimum_required(VERSION 2.8)
project(Serial_Logger)
set(target_name serial_logger)

set(CMAKE_C_COMPILER avr-gcc)
set(CMAKE_C_FLAGS "-std=c11 -g -Os -mmcu=atmega328p -Wcpp")
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")	# some shit, without it, it doesn't compile

add_subdirectory("avr-components/system/data_buffers")
add_subdirectory("avr-components/system/message_decoder")
add_subdirectory("avr-components/peripherals/usart")
add_subdirectory("avr-components/peripherals/timers")
add_subdirectory("avr-components/externals/DS18B20")
add_subdirectory("avr-components/peripherals/adc")

include_directories(${TIMERS_INCLUDE} ${DS18B20_INCLUDE} ${ADC_INCLUDE} ${DATA_BUFFERS_INCLUDE} ${MESSAGE_DECODER_INCLUDE} ${USART_INCLUDE})

add_executable(${target_name} main/main.c)
target_link_libraries(${target_name} usartlib adclib message_decoderlib data_bufferslib ds18b20lib timerslib)

# custom command needed to create .o and .hex files
add_custom_command(TARGET ${target_name} POST_BUILD COMMAND avr-objcopy -j .text -j .data -O ihex ${target_name} ${target_name}.hex)











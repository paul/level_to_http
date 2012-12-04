set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/arduino-cmake/cmake/ArduinoToolchain.cmake)

cmake_minimum_required(VERSION 2.8)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/libraries)

set(ARDUINO_C_FLAGS      "-ffunction-sections -fdata-sections")
set(ARDUINO_CXX_FLAGS    "${ARDUINO_C_FLAGS} -fno-exceptions")
set(ARDUINO_LINKER_FLAGS "-Wl,--gc-sections")

project(LevelToHTTP C CXX)

#generate_arduino_library(slideapp_client
#  SRCS   "src/level_to_http/slideapp_client.cpp"
#  HDRS   "src/level_to_http/slideapp_client.h"
#  BOARD  uno)

generate_arduino_firmware(level
  SKETCH "src/level_to_http"
  PORT   /dev/tty.usbmodem1421
  SERIAL screen @SERIAL_PORT@
  #LIBS   slideapp_client_lib
  BOARD  uno
)


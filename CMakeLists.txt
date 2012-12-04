set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/arduino-cmake/cmake/ArduinoToolchain.cmake)

cmake_minimum_required(VERSION 2.8)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/libraries)

project(LevelToHTTP C CXX)

generate_arduino_firmware(level
  SKETCH "src/level_to_http"
  PORT   /dev/tty.usbmodem1421
  SERIAL screen @SERIAL_PORT@
  BOARD  uno
)


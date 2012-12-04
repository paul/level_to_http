#include <Time.h>

#include <SPI.h>
#include <SD.h>

#include <HttpClient.h>
#include <Ethernet.h>
#include <EthernetClient.h>

#include <I2C.h>
#include <MMA8453_n0m1.h>

const int chipSelect = 4;
MMA8453_n0m1 accel;

const char hostname[] = "slideapp.dev";
const char timePath[] = "/current_time";

byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x9B, 0x55 };

const int networkTimeout = 30*1000;
const int networkDelay   = 1000;

const int accelUpdateInterval = 1;
const int sendUpdateInterval  = 10;

void setup() {
  Serial.begin(9600);

  Serial.print("Initializing SD Card... ");
  pinMode(10, OUTPUT);

  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed or not present");
    return;
  }
  Serial.println("Done");

  Serial.print("Initializing MMA8453 Accelerometer... ");
  accel.setI2CAddr(0x1D);
  accel.dataMode(true, 2); // 10bit, 2g range
  Serial.println("Done");

  Serial.print("Initializing Ethernet Connection... ");
  while (Ethernet.begin(mac) != 1) {
    Serial.println("Waiting for DHCP, trying again in 5s...");
    delay(5000);
  }
  Serial.println("DHCP Complete");

  debugMemory();

  Serial.print("Setting current time... ");
  EthernetClient c;
  HttpClient http(c);
  int err = 0;
  String timeStr = "";

  err = http.get(hostname, timePath);
  if (err == 0) {
    err = http.responseStatusCode();
    if (err == 200) {
      err = http.skipResponseHeaders();
      if (err >= 0) {
        unsigned long timeoutStart = millis();
        // Read while we haven't timed out or reached the end
        while ( (http.connected() || http.available()) && ((millis() - timeoutStart) < networkTimeout) ) {
          if (http.available()) {
            timeStr += (char)http.read();
            timeoutStart = millis();
          } else { // wait for more data
            delay(networkDelay);
          }
        }

        Serial.println(timeStr);
        setTime(timeStr.toInt());
        Serial.println(now());
      } else {
        Serial.println("Unable to skip headers");
      }
    } else {
      Serial.print("HTTP Error: ");
      Serial.println(err);
    }
  } else {
    Serial.print("Failed to connect to ");
    Serial.println(hostname);
  }

  http.stop();
  debugMemory();
}

void loop() {

  String data = "";

  accel.update();

  data += now();
  data += ",";
  data += accel.x();
  data += ",";
  data += accel.y();
  data += ",";
  data += accel.z();

  Serial.println(data);

  File dataFile = SD.open("data.csv", FILE_WRITE);

  if (dataFile) {
    dataFile.print(data);
    dataFile.close();
  }
  else {
    Serial.println("error opening file");
  }

  debugMemory();
  delay(accelUpdateInterval*1000);
}

void debugMemory() {
  Serial.print("Free: ");
  Serial.println(memoryFree());
}

// variables created by the build process when compiling the sketch
extern int __bss_end;
extern void *__brkval;
// function to return the amount of free RAM
int memoryFree() {
  int freeValue;
  if((int)__brkval == 0)
    freeValue = ((int)&freeValue) - ((int)&__bss_end);
  else
    freeValue = ((int)&freeValue) - ((int)__brkval);
  return freeValue;
}

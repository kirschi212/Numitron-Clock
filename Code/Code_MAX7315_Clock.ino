/* Please notice: All code regarding the setup of the WiFi and pulling the current from the time-server were not created by me.
 * Source of code: https://polluxlabs.net/esp8266-projekte/die-aktuelle-uhrzeit-mit-einem-esp8266-abfragen/
 */

#include <Wire.h>
#include <ESP8266WiFi.h>
#include <time.h>

// WiFi credentials
#ifndef STASSID
#define STASSID "XXX"    // set your SSID
#define STAPSK  "XXX" // set your WiFi password
#endif

// NTP configuration
#define MY_NTP_SERVER "0.de.pool.ntp.org"           
#define MY_TZ "CET-1CEST,M3.5.0/02,M10.5.0/03"   

// I2C Configuration
#define I2C_SDA 4
#define I2C_SCL 5

#define BUTTON 2

// I2C Expander Addresses
const int MAX1 = 0x27;
const int MAX2 = 0x24;
const int MAX3 = 0x22;
const int MAX4 = 0x20; 

// Number patterns for display
const byte number[] = {
  0b00100001,
  0b11111001,
  0b00010101,
  0b10010001,
  0b11001001,
  0b10000011,
  0b00000011,
  0b11110001,
  0b00000001,
  0b10000001
};

time_t now;                         // these are the seconds since Epoch (1970) - UTC
tm tm;

int button_state = 1;
bool second_state = false;
unsigned long lastSecondChange = 0;
unsigned long lastButtonChange = 0;

// Function to write a single byte to a register on an I2C device
int writeRegister(const int address, byte reg, byte val) {
  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.write(val);
  return Wire.endTransmission();
}

// Function to initialize I2C expanders
void initI2CExpander(const int address) {
  writeRegister(address, 0x03, 0x00); // Set all ports to outputs (configuration register)
  writeRegister(address, 0x01, 0xFF); // Turn off all outputs initially
}

// Function to write a number to a specific I2C expander
int writeNumberToExpander(const int address, byte val) {
  return writeRegister(address, 0x01, val);
}

void setup() {
  delay(1000);

  // Initialize I2C communication
  Wire.begin(I2C_SDA, I2C_SCL);
  Wire.setClock(100000);

  // Initialize I2C expanders
  initI2CExpander(MAX1);
  initI2CExpander(MAX2);
  initI2CExpander(MAX3);
  initI2CExpander(MAX4);

  configTime(MY_TZ, MY_NTP_SERVER);
  
  // Initialize WiFi connection
  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.begin(STASSID, STAPSK);
  while (WiFi.status() != WL_CONNECTED) {
    writeNumberToExpander(MAX4, 0b00000000);
    delay(50);
    writeNumberToExpander(MAX4, 0b00000001);
    delay(50);    
  }

  pinMode(BUTTON, INPUT);
}

void loop() {
  unsigned long currentMillis = millis();
    
  time(&now);                       // read the current time
  localtime_r(&now, &tm);

  int minutes, minute_ones, minute_tens, hours, hour_ones, hour_tens;

  // Check if 500ms went by after last decimal point update
  if (currentMillis - lastSecondChange >= 500) {
    second_state = !second_state; // invert second state
    lastSecondChange = currentMillis; // update time stamp
  }

  if (digitalRead(BUTTON) == LOW) { //read button state (input is pulled up to VCC)
    button_state = 0;
  } else 
  {
    button_state = 1;
  }
  
  hours = tm.tm_hour;
  minutes = tm.tm_min;

  minute_ones = minutes % 10;
  minute_tens = minutes / 10;

  hour_ones = hours % 10;
  hour_tens = hours / 10;

  if (button_state == 1) {
    writeNumberToExpander(MAX1, number[hour_tens]); 
    writeNumberToExpander(MAX2, number[hour_ones]);

    if (second_state) {
      writeNumberToExpander(MAX3, number[minute_tens] ^ 0b00000001); // update minutes with decimal point
      writeNumberToExpander(MAX4, number[minute_ones] ^ 0b00000001); // update minutes with decimal point
    } else {
      writeNumberToExpander(MAX3, number[minute_tens]); // update minutes without decimal point
      writeNumberToExpander(MAX4, number[minute_ones]); // update minutes without decimal point
    }
  } else {        // turn all segments off
    writeNumberToExpander(MAX1, 0xFF);
    writeNumberToExpander(MAX2, 0xFF);
    writeNumberToExpander(MAX3, 0xFF);
    writeNumberToExpander(MAX4, 0xFF);
  }
}

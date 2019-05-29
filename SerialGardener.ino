/**
 * This device relays on python scripts to make decisions from the host machine
 */

#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 2

#define PIN_MOIST_01 A2
#define PIN_MOIST_02 A3
#define PIN_MOIST_03 A6

#define PIN_LIGHT A7

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress thermometer01, thermometer02, thermometer03;

void setup() {
  Serial.begin(9600);
  Serial.println("<Gardener>");
  
  // Start thermometers
  sensors.begin();

  if (!sensors.getAddress(thermometer01, 0)) Serial.println("Thermometer 01 missing");
  if (!sensors.getAddress(thermometer02, 1)) Serial.println("Thermometer 02 missing");
  if (!sensors.getAddress(thermometer03, 2)) Serial.println("Thermometer 03 missing");
  
}

char rx_byte = 0;
String rx_str = "";

void loop() {
  if (Serial.available() > 0) {    // is a character available?
    rx_byte = Serial.read();       // get the character
    
    if (rx_byte != '\n') {
      // a character of the string was received
      rx_str += rx_byte;
    }
    else {
      // end of string
      if (rx_str.equals("temp")) {
        sensors.requestTemperatures();
        Serial.print("Temp->01:");
        printTemperature(thermometer01);
        Serial.print("02:");
        printTemperature(thermometer02);
        Serial.print("03:");
        printTemperature(thermometer03);
        Serial.println();
      }
      else if(rx_str.equals("light")) {
        Serial.print("Light->01:");
        Serial.print(analogRead(PIN_LIGHT));
        Serial.println(";");
      }
      else if(rx_str.equals("moist")) {
        Serial.print("Moist->01:");
        Serial.print(analogRead(PIN_MOIST_01));
        Serial.print(";02:");
        Serial.print(analogRead(PIN_MOIST_02));
        Serial.print(";03:");
        Serial.print(analogRead(PIN_MOIST_03));
        Serial.println(";");
      }
      else if(rx_str.equals("h02")) {
        Serial.println(34);
      }
      else if(rx_str.equals("h03")) {
        Serial.println(25);
      }
      else if(rx_str.equals("tankLvl")) {
        Serial.println(2);
      }
      else if(rx_str.equals("wtr01")) {
        Serial.println("OK");
      }
      else if(rx_str.equals("wtr02")) {
        Serial.println("OK");
      }
      else if(rx_str.equals("wtr03")) {
        Serial.println("OK");
      }
      else {
        Serial.println("Unknown Command");
      } 
      rx_str = "";                // clear the string for reuse
    }
  } // end: if (Serial.available() > 0)
}

// function to print the temperature for a device
void printTemperature(DeviceAddress deviceAddress)
{
  float tempC = sensors.getTempC(deviceAddress);
  Serial.print(tempC);
  Serial.print(";");
}


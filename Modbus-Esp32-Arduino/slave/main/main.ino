/*
  ModbusRTU ESP32 - Settup ESP32 as a Modbus RTU slave with a simple holding register initialized to 100. 
                    
  API documents: https://github.com/urboob21/modbus-esp8266/blob/master/documentation/API.md
*/

#include <ModbusRTU.h>

#define SLAVE_ID 1
#define REG_NUMBER  10

ModbusRTU modbus;

void setup()
{
  Serial.begin(9600, SERIAL_8N1); // RTU Commmunicates over serial as 9600 baud.
  modbus.begin(&Serial);
  modbus.slave(SLAVE_ID); // select and initialize slave mode, set ID

  // Add holding register at dress REG_NUMBER and initializes it with the value 100
  modbus.addHreg(REG_NUMBER); // add registers with offset REGN
  modbus.Hreg(REG_NUMBER, 100);  // write local register REGN to 10
  // modbus.removeHreg(REG_NUMBER);  // remove reg
}

void loop()
{
  modbus.task(); // Process transactions and manage Modbus communications.
  yield();       // Helps prevent system crashes caused by time-consuming functions
}
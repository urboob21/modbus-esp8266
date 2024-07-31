/*
  ModbusRTU ESP32 - Master
  Read/Write from slave device
    R/W   Output Coils      - 1bit
    Read  Input Contacts    - 1bit
    Read  Input Register    - 16bits
    R/W   Holding Register  - 16bits

  API documents: https://github.com/urboob21/modbus-esp8266/blob/master/documentation/API.md
*/

#include <ModbusRTU.h>

#define SLAVE_ID 1

#define HREG_ADDR 0 // 40001
#define HREG_COUNT 2

#define COIL_ADDR 0 // 00001
#define COIL_COUNT 1

bool coils[COIL_COUNT];     // save coils state
uint16_t hregs[HREG_COUNT]; // save holding registers value

ModbusRTU modbus;

// Callback transaction
bool _cbTransaction(Modbus::ResultCode event, uint16_t transactionId, void* data) { // Callback to monitor errors
  if (event != Modbus::EX_SUCCESS) {
    Serial.print("Request result: 0x");
    Serial.print(event, HEX);
  }
  return true;
}

void setup()
{
  Serial.begin(115200);
  Serial1.begin(9600, SERIAL_8N1);
  modbus.begin(&Serial1);
  modbus.master(); // select and initialize master mode to work
}

void loop()
{
  static uint8_t state = 0;

  switch (state)
  {
  case 0: // Send coil read request
    modbus.readCoil(SLAVE_ID, COIL_ADDR, coils, COIL_COUNT, _cbTransaction);
    state++;
    break;

  case 1:                // Check coil reading
    if (!modbus.slave()) // if no transaction in progress
    {
      Serial.print("Coil state: ");
      Serial.println(coils[0]);
      state++;
    }
    break;

  case 2: // Send coil write request to 0
    modbus.writeCoil(SLAVE_ID, COIL_ADDR, 0x0, COIL_COUNT, _cbTransaction);
    state++;
    break;

  case 3: // Send the holding register read request
    modbus.readHreg(SLAVE_ID, HREG_ADDR, hregs, HREG_COUNT, _cbTransaction);
    state++;
    break;

  case 4: // Check holding registers reading
    if (!modbus.slave())
    {
      Serial.print("Holding Registers: ");
      for (uint8_t i = 0; i < HREG_COUNT; i++)
      {
        Serial.print(hregs[i]);
        Serial.print(" ");
      }
      Serial.println();
      state++;
    }
    break;

  case 5: // Send holding registers write request to 0
    modbus.writeHreg(SLAVE_ID, HREG_ADDR, 0x0, _cbTransaction);
    state = 0;
    break;
  }

  modbus.task(); // Process transactions and manage Modbus communications.
  yield();       // Helps prevent system crashes caused by time-consuming functions
}
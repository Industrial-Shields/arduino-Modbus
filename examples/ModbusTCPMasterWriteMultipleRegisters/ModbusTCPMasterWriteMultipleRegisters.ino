/*
 * Copyright (c) 2025 Industrial Shields. All rights reserved
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <Ethernet.h>
#include <ModbusTCPMaster.h>



// Baudrate used by the USB serial communication
#define USB_SERIAL_BAUDRATE                     9600
// Ethernet's MAC
#define ETHERNET_MAC                            { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }
// Modbus master IP
#define ETHERNET_MASTER_IP                      IPAddress(10, 10, 10, 3);
// Modbus slave IP
#define ETHERNET_SLAVE_IP                       IPAddress(10, 10, 10, 4);
// Modbus slave port
#define ETHERNET_SLAVE_PORT                     502
// The Modbus address of the slave
#define MODBUS_SLAVE_ADDRESS                    31
// The holding register address to write
#define MODBUS_FIRST_HOLDING_REGISTER_TO_WRITE  0
// Number of holding registers to read
#define MODBUS_HOLDING_REGISTERS_TO_WRITE       5
// Number of milliseconds to wait between requests
#define MS_BETWEEN_REQUESTS                     1000



// Ethernet configuration values
static uint8_t masterMac[6] = ETHERNET_MAC;
static IPAddress masterIp = ETHERNET_MASTER_IP;
static IPAddress slaveIp = ETHERNET_SLAVE_IP;
static uint16_t slavePort = ETHERNET_SLAVE_PORT;


// Ethernet client object used to connect to the slave
static EthernetClient slaveEth;
// Modbus TCP master object used to interact with Modbus
static ModbusTCPMaster master;



////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(USB_SERIAL_BAUDRATE);

  // Begin Ethernet
  Ethernet.begin(masterMac, masterIp);
  Serial.println(Ethernet.localIP());

  // NOTE: it is not necessary to start the modbus master object
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {
  static uint16_t coilValues[MODBUS_HOLDING_REGISTERS_TO_WRITE];
  static unsigned long lastSentTime = 0UL;

  // Connect to slave if not connected
  // The ethernet connection is managed by the application, not by the library
  // In this case the connection is opened once
  if (!slaveEth.connected()) {
    slaveEth.stop();

    slaveEth.connect(slaveIp, slavePort);
    if (slaveEth.connected()) {
      Serial.println("Reconnected");
    }
  }

  // Send a request every MS_BETWEEN_REQUESTS if connected to slave
  if (slaveEth.connected()) {
    // Send a request every MS_BETWEEN_REQUESTS
    if (millis() - lastSentTime > MS_BETWEEN_REQUESTS) {
      // Send a Write Multiple Holding Registers request to the slave with address MODBUS_SLAVE_ADDRESS
      // It requests for setting MODBUS_HOLDING_REGISTERS_TO_WRITE holding registers starting in address MODBUS_FIRST_HOLDING_REGISTER_TO_WRITE
      // IMPORTANT: all read and write functions start a Modbus transmission, but they are not
      // blocking, so you can continue the program while the Modbus functions work. To check for
      // available responses, call modbus.available() function often.
      if (!master.writeMultipleRegisters(slaveEth, MODBUS_SLAVE_ADDRESS, MODBUS_FIRST_HOLDING_REGISTER_TO_WRITE, coilValues, MODBUS_HOLDING_REGISTERS_TO_WRITE)) {
        // Failure treatment
      }

      for (uint16_t c = 0; c < MODBUS_HOLDING_REGISTERS_TO_WRITE; c++) {
        coilValues[c] = coilValues[c] == 0 ? 1000 : 0;
      }
      lastSentTime = millis();
    }

    // Check available responses often
    if (master.isWaitingResponse()) {
      ModbusResponse response = master.available();
      if (response) {
        if (response.hasError()) {
          // Response failure treatment. You can use response.getErrorCode()
          // to get the error code.
        } else {
          // Treat the the response
        }
      }
    }
  }
}

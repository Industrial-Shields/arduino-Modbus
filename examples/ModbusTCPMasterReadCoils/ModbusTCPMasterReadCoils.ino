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
// The coil address to write
#define MODBUS_COILS_FIRST_ADDRESS              0
// Number of coils to read
#define MODBUS_COILS_TO_READ                    5
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
  Serial.print("Local IP is: ");
  Serial.println(Ethernet.localIP());

  // NOTE: it is not necessary to start the modbus master object

  // Wait for the Ethernet to initialize, and try the first connection
  delay(2000);
  Serial.println("Trying first connection...");
  if (slaveEth.connect(slaveIp, slavePort)) {
    Serial.println("First connection was successful");
  }
  else {
    Serial.println("The first connection attempt failed. The program will try to reconnect later");
  }
}


////////////////////////////////////////////////////////////////////////////////////////////////////
static void tryReconnectEth(void) {
  slaveEth.stop();

  if (slaveEth.connect(slaveIp, slavePort)) {
    Serial.println("Reconnected");
  }
}

static void sendModbusRequest(void) {
  // Send a Read Coils request to the slave with address MODBUS_SLAVE_ADDRESS
  // It requests for MODBUS_COILS_TO_READ coils starting at address MODBUS_COILS_FIRST_ADDRESS
  // IMPORTANT: all read and write functions start a Modbus transmission, but they are not
  // blocking, so you can continue the program while the Modbus functions work. To check for
  // available responses, call master.available() function often.
  if (!master.readCoils(slaveEth, MODBUS_SLAVE_ADDRESS, MODBUS_COILS_FIRST_ADDRESS, MODBUS_COILS_TO_READ)) {
    Serial.println("Request to read coils failed");
  }
}


static void printExceptionIfFound(void) {
  if (master.hasException()) {
    // Print the exception found in the Master
    Serial.print("Exception found in Modbus: ");
    Serial.println(master.getExceptionMessage());
    master.clearException();
  }
}

static void pollModbus(void) {
  if (master.isWaitingResponse()) {
    ModbusResponse response = master.available();
    // Check if there was an exception after polling
    printExceptionIfFound();

    if (response) {
      if (!response.hasError()) {
        // Successful response, print the results
        Serial.print("Coil values: ");
        for (uint16_t c = 0; c < (MODBUS_COILS_TO_READ - 1); c++) {
          Serial.print(response.isCoilSet(c));
          Serial.print(", ");
        }
        Serial.println(response.isCoilSet(MODBUS_COILS_TO_READ - 1));
      }
      else {
	// Slave answered with an error, print it
        Serial.print("The response contains an error: ");
        Serial.println(response.getErrorMessage());
      }
    }
  }
}

void loop() {
  // Connect to slave if not connected
  // The Ethernet connection is managed by the application, not by the library
  // In this case the connection is opened once
  if (!slaveEth.connected()) {
    tryReconnectEth();
  }

  if (slaveEth.connected()) {
    static unsigned long lastSentTime = 0UL;
    // Send a request every MS_BETWEEN_REQUESTS
    if (millis() - lastSentTime > MS_BETWEEN_REQUESTS && !master.isWaitingResponse()) {
      sendModbusRequest();
      lastSentTime = millis();
    }

    pollModbus();
  }
}
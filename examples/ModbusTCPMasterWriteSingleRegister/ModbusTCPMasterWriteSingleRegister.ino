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
#define MODBUS_HOLDING_REGISTER_TO_WRITE        0
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
  static uint16_t registerValue;

  // Send a Write Holding Register request to the slave with address MODBUS_SLAVE_ADDRESS
  // It toggles the holding register with address MODBUS_HOLDING_REGISTER_TO_WRITE periodically
  // IMPORTANT: all read and write functions start a Modbus transmission, but they are not
  // blocking, so you can continue the program while the Modbus functions work. To check for
  // available responses, call modbus.available() function often.
  if (master.writeSingleRegister(slaveEth, MODBUS_SLAVE_ADDRESS, MODBUS_HOLDING_REGISTER_TO_WRITE, registerValue)) {
    registerValue = registerValue == 0 ? 1000 : 0;
  }
  else {
    // For some reason, the request could not be completed.
    Serial.println("Request to write holding registers failed");
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
        // Successful response
        Serial.println("The slave successfully processed the request");
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
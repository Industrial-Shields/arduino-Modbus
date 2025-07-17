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

#include <ModbusRTUMaster.h>



// Baudrate used by the USB serial communication
#define USB_SERIAL_BAUDRATE                  9600
// Baudrate used in the Modbus communication
#define MODBUS_BAUDRATE                      38400
// Modbus communication duplex mode (only applicable when using RS-485)
#define MODBUS_DUPLEX                        HALFDUPLEX
// 8 bit data, even parity, 1 stop bit
#define MODBUS_SERIAL_CONFIG                 SERIAL_8E1
// The Modbus address of the slave
#define MODBUS_SLAVE_ADDRESS                 31
// The coil address to write
#define MODBUS_COIL_TO_WRITE                 0
// Number of milliseconds to wait between requests
#define MS_BETWEEN_REQUESTS                  1000


// Define the ModbusRTUMaster object, using the RS-485 or RS-232 port (depending on availability)
#if defined HAVE_RS485_HARD
#include <RS485.h>
ModbusRTUMaster master(RS485);

#elif defined HAVE_RS232_HARD
#include <RS232.h>
ModbusRTUMaster master(RS232);

#else
ModbusRTUMaster master(Serial1);
#endif



////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(USB_SERIAL_BAUDRATE);

  // Start the serial port
#if defined HAVE_RS485_HARD
  RS485.begin(MODBUS_BAUDRATE, MODBUS_DUPLEX, MODBUS_SERIAL_CONFIG);
#elif defined HAVE_RS232_HARD
  RS232.begin(MODBUS_BAUDRATE, MODBUS_SERIAL_CONFIG);
#else
  Serial1.begin(MODBUS_BAUDRATE, MODBUS_SERIAL_CONFIG);
#endif

  // Start the modbus master object. The default baudrate is 19200.
  master.begin(MODBUS_BAUDRATE);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
static void sendModbusRequest(void) {
  static bool coilValue;

  // Send a Write Coil request to the slave with address MODBUS_SLAVE_ADDRESS
  // It toggles the coil with address MODBUS_COIL_TO_WRITE periodically
  // IMPORTANT: all read and write functions start a Modbus transmission, but they are not
  // blocking, so you can continue the program while the Modbus functions work. To check for
  // available responses, call modbus.available() function often.
  if (master.writeSingleCoil(MODBUS_SLAVE_ADDRESS, MODBUS_COIL_TO_WRITE, coilValue)) {
    // Flip the coil value
    coilValue = !coilValue;
  }
  else {
    // For some reason, the request could not be completed.
    Serial.println("Request to write a coil failed");
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
  static unsigned long lastSentTime = 0UL;
  // Send a request every MS_BETWEEN_REQUESTS
  if (millis() - lastSentTime > MS_BETWEEN_REQUESTS && !master.isWaitingResponse()) {
    sendModbusRequest();
    lastSentTime = millis();
  }

  pollModbus();
}

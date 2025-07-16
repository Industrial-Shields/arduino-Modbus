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
#define USB_SERIAL_BAUDRATE            9600
// Baudrate used in the Modbus communication
#define MODBUS_BAUDRATE                38400
// Modbus communication duplex mode (only applicable when using RS-485)
#define MODBUS_DUPLEX                  HALFDUPLEX
// 8 bit data, even parity, 1 stop bit
#define MODBUS_SERIAL_CONFIG           SERIAL_8E1
// The Modbus address of the slave
#define MODBUS_SLAVE_ADDRESS           31
// The first coil address to read from the slave
#define MODBUS_COILS_FIRST_ADDRESS     0
// Number of coils to read
#define MODBUS_COILS_TO_READ           5
// Number of milliseconds to wait between requests
#define MS_BETWEEN_REQUESTS            1000


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
void loop() {
  static unsigned long lastSentTime = 0UL;

  // Send a request every MS_BETWEEN_REQUESTS
  if (millis() - lastSentTime > MS_BETWEEN_REQUESTS) {
    // Send a Read Coils request to the slave with address MODBUS_SLAVE_ADDRESS
    // It requests for MODBUS_COILS_TO_READ coils starting at address MODBUS_COILS_FIRST_ADDRESS
    // IMPORTANT: all read and write functions start a Modbus transmission, but they are not
    // blocking, so you can continue the program while the Modbus functions work. To check for
    // available responses, call master.available() function often.
    if (!master.readCoils(MODBUS_SLAVE_ADDRESS, MODBUS_COILS_FIRST_ADDRESS, MODBUS_COILS_TO_READ)) {
      // Failure treatment
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
        // Get the coil value from the response
        for (byte i = 0; i < MODBUS_COILS_TO_READ; i++) {
          bool coil = response.isCoilSet(i);

          Serial.print("Coil ");
          Serial.print(i);
          Serial.print(": ");
          Serial.println(coil);
        }
      }
    }
  }
}

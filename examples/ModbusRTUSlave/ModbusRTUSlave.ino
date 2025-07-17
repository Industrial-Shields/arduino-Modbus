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

#include <ModbusRTUSlave.h>



// Baudrate used by the USB serial communication
#define USB_SERIAL_BAUDRATE            9600
// Baudrate used in the Modbus communication
#define MODBUS_BAUDRATE                38400
// Modbus communication duplex mode (only applicable when using RS-485)
#define MODBUS_DUPLEX                  HALFDUPLEX
// 8 bit data, even parity, 1 stop bit
#define MODBUS_SERIAL_CONFIG           SERIAL_8E1
// The Modbus address of the slave
#define MODBUS_ADDRESS                 31


// Modbus registers mapping
// This example uses the M-Duino 21+ mapping
int digitalOutputsPins[] = {
#if defined(PIN_Q0_4)
  Q0_0, Q0_1, Q0_2, Q0_3, Q0_4,
#endif
};
int digitalInputsPins[] = {
#if defined(PIN_I0_6)
  I0_0, I0_1, I0_2, I0_3, I0_4, I0_5, I0_6,
#endif
};
int analogOutputsPins[] = {
#if defined(PIN_Q0_7)
  A0_5, A0_6, A0_7,
#endif
};
int analogInputsPins[] = {
#if defined(PIN_I0_12)
  I0_7, I0_8, I0_9, I0_10, I0_11, I0_12,
#endif
};


#define numDigitalOutputs (sizeof(digitalOutputsPins) / sizeof(int))
#define numDigitalInputs (sizeof(digitalInputsPins) / sizeof(int))
#define numAnalogOutputs (sizeof(analogOutputsPins) / sizeof(int))
#define numAnalogInputs (sizeof(analogInputsPins) / sizeof(int))

bool digitalOutputs[numDigitalOutputs];
bool digitalInputs[numDigitalInputs];
uint16_t analogOutputs[numAnalogOutputs];
uint16_t analogInputs[numAnalogInputs];



// Define the ModbusRTUSlave object with the Modbus RTU slave address,
// using the RS-485, RS-232 or Serial1 port, depending on availability
#if defined HAVE_RS485_HARD
#include <RS485.h>
ModbusRTUSlave modbus(RS485, MODBUS_ADDRESS);

#elif defined HAVE_RS232_HARD
#include <RS232.h>
ModbusRTUSlave modbus(RS232, MODBUS_ADDRESS);

#else
ModbusRTUSlave modbus(Serial1, MODBUS_ADDRESS);
#endif



////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(USB_SERIAL_BAUDRATE);

  // Init variables, inputs and outputs
  for (int i = 0; i < numDigitalOutputs; ++i) {
    digitalOutputs[i] = false;
    digitalWrite(digitalOutputsPins[i], digitalOutputs[i]);
  }
  for (int i = 0; i < numDigitalInputs; ++i) {
    digitalInputs[i] = digitalRead(digitalInputsPins[i]);
  }
  for (int i = 0; i < numAnalogOutputs; ++i) {
    analogOutputs[i] = 0;
    analogWrite(analogOutputsPins[i], analogOutputs[i]);
  }
  for (int i = 0; i < numAnalogInputs; ++i) {
    analogInputs[i] = analogRead(analogInputsPins[i]);
  }

  // Start the serial port
#if defined HAVE_RS485_HARD
  RS485.begin(MODBUS_BAUDRATE, MODBUS_DUPLEX, MODBUS_SERIAL_CONFIG);
#elif defined HAVE_RS232_HARD
  RS232.begin(MODBUS_BAUDRATE, MODBUS_SERIAL_CONFIG);
#else
  Serial1.begin(MODBUS_BAUDRATE, MODBUS_SERIAL_CONFIG);
#endif

  // Init ModbusRTUSlave object
  modbus.setCoils(digitalOutputs, numDigitalOutputs);
  modbus.setDiscreteInputs(digitalInputs, numDigitalInputs);
  modbus.setHoldingRegisters(analogOutputs, numAnalogOutputs);
  modbus.setInputRegisters(analogInputs, numAnalogInputs);

  modbus.begin(MODBUS_BAUDRATE);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {
  // Update inputs
  for (int i = 0; i < numDigitalInputs; ++i) {
    digitalInputs[i] = digitalRead(digitalInputsPins[i]);
  }
  for (int i = 0; i < numAnalogInputs; ++i) {
    analogInputs[i] = analogRead(analogInputsPins[i]);
  }

  // Process modbus requests
  modbus.update();

  // Update outputs
  for (int i = 0; i < numDigitalOutputs; ++i) {
    digitalWrite(digitalOutputsPins[i], digitalOutputs[i]);
  }
  for (int i = 0; i < numAnalogOutputs; ++i) {
    analogWrite(analogOutputsPins[i], analogOutputs[i]);
  }
}

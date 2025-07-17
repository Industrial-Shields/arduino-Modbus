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
#include <ModbusTCPSlave.h>



// Baudrate used by the USB serial communication
#define USB_SERIAL_BAUDRATE               9600
// Ethernet's MAC
#define ETHERNET_MAC                      { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEE }
// Modbus slave IP
#define ETHERNET_IP                       IPAddress(10, 10, 10, 4);
// Modbus slave port
#define ETHERNET_PORT                     502
// The Modbus address of the slave
#define MODBUS_ADDRESS                    31


// Modbus registers mapping
// This example uses the M-Duino21+ mapping
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


// Ethernet configuration values
static uint8_t mac[6] = ETHERNET_MAC;
static IPAddress ip = ETHERNET_IP;
static uint16_t port = ETHERNET_PORT;


#define numDigitalOutputs (sizeof(digitalOutputsPins) / sizeof(int))
#define numDigitalInputs (sizeof(digitalInputsPins) / sizeof(int))
#define numAnalogOutputs (sizeof(analogOutputsPins) / sizeof(int))
#define numAnalogInputs (sizeof(analogInputsPins) / sizeof(int))

bool digitalOutputs[numDigitalOutputs];
bool digitalInputs[numDigitalInputs];
uint16_t analogOutputs[numAnalogOutputs];
uint16_t analogInputs[numAnalogInputs];


// Define the ModbusTCPSlave object
ModbusTCPSlave modbus(port);

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

  // Init Ethernet
  Ethernet.begin(mac, ip);
  Serial.print("Local IP is: ");
  Serial.println(Ethernet.localIP());

  // Init ModbusTCPSlave object
  modbus.setCoils(digitalOutputs, numDigitalOutputs);
  modbus.setDiscreteInputs(digitalInputs, numDigitalInputs);
  modbus.setHoldingRegisters(analogOutputs, numAnalogOutputs);
  modbus.setInputRegisters(analogInputs, numAnalogInputs);

  modbus.begin();
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

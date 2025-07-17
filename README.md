# arduino-Modbus
### by [Industrial Shields](https://www.industrialshields.com)

The Modbus library provides a way to transform our PLCs in Modbus masters or slaves over various hardware interfaces, such as RS-485/RS-232 or Ethernet. It provides non-blocking functions for reading and writing Modbus data types, and features error handling and response retrieval mechanisms.

## Gettings started

### Prerequisites
1. The [Arduino IDE](http://www.arduino.cc) 1.8.0 or higher
2. The [Industrial Shields Arduino boards](https://www.industrialshields.com/blog/industrial-shields-blog-1/post/installing-the-industrial-shields-boards-in-the-arduino-ide-63) or the [Industrial Shields ESP32 boards](https://www.industrialshields.com/blog/industrial-shields-blog-1/post/installing-the-industrial-shields-boards-in-the-arduino-ide-63) equivalent (optional, used in the examples).

### Installing
1. Download the [library](https://github.com/Industrial-Shields/arduino-Modbus) from the GitHub as a "ZIP" file.
2. From the Arduino IDE, select the downloaded "ZIP" file in the menu "Sketch/Include library/Add .ZIP library".
3. Now you can open any example from the "File/Examples/Modbus" menu.


## Usage

### RTU or TCP?

This library is compatible with both RTU and TCP modes.

#### RTU

RTU mode is primarly used for serial communication, typically over standards like RS-485 or RS-232 (although it can be used with every Hardware Serial Stream, like UART Serial1).

``` c++
#include <RS485.h>
#include <ModbusRTUMaster.h>
#include <ModbusRTUSlave.h>


ModbusRTUMaster master(RS485);
ModbusRTUSlave slave(Serial1);
```

**WARNING**: Before using the RTU classes, it is required to call the begin function in the setup for both the serial port and the Modbus variable. Note that you must pass the baudrate to the master.begin function, as this defines the internal Modbus timeouts. For example:

``` c++
void setup() {
	// ...
	RS485.begin(9600, HALFDUPLEX, SERIAL_8E1);
	master.begin(9600);

	Serial1.begin(9600, SERIAL_8E1);
	slave.begin(9600);
	// ...
}
```

#### TCP

TCP mode is used for communication over Ethernet networks. It uses a different frame structure adapted to the Ethernet protocol. The default TCP port is `502` but you can change it in the Modbus object constructor:

```c++
#include <Ethernet.h>
#include <ModbusTCPMaster.h>
#include <ModbusTCPSlave.h>


ModbusTCPMaster master(502);
ModbusTCPSlave slave(510);
```

**WARNING**: Before using the TCP classes, it is required to call the begin function in the setup for both Ethernet and the Modbus variable. This function will start the ModbusTCP server/client. It is important to begin the Ethernet before calling the ModbusTCP `begin` methods.


``` c++
void setup() {
	// ...
	Ethernet.begin(mac, ip);
	master.begin();
	slave.begin();
	// ...
}
```


### Modbus Master

ModbusRTUMaster and ModbusTCPMaster are the modules that implement the master capabilities.

The functions to read and write slave values are:

``` c++
readCoils(slave_address, address, quantity);
readDiscreteInputs(slave_address, address, quantity);
readHoldingRegisters(slave_address, address, quantity);
readInputRegisters(slave_address, address, quantity);
writeSingleCoil(slave_address, address, value);
writeSingleRegister(slave_address, address, value);
writeMultipleCoils(slave_address, address, values, quantity);
writeMultipleRegisters(slave_address, address, values, quantity);
hasException();
clearException();
getException();
getExceptionMessage();
```

Where
* `slave_address` is the Modbus RTU slave address.
* `address` is the coil, digital input, holding register or input register address. Usually this address is the coil, digital input, holding register or input register number minus 1: the holding register number `40009` has the address `8`.
* `quantity` is the number of coils, digital inputs, holding registers or input registers to read/write.
* `value` is the given value of the coil or holding registers on a write operation. Depending on the function the data type changes. A coil is represented by a `bool` value and a holding register is represented by a `uint16_t` value.

On a multiple read/write function the `address` argument is the first element address. On a multiple write function the `values` argument is an array of values to write.

It is important to notice that these functions are non-blocking, so they don't return the read value. They return `true` or `false` depending on the current module state. If there is a pending Modbus request, they return `false`.

``` c++
// Read 5 holding registers from address 0x24 of slave with address 0x10
if (master.readHoldingRegisters(0x10, 0x24, 5)) {
	// OK, the request is being processed
}
else {
	// ERROR, the master is not in an IDLE state
}
```

There is the `available()` function to check for responses from the slave.

``` c++
ModbusResponse response = master.available();
if (response) {
	// Process response
}
```

The `ModbusResponse` implements some functions to get the response information:

``` c++
hasError();
getErrorCode();
getErrorMessage();
getSlave();
getFC();
isCoilSet(offset);
isDiscreteInputSet(offset);
isDiscreteSet(offset);
getRegister(offset);
```

``` c++
ModbusResponse response = master.available();
if (response) {
	if (response.hasError()) {
		// There is an error. You can also get the error code with response.getErrorCode()
		Serial.print("The response contains an error: ");
		Serial.println(response.getErrorMessage());
	}
	else {
		// Response ready: print the read holding registers
		for (int i = 0; i < 5; ++i) {
			Serial.println(response.getRegister(i);
		}
	}
}
```

The possible error codes are:

```
0x01 ILLEGAL FUNCTION
0x02 ILLEGAL DATA ADDRESS
0x03 ILLEGAL DATA VALUE
0x04 SERVER DEVICE FAILURE
```

If an error occurs during the communication (e.g, the slave didn't respond to the request), the
master will set an internal flag that the application maker can retrieve to determine what happened:

``` c++
if (master.hasException()) {
	// Something happened...
	Serial.print("An exception ocurred: ");
	Serial.println(master.getExceptionMessage());
	// After processing the exception, clear it from the master
	master.clearException();
}
```

### Modbus Slave

ModbusRTUSlave and ModbusTCPSlave are the modules that implement the slave capabilities.

To map the coils, discrete inputs, holding registers and input registers addresses with the desired variables values, you have to declare four arrays:

``` c++
#define NUM_COILS 5
#define NUM_DISCRETE_INPUTS 5
#define NUM_HOLDING_REGISTERS 5
#define NUM_INPUT_REGISTERS 5

bool coils[NUM_COILS];
bool discreteInputs[NUM_DISCRETE_INPUTS];
uint16_t holdingRegistesr[NUM_HOLDING_REGISTERS];
uint16_t inputRegisters[NUM_INPUT_REGISTERS];
```

The lengths of these arrays depend on the application and the registers usages. Obviously, the names of the arrays also depend on your preferences.

Then you need to associate the registers arrays with the library. You have to use the following functions in the `setup`:

``` c++
slave.setCoils(coils, NUM_COILS);
slave.setDiscreteInputs(discreteInputs, NUM_DISCRETE_INPUTS);
slave.setHoldingRegisters(holdingRegisters, NUM_HOLDING_REGISTERS);
slave.setInputRegisters(inputRegisters, NUM_INPUT_REGISTERS);
```

It is not necessary  to have all the register types mapped to work, you can only declare those used by your application.


After the begin method is called in the slave, the only important thing to do is to update the Modbus object often in the `loop` function (with the `update` method), and refresh the contents of the arrays passed to the library:

```c++
void loop() {
	// ...
	// Update discrete inputs and input registers values
	discreteInputs[0] = digitalRead(I0_7);
	inputRegisters[0] = analogRead(I0_0);
	// ...

	// Update the ModbusTCPSlave object
	slave.update();
	
	// Update coils and holding registers
	digitalWrite(Q0_0, coils[0]);
	// ...
}
```

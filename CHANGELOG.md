# Changelog

All notable changes to this project will be documented in this file.

## [1.1.0] - 2025-07-17

### 🚀 Features

- *(examples)* Slave examples compatible with other types of our PLCs ([c5c7b6f](c5c7b6f0a4dd79ee365c1aeaddc1868ab05a0a08))

- *(examples)* Handle exceptions and errors, and make the code clearer ([9fc5695](9fc569506ebc6050be333cb3eba5208eff913960))

- Add getErrorMessage() method to Modbus responses ([351e2b7](351e2b71b1a3490b1e3fbaefaecd05a214285f44))


### 🐛 Bug Fixes

- *(RTU)* Ignore MSB in received FC to determine bad function code ([1f295cb](1f295cb97a4fbb37b48a8e7eae65537bab15b02a))


### 💼 Other

- Bump library version ([b41b956](b41b956ed617b58449fd6a47a5f80077100466c9))


### 🚜 Refactor

- *(examples)* Parametrize Modbus Slave examples ([648ae8f](648ae8f064f3c513a7f4df8a437381d50d024b5d))

- *(examples)* Parametrize Modbus Master examples ([811e74e](811e74eed5a58f9f7676b24fa6bc5a3f2ce57964))


### ⚙️ Miscellaneous Tasks

- Add cliff.toml ([2a691a8](2a691a8d40777bb8f8d4649cbbb5a954b529bd1b))


## [1.0.1] - 2024-10-14

### 💼 Other

- Add ModbusTCPMasterReadHoldingRegisters ([dccb8e8](dccb8e80c4da80671f9cee4c8d5b0cc17d47ca38))

- [#3] Initialize variables of ModbusRTUMaster when compiling against ESP32 ([0691961](0691961f9b64567b575ed5c55b703ec7ac258b47))

- Update ModbusTCP.h

Added ModbusTCP functionality for PLC14IOS ([2722baa](2722baa5819245ba9c8beadc0fe9d41999d073d6))


## [1.0.0] - 2024-01-11

### 🐛 Bug Fixes

- Fix ModbusRTUSlave example
The baudrates by the Serial and the Modbus library could differ if not defined in two different places. ([b3faaa6](b3faaa65ce46c481131be0c7c867932e2ce72c01))


### 💼 Other

- Extract Modbus from the arduino-Tools40 + Bump version ([05788ce](05788cede1cbd87d1b6e905e5901b5f53840cf8f))

- Added exceptions to notify about error conditions ([b5133a6](b5133a61ee4caa234d961679d786cf9df546e9a6))

- Compatibility between architectures SimpleComm ([007db8c](007db8c9ad37ececdeab37b19e7b7313ad52c39b))

- Update README for ESP32 ([409402b](409402b3e47ba6a45f3d5ee7a9cec560afe70007))

- Test ESP32 + Make SimpleComm compatible between architectures ([d285164](d2851649877960c5cc0f8195b748c5c02ef50eac))

- Better examples for Modbus (adapted for ESP32) ([6579311](65793112a2b374fa6a47d192a8ba7fe89b8b3705))

- Modbus adaptation for ESP32 ([113e848](113e848583eb1108600158b95c11d13af23340b0))


## [1.0.0-beta] - 2022-04-19

### 🐛 Bug Fixes

- BUG FIX: solve format issues ([b8ed565](b8ed5657c602d52b42e797190de9435f628bbc63))

- BUG FIX: AnalogFilter update time ([5f21fd9](5f21fd9e8d6cabc09aef4d48a53a34448baf9c7b))

- BUG FIX: Pulses library for Nano boards ([112b1cc](112b1cc2a93ad74c1f4ca2dfb30d321eddc3d3be))

- BUG FIX: AnalogFilter types ([9cd0cb4](9cd0cb456612875d86b520a1a2d40b68f3cdda19))

- BUG FIX: ModbusSlave writeSingleRegister response ([537781f](537781f4219022999d2e1d469feb77e807bbb3c5))

- BUG FIX: ModbusRTUSlave process requests ([6a3e025](6a3e02542193d26cf7f7e2ff85b7910def76c2a0))

- BUG FIX: new Ethernet.h library ([0e463b9](0e463b992a0a46ff69edede3c426f7b58dec59ff))

- Fix ModbusTCP check error

Check for bad function code fixed ([d448cb2](d448cb28854f53d33085d40dc8acc17003878fad))

- BUG FIX: get discrete values from ModbusResponse ([fceb33c](fceb33c8bca0672fcab476e606861f54f68b8f91))

- BUG FIX: build ModbusRTUMasterReadCoils example ([86cc3d9](86cc3d979a38bbf2df97fdc6300f1f8ece1be1d7))

- BUG FIX: README.md pulses link ([afcc8d1](afcc8d189290f2fe1b1d2a378ab0110a1730e32f))

- BUG FIX: README lists ([39daa68](39daa68487cd01f25b541479af27e50a19e4f6bf))


### 💼 Other

- Added setTimeout ([e0bd015](e0bd0154e8588f69eb8b0b4e8da62c05eb3a408d))

- Add support for ESP32PLC ([4c823a6](4c823a6f775bea651c214c13237fad876d12d397))

- Add ModbusRTU examples ([11ef0ee](11ef0ee9205b25791e078964906dec6414a87026))

- Make Spartan 19R compatible with ModbusTCP ([e999455](e99945552c327ccdac02fc243c086384f27e1f43))

- Update ModbusTCP.h ([a23d29c](a23d29c0e6cdb8e6d7b0886e935a7212951aeff4))

- ModbusTCP supported by 10IOs PLC ([fbdfc12](fbdfc12988bef4c31b0d7cecc3400b30e48ca66f))

- Disable some functions when needed ([ee82649](ee826499102d81a954bd5aa5952530d0d49a204d))

- Update README.md ([a07ff43](a07ff43a19e68dfa9a0e9b4ddd126166a4d53f73))

- Add initialValue in both AnalogFilter and DigitalFilter ([9b64f34](9b64f346443081f01ad14cfc6f8a405cfe21de11))

- Add ModbusRTUSlave library ([a700c5e](a700c5e5414996da3b865d98cfbbb13f50b8cd38))

- Update README.md ([648c9f6](648c9f613034d62064e35fd9884dcdb6d37830ac))

- Prevent initial 0x00 on Modbus RTU from some devices ([9018d2c](9018d2c59231e3ad81e33dab80eb81731dfa892b))

- Update ModbusTCP.h

New Ethernet library usage ([7396011](7396011a70030cc7196bacdcdcfa73fbf83b2059))

- Update README.md ([3befe02](3befe024b4310749770319feac4140e0c0dae1bc))

- Update README.md ([6b24457](6b244573f138c8aa063a49d2cb55f81270aa3524))

- Support all platforms ([8dfcbbb](8dfcbbb197f7e510aff349c6684ba772c0b5dd4a))

- Only for avr boards ([f3b7a7f](f3b7a7fee7f2e11731506ff19f5ff1ac5d72f062))

- Update README.md ([02c94e2](02c94e20cdc79c1b755939419620c9b495be98e8))

- Update README ([34626e3](34626e36d5a52bc80f3538457b5b20364a9b619f))

- Begin serial port outside the modbus library ([99f666a](99f666a90a56b6b4a2b4486f02d57cfecf0735ea))

- Use Ethernet2 instead of Ethernet on M-Duino+ family ([25d98a6](25d98a684cf4933835334c9408914662f00bcd1f))

- Update README.md ([17fc196](17fc1964ae784ad44db0e8055f1f8af964e42eec))

- Update README.md ([a58b33d](a58b33de4c3deb2cac28df61320720f856759557))

- Set default ModbusTCPSlave port to 502 ([f24a28a](f24a28a33ffb4ed76bb72587ee8c845e80084d20))

- Prevent compilation errors on other devices ([0df6017](0df601756b201f89efdbbc058a8f016c4aa203df))

- Add ModbusTCPSlave library ([6901dbf](6901dbfa6c1cef9e2ac80b57e5695ddbfc17971b))

- Remove debugging info ([137647b](137647bfb55e35a7e08e5abcfea8b7d219c04bdd))

- Update README.md ([d7300d3](d7300d30c18addc35a160334fcebe12deb37c9c3))

- Update README ([cea3cd1](cea3cd185293ba07f1c02130b412ac0f6ba8dfe8))

- Add sniffer mode and broadcast address ([461e419](461e419a4e9f5fb9170cb6a1fe4257edf0d3ec97))

- Add ModbusTCPMaster ([b69dd02](b69dd02ca415d15b58fd7afce41cf8bcb22eb4b5))

- Add ModbusRTUMaster module ([855a14b](855a14bd6b70058ce3c52b5392877aef8ed36526))

- Add Counter module ([3a7ede3](3a7ede3ae4212f4a6cdb3e52a997b0e4f8312d71))

- Add DigitalFilter class ([1917d81](1917d81c46e173d19a726899fde39931059778c9))

- README.md style modifications ([ada620b](ada620b871d2b61c2998c132b9322d138f6268df))

- Add files via upload ([eb64148](eb64148dbd7756927d5e95d69f3b2b54c8670297))

- Add files via upload ([ed5c2fc](ed5c2fc09bc0a6b424c6c0cbdae22614d416ce8a))

- Add files via upload ([43a6b7f](43a6b7fa02048483307f80b564423695ff350f51))

- Add files via upload ([d1eef34](d1eef343f531ad00553abbbf18df48d295b7b4f5))

- Add Pulses module ([bc96cdf](bc96cdf6a259ab104ffddcd49ddd354d52a8c660))

- Replace NULL with nullptr ([d04348a](d04348ac35de815ce2c52b9dc38580d0fd6460e1))

- Make SimpleComm non-blocking on messages reception ([52f3228](52f3228c78e545bf2848ea44e6ac432bae096ad5))

- Update README.md ([631626e](631626e2db69b074b5c0c9d41cd4327448a914cd))

- Create README reference links ([3af9491](3af949159ff61a512585c9eae699f6abe4058506))

- Add README file ([d987594](d987594291f6843d424796f9260b7de02386d7cf))

- Build examples for all devices ([98ebc94](98ebc94b702ca58e85298db91946f3d4dac8c3a7))

- Join similar examples ([e60577f](e60577fdc5b6422d57bbe4efb8fde7692ecb11fe))

- Add SimpleComm ([1a2be41](1a2be413721b04f385d6667ffdd6b9f4950f678b))

- First commit ([443e83b](443e83b19451f62dd5394a1b9c4f3493c2fff050))


<!-- generated by git-cliff -->

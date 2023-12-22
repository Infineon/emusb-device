# SEGGER emUSB-Device for ModusToolbox Release Notes

Please refer to the [README.md](./README.md) and the [emUSB-Device API Reference Guide](https://infineon.github.io/emusb-device/html/index.html) for a complete description of the emUSB-Device Middleware.

## What's Included?

* Added CCID and new Audio classes
* Updated the emUSB-Device stack to 3.62.0
* Other minor improvements

To view the full list of changes, refer to [Changelog section](https://infineon.github.io/emusb-device/html/index.html#section_emusb_device_changelog).

## Known Issues and Limitations

* emUSB-Device does not support Manual and Automatic DMA modes (Mode 2 and Mode 3) for CAT1A devices.

Please refer to the [SEGGER emUSB-Device User Guide & Reference Manual](./docs/UM09001_emUSBD.pdf) Target USB Driver chapter for restrictions of the supported drivers.

## Supported Software and Tools

This version of the emUSB-Device was validated for the compatibility with the following Software and Tools:

| Software and Tools                                      | Version |
| :---                                                    | :----:  |
| ModusToolbox Software Environment                       | 3.1.0   |
| MTB CAT1 Peripheral Driver Library                      | 3.8.0   |
| MTB CAT3 XMC Library                                    | 4.2.0   |
| Hardware Abstraction Layer                              | 2.4.0   |
| RTOS Abstraction                                        | 1.7.4   |
| Retarget IO                                             | 1.5.0   |
| Retarget IO (CAT3)                                      | 1.1.0   |
| GCC Compiler                                            | 11.3.1  |
| IAR Compiler                                            | 9.3.1   |
| ARM Compiler 6                                          | 6.16    |

**_NOTE:_** CAT3 device family supports only GCC_ARM compiler

## More information

For more information, refer to the following documents:

* [emUSB-Device README.md](./README.md)
* [emUSB-Device API Reference Guide](https://infineon.github.io/emusb-device/html/index.html)
* [ModusToolbox Software Environment, Quick Start Guide, Documentation, and Videos](https://www.infineon.com/cms/en/design-support/tools/sdk/modustoolbox-software/)
* [Infineon Technologies AG](https://www.infineon.com)

---
© 2022-2023, Cypress Semiconductor Corporation (an Infineon company) or an affiliate of Cypress Semiconductor Corporation.

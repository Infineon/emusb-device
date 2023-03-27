# SEGGER emUSB-Device for ModusToolbox Release Notes

Please refer to the [README.md](./README.md) and the [emUSB-Device API Reference Guide](https://infineon.github.io/emusb-device/html/index.html) for a complete description of the emUSB-Device Middleware.

## What's Included?

* Updated the emUSB-Device stack to 3.58.0
* Added the Audio class support
* Extended the support of RTOS
* The driver for CAT1A devices supports the remote wake-up functionality now
* The maximum numbers of DATA Endpoints increased from 7 -> 8
* The maximum numbers of Bulk interface increased from 1 -> 4
* Improved the Debug Message Output experience
* Other minor improvements

To view the full list of changes, refer to [Changelog section](https://infineon.github.io/emusb-device/html/index.html#section_emusb_device_changelog).

## Known Issues and Limitations

* emUSB-Device does not support Manual and Automatic DMA modes (Mode 2 and Mode 3).

Please refer to the [SEGGER emUSB-Device User Guide & Reference Manual](./docs/UM09001_emUSBD.pdf) Target USB Driver chapter for restrictions of the supported drivers.

## Supported Software and Tools

This version of the emUSB-Device was validated for the compatibility with the following Software and Tools:

| Software and Tools                                      | Version |
| :---                                                    | :----:  |
| ModusToolbox Software Environment                       | 3.0.0   |
| MTB CAT1 Peripheral Driver Library                      | 3.2.0   |
| Hardware Abstraction Layer                              | 2.3.0   |
| RTOS Abstraction                                        | 1.7.1   |
| Retarget IO                                             | 1.4.0   |
| GCC Compiler                                            | 10.3.1  |
| IAR Compiler                                            | 9.3.1   |
| ARM Compiler 6                                          | 6.16    |

## More information

For more information, refer to the following documents:

* [emUSB-Device README.md](./README.md)
* [emUSB-Device API Reference Guide](https://infineon.github.io/emusb-device/html/index.html)
* [ModusToolbox Software Environment, Quick Start Guide, Documentation, and Videos](https://www.infineon.com/cms/en/design-support/tools/sdk/modustoolbox-software/)
* [Infineon Technologies AG](https://www.infineon.com)

---
© 2022-2023, Cypress Semiconductor Corporation (an Infineon company) or an affiliate of Cypress Semiconductor Corporation.

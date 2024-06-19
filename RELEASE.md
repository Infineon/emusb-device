# SEGGER emUSB-Device for ModusToolbox Release Notes

Please refer to the [README.md](./README.md) and the [emUSB-Device API Reference Guide](https://infineon.github.io/emusb-device/html/index.html) for a complete description of the emUSB-Device Middleware.

## What's Included?

* Add a new driver for PSoC 6 with DMA support
* Updated the emUSB-Device stack to 3.64.1
* Other minor improvements

To view the full list of changes, refer to [Changelog section](https://infineon.github.io/emusb-device/html/index.html#section_emusb_device_changelog).

## Known Issues and Limitations

The emUSB personality with DMA support for PSoC(TM) 6 is not updated in the mtb-pdl-cat1 3.10.1 library. So, the DMA must be configured in the user application. The updated emUSB personality with DMA support will be included in the next release of mtb-pdl-cat1.
The next code snippets show the DMA configuration for the PSoC 61 and PSoC 62 devices.

Note: This code is device-specific and cannot work for all devices. Also, some DMA channels can be reserved for other purposes.

Insert this code into the usbd_config.c file directly or save in another header file and include it into usbd_config.c.
For example: Insert this code under the first condition of `#if USBD_ENABLE_DMA == true` and before `#define USBD_MEMORY_POOL_SIZE                   (2048U)` (line 87 of usbd_config.c)

**For PSoC 61**
```c
#define USBD_DMA_CHANNEL_PRIORITY_EP0 3U
#define USBD_DMA_CHANNEL_PRIORITY_EP1 3U
#define USBD_DMA_CHANNEL_PRIORITY_EP2 3U
#define USBD_DMA_CHANNEL_PRIORITY_EP3 3U
#define USBD_DMA_CHANNEL_PRIORITY_EP4 3U
#define USBD_DMA_CHANNEL_PRIORITY_EP5 3U
#define USBD_DMA_CHANNEL_PRIORITY_EP6 3U
#define USBD_DMA_CHANNEL_PRIORITY_EP7 3U
#define USBD_DMA_CHANNEL_ADDRESS_EP0 DW0_CH_STRUCT2
#define USBD_DMA_CHANNEL_ADDRESS_EP1 DW0_CH_STRUCT3
#define USBD_DMA_CHANNEL_ADDRESS_EP2 DW0_CH_STRUCT4
#define USBD_DMA_CHANNEL_ADDRESS_EP3 DW0_CH_STRUCT5
#define USBD_DMA_CHANNEL_ADDRESS_EP4 DW0_CH_STRUCT6
#define USBD_DMA_CHANNEL_ADDRESS_EP5 DW0_CH_STRUCT7
#define USBD_DMA_CHANNEL_ADDRESS_EP6 DW0_CH_STRUCT8
#define USBD_DMA_CHANNEL_ADDRESS_EP7 DW0_CH_STRUCT9
#define USBD_DMA_OUT_TRIG_MUX_EP0 TRIG9_OUT_USB_DMA_BURSTEND0
#define USBD_DMA_OUT_TRIG_MUX_EP1 TRIG9_OUT_USB_DMA_BURSTEND1
#define USBD_DMA_OUT_TRIG_MUX_EP2 TRIG9_OUT_USB_DMA_BURSTEND2
#define USBD_DMA_OUT_TRIG_MUX_EP3 TRIG9_OUT_USB_DMA_BURSTEND3
#define USBD_DMA_OUT_TRIG_MUX_EP4 TRIG9_OUT_USB_DMA_BURSTEND4
#define USBD_DMA_OUT_TRIG_MUX_EP5 TRIG9_OUT_USB_DMA_BURSTEND5
#define USBD_DMA_OUT_TRIG_MUX_EP6 TRIG9_OUT_USB_DMA_BURSTEND6
#define USBD_DMA_OUT_TRIG_MUX_EP7 TRIG9_OUT_USB_DMA_BURSTEND7
```

**For PSoC 62**
```c
#define USBD_DMA_CHANNEL_PRIORITY_EP0 3U
#define USBD_DMA_CHANNEL_PRIORITY_EP1 3U
#define USBD_DMA_CHANNEL_PRIORITY_EP2 3U
#define USBD_DMA_CHANNEL_PRIORITY_EP3 3U
#define USBD_DMA_CHANNEL_PRIORITY_EP4 3U
#define USBD_DMA_CHANNEL_PRIORITY_EP5 3U
#define USBD_DMA_CHANNEL_PRIORITY_EP6 3U
#define USBD_DMA_CHANNEL_PRIORITY_EP7 3U
#define USBD_DMA_CHANNEL_ADDRESS_EP0 DW0_CH_STRUCT8
#define USBD_DMA_CHANNEL_ADDRESS_EP1 DW0_CH_STRUCT9
#define USBD_DMA_CHANNEL_ADDRESS_EP2 DW0_CH_STRUCT10
#define USBD_DMA_CHANNEL_ADDRESS_EP3 DW0_CH_STRUCT11
#define USBD_DMA_CHANNEL_ADDRESS_EP4 DW0_CH_STRUCT12
#define USBD_DMA_CHANNEL_ADDRESS_EP5 DW0_CH_STRUCT13
#define USBD_DMA_CHANNEL_ADDRESS_EP6 DW0_CH_STRUCT14
#define USBD_DMA_CHANNEL_ADDRESS_EP7 DW0_CH_STRUCT15
#define USBD_DMA_OUT_TRIG_MUX_EP0 TRIG_OUT_1TO1_6_PDMA0_TR_OUT8_TO_USB_ACK0
#define USBD_DMA_OUT_TRIG_MUX_EP1 TRIG_OUT_1TO1_6_PDMA0_TR_OUT9_TO_USB_ACK1
#define USBD_DMA_OUT_TRIG_MUX_EP2 TRIG_OUT_1TO1_6_PDMA0_TR_OUT10_TO_USB_ACK2
#define USBD_DMA_OUT_TRIG_MUX_EP3 TRIG_OUT_1TO1_6_PDMA0_TR_OUT11_TO_USB_ACK3
#define USBD_DMA_OUT_TRIG_MUX_EP4 TRIG_OUT_1TO1_6_PDMA0_TR_OUT12_TO_USB_ACK4
#define USBD_DMA_OUT_TRIG_MUX_EP5 TRIG_OUT_1TO1_6_PDMA0_TR_OUT13_TO_USB_ACK5
#define USBD_DMA_OUT_TRIG_MUX_EP6 TRIG_OUT_1TO1_6_PDMA0_TR_OUT14_TO_USB_ACK6
#define USBD_DMA_OUT_TRIG_MUX_EP7 TRIG_OUT_1TO1_6_PDMA0_TR_OUT15_TO_USB_ACK7
```

Execute this code before calling any of the emUSB-Device APIs or in the USBD_X_Config() function.
For example: Insert the code into USBD_X_Config(), under condition `#if USBD_ENABLE_DMA == true` and before `Cy_DMA_Enable(DW0);` (line 247 of usbd_config.c).

**For PSoC 61**
```c
Cy_TrigMux_Connect(TRIG0_IN_TR_GROUP13_OUTPUT2, TRIG0_OUT_CPUSS_DW0_TR_IN7, false, TRIGGER_TYPE_LEVEL);
Cy_TrigMux_Connect(TRIG0_IN_TR_GROUP13_OUTPUT3, TRIG0_OUT_CPUSS_DW0_TR_IN6, false, TRIGGER_TYPE_LEVEL);
Cy_TrigMux_Connect(TRIG0_IN_TR_GROUP13_OUTPUT4, TRIG0_OUT_CPUSS_DW0_TR_IN5, false, TRIGGER_TYPE_LEVEL);
Cy_TrigMux_Connect(TRIG0_IN_TR_GROUP13_OUTPUT5, TRIG0_OUT_CPUSS_DW0_TR_IN4, false, TRIGGER_TYPE_LEVEL);
Cy_TrigMux_Connect(TRIG0_IN_TR_GROUP13_OUTPUT6, TRIG0_OUT_CPUSS_DW0_TR_IN3, false, TRIGGER_TYPE_LEVEL);
Cy_TrigMux_Connect(TRIG0_IN_TR_GROUP13_OUTPUT7, TRIG0_OUT_CPUSS_DW0_TR_IN2, false, TRIGGER_TYPE_LEVEL);
Cy_TrigMux_Connect(TRIG0_IN_TR_GROUP13_OUTPUT8, TRIG0_OUT_CPUSS_DW0_TR_IN8, false, TRIGGER_TYPE_LEVEL);
Cy_TrigMux_Connect(TRIG0_IN_TR_GROUP13_OUTPUT9, TRIG0_OUT_CPUSS_DW0_TR_IN9, false, TRIGGER_TYPE_LEVEL);
Cy_TrigMux_Connect(TRIG13_IN_USB_DMA_REQ0, TRIG13_OUT_TR_GROUP0_INPUT34, false, TRIGGER_TYPE_LEVEL);
Cy_TrigMux_Connect(TRIG13_IN_USB_DMA_REQ1, TRIG13_OUT_TR_GROUP0_INPUT33, false, TRIGGER_TYPE_LEVEL);
Cy_TrigMux_Connect(TRIG13_IN_USB_DMA_REQ2, TRIG13_OUT_TR_GROUP0_INPUT32, false, TRIGGER_TYPE_LEVEL);
Cy_TrigMux_Connect(TRIG13_IN_USB_DMA_REQ3, TRIG13_OUT_TR_GROUP0_INPUT31, false, TRIGGER_TYPE_LEVEL);
Cy_TrigMux_Connect(TRIG13_IN_USB_DMA_REQ4, TRIG13_OUT_TR_GROUP0_INPUT30, false, TRIGGER_TYPE_LEVEL);
Cy_TrigMux_Connect(TRIG13_IN_USB_DMA_REQ5, TRIG13_OUT_TR_GROUP0_INPUT29, false, TRIGGER_TYPE_LEVEL);
Cy_TrigMux_Connect(TRIG13_IN_USB_DMA_REQ6, TRIG13_OUT_TR_GROUP0_INPUT35, false, TRIGGER_TYPE_LEVEL);
Cy_TrigMux_Connect(TRIG13_IN_USB_DMA_REQ7, TRIG13_OUT_TR_GROUP0_INPUT36, false, TRIGGER_TYPE_LEVEL);
Cy_TrigMux_Connect(TRIG9_IN_CPUSS_DW0_TR_OUT2, TRIG9_OUT_USB_DMA_BURSTEND0, false, TRIGGER_TYPE_EDGE);
Cy_TrigMux_Connect(TRIG9_IN_CPUSS_DW0_TR_OUT3, TRIG9_OUT_USB_DMA_BURSTEND1, false, TRIGGER_TYPE_EDGE);
Cy_TrigMux_Connect(TRIG9_IN_CPUSS_DW0_TR_OUT4, TRIG9_OUT_USB_DMA_BURSTEND2, false, TRIGGER_TYPE_EDGE);
Cy_TrigMux_Connect(TRIG9_IN_CPUSS_DW0_TR_OUT5, TRIG9_OUT_USB_DMA_BURSTEND3, false, TRIGGER_TYPE_EDGE);
Cy_TrigMux_Connect(TRIG9_IN_CPUSS_DW0_TR_OUT6, TRIG9_OUT_USB_DMA_BURSTEND4, false, TRIGGER_TYPE_EDGE);
Cy_TrigMux_Connect(TRIG9_IN_CPUSS_DW0_TR_OUT7, TRIG9_OUT_USB_DMA_BURSTEND5, false, TRIGGER_TYPE_EDGE);
Cy_TrigMux_Connect(TRIG9_IN_CPUSS_DW0_TR_OUT8, TRIG9_OUT_USB_DMA_BURSTEND6, false, TRIGGER_TYPE_EDGE);
Cy_TrigMux_Connect(TRIG9_IN_CPUSS_DW0_TR_OUT9, TRIG9_OUT_USB_DMA_BURSTEND7, false, TRIGGER_TYPE_EDGE);
```

**For PSoC 62**
```c
Cy_TrigMux_Select(TRIG_OUT_1TO1_5_USB_DMA0_TO_PDMA0_TR_IN8, false, TRIGGER_TYPE_EDGE);
Cy_TrigMux_Select(TRIG_OUT_1TO1_5_USB_DMA1_TO_PDMA0_TR_IN9, false, TRIGGER_TYPE_EDGE);
Cy_TrigMux_Select(TRIG_OUT_1TO1_5_USB_DMA2_TO_PDMA0_TR_IN10, false, TRIGGER_TYPE_EDGE);
Cy_TrigMux_Select(TRIG_OUT_1TO1_5_USB_DMA3_TO_PDMA0_TR_IN11, false, TRIGGER_TYPE_EDGE);
Cy_TrigMux_Select(TRIG_OUT_1TO1_5_USB_DMA4_TO_PDMA0_TR_IN12, false, TRIGGER_TYPE_EDGE);
Cy_TrigMux_Select(TRIG_OUT_1TO1_5_USB_DMA5_TO_PDMA0_TR_IN13, false, TRIGGER_TYPE_EDGE);
Cy_TrigMux_Select(TRIG_OUT_1TO1_5_USB_DMA6_TO_PDMA0_TR_IN14, false, TRIGGER_TYPE_EDGE);
Cy_TrigMux_Select(TRIG_OUT_1TO1_5_USB_DMA7_TO_PDMA0_TR_IN15, false, TRIGGER_TYPE_EDGE);
Cy_TrigMux_Select(TRIG_OUT_1TO1_6_PDMA0_TR_OUT10_TO_USB_ACK2, false, TRIGGER_TYPE_EDGE);
Cy_TrigMux_Select(TRIG_OUT_1TO1_6_PDMA0_TR_OUT11_TO_USB_ACK3, false, TRIGGER_TYPE_EDGE);
Cy_TrigMux_Select(TRIG_OUT_1TO1_6_PDMA0_TR_OUT12_TO_USB_ACK4, false, TRIGGER_TYPE_EDGE);
Cy_TrigMux_Select(TRIG_OUT_1TO1_6_PDMA0_TR_OUT13_TO_USB_ACK5, false, TRIGGER_TYPE_EDGE);
Cy_TrigMux_Select(TRIG_OUT_1TO1_6_PDMA0_TR_OUT14_TO_USB_ACK6, false, TRIGGER_TYPE_EDGE);
Cy_TrigMux_Select(TRIG_OUT_1TO1_6_PDMA0_TR_OUT15_TO_USB_ACK7, false, TRIGGER_TYPE_EDGE);
Cy_TrigMux_Select(TRIG_OUT_1TO1_6_PDMA0_TR_OUT8_TO_USB_ACK0, false, TRIGGER_TYPE_EDGE);
Cy_TrigMux_Select(TRIG_OUT_1TO1_6_PDMA0_TR_OUT9_TO_USB_ACK1, false, TRIGGER_TYPE_EDGE);
```

## Supported Software and Tools

This version of the emUSB-Device was validated for the compatibility with the following Software and Tools:

| Software and Tools                                      | Version |
| :---                                                    | :----:  |
| ModusToolbox Software Environment                       | 3.2.0   |
| MTB CAT1 Peripheral Driver Library                      | 3.10.1  |
| MTB CAT3 XMC Library                                    | 4.4.0   |
| Hardware Abstraction Layer                              | 2.6.0   |
| RTOS Abstraction                                        | 1.7.6   |
| Retarget IO                                             | 1.5.0   |
| Retarget IO (CAT3)                                      | 1.1.0   |
| GCC Compiler                                            | 11.3.1  |
| IAR Compiler                                            | 9.40.2  |
| ARM Compiler 6                                          | 6.16    |

**_NOTE:_** CAT3 device family supports only GCC_ARM compiler

## More information

For more information, refer to the following documents:

* [emUSB-Device README.md](./README.md)
* [emUSB-Device API Reference Guide](https://infineon.github.io/emusb-device/html/index.html)
* [ModusToolbox Software Environment, Quick Start Guide, Documentation, and Videos](https://www.infineon.com/cms/en/design-support/tools/sdk/modustoolbox-software/)
* [Infineon Technologies AG](https://www.infineon.com)

---
© 2022-2024, Cypress Semiconductor Corporation (an Infineon company) or an affiliate of Cypress Semiconductor Corporation.

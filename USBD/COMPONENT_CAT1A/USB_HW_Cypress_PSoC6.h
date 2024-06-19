/*********************************************************************
*                   (c) SEGGER Microcontroller GmbH                  *
*                        The Embedded Experts                        *
**********************************************************************
*                                                                    *
*       (c) 2003 - 2024     SEGGER Microcontroller GmbH              *
*                                                                    *
*       www.segger.com     Support: www.segger.com/ticket            *
*                                                                    *
**********************************************************************
*                                                                    *
*       emUSB-Device * USB Device stack for embedded applications    *
*                                                                    *
*       Please note: Knowledge of this file may under no             *
*       circumstances be used to write a similar product.            *
*       Thank you for your fairness !                                *
*                                                                    *
**********************************************************************
*                                                                    *
*       emUSB-Device version: V3.64.1                                *
*                                                                    *
**********************************************************************
----------------------------------------------------------------------
All Intellectual Property rights in the software belongs to SEGGER.
emUSB Device is protected by international copyright laws. This file
may only be used in accordance with the following terms:

The source code of the emUSB Device software has been licensed to Cypress
Semiconductor Corporation, whose registered office is 198 Champion
Court, San Jose, CA 95134, USA including the 
right to create and distribute the object code version of 
the emUSB Device software for its Cortex M0, M0+, M4, M33 and M55 based devices.
The object code version can be used by Cypress customers under the 
terms and conditions of the associated End User License Agreement.
Support for the object code version is provided by Cypress, 
full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
Licensing information
Licensor:                 SEGGER Microcontroller Systems LLC
Licensed to:              Cypress Semiconductor Corp, 198 Champion Ct., San Jose, CA 95134, USA
Licensed SEGGER software: emUSB-Device
License number:           USBD-00500
License model:            Cypress Services and License Agreement, signed November 17th/18th, 2010
                          and Amendment Number One, signed December 28th, 2020 and February 10th, 2021
                          and Amendment Number Three, signed May 2nd, 2022 and May 5th, 2022 and Amendment Number Four, signed August 28th, 2023
Licensed platform:        Cypress devices containing ARM Cortex M cores: M0, M0+, M4, M33 and M55
----------------------------------------------------------------------
Support and Update Agreement (SUA)
SUA period:               2022-05-12 - 2024-05-19
Contact to extend SUA:    sales@segger.com
----------------------------------------------------------------------
Purpose : USB driver header file for EHCI USB controller.
-------------------------- END-OF-HEADER -----------------------------
*/

#ifndef USB_HW_CYPRESS_PSOC6_H
#define USB_HW_CYPRESS_PSOC6_H

/*********************************************************************
*
*       #include Section
*
**********************************************************************
*/
#include "USB.h"

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

/*********************************************************************
*
*       Types
*
**********************************************************************
*/
typedef struct {
  void *     pDmaChannel;
  U8         Prio;
} USB_CYPRESS_PSoC6_DMA_CH_CFG;

typedef struct {
  void     (*pfTrigger)(unsigned Endpoint);
  USB_CYPRESS_PSoC6_DMA_CH_CFG ChannelCfg[8];
} USB_CYPRESS_PSoC6_DMA_CONFIG;

typedef struct {
  unsigned (*pfInitDMADescr)   (U32 * pDesc, int IsIn, U32 Size, PTR_ADDR DataRegAddr, const void *pBuffer);
  unsigned (*pfUpdateDMADescr) (U32 * pDesc, U32 Size, const void *pData);
  void     (*pfEnableDMA)      (const USB_CYPRESS_PSoC6_DMA_CH_CFG *pDMA, U32 * pDesc, U8 FirstDescr);
  void     (*pfDisableDMA)     (const USB_CYPRESS_PSoC6_DMA_CH_CFG *pDMA);
} USB_CYPRESS_PSoC6_DMA_API;

extern const USB_CYPRESS_PSoC6_DMA_API USB_DRIVER_Cypress_PSoC6_DWx;

/*********************************************************************
*
*       API functions
*
**********************************************************************
*/
void USB_DRIVER_Cypress_PSoC6_SysTick(void);
void USB_DRIVER_Cypress_PSoC6_Resume(void);
void USB_DRIVER_Cypress_PSoC6_ConfigDMA(const USB_CYPRESS_PSoC6_DMA_API *pAPI, const USB_CYPRESS_PSoC6_DMA_CONFIG *pCFG);

/*********************************************************************
*
*       Available target USB drivers
*
**********************************************************************
*/
extern const USB_HW_DRIVER USB_Driver_Cypress_PSoC6;
extern const USB_HW_DRIVER USB_Driver_Cypress_PSoC6_DMA;

#if defined(__cplusplus)
  }              /* Make sure we have C-declarations in C++ programs */
#endif

#endif /* USB_HW_CYPRESS_PSOC6_H */

/*************************** End of file ****************************/

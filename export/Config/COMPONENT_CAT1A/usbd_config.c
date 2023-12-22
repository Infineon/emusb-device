/*********************************************************************
*                   (c) SEGGER Microcontroller GmbH                  *
*                        The Embedded Experts                        *
**********************************************************************
*                                                                    *
*       (c) 2003 - 2023     SEGGER Microcontroller GmbH              *
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
*       emUSB-Device version: V3.62.0                                *
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
File        : usbd_config.c
Purpose     : emUSB-Device configuration file for CAT1A device
--------  END-OF-HEADER  ---------------------------------------------
*/

#include "USB.h"
#if defined (USBD_USE_PDL) && (USBD_USE_PDL == 1U)
#include "cy_pdl.h"
#else
#include "cyhal.h"
#endif /* #if defined (USBD_USE_PDL) && (USBD_USE_PDL == 1U) */

/* Define interrupt priority */
#define USBD_ISR_PRIO                           (3U)

/* Define interrupt source */
#if (COMPONENT_CM0P)
/* The number of CM0+ interrupt vectors are different between device,
 * change this macro for your selected device.
 */
#define USBD_INTERRUPT_NUM                      (NvicMux7_IRQn)
#else
#define USBD_INTERRUPT_NUM                      (usb_interrupt_med_IRQn)
#endif /* #if (COMPONENT_CM0P) */


/*********************************************************************
*
*       enable_isr
*  Function description
*    Configure and enable interrupts.
*/
static void enable_isr(USB_ISR_HANDLER * pfISRHandler)
{
#if defined (USBD_USE_PDL) && (USBD_USE_PDL == 1U)
    cy_stc_sysint_t usb_int_cfg =
    {
        .intrSrc = USBD_INTERRUPT_NUM,
#if defined (COMPONENT_CM0P)
        .cm0pSrc = usb_interrupt_med_IRQn,
#endif /* #if defined (COMPONENT_CM0P) */
        .intrPriority = USBD_ISR_PRIO
    };

    cy_en_sysint_status_t status;

    /* Install the interrupt service routine */
    status = Cy_SysInt_Init(&usb_int_cfg, pfISRHandler);
    CY_ASSERT(CY_RSLT_SUCCESS == status);
    (void) status; /* To avoid the compiler warning in Release mode */
#else
    /* Install the interrupt service routine */
    cy_rslt_t result;
    result = cyhal_system_set_isr(USBD_INTERRUPT_NUM, usb_interrupt_med_IRQn, USBD_ISR_PRIO, pfISRHandler);
    CY_ASSERT(CY_RSLT_SUCCESS == result);
    (void) result; /* To avoid the compiler warning in Release mode */
#endif /* #if defined (USBD_USE_PDL) && (USBD_USE_PDL == 1U) */
    NVIC_EnableIRQ(USBD_INTERRUPT_NUM);
}

/*********************************************************************
*
*       USBD_X_Config
*
*  Function description
*    Configure the USB stack. This function is always called from
*    USBD_Init().
*
*/
void USBD_X_Config(void)
{
    /* Add USB Driver */
    USBD_AddDriver(&USB_Driver_Cypress_PSoC6);
    /* Configure interrupt */
    USBD_SetISREnableFunc(enable_isr);
}

/*********************************************************************
*
*       USBD_X_EnableInterrupt
*
*  Function description
*    This function is called by the stack to enable USB interrupt(s)
*    after they have been disabled by USBD_X_DisableInterrupt().
*
*/
void USBD_X_EnableInterrupt(void)
{
    NVIC_EnableIRQ(USBD_INTERRUPT_NUM);
}

/*********************************************************************
*
*       USBD_X_DisableInterrupt
*
*  Function description
*    This function is called by the stack in cases where the stack
*    must perform a critical operation which can not be interrupted
*    by a new incoming USB interrupt event.
*
*/
void USBD_X_DisableInterrupt(void)
{
    NVIC_DisableIRQ(USBD_INTERRUPT_NUM);
}
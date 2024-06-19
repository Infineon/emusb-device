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
File        : usbd_config.c
Purpose     : emUSB-Device configuration file for CAT3 (XMC4xxx) device
--------  END-OF-HEADER  ---------------------------------------------
*/

#include "USB.h"
#include "xmc_device.h"
#include "xmc_scu.h"
#include "xmc4_scu.h"
#include "cy_utils.h"

/* Define interrupt priority. In case of FreeRTOS ensure that
 * configMAX_SYSCALL_INTERRUPT_PRIORITY in FreeRTOSConfig.h is
 * configured in accordance to XMC4xxx interrupts configuration
 */
#define USBD_ISR_PRIO                           (63U)

#define USBD_CPU_DRIVER                         (0U)
#define USBD_DYN_MEM_DRIVER                     (1U)
#define USBD_DMA_DRIVER                         (2U)


#if !defined (USBD_DRIVER)
#define USBD_DRIVER                             (USBD_CPU_DRIVER)
#endif /* #if !defined (USBD_DRIVER) */

/* Disable OTG in default configuration */
#if !defined(USBH_ENABLE_OTG)
#define USBH_ENABLE_OTG                         (0U)
#endif /* #if !defined(USBH_ENABLE_OTG) */

#if ((USBD_DRIVER) == (USBD_DYN_MEM_DRIVER)) || ((USBD_DRIVER) == (USBD_DMA_DRIVER))
/* Define the size of memory dedicated for drivers with DMA or
 * Dynamic memory support in bytes. The memory is used for the
 * endpoints buffers and transfer descriptors. Update this value
 * with the optimal memory pool size (strongly recommended) for
 * the application. For details on selecting the optimal memory
 * pool size, refer to the USBD_AssignMemory() description in
 * emUSB-Device User Guide & Reference Manual.
 */
#define USBD_MEMORY_POOL_SIZE                   (2048U)
#endif /* #if ((USBD_DRIVER) == (USBD_DYN_MEM_DRIVER)) || ((USBD_DRIVER) == (USBD_DMA_DRIVER)) */


#if (USBD_DRIVER) == (USBD_DYN_MEM_DRIVER)
static uint32_t mem_pool[USBD_MEMORY_POOL_SIZE / 4U];
#elif (USBD_DRIVER) == (USBD_DMA_DRIVER)
CY_SECTION("USB_RAM") __USED static uint32_t mem_pool[USBD_MEMORY_POOL_SIZE / 4U];
extern uint32_t USB_RAM_start;
extern uint32_t USB_RAM_end;
#endif /* #if (USBD_DRIVER) == (USBD_DYN_MEM_DRIVER) */

#if (USBH_ENABLE_OTG) == 0
static USB_ISR_HANDLER * p_usb_isr_handler;
#else
/* Function declaration is a part of emUSB-Host templates */
void usb_enable_isr(IRQn_Type isr_index, USB_ISR_HANDLER * p_isr_handler, U32 isr_prio);
#endif /* #if (USBH_ENABLE_OTG) == 0 */


#if (USBD_DRIVER) == (USBD_DMA_DRIVER)
/*********************************************************************
*
*       check_mem_addr
*  Function description
*    Checks if an address can be used for DMA transfers.
*    The function must return 0, if DMA access is allowed for the given
*    address, 1 otherwise.
*
*/
static int check_mem_addr(const void * pMem)
{
    int status = 0;
    if ((SEGGER_PTR2ADDR(pMem) < USB_RAM_start) || (SEGGER_PTR2ADDR(pMem) > USB_RAM_end))
    {
        status = 1;
    }
    return status;
}
#endif /* #if (USBD_DRIVER) == (USBD_DMA_DRIVER) */

#if (USBH_ENABLE_OTG) == 0
/*********************************************************************
*
*       USB0_0_IRQHandler
*  Function description
*    USB IRQ Handler
*
*/
void USB0_0_IRQHandler(void)
{
    if (p_usb_isr_handler != NULL)
    {
        (p_usb_isr_handler)();
    }
}
#endif /* #if (USBH_ENABLE_OTG) == 0 */

/*********************************************************************
*
*       usbd_enable_isr
*  Function description
*    Configure and enable interrupts.
*
*/
static void usbd_enable_isr(USB_ISR_HANDLER * pfISRHandler)
{
#if (USBH_ENABLE_OTG) == 0
    p_usb_isr_handler = pfISRHandler;
    NVIC_SetPriority(USB0_0_IRQn, USBD_ISR_PRIO);
    NVIC_EnableIRQ(USB0_0_IRQn);
#else
    usb_enable_isr(USB0_0_IRQn, pfISRHandler, USBD_ISR_PRIO);
#endif /* #if (USBH_ENABLE_OTG) == 0 */
}

/*********************************************************************
*
*       hw_attach
*  Function description
*    Enable USB weak pull-up at PADN state
*
*/
static void hw_attach(void)
{
    XMC_SCU_PCU_EnableUsbPullUp();
}

/*********************************************************************
*
*       USBD_X_Config
*
*  Function description
*    Configure the USB stack. This function is always called from
*    USBD_Init().
*/
void USBD_X_Config(void)
{
    /* Enable power for USB */
    XMC_SCU_POWER_EnableUsb();
    /* De-assert Reset from USB controller */
    XMC_SCU_RESET_DeassertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_USB0);
    /* Add USB Driver */
#if (USBD_DRIVER) == (USBD_CPU_DRIVER)
    /* Add USB Driver */
    USBD_AddDriver(&USB_Driver_Infineon_XMC45xx);
#elif (USBD_DRIVER) == (USBD_DYN_MEM_DRIVER)
    USBD_AddDriver(&USB_Driver_Infineon_XMC45xx_DynMem);
    USBD_AssignMemory(mem_pool, sizeof(mem_pool));
#elif (USBD_DRIVER) == (USBD_DMA_DRIVER)
    USBD_AddDriver(&USB_Driver_Infineon_XMC45xx_DMA);
    USBD_AssignMemory(mem_pool, sizeof(mem_pool));
    USBD_SetCheckAddress(check_mem_addr);
#endif /* #if (USBD_DRIVER) == (USBD_CPU_DRIVER) */
    /* Configure interrupt */
    USBD_SetISREnableFunc(usbd_enable_isr);
    USBD_SetAttachFunc(hw_attach);
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
void USBD_X_EnableInterrupt(void) {
    NVIC_EnableIRQ(USB0_0_IRQn);
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
void USBD_X_DisableInterrupt(void) {
    NVIC_DisableIRQ(USB0_0_IRQn);
}

/*************************** End of file ****************************/

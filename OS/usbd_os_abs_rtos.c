/*********************************************************************
*                   (c) SEGGER Microcontroller GmbH                  *
*                        The Embedded Experts                        *
**********************************************************************
*                                                                    *
*       (c) 2003 - 2022     SEGGER Microcontroller GmbH              *
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
*       emUSB-Device version: V3.52.2                                *
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
the emUSB Device software for its Cortex M0, M0+ and M4 based devices.
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
                          and Amendment Number Three, signed May 2nd, 2022 and May 5th, 2022
Licensed platform:        Cypress devices containing ARM Cortex M cores: M0, M0+, M4
----------------------------------------------------------------------
Support and Update Agreement (SUA)
SUA period:               2022-05-12 - 2024-05-19
Contact to extend SUA:    sales@segger.com
----------------------------------------------------------------------
File        : usbd_os_abs_rtos.c
Purpose     : OS Layer for the emUSB-Device in non-RTOS scenarios.
---------------------------END-OF-HEADER------------------------------
*/

#if !defined(COMPONENT_RTOS_AWARE)

#include "USB.h"
/* For __get_IPSR() */
#include "cy_pdl.h"


#define USBD_NUM_ALL_EVENTS                      (USB_NUM_EPS + USB_EXTRA_EVENTS)

static uint32_t critical_section_count;
static volatile unsigned usbd_event_transact_cnt[USBD_NUM_ALL_EVENTS];

static volatile bool usbd_event_transact_flag[USBD_NUM_ALL_EVENTS];

#include "cyhal.h"

#if !defined (USBD_NORTOS_TICKCNT_ENABLE)
#define USBD_NORTOS_TICKCNT_ENABLE              (1U)
#endif /* #if !defined USBD_NORTOS_TICKCNT_ENABLE */

#if (USBD_NORTOS_TICKCNT_ENABLE == 1U)

static cy_rslt_t usbd_timer_config(void);
static void isr_timer(void* callback_arg, cyhal_timer_event_t event);

static uint32_t timer_tick_count;
static cyhal_timer_t timer_obj;

#endif /* (USBD_NORTOS_TICKCNT_ENABLE == 1U) */


/*********************************************************************
*
*       USB_OS_Init
*
*  Function description
*    This function initializes all OS objects that are necessary.
*/
void USB_OS_Init(void)
{
    critical_section_count = 0U;
#if (USBD_NORTOS_TICKCNT_ENABLE == 1U)
    cy_rslt_t result;
    timer_tick_count = 0U;
    result = usbd_timer_config();
    CY_ASSERT(CY_RSLT_SUCCESS == result);

    for (uint32_t i = 0U; i < USBD_NUM_ALL_EVENTS; i++)
    {
        usbd_event_transact_flag[i] = false;
    }

    (void)result;  /* To avoid the compiler warning in Release mode */
#endif /* (USBD_NORTOS_TICKCNT_ENABLE == 1U) */
}

/*********************************************************************
*
*       USB_OS_DeInit
*
*  Function description
*    Frees all resources used by the OS layer.
*
*/
void USB_OS_DeInit(void)
{
#if (USBD_NORTOS_TICKCNT_ENABLE == 1U)
    cyhal_timer_free(&timer_obj);
#endif /* (USBD_NORTOS_TICKCNT_ENABLE == 1U) */
}

/*********************************************************************
*
*       USB_OS_Delay
*
*  Function description
*    Delays for a given number of ms.
*
*  Parameters
*    ms:     Number of ms.
*/
void USB_OS_Delay(int ms)
{
    cy_rslt_t result = cyhal_system_delay_ms((uint32_t)ms);
    CY_ASSERT(CY_RSLT_SUCCESS == result);
    (void)result;  /* To avoid the compiler warning in Release mode */
}

/*********************************************************************
*
*       USB_OS_DecRI
*
*  Function description
*    Leave a critical region for the USB stack: Decrements interrupt disable count and
*    enable interrupts if counter reaches 0.
*
*  Additional information
*    The USB stack will perform nested calls to USB_OS_IncDI() and USB_OS_DecRI().
*    This function may be called from a task context or from within an interrupt. If called
*    from an interrupt, it need not do anything.
*
*    An alternate implementation would be to
*      * enable the USB interrupts,
*      * unlock the mutex or semaphore locked in  USB_OS_IncDI()
*    if the disable count reaches 0.
*
*    This may be more efficient, because interrupts of other peripherals can be serviced
*    while inside a critical section of the USB stack.
*/
void USB_OS_DecRI(void)
{

    /* Alternate implementation has two variants: the RTOS is not used - the function
    * only disables the interrupt and increments the critical_section_count variable;
    * the RTOS is used - additionally, the function uses Mutex to check the lock state
    * of the critical_section_count variable.
    */

    /* If called from an interrupt, it need not do anything. */
    if (0U == __get_IPSR())
    {
    /* Use mutex to allow/block critical_section_count variable incrementing
    * for other tasks
    */

        /* Check if interrupt is already disabled */
        CY_ASSERT(0U != critical_section_count);

        /* Enable the interrupt even if the RTOS is not used. */
        critical_section_count--;
        if (0U == critical_section_count)
        {
            USBD_X_EnableInterrupt();
        }

    /* Use mutex to allow/block critical_section_count variable incrementing
    * for other tasks
    */
    }
}

/*********************************************************************
*
*        USB_OS_IncDI
*
*  Function description
*    Enter a critical region for the USB stack: Increments interrupt disable count and
*    disables interrupts.
*
*  Additional information
*    The USB stack will perform nested calls to USB_OS_IncDI() and USB_OS_DecRI().
*    This function may be called from a task context or from within an interrupt. If called
*    from an interrupt, it need not do anything.
*
*    An alternate implementation would be to
*      * perform a lock using a mutex or semaphore and
*      * disable the USB interrupts.
*
*    This may be more efficient, because interrupts of other peripherals can be serviced
*    while inside a critical section of the USB stack.
*/
void USB_OS_IncDI(void)
{

    /* Alternate implementation has two variants: the RTOS is not used - the function
    * only disables the interrupt and increments the critical_section_count variable;
    * the RTOS is used - additionally, the function uses Mutex to check the lock state
    * of the critical_section_count variable.
    */

    /* If called from an interrupt, it need not do anything. */
    if (0U == __get_IPSR())
    {
    /* Use mutex to allow/block critical_section_count variable decrementing
    * for other tasks
    */

        /* Disable the interrupt even if the RTOS is not used. */
        if (0U == critical_section_count)
        {
            USBD_X_DisableInterrupt();
        }
        critical_section_count++;

    /* Use mutex to allow/block critical_section_count variable decrementing
    * for other tasks
    */
    }
}

/*********************************************************************
*
*       USB_OS_Signal
*
*  Function description
*    Wakes the task waiting for signal.
*
*  Parameters
*    EPIndex:     Endpoint index. Signaling must be independent for all endpoints.
*    TransactCnt: Transaction counter. Specifies which transaction has been finished.
*
*  Additional information
*    This routine is typically called from within an interrupt
*    service routine.
*/
void USB_OS_Signal(unsigned EPIndex, unsigned TransactCnt)
{
    usbd_event_transact_cnt[EPIndex] = TransactCnt;
    usbd_event_transact_flag[EPIndex] = true;
}

/*********************************************************************
*
*        USB_OS_Wait
*
*  Function description
*    Blocks the task until USB_OS_Signal() is called for a given transaction.
*
*  Parameters
*    EPIndex:     Endpoint index. Signaling must be independent for all endpoints.
*    TransactCnt: Transaction counter.  Specifies the transaction to wait for.
*
*  Additional information
*    The function must ignore signaling transactions other than given in TransactCnt. If
*    this transaction was signaled before this function was called, it must return immediately.
*
*    This routine is called from a task.
*/
void USB_OS_Wait(unsigned EPIndex, unsigned TransactCnt)
{
    while (true)
    {
        if (usbd_event_transact_flag[EPIndex])
        {
            usbd_event_transact_flag[EPIndex] = false;

            /* Exit from USB_OS_Wait() only if signaling transaction is
             * the same as TransactCnt, otherwise continue to wait.
             */
            if(usbd_event_transact_cnt[EPIndex] == TransactCnt)
            {
                break;
            }
        }
    }
}

/*********************************************************************
*
*        USB_OS_WaitTimed
*
*  Function description
*    Blocks the task until USB_OS_Signal() is called for a given transaction or a timeout
*    occurs.
*
*  Parameters
*    EPIndex:     Endpoint index. Signaling must be independent for all endpoints.
*    ms:          Timeout time given in ms.
*    TransactCnt: Transaction counter.  Specifies the transaction to wait for.
*
*  Return value
*    == 0:        Task was signaled within the given timeout.
*    == 1:        Timeout occurred.
*
*  Additional information
*    The function must ignore signaling transactions other than given in TransactCnt. If
*    this transaction was signaled before this function was called, it must return immediately.
*
*    USB_OS_WaitTimed() is called from a task. This function is used by all available timed
*    routines.
*
*    Alternatively this function may take the given timeout in units of system ticks of the
*    underlying operating system instead of milliseconds. In this case all API functions
*    that support a timeout parameter should also use system ticks for the timeout.
*/
int USB_OS_WaitTimed(unsigned EPIndex, unsigned ms, unsigned TransactCnt)
{
    bool first_check = true;
    /* Read current system time */
    U32 tick_count_to_wait = USB_OS_GetTickCnt() + (U32) ms;
    while (0U != ms)
    {
        /* Do not wait 1 ms during first iteration of loop,
         * because if this transaction was signaled before this function
         * was called, it must return immediately
         */
        if (first_check)
        {
            first_check = false;
        }
        else
        {
            /* Wait 1 ms */
            USB_OS_Delay(1);
            ms--;
        }
        if (usbd_event_transact_flag[EPIndex])
        {
            usbd_event_transact_flag[EPIndex] = false;

            /* Exit from USB_OS_WaitTimed() only if signaling transaction is
             * the same as TransactCnt or timeout happens, otherwise continue to wait.
             */
            if(usbd_event_transact_cnt[EPIndex] == TransactCnt)
            {
                break;
            }
        }

        /* To avoid significant increasing the wait time by sum time of
         * USB_OS_Delay() and time of execution code of another functions,
         * additional check of delay by systime is used.
         */
        if (tick_count_to_wait < USB_OS_GetTickCnt())
        {
            ms = 0U;
            break;
        }
    }

    return (0U == ms) ? 1 : 0;
}

/*********************************************************************
*
*        USB_OS_GetTickCnt
*
*  Function description
*    Returns the current system time in milliseconds or system ticks.
*
*  Return value
*    Current system time.
*/
#if (USBD_NORTOS_TICKCNT_ENABLE == 1U)
U32 USB_OS_GetTickCnt(void)
{

    return (U32) timer_tick_count;
}
#endif /* (USBD_NORTOS_TICKCNT_ENABLE == 1U) */


#if (USBD_NORTOS_TICKCNT_ENABLE == 1U)

/*********************************************************************
*
*        usbd_timer_config
*
*  Function description
*    Configure timer to generate interrupt every 1 ms.
*
*  Return value
*    Status of configuration.
*/
static cy_rslt_t usbd_timer_config()
{
    cy_rslt_t result = CY_RSLT_SUCCESS;

    /* Timer configuration structure */
    const cyhal_timer_cfg_t timer_cfg =
    {
        .compare_value = 0U,   /* Set to 0, is not used */
        .period        = 999U,
        .direction     = CYHAL_TIMER_DIR_UP,
        .is_compare    = false,
        .is_continuous = true,
        .value         = 0U,
    };

    /* Initialize the timer object without output pin and pre-configured clock source */
    result = cyhal_timer_init(&timer_obj, NC, NULL);

    if (CY_RSLT_SUCCESS == result)
    {
        result = cyhal_timer_configure(&timer_obj, &timer_cfg);

        if (CY_RSLT_SUCCESS == result)
        {
            /* Set the frequency of timer to 1000000 Hz */
            result = cyhal_timer_set_frequency(&timer_obj, 1000000U);

            if (CY_RSLT_SUCCESS == result)
            {
                /* Assign the ISR to execute on timer interrupt */
                cyhal_timer_register_callback(&timer_obj, isr_timer, NULL);
                /* Set the event on which timer interrupt occurs and enable it */
                cyhal_timer_enable_event(&timer_obj, CYHAL_TIMER_IRQ_TERMINAL_COUNT, 3U, true);
                /* Start the timer with the configured settings */
                result = cyhal_timer_start(&timer_obj);
            }
        }
    }
    return result;
}

/*********************************************************************
*
*        isr_timer
*
*  Function description
*    Interrupt service routine for timer. Increments value every ms.
*/
static void isr_timer(void* callback_arg, cyhal_timer_event_t event)
{
    (void)callback_arg;
    (void)event;

    timer_tick_count++;
}
#endif /* (USBD_NORTOS_TICKCNT_ENABLE == 1U) */

#if USB_NUM_MUTEXES > 0

/*********************************************************************
*
*        USB_OS_MutexAlloc
*
*  Function description
*    Allocates a new mutex to be used by USB_OS_MutexLock() / USB_OS_MutexUnlock() calls.
*
*  Return value
*    >= 0:        Valid index to be used for USB_OS_MutexLock() / USB_OS_MutexUnlock().
*    <  0:        Error: No mutex available.
*/
int USB_OS_MutexAlloc(void)
{
    return -1;
}

/*********************************************************************
*
*        USB_OS_MutexFree
*
*  Function description
*    Releases all mutexes allocated by USB_OS_MutexAlloc()
*/
void USB_OS_MutexFree(void)
{
}

/*********************************************************************
*
*        USB_OS_MutexLock
*
*  Function description
*    This function locks a mutex object that was allocated by USB_OS_MutexAlloc().
*
*  Parameters
*    Idx:     Index of the mutex to be locked (from USB_OS_MutexAlloc()).
*/
void USB_OS_MutexLock(int Idx)
{
    (void)Idx;
}

/*********************************************************************
*
*        USB_OS_MutexUnlock
*
*  Function description
*    This function unlocks a mutex object that was allocated by USB_OS_MutexAlloc().
*
*  Parameters
*    Idx:     Index of the mutex to be unlocked (from USB_OS_MutexAlloc()).
*/
void USB_OS_MutexUnlock(int Idx)
{
    (void)Idx;
}

#endif /* USB_NUM_MUTEXES > 0 */

#endif /* #if !defined(COMPONENT_RTOS_AWARE) */
/*************************** End of file ****************************/
/**
 * @file itm.h
 * @author Matt Pamenter
 * @copyright MIT License
 * 
 * @brief Lightweight ITM (Instrumentation Trace Macrocell) logging module for STM32.
 *
 * The ITM interface available on STM32 microcontrollers based 
 * on ARM Cortex-M3 and higher cores (e.g., M4, M7, M33, M55).
 * It supports usage in both bare-metal and FreeRTOS-based projects (CMSIS v1 and v2).
 *
 * Note: ITM is not available on STM32 series using Cortex-M0 or Cortex-M0+ cores.
 */

#ifndef INC_ITM_H_
#define INC_ITM_H_

#include "main.h"
#include <stdio.h>

#if __has_include("cmsis_os.h")
  #include "cmsis_os.h"
#endif

/**
 * @brief Sends a debug message over the Instrumentation Trace Macrocell interface (ITM).
 *
 * Appends a newline after the message. 
 * In FreeRTOS projects it is recommended for early boot code, interrupts, or use outside of the RTOS context.
 *
 * @param itmMessage Null-terminated string to send.
 */
void ITM_Print(const char *itmMessage);

/**
 * @brief Returns a timestamp string for log messages.
 *
 * Format: "HH:MM:SS:MMM: ".
 * - In FreeRTOS, timestamps reflect the system tick (assumes 1 kHz tick rate).
 * - In bare-metal or before the RTOS starts, a fixed placeholder is returned: "00:00:00:000: ".
 *
 * @return Pointer to a static, null-terminated timestamp string.
 *
 * @note Used internally by ITM_Error() and ITM_Event(). User code should not call this directly.
 */
char *getTimeStamp();

/**
 * @brief Sends a timestamped error message over ITM.
 *
 * The message is prefixed with a timestamp and an "ERROR: " tag.
 * - Timestamps are valid only after the RTOS kernel has started.
 * - Before that (or on bare-metal systems), a placeholder is used.
 *
 * @param itmError Null-terminated string describing the error.
 */
void ITM_Error(const char *itmError);

/**
 * @brief Sends a timestamped event message over ITM.
 *
 * The message is prefixed with a timestamp and an "Event: " tag.
 * - Timestamps are valid only after the RTOS kernel has started.
 * - Before that (or on bare-metal), a placeholder is used.
 *
 * @param itmEvent Null-terminated string describing the event.
 */
void ITM_Event(const char *itmEvent);

#endif /* INC_ITM_H_ */

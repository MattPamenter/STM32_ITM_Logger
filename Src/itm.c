/**
 * @file itm.c
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

#include "itm.h"

/**
 * @brief Sends a string over ITM, character by character.
 *
 * @param itmMessage Null-terminated string.
 */
static void sendString(const char *itmMessage)
{
  // Iterate through the characters until the null terminator
  for(int i = 0; itmMessage[i] != '\0'; i++)
  {
    ITM_SendChar(itmMessage[i]); // send char
  }
}

/**
 * @brief Sends a timestamp string.
 *
 * Format: "HH:MM:SS:MMM: ".
 * - In FreeRTOS, timestamps reflect the system tick (assumes 1 kHz tick rate).
 * - In bare-metal or before the RTOS starts, a fixed placeholder is returned: "00:00:00:000: ".
 */
static void sendTimeStamp()
{
  char timeStamp[18]; // "HH:MM:SS:MMM: " + null terminator

  #ifdef FREERTOS_CONFIG_H
    // Get the current tick count
    uint32_t currentTick;
    #ifdef CMSIS_OS2_H_
      currentTick = osKernelGetTickCount(); // FreeRTOS CMSIS v2
    #else
      currentTick = osKernelSysTick(); // FreeRTOS CMSIS v1
    #endif

    // Convert ticks to hours, minutes, seconds, and milliseconds
    uint32_t hours = currentTick / 3600000;
    uint32_t minutes = (currentTick % 3600000) / 60000;
    uint32_t seconds = (currentTick % 60000) / 1000;
    uint32_t milliseconds = currentTick % 1000;

    // Create a time string
    snprintf(timeStamp, sizeof(timeStamp), "%02lu:%02lu:%02lu:%03lu: ", hours, minutes, seconds, milliseconds);
  #else
    // No RTOS present.  Use placeholder timestamp.
    snprintf(timeStamp, sizeof(timeStamp), "00:00:00:000: ");
  #endif

  sendString(timeStamp);
}

void ITM_Print(const char *itmMessage)
{
  sendString(itmMessage);
  ITM_SendChar('\n');
}

void ITM_Error(const char *itmError)
{
  sendTimeStamp();
  sendString("ERROR: ");
  sendString(itmError);
  ITM_SendChar('\n');
}

void ITM_Event(const char *itmEvent)
{
  sendTimeStamp();
  sendString("Event: ");
  sendString(itmEvent);
  ITM_SendChar('\n');
}

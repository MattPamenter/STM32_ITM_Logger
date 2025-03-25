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

void ITM_Print(const char *itmMessage)
{
  // Iterate through the characters in the string until the null terminator
  for(int i = 0; itmMessage[i] != '\0'; i++)
  {
    ITM_SendChar(itmMessage[i]); // send char
  }
  ITM_SendChar('\n'); // send newline
}

char *getTimeStamp()
{
    static char timeStr[18]; // "HH:MM:SS:MMM: " + null terminator

    #ifdef FREERTOS_CONFIG_H
      // Get the current tick count
      uint32_t currentTick;
      #ifdef CMSIS_OS2_H_
        currentTick = osKernelGetTickCount(); // FreeRTOS CMSIS v2
      #else
        currentTick = osKernelSysTick(); // FreeRTOS CMSIS v1
      #endif

      // Convert ticks to hours, minutes, and seconds
      uint32_t hours = currentTick / 3600000;
      uint32_t minutes = (currentTick % 3600000) / 60000;
      uint32_t seconds = (currentTick % 60000) / 1000;
      uint32_t milliseconds = currentTick % 1000;

      // Create a time string
      snprintf(timeStr, sizeof(timeStr), "%02lu:%02lu:%02lu:%03lu: ", hours, minutes, seconds, milliseconds);
    #else
      // No RTOS present.  Use fixed timestamp.
      snprintf(timeStr, sizeof(timeStr), "00:00:00:000: ");
    #endif

    return timeStr;
}

void ITM_Error(const char *itmError)
{
  // Send the timestamp
  char* timeStamp = getTimeStamp();
  for(int i = 0; timeStamp[i] != '\0'; i++)
  {
    ITM_SendChar(timeStamp[i]); // send char
  }

  // Send the prefix
  const char *prefix = "ERROR: ";
  for(int i = 0; prefix[i] != '\0'; i++)
  {
    ITM_SendChar(prefix[i]); // send char
  }

  // Iterate through the characters in the string until the null terminator
  for(int i = 0; itmError[i] != '\0'; i++)
  {
    ITM_SendChar(itmError[i]); // send char
  }
  ITM_SendChar('\n'); // send newline
}

void ITM_Event(const char *itmEvent)
{
  // Send the timestamp
  char* timeStamp = getTimeStamp();
  for(int i = 0; timeStamp[i] != '\0'; i++)
  {
    ITM_SendChar(timeStamp[i]); // send char
  }

  // Send the prefix
  const char *prefix = "Event: ";
  for(int i = 0; prefix[i] != '\0'; i++)
  {
    ITM_SendChar(prefix[i]); // send char
  }

  // Iterate through the characters in the string until the null terminator
  for(int i = 0; itmEvent[i] != '\0'; i++)
  {
    ITM_SendChar(itmEvent[i]); // send char
  }
  ITM_SendChar('\n'); // send newline
}

/********************************************************************
rtc.h

Copyright (c) 2016, Jonathan Nutzmann

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
********************************************************************/

// Some code used from:

/*   ----------------------------------------------------------------------
    Copyright (C) Tilen Majerle, 2015
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.
     
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
   ----------------------------------------------------------------------*/

#ifndef RTC_H
#define RTC_H

/****************************************************************************
 * Includes
 ***************************************************************************/
#include "stdbool.h"
#include "stdint.h"
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

 
/****************************************************************************
 * Definitions
 ***************************************************************************/

/* RTC clock is: f_clk = RTCCLK(LSI or LSE) / ((RTC_SYNC_PREDIV + 1) * (RTC_ASYNC_PREDIV + 1)) */

#define RTC_SYNC_PREDIV             (0x3FF)  /* Sync pre division for clock */
#define RTC_ASYNC_PREDIV            (0x1F)   /* Async pre division for clock */
#define RTC_PRIORITY					      (0x04)   /* NVIC global Priority set */
#define RTC_WAKEUP_SUBPRIORITY			(0x00)   /* Sub priority for wakeup trigger */
#define RTC_ALARM_SUBPRIORITY       (0x01)   /* Sub priority for alarm trigger */

/****************************************************************************
 * Typedefs
 ***************************************************************************/

/**
 * @brief  RTC Struct for date/time
 */
typedef struct {
  uint8_t seconds;     /*!< Seconds parameter, from 00 to 59 */
  uint16_t subseconds; /*!< Subsecond downcounter. When it reaches zero, it's reload value is the same as
                               @ref RTC_SYNC_PREDIV, so in our case 0x3FF = 1023, 1024 steps in one second */
  uint8_t minutes;     /*!< Minutes parameter, from 00 to 59 */
  uint8_t hours;       /*!< Hours parameter, 24Hour mode, 00 to 23 */
  uint8_t day;         /*!< Day in a week, from 1 to 7 */
  uint8_t date;        /*!< Date in a month, 1 to 31 */
  uint8_t month;       /*!< Month in a year, 1 to 12 */
  uint8_t year;        /*!< Year parameter, 00 to 99, 00 is 2000 and 99 is 2099 */
  uint32_t unix_time;  /*!< Seconds from 01.01.1970 00:00:00 */
} rtc_datetime_t;

/**
 * @brief  RTC Interrupt enumeration
 */
typedef enum {
  RTC_INTERRUPTS_Disable = 0x00, /*!< Disable RTC wakeup interrupts */
  RTC_INTERRUPTS_60s,            /*!< RTC Wakeup interrupt every 60 seconds */
  RTC_INTERRUPTS_30s,            /*!< RTC Wakeup interrupt every 30 seconds */
  RTC_INTERRUPTS_15s,            /*!< RTC Wakeup interrupt every 15 seconds */
  RTC_INTERRUPTS_10s,            /*!< RTC Wakeup interrupt every 10 seconds */
  RTC_INTERRUPTS_5s,             /*!< RTC Wakeup interrupt every 5 seconds */
  RTC_INTERRUPTS_2s,             /*!< RTC Wakeup interrupt every 2 seconds */
  RTC_INTERRUPTS_1s,             /*!< RTC Wakeup interrupt every 1 seconds */
  RTC_INTERRUPTS_500ms,          /*!< RTC Wakeup interrupt every 500 milliseconds */
  RTC_INTERRUPTS_250ms,          /*!< RTC Wakeup interrupt every 250 milliseconds */
  RTC_INTERRUPTS_125ms           /*!< RTC Wakeup interrupt every 125 milliseconds */
} rtc_interrupts_t;

/**
 * @brief  Select RTC clock source
 * @note   Internal clock is not accurate and should not be used in production
 */
typedef enum {
  RTC_CLOCKSOURCE_INTERNAL = 0x00, /*!< Use internal clock source for RTC (LSI oscillator) */
  RTC_CLOCKSOURCE_EXTERNAL         /*!< Use external clock source for RTC (LSE oscillator) */
} rtc_clocksource_t;

/**
 * @brief  RTC Alarm type
 */
typedef enum {
  RTC_ALARMTYPE_DAYINWEEK, /*!< Trigger alarm every day in a week, days from 1 to 7 (Monday to Sunday) */
  RTC_ALARMTYPE_DAYINMONTH /*!< Trigger alarm every month */
} rtc_alarmtype_t;

/**
 * @brief  Alarm identifier you will use for Alarm functions
 */
typedef enum {
  RTC_ALARM_A = 0x00, /*!< Work with alarm A */
  RTC_ALARM_B         /*!< Work with alarm B */
} rtc_alarm_t;

/**
 * @brief  RTC structure for alarm time
 */
typedef struct {
  rtc_alarmtype_t alarmtype; /*!< Alarm type setting. @ref rtc_alarmtype_t for more info */
  uint8_t seconds;              /*!< Alarm seconds value */
  uint8_t minutes;              /*!< Alarm minutes value */
  uint8_t hours;                /*!< Alarm hours value */
  uint8_t day;                  /*!< Alarm day value. If you select trigger for alarm every week, then this parameter has value between
                                    1 and 7, representing days in a week, Monday to Sunday
                                    If you select trigger for alarm every month, then this parameter has value between
                                    1 - 31, representing days in a month. */
} rtc_alarm_type_t;


/****************************************************************************
 * Public Functions
 ***************************************************************************/


/**
 * @brief  Initializes RTC and starts counting
 * @param  source. RTC Clock source @ref rtc_clocksource_t to be used for RTC
 * @note   Internal clock source is not so accurate
 * @note   If you reset your MCU and RTC still has power, it will count independent of MCU status
 * @retval Returns RTC status.
 *            - 1: RTC has already been initialized and time is set
 *            - 0: RTC was now initialized first time. Now you can set your first clock
 */
uint32_t rtc_init(rtc_clocksource_t source);

/**
 * @brief  Get number of seconds from date and time since 01.01.1970 00:00:00
 * @param  *data: Pointer to @ref rtc_datetime_t data structure
 * @retval Calculated seconds from date and time since 01.01.1970 00:00:00
 */
uint32_t rtc_get_unix_time(rtc_datetime_t* data);

/**
 * @brief  Get formatted time from seconds till 01.01.1970 00:00:00
 *         It fills struct with valid data
 * @note   Valid if year is greater or equal (>=) than 2000
 * @param  *data: Pointer to @ref rtc_datetime_time_t struct to store formatted data in
 * @param  unix: Seconds from 01.01.1970 00:00:00 to calculate user friendly time
 * @retval None
 */
void rtc_get_datetime_from_unix(rtc_datetime_t* data, uint32_t unix);

/**
 * @brief  Select RTC wakeup interrupts interval
 * @note   This function can also be used to disable interrupt
 * @param  int_value: Look for @ref rtc_interrupts_t for valid inputs
 * @retval None
 */
void rtc_interrupts_config(rtc_interrupts_t int_value);

/**
 * @brief  Set date and time to internal RTC registers
 * @param  *data: Pointer to filled @ref rtc_datetime_t structure with date and time
 * @param  format: Format of your structure. This parameter can be RTC_Format_BIN or RTC_Format_BCD
 * @retval RTC datetime status @ref TM_RTC_Result_t:
 *            - @ref TM_RTC_Result_Ok: Date and Time set OK
 *            - @ref TM_RTC_Result_Error: Date and time is wrong
 */
bool rtc_set_datetime(rtc_datetime_t* data, uint32_t format);

/**
 * @brief  Set date and time using string formatted date time
 * @note   Valid string format is: <b>dd.mm.YY.x;HH:ii:ss</b>
 *            - <b>dd</b>: date, 2 digits, decimal
 *            - <b>mm</b>: month, 2 digits, decimal
 *            - <b>YY</b>: year, last 2 digits, decimal
 *            - <b>x</b>: day in a week: 1 digit, 1 = Monday, 7 = Sunday
 *            - <b>HH</b>: hours, 24-hour mode, 2 digits, decimal
 *            - <b>ii</b>: minutes, 2 digits, decimal
 *            - <b>ss</b>: seconds, 2 digits, decimal
 * @param  *str: Pointer to string with datetime format
 * @retval RTC datetime status @ref TM_RTC_Result_t:
 *            - @ref TM_RTC_Result_Ok: Date and Time set OK
 *            - @ref TM_RTC_Result_Error: Date and time is wrong
 */
bool rtc_set_datetimeString(char* str);

/**
 * @brief  Get date and time from internal RTC registers
 * @param  *data: Pointer to @ref rtc_datetime_t structure to save data to
 * @param  format: Format of your structure. This parameter can be RTC_Format_BIN or RTC_Format_BCD
 * @retval None
 */
void rtc_get_datetime(rtc_datetime_t* data, uint32_t format);

/**
 * @brief  Get number of days in month
 * @note   This function also detects if it is leap year and returns different number for February
 * @param  month: Month number in year, valid numbers are 1 - 12
 * @param  year: Year number where you want to get days in month, last 2 digits
 * @retval Number of days in specific month and year
 */
uint8_t rtc_get_days_in_month(uint8_t month, uint8_t year);

/**
 * @brief  Get number of days in specific year
 * @note   This function also detects if it is leap year
 * @param  year: Year number where you want to get days in month, last 2 digits
 * @retval Number of days in year
 */
uint16_t rtc_get_days_in_year(uint8_t year);

/**
 * @brief  Write RTC backup register value.
 *            This method allows you to write 32bit value from backup register 0 - 18.
 * @note   RTC has 20 backup registers where you can store data which will be available all the time RTC is running and has power.
 *
 * @note   My library uses register 19 to store info about RTC settings and is not available for USER to store data there.
 *
 * @note   RTC HAS to be initialized first before you can use this method.
 * @param  location: RTC backup register location. 0 to 18 are valid
 * @param  value: 32-bit long value to be stored in RTC backup register
 * @retval Value at specific RTC backup register location
 */
void rtc_write_backup_register(uint8_t location, uint32_t value);

/**
 * @brief  Read RTC backup register value.
 *            This method allows you to read 32bit value from backup register 0 - 18.
 * @note   RTC has 20 backup registers where you can store data which will be available all the time RTC is running and has power.
 *
 * @note   My library uses register 19 to store info about RTC settings and is not available for USER to store data there.
 *
 * @note   RTC HAS to be initialized first before you can use this method.
 * @param  location: RTC backup register location. 0 to 18 are valid
 * @retval Value at specific RTC backup register location
 */
uint32_t rtc_read_backup_register(uint8_t location);

/**
 * @brief  Enables alarm A or alarm B
 * @param  Alarm: Specify which alarm to set. This parameter can be a value of @ref rtc_alarm_t enumeration
 * @param  *AlarmTime: Pointer to @ref rtc_alarm_type_t structure to get data from.
 * @param  format: RTC date and time format. This parameter can be a RTC_Format_BIN or RTC_Format_BCD
 * @retval None
 */
void rtc_set_alarm(rtc_alarm_t Alarm, rtc_alarm_type_t* AlarmTime, uint32_t format);

/**
 * @brief  Disables specific alarm
 * @param  Alarm: Select which alarm you want to disable. This parameter can be a value of @ref rtc_alarm_t enumeration
 * @retval None
 */
void rtc_disable_alarm(rtc_alarm_t Alarm);

/**
 * @brief  RTC Wakeup handler function. Called when wakeup interrupt is triggered
 * @note   Called from my RTC library
 * @param  None
 * @retval None
 * @note   With __weak parameter to prevent link errors if not defined by user
 */
void rtc_request_handler(void);

/**
 * @brief  RTC Alarm A handler function. Called when interrupt is triggered for alarm A
 * @note   Called from my RTC library
 * @param  None
 * @retval None
 * @note   With __weak parameter to prevent link errors if not defined by user
 */
void rtc_alarm_a_handler(void);

/**
 * @brief  RTC Alarm B handler function. Called when interrupt is triggered for alarm B.
 * @note   Called from my RTC library
 * @param  None
 * @retval None
 * @note   With __weak parameter to prevent link errors if not defined by user
 */
void rtc_alarm_b_handler(void);

#endif /* RTC_H */

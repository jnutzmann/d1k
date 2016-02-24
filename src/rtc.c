/********************************************************************
rtc.c

Copyright (c) 2015, Jonathan Nutzmann

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


/****************************************************************************
 * Includes
 ***************************************************************************/

#include "rtc.h"
#include "stm32f4xx_rtc.h"
#include "stm32f4xx_pwr.h"


/* Private macros */
/* Internal status registers for RTC */
#define RTC_STATUS_REG      			RTC_BKP_DR19 /* Status Register */
#define RTC_STATUS_INIT_OK  			0x1234       /* RTC initialised */
#define RTC_STATUS_TIME_OK  			0x4321       /* RTC time OK */
#define	RTC_STATUS_ZERO					0x0000

/* Internal RTC defines */
#define RTC_LEAP_YEAR(year) 			((((year) % 4 == 0) && ((year) % 100 != 0)) || ((year) % 400 == 0))
#define RTC_DAYS_IN_YEAR(x)			RTC_LEAP_YEAR(x) ? 366 : 365
#define RTC_OFFSET_YEAR				1970
#define RTC_SECONDS_PER_DAY			86400
#define RTC_SECONDS_PER_HOUR			3600
#define RTC_SECONDS_PER_MINUTE		60
#define RTC_BCD2BIN(x)				((((x) >> 4) & 0x0F) * 10 + ((x) & 0x0F))
#define RTC_CHAR2NUM(x)				((x) - '0')
#define RTC_CHARISNUM(x)				((x) >= '0' && (x) <= '9')

/* Internal functions */
void RTC_Config(rtc_clocksource_t source);
/* Default RTC status */
uint32_t RTC_Status = RTC_STATUS_ZERO;
/* RTC declarations */
RTC_TimeTypeDef RTC_TimeStruct;
RTC_InitTypeDef RTC_InitStruct;
RTC_DateTypeDef RTC_DateStruct;
NVIC_InitTypeDef NVIC_InitStruct;
EXTI_InitTypeDef EXTI_InitStruct;

/* Days in a month */
uint8_t RTC_Months[2][12] = {
  {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},	/* Not leap year */
  {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}	/* Leap year */
};

uint32_t rtc_init(rtc_clocksource_t source)
{
  uint32_t status;
  uint8_t stat = 1;
  rtc_datetime_t datatime;

  /* Enable PWR peripheral clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

  /* Allow access to BKP Domain */
  PWR_BackupAccessCmd(ENABLE);

  /* Get RTC status */
  status = RTC_ReadBackupRegister(RTC_STATUS_REG);

  if (status == RTC_STATUS_TIME_OK)
  {
    RTC_Status = RTC_STATUS_TIME_OK;

    /* Start internal clock if we choose internal clock */
    if (source == RTC_CLOCKSOURCE_INTERNAL)
    {
      RTC_Config(RTC_CLOCKSOURCE_INTERNAL);
    }

    /* Wait for RTC APB registers synchronisation (needed after start-up from Reset) */
    RTC_WaitForSynchro();

    /* Clear interrupt flags */
    RTC_ClearITPendingBit(RTC_IT_WUT);
    EXTI->PR = 0x00400000;

    /* Get date and time */
    rtc_get_datetime(&datatime, RTC_Format_BIN);
  }
  else if (status == RTC_STATUS_INIT_OK)
  {
    RTC_Status = RTC_STATUS_INIT_OK;

    /* Start internal clock if we choose internal clock */
    if (source == RTC_CLOCKSOURCE_INTERNAL)
    {
      RTC_Config(RTC_CLOCKSOURCE_INTERNAL);
    }

    /* Wait for RTC APB registers synchronisation (needed after start-up from Reset) */
    RTC_WaitForSynchro();

    /* Clear interrupt flags */
    RTC_ClearITPendingBit(RTC_IT_WUT);
    EXTI->PR = 0x00400000;

    /* Get date and time */
    rtc_get_datetime(&datatime, RTC_Format_BIN);
  }
  else
  {
    RTC_Status = RTC_STATUS_ZERO;
    /* Return status = 0 -> RTC Never initialized before */
    stat = RTC_STATUS_ZERO;
    /* Config RTC */
    RTC_Config(source);

    /* Set date and time */
    datatime.date = 1;
    datatime.day = 1;
    datatime.month = 1;
    datatime.year = 0;
    datatime.hours = 0;
    datatime.minutes = 0;
    datatime.seconds = 0;

    /* Set date and time */
    rtc_set_datetime(&datatime, RTC_Format_BIN);

    /* Initialized OK */
    RTC_Status = RTC_STATUS_INIT_OK;
  }

  /* If first time initialized */
  if (stat == RTC_STATUS_ZERO)
  {
    return 0;
  }

  return RTC_Status;
}

bool rtc_set_datetime(rtc_datetime_t* data, uint32_t format)
{
  rtc_datetime_t tmp;

  /* Check date and time validation */
  if (format == RTC_Format_BCD) {
    tmp.date = RTC_BCD2BIN(data->date);
    tmp.month = RTC_BCD2BIN(data->month);
    tmp.year = RTC_BCD2BIN(data->year);
    tmp.hours = RTC_BCD2BIN(data->hours);
    tmp.minutes = RTC_BCD2BIN(data->minutes);
    tmp.seconds = RTC_BCD2BIN(data->seconds);
    tmp.day = RTC_BCD2BIN(data->day);
  } else {
    tmp.date = data->date;
    tmp.month = data->month;
    tmp.year = data->year;
    tmp.hours = data->hours;
    tmp.minutes = data->minutes;
    tmp.seconds = data->seconds;
    tmp.day = data->day;
  }

  /* Check year and month */
  if (
      tmp.year > 99 ||
      tmp.month == 0 ||
      tmp.month > 12 ||
      tmp.date == 0 ||
      tmp.date > RTC_Months[RTC_LEAP_YEAR(2000 + tmp.year) ? 1 : 0][tmp.month - 1] ||
      tmp.hours > 23 ||
      tmp.minutes > 59 ||
      tmp.seconds > 59 ||
      tmp.day == 0 ||
      tmp.day > 7
      ) {
    /* Invalid date */
    return false;
  }

  /* Fill time */
  RTC_TimeStruct.RTC_Hours = data->hours;
  RTC_TimeStruct.RTC_Minutes = data->minutes;
  RTC_TimeStruct.RTC_Seconds = data->seconds;
  /* Fill date */
  RTC_DateStruct.RTC_Date = data->date;
  RTC_DateStruct.RTC_Month = data->month;
  RTC_DateStruct.RTC_Year = data->year;
  RTC_DateStruct.RTC_WeekDay = data->day;

  /* Set the RTC time base to 1s and hours format to 24h */
  RTC_InitStruct.RTC_HourFormat = RTC_HourFormat_24;
  RTC_InitStruct.RTC_AsynchPrediv = RTC_ASYNC_PREDIV;
  RTC_InitStruct.RTC_SynchPrediv = RTC_SYNC_PREDIV;
  RTC_Init(&RTC_InitStruct);

  RTC_SetTime(format, &RTC_TimeStruct);
  RTC_SetDate(format, &RTC_DateStruct);

  if (RTC_Status != RTC_STATUS_ZERO) {
    /* Write backup registers */
    RTC_WriteBackupRegister(RTC_STATUS_REG, RTC_STATUS_TIME_OK);
  }

  /* Return OK */
  return true;
}

bool rtc_set_datetimeString(char* str) {
  rtc_datetime_t tmp;
  uint8_t i = 0;

  /* Get date */
  tmp.date = 0;
  while (RTC_CHARISNUM(*(str + i))) {
    tmp.date = tmp.date * 10 + RTC_CHAR2NUM(*(str + i));
    i++;
  }
  i++;

  /* Get month */
  tmp.month = 0;
  while (RTC_CHARISNUM(*(str + i))) {
    tmp.month = tmp.month * 10 + RTC_CHAR2NUM(*(str + i));
    i++;
  }
  i++;

  /* Get year */
  tmp.year = 0;
  while (RTC_CHARISNUM(*(str + i))) {
    tmp.year = tmp.year * 10 + RTC_CHAR2NUM(*(str + i));
    i++;
  }
  i++;

  /* Get day in a week */
  tmp.day = 0;
  while (RTC_CHARISNUM(*(str + i))) {
    tmp.day = tmp.day * 10 + RTC_CHAR2NUM(*(str + i));
    i++;
  }
  i++;

  /* Get hours */
  tmp.hours = 0;
  while (RTC_CHARISNUM(*(str + i))) {
    tmp.hours = tmp.hours * 10 + RTC_CHAR2NUM(*(str + i));
    i++;
  }
  i++;

  /* Get minutes */
  tmp.minutes = 0;
  while (RTC_CHARISNUM(*(str + i))) {
    tmp.minutes = tmp.minutes * 10 + RTC_CHAR2NUM(*(str + i));
    i++;
  }
  i++;

  /* Get seconds */
  tmp.seconds = 0;
  while (RTC_CHARISNUM(*(str + i))) {
    tmp.seconds = tmp.seconds * 10 + RTC_CHAR2NUM(*(str + i));
    i++;
  }
  i++;

  /* Return status from set date time function */
  return rtc_set_datetime(&tmp, RTC_Format_BIN);
}

void rtc_get_datetime(rtc_datetime_t* data, uint32_t format) {
  uint32_t unix_time;

  RTC_GetTime(format, &RTC_TimeStruct);

  /* Format hours */
  data->hours = RTC_TimeStruct.RTC_Hours;
  data->minutes = RTC_TimeStruct.RTC_Minutes;
  data->seconds = RTC_TimeStruct.RTC_Seconds;

  /* Get subseconds */
  data->subseconds = RTC->SSR;

  RTC_GetDate(format, &RTC_DateStruct);

  /* Format date */
  data->year = RTC_DateStruct.RTC_Year;
  data->month = RTC_DateStruct.RTC_Month;
  data->date = RTC_DateStruct.RTC_Date;
  data->day = RTC_DateStruct.RTC_WeekDay;

  /* Calculate unix_time offset */
  unix_time = rtc_get_unix_time(data);
  data->unix_time = unix_time;
}

uint8_t rtc_get_days_in_month(uint8_t month, uint8_t year) {
  /* Check input data */
  if (
      month == 0 ||
      month > 12
      ) {
    /* Error */
    return 0;
  }

  /* Return days in month */
  return RTC_Months[RTC_LEAP_YEAR(2000 + year) ? 1 : 0][month - 1];
}

uint16_t rtc_get_days_in_year(uint8_t year) {
  /* Return days in year */
  return RTC_DAYS_IN_YEAR(2000 + year);
}

void RTC_Config(rtc_clocksource_t source) {
  if (source == RTC_CLOCKSOURCE_INTERNAL) {
    /* Enable the LSI OSC */
    RCC_LSICmd(ENABLE);

    /* Wait till LSI is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);

    /* Select the RTC Clock Source */
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
  } else if (source == RTC_CLOCKSOURCE_EXTERNAL) {
    /* Enable the LSE OSC */
    RCC_LSEConfig(RCC_LSE_ON);

    /* Wait till LSE is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);

    /* Select the RTC Clock Source */
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
  }

  /* Enable the RTC Clock */
  RCC_RTCCLKCmd(ENABLE);

  /* Wait for register synchronization */
  RTC_WaitForSynchro();

  /* Write status */
  RTC_WriteBackupRegister(RTC_STATUS_REG, RTC_STATUS_INIT_OK);
}

void rtc_interrupts_config(rtc_interrupts_t int_value) {
  uint32_t int_val = 0;

  /* Clear pending bit */
  EXTI->PR = 0x00400000;

  /* Disable wakeup interrupt */
  RTC_WakeUpCmd(DISABLE);

  /* Disable RTC interrupt flag */
  RTC_ITConfig(RTC_IT_WUT, DISABLE);

  /* NVIC init for RTC */
  NVIC_InitStruct.NVIC_IRQChannel = RTC_WKUP_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = RTC_PRIORITY;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = RTC_WAKEUP_SUBPRIORITY;
  NVIC_InitStruct.NVIC_IRQChannelCmd = DISABLE;
  NVIC_Init(&NVIC_InitStruct);

  /* RTC connected to EXTI_Line22 */
  EXTI_InitStruct.EXTI_Line = EXTI_Line22;
  EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStruct.EXTI_LineCmd = DISABLE;
  EXTI_Init(&EXTI_InitStruct);

  if (int_value != RTC_INTERRUPTS_Disable) {
    /* Enable NVIC */
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
    /* Enable EXT1 interrupt */
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStruct);

    /* First disable wake up command */
    RTC_WakeUpCmd(DISABLE);

    if (int_value == RTC_INTERRUPTS_60s) {
      int_val = 0x3BFFF; 		/* 60 seconds = 60 * 4096 / 1 = 245760 */
    } else if (int_value == RTC_INTERRUPTS_30s) {
      int_val = 0x1DFFF;		/* 30 seconds */
    } else if (int_value == RTC_INTERRUPTS_15s) {
      int_val = 0xEFFF;		/* 15 seconds */
    } else if (int_value == RTC_INTERRUPTS_10s) {
      int_val = 0x9FFF;		/* 10 seconds */
    } else if (int_value == RTC_INTERRUPTS_5s) {
      int_val = 0x4FFF;		/* 5 seconds */
    } else if (int_value == RTC_INTERRUPTS_2s) {
      int_val = 0x1FFF;		/* 2 seconds */
    } else if (int_value == RTC_INTERRUPTS_1s) {
      int_val = 0x0FFF;		/* 1 second */
    } else if (int_value == RTC_INTERRUPTS_500ms) {
      int_val = 0x7FF;		/* 500 ms */
    } else if (int_value == RTC_INTERRUPTS_250ms) {
      int_val = 0x3FF;		/* 250 ms */
    } else if (int_value == RTC_INTERRUPTS_125ms) {
      int_val = 0x1FF;		/* 125 ms */
    }

    /* Clock divided by 8, 32768 / 8 = 4096 */
    /* 4096 ticks for 1second interrupt */
    RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div8);

    /* Set RTC wakeup counter */
    RTC_SetWakeUpCounter(int_val);
    /* Enable wakeup interrupt */
    RTC_ITConfig(RTC_IT_WUT, ENABLE);
    /* Enable wakeup command */
    RTC_WakeUpCmd(ENABLE);
  }
}

uint32_t rtc_get_unix_time(rtc_datetime_t* data) {
  uint32_t days = 0, seconds = 0;
  uint16_t i;
  uint16_t year = (uint16_t) (data->year + 2000);
  /* Year is below offset year */
  if (year < RTC_OFFSET_YEAR) {
    return 0;
  }
  /* Days in back years */
  for (i = RTC_OFFSET_YEAR; i < year; i++) {
    days += RTC_DAYS_IN_YEAR(i);
  }
  /* Days in current year */
  for (i = 1; i < data->month; i++) {
    days += RTC_Months[RTC_LEAP_YEAR(year)][i - 1];
  }
  /* Day starts with 1 */
  days += data->date - 1;
  seconds = days * RTC_SECONDS_PER_DAY;
  seconds += data->hours * RTC_SECONDS_PER_HOUR;
  seconds += data->minutes * RTC_SECONDS_PER_MINUTE;
  seconds += data->seconds;

  /* seconds = days * 86400; */
  return seconds;
}

void rtc_get_datetime_from_unix(rtc_datetime_t* data, uint32_t unix_time) {
  uint16_t year;

  /* Store unix_time time to unix_time in struct */
  data->unix_time = unix_time;
  /* Get seconds from unix_time */
  data->seconds = unix_time % 60;
  /* Go to minutes */
  unix_time /= 60;
  /* Get minutes */
  data->minutes = unix_time % 60;
  /* Go to hours */
  unix_time /= 60;
  /* Get hours */
  data->hours = unix_time % 24;
  /* Go to days */
  unix_time /= 24;

  /* Get week day */
  /* Monday is day one */
  data->day = (unix_time + 3) % 7 + 1;

  /* Get year */
  year = 1970;
  while (1) {
    if (RTC_LEAP_YEAR(year)) {
      if (unix_time >= 366) {
        unix_time -= 366;
  } else {
  break;
  }
  } else if (unix_time >= 365) {
  unix_time -= 365;
  } else {
  break;
  }
  year++;
  }
  /* Get year in xx format */
  data->year = (uint8_t) (year - 2000);
  /* Get month */
  for (data->month = 0; data->month < 12; data->month++) {
  if (RTC_LEAP_YEAR(year) && unix_time >= (uint32_t)RTC_Months[1][data->month]) {
  unix_time -= RTC_Months[1][data->month];
  } else if (unix_time >= (uint32_t)RTC_Months[0][data->month]) {
  unix_time -= RTC_Months[0][data->month];
  } else {
  break;
  }
}
/* Get month */
/* Month starts with 1 */
data->month++;
/* Get date */
/* Date starts with 1 */
data->date = unix_time + 1;
}

void rtc_set_alarm(rtc_alarm_t Alarm, rtc_alarm_type_t* DataTime, uint32_t format) {
  RTC_AlarmTypeDef RTC_AlarmStruct;

  /* Disable alarm first */
  rtc_disable_alarm(Alarm);

  /* Set RTC alarm settings */
  /* Set alarm time */
  RTC_AlarmStruct.RTC_AlarmTime.RTC_Hours = DataTime->hours;
  RTC_AlarmStruct.RTC_AlarmTime.RTC_Minutes = DataTime->minutes;
  RTC_AlarmStruct.RTC_AlarmTime.RTC_Seconds = DataTime->seconds;
  RTC_AlarmStruct.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;

  /* Alarm type is every week the same day in a week */
  if (DataTime->alarmtype == RTC_ALARMTYPE_DAYINWEEK) {
    /* Alarm trigger every week the same day in a week */
    RTC_AlarmStruct.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_WeekDay;

    /* Week day can be between 1 and 7 */
    if (DataTime->day == 0) {
      RTC_AlarmStruct.RTC_AlarmDateWeekDay = 1;
    } else if (DataTime->day > 7) {
      RTC_AlarmStruct.RTC_AlarmDateWeekDay = 7;
    } else {
      RTC_AlarmStruct.RTC_AlarmDateWeekDay = DataTime->day;
    }
  } else { /* Alarm type is every month the same day */
    /* Alarm trigger every month the same day in a month */
    RTC_AlarmStruct.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date;

    /* Month day can be between 1 and 31 */
    if (DataTime->day == 0) {
      RTC_AlarmStruct.RTC_AlarmDateWeekDay = 1;
    } else if (DataTime->day > 31) {
      RTC_AlarmStruct.RTC_AlarmDateWeekDay = 31;
    } else {
      RTC_AlarmStruct.RTC_AlarmDateWeekDay = DataTime->day;
    }
  }

  switch (Alarm) {
    case RTC_ALARM_A:
      /* Configure the RTC Alarm A */
      RTC_SetAlarm(format, RTC_Alarm_A, &RTC_AlarmStruct);

      /* Enable Alarm A */
      RTC_AlarmCmd(RTC_Alarm_A, ENABLE);

      /* Enable Alarm A interrupt */
      RTC_ITConfig(RTC_IT_ALRA, ENABLE);

      /* Clear Alarm A pending bit */
      RTC_ClearFlag(RTC_IT_ALRA);
      break;
    case RTC_ALARM_B:
      /* Configure the RTC Alarm B */
      RTC_SetAlarm(format, RTC_Alarm_B, &RTC_AlarmStruct);

      /* Enable Alarm B */
      RTC_AlarmCmd(RTC_Alarm_B, ENABLE);

      /* Enable Alarm B interrupt */
      RTC_ITConfig(RTC_IT_ALRB, ENABLE);

      /* Clear Alarm B pending bit */
      RTC_ClearFlag(RTC_IT_ALRB);
      break;
    default:
      break;
  }
}

void rtc_disable_alarm(rtc_alarm_t Alarm) {
  switch (Alarm) {
    case RTC_ALARM_A:
      /* Disable Alarm A */
      RTC_AlarmCmd(RTC_Alarm_A, DISABLE);

      /* Disable Alarm A interrupt */
      RTC_ITConfig(RTC_IT_ALRA, DISABLE);

      /* Clear Alarm A pending bit */
      RTC_ClearFlag(RTC_IT_ALRA);
      break;
    case RTC_ALARM_B:
      /* Disable Alarm B */
      RTC_AlarmCmd(RTC_Alarm_B, DISABLE);

      /* Disable Alarm B interrupt */
      RTC_ITConfig(RTC_IT_ALRB, DISABLE);

      /* Clear Alarm B pending bit */
      RTC_ClearFlag(RTC_IT_ALRB);
      break;
    default:
      break;
  }

  /* Clear RTC Alarm pending bit */
  EXTI->PR = 0x00020000;

  /* Configure EXTI 17 as interrupt */
  EXTI_InitStruct.EXTI_Line = EXTI_Line17;
  EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStruct.EXTI_LineCmd = ENABLE;

  /* Initialite Alarm EXTI interrupt */
  EXTI_Init(&EXTI_InitStruct);

  /* Configure the RTC Alarm Interrupt */
  NVIC_InitStruct.NVIC_IRQChannel = RTC_Alarm_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = RTC_PRIORITY;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = RTC_ALARM_SUBPRIORITY;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;

  /* Initialize RTC Alarm Interrupt */
  NVIC_Init(&NVIC_InitStruct);
}

void rtc_write_backup_register(uint8_t location, uint32_t value) {
  /* Check input, 0 to 18 registers are allowed */
  if (location > 18) {
    return;
  }

  /* Write data to backup register */
  *(uint32_t *)((&RTC->BKP0R) + 4 * location) = value;
}

uint32_t rtc_read_backup_register(uint8_t location){
  /* Check input, 0 to 18 registers are allowed */
  if (location > 18) {
    return 0;
  }

  /* Read data from backup register */
  return *(uint32_t *)((&RTC->BKP0R) + 4 * location);
}

/* Callbacks */
void rtc_request_handler(void) {
  /* If user needs this function, then they should be defined separatelly in your project */
}

void rtc_alarm_a_handler(void) {
  /* If user needs this function, then they should be defined separatelly in your project */
}

void rtc_alarm_b_handler(void) {
  /* If user needs this function, then they should be defined separatelly in your project */
}

/* Private RTC IRQ handlers */
void RTC_WKUP_IRQHandler(void) {
  /* Check for RTC interrupt */
  if (RTC_GetITStatus(RTC_IT_WUT) != RESET) {
    /* Clear interrupt flags */
    RTC_ClearITPendingBit(RTC_IT_WUT);

    /* Call user function */
    rtc_request_handler();
  }

  /* Clear EXTI line 22 bit */
  EXTI->PR = 0x00400000;
}

void RTC_Alarm_IRQHandler(void) {
  /* RTC Alarm A check */
  if (RTC_GetITStatus(RTC_IT_ALRA) != RESET) {
    /* Clear RTC Alarm A interrupt flag */
    RTC_ClearITPendingBit(RTC_IT_ALRA);

    /* Call user function for Alarm A */
    rtc_alarm_a_handler();
  }

  /* RTC Alarm B check */
  if (RTC_GetITStatus(RTC_IT_ALRB) != RESET) {
    /* Clear RTC Alarm A interrupt flag */
    RTC_ClearITPendingBit(RTC_IT_ALRB);

    /* Call user function for Alarm B */
    rtc_alarm_b_handler();
  }

  /* Clear EXTI line 17 bit */
  EXTI->PR = 0x00020000;
}
/**
 * Keil project example for RTC peripheral
 *
 * Before you start, select your target, on the right of the "Load" button
 *
 * @author    Tilen MAJERLE
 * @email     tilen@majerle.eu
 * @website   http://stm32f4-discovery.net
 * @ide       Keil uVision 5
 * @conf      PLL parameters are set in "Options for Target" -> "C/C++" -> "Defines"
 * @packs     STM32F4xx/STM32F7xx Keil packs are requred with HAL driver support
 * @stdperiph STM32F4xx/STM32F7xx HAL drivers required
 */
/* Include core modules */
#include "stm32fxxx_hal.h"
/* Include my libraries here */
#include "defines.h"
#include "tm_stm32_disco.h"
#include "tm_stm32_delay.h"
#include "tm_stm32_rtc.h"

/* RTC structure */
TM_RTC_t RTCD;

/* RTC Alarm structure */
TM_RTC_AlarmTime_t RTCA;

/* String variable for LCD */
char str[100];

/* Current seconds value */
uint8_t sec = 0;
	
int main(void) {
	/* Init system clock for maximum system speed */
	TM_RCC_InitSystem();
	
	/* Init HAL layer */
	HAL_Init();
	
	/* Init leds */
	TM_DISCO_LedInit();
	
	/* Init button */
	TM_DISCO_ButtonInit();
	
	/* Init RTC */
	if (TM_RTC_Init(TM_RTC_ClockSource_External)) {
		/* RTC was already initialized and time is running */
		/* No need to set clock now */
	} else {
		/* RTC was now initialized */
		/* If you need to set new time, now is the time to do it */
	}
	
	/* Wakeup IRQ every 1 second */
	TM_RTC_Interrupts(TM_RTC_Int_1s);
	
	while (1) {
		/* Read RTC */
		TM_RTC_GetDateTime(&RTCD, TM_RTC_Format_BIN);
		
		/* Check difference */
		if (RTCD.Seconds != sec) {
			sec = RTCD.Seconds;
			
			/* Format date */
			sprintf(str, "Date: %02d.%02d.%04d", RTCD.Day, RTCD.Month, RTCD.Year + 2000);
			
			/* Format time */
			sprintf(str, "Time: %02d.%02d.%02d", RTCD.Hours, RTCD.Minutes, RTCD.Seconds);
		}
		
		/* Check if button pressed */
		if (TM_DISCO_ButtonPressed()) {
			/* Set new date and time */
			RTCD.Day = 4;
			RTCD.Month = 5;
			RTCD.Year = 15;
			RTCD.WeekDay = 5;
			RTCD.Hours = 23;
			RTCD.Minutes = 59;
			RTCD.Seconds = 50;
			
			/* Save new date and time */
			TM_RTC_SetDateTime(&RTCD, TM_RTC_Format_BIN);
			
			/* Trigger alarm now after new time was set */
			RTCA.Type = TM_RTC_AlarmType_DayInWeek; /*!< Alarm type is day in a week */
			RTCA.Day = 5;                           /*!< Day 5 in week = Friday */
			RTCA.Hours = RTCD.Hours;                /*!< Select time for alarm */
			RTCA.Minutes = RTCD.Minutes;
			RTCA.Seconds = RTCD.Seconds + 5;
			
			/* Enable alarm A */
			TM_RTC_EnableAlarm(TM_RTC_Alarm_A, &RTCA, TM_RTC_Format_BIN);
		}
	}
}

/* Called when wakeup interrupt happens */
void TM_RTC_WakeupHandler(void) {
	/* Toggle leds */
	TM_DISCO_LedToggle(LED_ALL);
}

/* Handle Alarm A event */
void TM_RTC_AlarmAHandler(void) {
	/* Alarm A triggered */
	/* Do you stuff */
	
	/* Disable alarm */
	TM_RTC_DisableAlarm(TM_RTC_Alarm_A);
}

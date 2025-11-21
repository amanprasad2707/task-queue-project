#include "main.h"
#include "stm32f4xx_hal_rtc.h"

void show_time_date_ITM(void){
  RTC_DateTypeDef RTC_date;
  RTC_TimeTypeDef RTC_time;

  memset(&RTC_date, 0, sizeof(RTC_date));
  memset(&RTC_time, 0, sizeof(RTC_time));

  HAL_RTC_GetTime(&hrtc, &RTC_time, RTC_FORMAT_BIN);
  HAL_RTC_GetDate(&hrtc, &RTC_date, RTC_FORMAT_BIN);

  char *format;
  format = (RTC_time.TimeFormat == RTC_HOURFORMAT12_AM) ? "AM" : "PM";

  printf("%02d:%02d:%02d [%s]", RTC_time.Hours, RTC_time.Minutes, RTC_time.Seconds, format);
  printf("\t%02d-%02d-%02d", RTC_date.Date, RTC_date.Month, 2000 + RTC_date.Year);
}


void show_time_date(void){
  static char show_time[40];
  static char show_date[40];

  RTC_DateTypeDef RTC_date;
  RTC_TimeTypeDef RTC_time;

  static char *time = show_time;
  static char *date = show_date;

  memset(&RTC_date, 0, sizeof(RTC_date));
  memset(&RTC_time, 0, sizeof(RTC_time));

  HAL_RTC_GetTime(&hrtc, &RTC_time, RTC_FORMAT_BIN);
  HAL_RTC_GetDate(&hrtc, &RTC_date, RTC_FORMAT_BIN);

  char *format;
  format = (RTC_time.TimeFormat == RTC_HOURFORMAT12_AM) ? "AM" : "PM";

  /* Display time Format : hh:mm:ss [AM/PM] */
	sprintf((char*)show_time,"%s:\t%02d:%02d:%02d [%s]","\nCurrent Time&Date",RTC_time.Hours, RTC_time.Minutes, RTC_time.Seconds,format);
	xQueueSend(q_print,&time,portMAX_DELAY);

	/* Display date Format : date-month-year */
	sprintf((char*)show_date,"\t%02d-%02d-%2d\n",RTC_date.Date, RTC_date.Month, 2000 + RTC_date.Year);
	xQueueSend(q_print,&date,portMAX_DELAY);
}


void config_rtc_time(RTC_TimeTypeDef *time){
  // time->TimeFormat = RTC_HOURFORMAT12_AM;  // todo: take this from user
  time->DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  time->StoreOperation = RTC_STOREOPERATION_RESET;

  HAL_RTC_SetTime(&hrtc, time, RTC_FORMAT_BIN);
}

void config_rtc_date(RTC_DateTypeDef *date){
  HAL_RTC_SetDate(&hrtc, date, RTC_FORMAT_BIN);
}

uint8_t validate_rtc_info(RTC_TimeTypeDef *time, RTC_DateTypeDef *date){

  if(time){
    if((time->Hours > 12) || (time->Minutes > 59) || (time->Seconds > 59)){
      return 1;
    }
  }

  if(date){
    if((date->Date > 31) || (date->Month > 12) || (date->Year > 99) || (date->WeekDay > 7)){
      return 1;
    }
  }
  return 0;
}

#include "FreeRTOSConfig.h"
#include "main.h"
#include "portmacro.h"
#include "projdefs.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_def.h"
#include "stm32f4xx_hal_uart.h"

int extract_command(command_t *cmd);
void process_command(command_t *cmd);
uint8_t get_number(uint8_t *p, int len);

const char *msg_invalid = "\n[ERROR] Invalid command received.\n";

void menu_task(void *param) {
  UNUSED(param);
  BaseType_t xreturned;

  uint32_t cmd_addr; // stores the 32-bit notification value
  command_t *cmd;
  int option;

  const char *msg_menu ="\n"
                        "========================================\n"
                        "               MAIN MENU                \n"
                        "========================================\n"
                        "  [0]  LED Effects\n"
                        "  [1]  Date & Time Settings\n"
                        "  [2]  Exit\n"
                        "----------------------------------------\n"
                        "  Enter your choice: ";

  while (1) {
    xreturned = xQueueSend(q_print, &msg_menu, portMAX_DELAY);
    // NOTE: here we use &msg_menu because this function want pointer to the
    // item we want to copy.
    configASSERT(xreturned == pdTRUE);

    xreturned = xTaskNotifyWait(0, 0, &cmd_addr, portMAX_DELAY);
    configASSERT(xreturned == pdTRUE);

    cmd = (command_t *)cmd_addr;

    if (cmd->len == 1) {
      option =
          cmd->payload[0] - 48; // Take the first character the user typed and
                                // convert it from ASCII to a real integer (0–9)

      switch (option) {
        case 0:
          curr_screen_state = sLedEffect;
          xTaskNotify(handle_task_led, 0, eNoAction);
          break;
        case 1:
          curr_screen_state = sRtcMenu;
          xTaskNotify(handle_task_rtc, 0, eNoAction);
          break;
        case 2:
          break; // implement exit
        default:
          xQueueSend(q_print, &msg_invalid, portMAX_DELAY);
          continue;
      }

    } 
    else { // invalid entry
      xQueueSend(q_print, &msg_invalid, portMAX_DELAY);
      continue;
    }

    // wait to run again when some other task notifies
    xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);
  }
}

void print_task(void *param) {
  UNUSED(param);
  BaseType_t xreturned;
  char *msg_print;
  HAL_StatusTypeDef hal_status;

  while (1) {
    xreturned = xQueueReceive(q_print, &msg_print, portMAX_DELAY);
    configASSERT(xreturned == pdTRUE);

    xSemaphoreTake(uart_mutex, portMAX_DELAY);
    hal_status = HAL_UART_Transmit(&huart2, (uint8_t *)msg_print,strlen(msg_print), HAL_MAX_DELAY);
    if (hal_status != HAL_OK) {
      Error_Handler();
    }
    xSemaphoreGive(uart_mutex);
  }
}

void cmd_task(void *param) {
  UNUSED(param);
  BaseType_t xreturned;
  static command_t cmd;

  while (1) {
    // wait for notification
    xreturned = xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);

    if (xreturned == pdTRUE) {
      // process the user command and stored in input queue
      process_command(&cmd);
    }
  }
}

void led_task(void *param) {
  UNUSED(param);
  BaseType_t xreturned;

  uint32_t cmd_addr; // stores the 32-bit notification value
  command_t *cmd;
  // int option;

const char *msg_led =
"\n"
"========================================\n"
"               LED EFFECTS              \n"
"========================================\n"
" Available Effects:\n"
"   [none]  Stop All Effects\n"
"\n"
"   [e1]    All Blink\n"
"   [e2]    Ping-Pong Sweep\n"
"   [e3]    Shift Left\n"
"   [e4]    Shift Right\n"
"   [e5]    Edge-to-Center Wave\n"
"   [e6]    Binary Counter\n"
"   [e7]    Random Sparkle\n"
"   [e8]    Wave Expand\n"
"   [e9]    Alternate Flash\n"
"   [e10]   Growing Bar\n"
"\n"
"----------------------------------------\n"
" Select an effect: ";


  while (1) {
    // Wait for notification from the menu task to become active
    xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);

    // print LED menu
    xreturned = xQueueSend(q_print, &msg_led, portMAX_DELAY);
    configASSERT(xreturned == pdTRUE);

    // wait for command to receive
    xreturned = xTaskNotifyWait(0, 0, &cmd_addr, portMAX_DELAY);
    configASSERT(xreturned == pdTRUE);


    cmd = (command_t *)cmd_addr;

    if (cmd->len <= 4) {
      if (!strcmp((char *)cmd->payload, "none")) {
        led_effect_stop();
      }

      else if (!strcmp((char *)cmd->payload, "e1")) {
        led_effect(1);
      }
      
      else if (!strcmp((char *)cmd->payload, "e2")) {
        led_effect(2);
      }
      
      else if (!strcmp((char *)cmd->payload, "e3")) {
        led_effect(3);
      }
      
      else if (!strcmp((char *)cmd->payload, "e4")) {
        led_effect(4);
      }
      
      else if (!strcmp((char *)cmd->payload, "e5")) {
        led_effect(5);
      }

      else if (!strcmp((char *)cmd->payload, "e6")) {
        led_effect(6);
      }

      else if (!strcmp((char *)cmd->payload, "e7")) {
        led_effect(7);
      }

      else if (!strcmp((char *)cmd->payload, "e8")) {
        led_effect(8);
      }

      else if (!strcmp((char *)cmd->payload, "e9")) {
        led_effect(9);
      }

      else if (!strcmp((char *)cmd->payload, "e10")) {
        led_effect(10);
      }

      else {
        xQueueSend(q_print, &msg_invalid,
                   portMAX_DELAY); // print invalid message
      }
    } else {
      xQueueSend(q_print, &msg_invalid, portMAX_DELAY); // print invalid message
    }
    // update state variable
    curr_screen_state = sMainMenu;

    // notify menu task
    xTaskNotify(handle_task_menu, 0, eNoAction);

  }
}

void rtc_task(void *param) {
  UNUSED(param);
  const char *msg_rtc1 ="\n"
                        "========================================\n"
                        "               RTC                      \n"
                        "========================================\n";

  const char *msg_rtc2 ="  [0]  Configure Time\n"
                        "  [1]  Configure Date\n"
                        "  [2]  Enable Reporting\n"
                        "  [3]  Exit\n"
                        "----------------------------------------\n"
                        "  Enter your choice: ";

  const char *msg_rtc_hh = "Enter hour(1-12):";
  const char *msg_rtc_mm = "Enter minutes(0-59):";
  const char *msg_rtc_ss = "Enter seconds(0-59):";
  const char *msg_rtc_am_pm = "Enter AM/PM:";

  const char *msg_rtc_dd = "Enter date(1-31):";
  const char *msg_rtc_mo = "Enter month(1-12):";
  const char *msg_rtc_dow = "Enter day(1-7 sun:1):";
  const char *msg_rtc_yr = "Enter year(0-99):";

  const char *msg_conf = "Configuration successful\n";
  const char *msg_rtc_report = "Enable time & date reporting(y/n)?: ";

  uint32_t cmd_addr;
  command_t *cmd;

  static int rtc_state = 0;
  int menu_code;

  RTC_TimeTypeDef time;
  RTC_DateTypeDef date;

#define HH_CONFIG      0
#define MM_CONFIG      1
#define SS_CONFIG      2
#define AM_PM_CONFIG   3

#define DATE_CONFIG    0
#define MONTH_CONFIG   1
#define YEAR_CONFIG    2
#define DAY_CONFIG     3

  while (1) {
    // wait till someone notifies
    xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);

    // print the menu and show current date and time information
    xQueueSend(q_print, &msg_rtc1, portMAX_DELAY);
    show_time_date();
    xQueueSend(q_print, &msg_rtc2, portMAX_DELAY);

    while (curr_screen_state != sMainMenu) {

      // wait for command notification
      xTaskNotifyWait(0, 0, &cmd_addr, portMAX_DELAY);
      cmd = (command_t *)cmd_addr;

      switch (curr_screen_state) {
        case sRtcMenu: {
          // process RTC menu commands
          if (cmd->len == 1) {
            menu_code = cmd->payload[0] - 48;

            switch (menu_code) {
            case 0:
              curr_screen_state = sRtcTimeConfig;
              xQueueSend(q_print, &msg_rtc_hh, portMAX_DELAY);
              break;

            case 1:
              curr_screen_state = sRtcDateConfig;
              xQueueSend(q_print, &msg_rtc_dd, portMAX_DELAY);
              break;

            case 2:
              curr_screen_state = sRtcReport;
              xQueueSend(q_print, &msg_rtc_report, portMAX_DELAY);
              break;

            case 3:
              curr_screen_state = sMainMenu;
              break;

            default:
              curr_screen_state = sMainMenu;
              xQueueSend(q_print, &msg_invalid, portMAX_DELAY);
            }
          } else {
            curr_screen_state = sMainMenu;
            xQueueSend(q_print, &msg_invalid, portMAX_DELAY);
          }
          break;
        }

        case sRtcTimeConfig: {
          
          switch (rtc_state) {
            
          case HH_CONFIG:{
            uint8_t hour = get_number(cmd->payload, cmd->len);
            time.Hours = hour;
            rtc_state = MM_CONFIG;
            xQueueSend(q_print, &msg_rtc_mm, portMAX_DELAY);
            break;
          }
            
          case MM_CONFIG:{
            uint8_t minute = get_number(cmd->payload, cmd->len);
            time.Minutes = minute;
            rtc_state = SS_CONFIG;
            xQueueSend(q_print, &msg_rtc_ss, portMAX_DELAY);
            break;
          }

          case SS_CONFIG:{
            uint8_t second = get_number(cmd->payload, cmd->len);
            time.Seconds = second;
            rtc_state = AM_PM_CONFIG;
            xQueueSend(q_print, &msg_rtc_am_pm, portMAX_DELAY);
            break;
          }

          case AM_PM_CONFIG:{
            if(!strcmp((char *) cmd->payload, "AM")){
              time.TimeFormat = RTC_HOURFORMAT12_AM;
            }
            else if(!strcmp((char *) cmd->payload, "PM")){
              time.TimeFormat = RTC_HOURFORMAT12_PM;
            }

            else {
              xQueueSend(q_print, &msg_invalid, portMAX_DELAY);
              curr_screen_state = sMainMenu;
              rtc_state = 0;
              break;
            }

            if (!validate_rtc_info(&time, NULL)) {
              config_rtc_time(&time);
              xQueueSend(q_print, &msg_conf, portMAX_DELAY);
              show_time_date();
            } else {
              xQueueSend(q_print, &msg_invalid, portMAX_DELAY);
            }
            curr_screen_state = sMainMenu;
              rtc_state = 0;
              break;
          }
        }
        break;
        }

        case sRtcDateConfig: {
          /*get date, month, day , year info and configure RTC */
          /*take care of invalid entries */
          switch (rtc_state) {
            case DATE_CONFIG: {
              uint8_t d = get_number(cmd->payload, cmd->len);
              date.Date = d;
              rtc_state = MONTH_CONFIG;
              xQueueSend(q_print, &msg_rtc_mo, portMAX_DELAY);
              break;
            }

            case MONTH_CONFIG: {
              uint8_t m = get_number(cmd->payload, cmd->len);
              date.Month = m;
              rtc_state = DAY_CONFIG;
              xQueueSend(q_print, &msg_rtc_dow, portMAX_DELAY);
              break;
            }

            case DAY_CONFIG: {
              uint8_t day = get_number(cmd->payload, cmd->len);
              date.WeekDay = day;
              rtc_state = YEAR_CONFIG;
              xQueueSend(q_print, &msg_rtc_yr, portMAX_DELAY);
              break;
            }

            case YEAR_CONFIG: {
              uint8_t year = get_number(cmd->payload, cmd->len);
              date.Year = year;

              if (!validate_rtc_info(NULL, &date)) {
                config_rtc_date(&date);
                xQueueSend(q_print, &msg_conf, portMAX_DELAY);
                show_time_date();
              } else {
                xQueueSend(q_print, &msg_invalid, portMAX_DELAY);
              }

              curr_screen_state = sMainMenu;
              rtc_state = 0;
              break;
            }
          }
          break;
        }
        case sRtcReport: {
          // enable or disable RTC current time reporting over ITM printf
          if (cmd->len == 1) {
            if (cmd->payload[0] == 'y') {
              if (xTimerIsTimerActive(timer_rtc) == pdFALSE) {
                xTimerStart(timer_rtc, portMAX_DELAY);
              } else if (cmd->payload[0] == 'n') {
                xTimerStop(timer_rtc, portMAX_DELAY);
              } else {
                xQueueSend(q_print, &msg_invalid, portMAX_DELAY);
              }
            }
          } else {
            xQueueSend(q_print, &msg_invalid, portMAX_DELAY);
          }
          curr_screen_state = sMainMenu;
          break;
        }

      } // switch end

    

    }  // while end
    
    // notify menu task
    xTaskNotify(handle_task_menu, 0, eNoAction);
  } // while super loop end
}

int extract_command(command_t *cmd) {
  BaseType_t status;
  uint8_t item;

  status = uxQueueMessagesWaiting(q_data);
  if (!status)
    return -1;

  uint8_t i = 0;

  do {
    status = xQueueReceive(q_data, &item, 0);
    if (status == pdTRUE) {
      cmd->payload[i++] = item;
    }
  } while (item != '\n');

  cmd->payload[i - 1] = '\0';
  cmd->len = i - 1; // saves length of cmd excluding null char
  return 0;
}

void process_command(command_t *cmd) {
  extract_command(cmd);
  switch (curr_screen_state) {
  case sMainMenu:
    xTaskNotify(handle_task_menu, (uint32_t)cmd, eSetValueWithOverwrite);
    break;

  case sLedEffect:
    xTaskNotify(handle_task_led, (uint32_t)cmd, eSetValueWithOverwrite);
    break;

  case sRtcMenu:
  case sRtcTimeConfig:
  case sRtcDateConfig:
  case sRtcReport:
    xTaskNotify(handle_task_rtc, (uint32_t)cmd, eSetValueWithOverwrite);
    break;

  default:
    break;
  }
}

uint8_t get_number(uint8_t *p, int len) {
  int val;
  if (len > 1) {
    val = (((p[0] - 48) * 10) + (p[1] - 48));
  } else {
    val = p[0] - 48;
  }
  return val;
}

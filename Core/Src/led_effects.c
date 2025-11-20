#include "main.h"
#include "stm32f4xx_hal_gpio.h"

void led_effect_stop(void){
  for(int i = 0 ; i < 4 ; i++)
		xTimerStop(timer_led[i],portMAX_DELAY);
}

void led_effect(int n){
  led_effect_stop();
	xTimerStart(timer_led[n-1], portMAX_DELAY);
}

void turn_off_all_leds(void){
  HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LED_ORANGE_GPIO_Port, LED_ORANGE_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, GPIO_PIN_RESET);
}

void turn_on_all_leds(void){
  HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LED_ORANGE_GPIO_Port, LED_ORANGE_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, GPIO_PIN_SET);
}

void turn_on_odd_leds(void){
  HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LED_ORANGE_GPIO_Port, LED_ORANGE_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, GPIO_PIN_RESET);
}

void turn_on_even_leds(void){
  HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LED_ORANGE_GPIO_Port, LED_ORANGE_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, GPIO_PIN_SET);
}

void LED_control(int value){
  for(int i = 0; i < 4; i++){
    HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin << i, ((value >> i)&0x1));
  }
}

void led_effect1(void){
  static int flag = 1;
  (flag ^= 1) ? turn_on_all_leds() : turn_off_all_leds();
}

void led_effect2(void){
  static int flag = 1;
  (flag ^= 1) ? turn_off_all_leds() : turn_on_all_leds();
}

void led_effect3(void){
  static int i = 0;
  LED_control(0x1 << (i++ % 4));
}

void led_effect4(void){
  static int i = 0;
  LED_control(0x08 >> (i++ % 4));
}

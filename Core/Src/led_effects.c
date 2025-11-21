#include "main.h"
#include "stm32f4xx_hal_gpio.h"



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

void all_blink(void){
  static int flag = 1;
  (flag ^= 1) ? turn_on_all_leds() : turn_off_all_leds();
}


// bounce / ping-pong
void ping_pong_sweep(void){
  static int pos = 0;
  static int dir = 1;   // 1 = forward, -1 = backward

  LED_control(1 << pos);
  pos += dir;

  if(pos == 3 || pos == 0){ // reverse direction at the ends
    dir = -dir;
  } 
}


void shift_left(void){
  static int i = 0;
  LED_control(0x1 << (i++ % 4));
}

void shift_right(void){
  static int i = 0;
  LED_control(0x08 >> (i++ % 4));
}

void edge_to_center(void){
  static int step = 0;
    uint8_t pattern[] = {
        0x9,   // 1001
        0x6,   // 0110
        0x2,   // 0010
        0x0    // 0000 (pause)
    };

    LED_control(pattern[step]);

    step = (step + 1) % 4;
}


void binary_counter(void){
    static uint8_t count = 0;
    LED_control(count & 0x0F);
    count++;
}


void random_sparkle(void){
    int r = rand() % 16;   // 0 to 15
    LED_control(r);
}


void wave_expand(void){
    static int step = 0;
    uint8_t patterns[] = {0x6, 0xF, 0x6, 0x4, 0x0};

    LED_control(patterns[step]);

    step = (step + 1) % 5;
}


void alternate_flash(void){
    static int flag = 0;
    LED_control(flag ? 0xA : 0x5);   // 1010 <-> 0101
    flag ^= 1;
}


void growing_bar(void){
    static int i = 0;

    if(i <= 4)
        LED_control((1 << i) - 1);
    
    i = (i + 1) % 5;
}


void led_effect_stop(void){
  for(int i = 0 ; i < 10 ; i++){
    xTimerStop(timer_led[i],portMAX_DELAY);
  }
  // Add a small delay to allow the timer command queue to be processed.
  vTaskDelay(pdMS_TO_TICKS(50));
  turn_off_all_leds();
}

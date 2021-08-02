/*
 * Emma cupboard light
 *
 */
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/sleep.h"

#include "hardware/clocks.h"
#include "hardware/gpio.h"
#include "hardware/rtc.h"
#include "hardware/pwm.h"

int main (void) {
  stdio_init_all();
  //Constants
  const int BOARD_LED = 25, DOOR_GPIO = 10, LED_GPIO = 14;
  const float conversion_factor = 3.3f / (1 << 12);

  //Variable Defs
  int count;

  //Init PWM functionality for LED dimming
  gpio_set_function(BOARD_LED, GPIO_FUNC_PWM);
  uint slice_num1 = pwm_gpio_to_slice_num(BOARD_LED);
  pwm_set_wrap(slice_num1, 4095);
  pwm_set_chan_level(slice_num1, PWM_CHAN_B, 0);
  pwm_set_enabled(slice_num1, true);

  gpio_set_function(LED_GPIO, GPIO_FUNC_PWM);
  uint slice_num2 = pwm_gpio_to_slice_num(LED_GPIO);
  pwm_set_wrap(slice_num2, 4095);
  pwm_set_chan_level(slice_num2, PWM_CHAN_A, 0);
  pwm_set_enabled(slice_num2, true);

  //Init GPIO pins
  gpio_init(DOOR_GPIO);
  gpio_set_dir(DOOR_GPIO,GPIO_IN);

  //Setup low power sleep mode
  sleep_run_from_xosc();

  while(1) {
    //Wait till interrupt from power on
    sleep_goto_dormant_until_level_high(DOOR_GPIO);

    //Fade lights on in 200 milliseconds
    for(count = 0; count<10; count++) {
      pwm_set_chan_level(slice_num2, PWM_CHAN_A, 400*count);
      pwm_set_chan_level(slice_num1, PWM_CHAN_B, 400*count);
      sleep_ms(20);
    }
    //Wait till door closes
    while(gpio_get(DOOR_GPIO)!=0) {

    }

    //Dim lights over 1 seconds
    for(count = 20; count>0; count--) {
      pwm_set_chan_level(slice_num2, PWM_CHAN_A, 200*count);
      pwm_set_chan_level(slice_num1, PWM_CHAN_B, 200*count);
      sleep_ms(50);
    }

  }
}


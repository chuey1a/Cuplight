/*
 * Emma cupboard light
 *
 */
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/sleep.h"
#include "hardware/gpio.h"
#include "hardware/rtc.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"

int main (void) {
  stdio_init_all();
  //Constants
  const int BOARD_LED = 25, DOOR_GPIO = 10, LED_GPIO = 1, ADC_PIN = 26,
            ADC_CHAN = 0;
  const float conversion_factor = 3.3f / (1 << 12);

  //Variable Defs
  int count;

  //ADC init
  adc_init();
  adc_gpio_init(ADC_PIN);
  adc_select_input(ADC_CHAN);

  //Init PWM functionality for LED dimming
  gpio_set_function(BOARD_LED, GPIO_FUNC_PWM);
  uint slice_num = pwm_gpio_to_slice_num(BOARD_LED);

  pwm_set_wrap(slice_num, 4095);
  pwm_set_chan_level(slice_num, PWM_CHAN_B, 0);
  pwm_set_enabled(slice_num, true);

  //Init GPIO pins
  gpio_init(LED_GPIO);
  gpio_init(DOOR_GPIO);
  gpio_set_dir(LED_GPIO,GPIO_OUT);
  gpio_set_dir(DOOR_GPIO,GPIO_IN);

  //Setup low power sleep mode
  sleep_run_from_xosc();

  while(1) {
    //Wait till interrupt from power on
    sleep_goto_dormant_until_level_high(DOOR_GPIO);
    //sleep_ms(1000);
    gpio_put(LED_GPIO, 1);

    //Check ADC input for brightness


    //Fade lights on in 200 milliseconds
    for(count = 0; count<10; count++) {
      pwm_set_chan_level(slice_num, PWM_CHAN_B, 400*count);
      sleep_ms(20);
    }
    //Wait till door closes
    while(gpio_get(DOOR_GPIO)!=0){
      //Constantly check for brightness and adjust as required.
      uint16_t brightness = adc_read();
      pwm_set_chan_level(slice_num, PWM_CHAN_B, brightness);
      sleep_ms(50);
    }

    //Dim lights over 1 seconds
    for(count = 20; count>0; count--) {
      pwm_set_chan_level(slice_num, PWM_CHAN_B, 200*count);
      sleep_ms(50);
    }
    gpio_put(LED_GPIO, 0);
    gpio_put(BOARD_LED, 0);

  }
}


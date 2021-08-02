// Emma cupboard light
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/sleep.h"
#include "hardware/rtc.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"

int main (void) {
  stdio_init_all();
  //Constants
  const int BOARD_LED = 25, DOOR_GPIO = 10, LED_GPIO = 1, ADC_PIN = 26;
  const float conversion_factor = 3.3f / (1 << 12);

  //Variable Defs
  uint16_t result;

  //Init GPIO pins
  gpio_init(LED_GPIO);
  gpio_init(BOARD_LED);
  gpio_init(DOOR_GPIO);
  gpio_set_dir(LED_GPIO,GPIO_OUT);
  gpio_set_dir(BOARD_LED,GPIO_OUT);
  gpio_set_dir(DOOR_GPIO,GPIO_IN);

  //Add PWM functionality for LED dimming
  gpio_set_function(BOARD_LED, GPIO_FUNC_PWM);

  uint slice_num = pwm_gpio_to_slice_num(PICO_DEFAULT_LED_PIN);
  // Get some sensible defaults for the slice configuration. By default, the
  // counter is allowed to wrap over its maximum range (0 to 2**16-1)
  pwm_config config = pwm_get_default_config();
  // Set divider, reduces counter clock to sysclock/this value
  pwm_config_set_clkdiv(&config, 2.f);
  // Load the configuration into our PWM slice, and set it running.
  pwm_init(slice_num, &config, true);

  //ADC init
  adc_init();
  // Make sure GPIO is high-impedance, no pullups etc
  adc_gpio_init(ADC_PIN);
  // Select ADC input 0 (GPIO26)
  adc_select_input(0);

  //Setup low power sleep mode
  sleep_run_from_xosc();

  while(1) {
    //Wait till interrupt from power on
    sleep_goto_dormant_until_level_high(DOOR_GPIO);
    gpio_put(LED_GPIO, 1);
    result = (adc_read()*16)/12;
    pwm_set_gpio_level(BOARD_LED, 30000);
    sleep_ms(100);
    //Wait till door closes
    while(gpio_get(DOOR_GPIO)!=0){
      result = (adc_read()*16)/12;
      pwm_set_gpio_level(BOARD_LED, 30000);
      sleep_ms(100);
    }
    gpio_put(LED_GPIO, 0);
    gpio_put(BOARD_LED, 0);

  }
}


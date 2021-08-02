// Emma cupboard light
#include "pico/stdlib.h"
#include "pico/sleep.h"
#include "hardware/rtc.h"
#include "hardware/pwm.h"

int main (void) {
  stdio_init_all();

  //Set LED constants
  const int BOARD_LED = 25, DOOR_GPIO = 10, LED_GPIO = 1;

  //Init GPIO pins
  gpio_init(LED_GPIO);
  gpio_init(BOARD_LED);
  gpio_init(DOOR_GPIO);
  gpio_set_dir(LED_GPIO,GPIO_OUT);
  gpio_set_dir(BOARD_LED,GPIO_OUT);
  gpio_set_dir(DOOR_GPIO,GPIO_IN);

  gpio_set_function(PICO_DEFAULT_LED_PIN, GPIO_FUNC_PWM);

  //Setup low power sleep mode
  sleep_run_from_xosc();

  while(1) {
    // wait till interrupt from power on
    sleep_goto_dormant_until_level_high(DOOR_GPIO);
    gpio_put(LED_GPIO, 1);
    gpio_put(BOARD_LED, 1);
    while(gpio_get(DOOR_GPIO)!=0) {}
    gpio_put(LED_GPIO, 0);
    gpio_put(BOARD_LED, 0);

  }
}


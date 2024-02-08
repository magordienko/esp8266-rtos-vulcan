#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_log.h"

#include "driver/gpio.h"
#include "driver/hw_timer.h"

#define GPIO_LED 2
#define GPIO_LED_SEL 1ULL << GPIO_LED

/* Print chip information */
void chipInfo()
{
  esp_chip_info_t chip_info;

  esp_chip_info(&chip_info);

  printf("This is ESP8266 chip with %d CPU cores, WiFi, ",
      chip_info.cores);
  printf("silicon revision %d, ", chip_info.revision);
  printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
      (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");
}

/* GPIO configuration */
void gpioConfig()
{
  gpio_config_t io_conf;

  io_conf.intr_type = GPIO_INTR_DISABLE;
  io_conf.mode = GPIO_MODE_OUTPUT;
  io_conf.pin_bit_mask = GPIO_LED_SEL;
  io_conf.pull_down_en = 0;
  io_conf.pull_up_en = 0;
  gpio_config(&io_conf);
}

/* Setting the behavior of the LED */
void ledBehaviour()
{
  static int state = 0;
  gpio_set_level(GPIO_LED, (state ++) % 2);
}

/* Timer Interrupt Handler */
void hw_timer_callback(void *arg)
{
	ledBehaviour();
}

/* Timer configuration */
void timerConfig()
{
  hw_timer_init(hw_timer_callback, NULL);
  hw_timer_alarm_us(100000, true);
}

/* The entry point of the program */
void app_main()
{
  chipInfo();
  gpioConfig();
  timerConfig();
  while(1)
  {

  }
}

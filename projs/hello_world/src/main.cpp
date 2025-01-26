/**
 * @file main.cpp
 * @brief Implementation of the pep task.
 *
 * This file contains the main implementation of the pep task. The task handles readings from the input
 * of one GPIO and based on the input value performs LED on/off operation on the second GPIO configured as output.
 * The code involves interaction with hardware peripherals and GPIO pins, with special attention to the ESP32 GPIO reservation settings.
 * To build thsi project, use the following command:
 * 	west build -b esp32_devkitc_wroom/esp32/procpu [proj_path]
 * 
 * @note
 * 	 - The LED and BUTTON overlay configuration for ESP32 was not successful. 
 *   Although I am "familiar" with how the overlay should work, I chose not to spend additional time on this.
 *
 * Reserved GPIO pins for ESP32:
 * 
 * &gpio0 {
 *     gpio-reserved-ranges = <6 6>, // flash
 *                            <20 1>, <24 1>, <28 4>; // NC
 * };
 * 
 * &gpio1 {
 *     gpio-reserved-ranges = <5 2>; // GPIO37-38 NC
 * };
 */

#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <stdio.h>
#include <zephyr/zbus/zbus.h>
#include "ReadClass.h"
#include "ReactClass.h"


#define REACT_PIN_OUT 25
#define READ_PIN_IN 27

/* 	Device which hold the pin configuration, for this purpose esp32 on gpio0 has 32
	pins so only gpio0 is needed */
static const struct device *gpio_dev = DEVICE_DT_GET(DT_NODELABEL(gpio0));

/* Classes instances are declared here because of the notification callback, it uses ReactClass instance */
static ReadClass read_class(gpio_dev, READ_PIN_IN);
static ReactClass react_class(gpio_dev, REACT_PIN_OUT);


void gpioInputNotificationCallback(const struct zbus_channel *chan);

/**
 * @brief
 * zBus channel used for communication of input level value.
 */
ZBUS_CHAN_DEFINE(gpio_input_data_chan,      /* Name */
                 uint8_t,                    /* Message type */
                 NULL,                       /* Validator */
                 NULL,                       /* User data */
                 ZBUS_OBSERVERS(gpioInObs), /* Observers */
                 ZBUS_MSG_INIT(0));          /* Initial value */

/**
 * @brief
 * zBus gpio_input_data_chan notification callback
 */
ZBUS_LISTENER_DEFINE(gpioInObs, gpioInputNotificationCallback);

int main(void) {	
	/* Check if gpio can be used */
	if(!device_is_ready(gpio_dev)) {
		printf("Device not ready");
		return 0;
	}
	// Initialize ReadClass and ReactClass
	read_class.init();
	react_class.init();

	// Loop for the app to run
	while(1){}
	return 0;
}

/**
 * @brief Callback function for GPIO input interrupt notifications.
 */
void gpioInputNotificationCallback(const struct zbus_channel *chan) {
    const uint8_t *inputLevel = (uint8_t*)zbus_chan_const_msg(chan);
	if(inputLevel){
		react_class.handleNotification(inputLevel);
	}
}
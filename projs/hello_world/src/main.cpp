#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <stdio.h>
#include <zephyr/zbus/zbus.h>
#include "ReadClass.h"
#include "ReactClass.h"

/* Reserved GPIO pins for esp32
&gpio0 {
	gpio-reserved-ranges = <6 6>, // flash
			<20 1>, <24 1>, <28 4>; // NC
};

&gpio1 {
	gpio-reserved-ranges = <5 2>; // GPIO37-38 NC
}; */
#define REACT_PIN_OUT 25
#define READ_PIN_IN 27

/* 	Device which hold the pin configuration, for this purpose esp32 on gpio0 has 32
	pins so only gpio0 is needed */
static const struct device *gpio_dev = DEVICE_DT_GET(DT_NODELABEL(gpio0));

// Define the callback function used for gpio input interrupt notification to observers
void gpio_in_lis_callback(const struct zbus_channel *chan)
{
    const int *inputLevel = (int*)zbus_chan_const_msg(chan);

	// TODO, iterate through all the classes of instance ReactClass and call x.notify(inputLevel);
    if (*inputLevel == 1) {
        printf("gpio_in_lis_callback_HIGH\n");
    } else {
        printf("gpio_in_lis_callback_LOW\n");
    }
}

// Define the ZBUS channel with the specific initialization
ZBUS_CHAN_DEFINE(gpio_input_data_chan,      /* Name */
                 uint8_t,                    /* Message type */
                 NULL,                       /* Validator */
                 NULL,                       /* User data */
                 ZBUS_OBSERVERS(gpio_in_lis), /* Observers */
                 ZBUS_MSG_INIT(0));          /* Initial value */

// Define the listener with the callback
ZBUS_LISTENER_DEFINE(gpio_in_lis, gpio_in_lis_callback);

int main(void)
{	
	/* Check if gpio can be used */
	if(!device_is_ready(gpio_dev)) {
		printf("Device not ready");
		return 0;
	}


	// Initialize ReadClass and ReactClass
    ReadClass read_class(gpio_dev, READ_PIN_IN);
	read_class.init();
    ReactClass react_class(gpio_dev, REACT_PIN_OUT);
	react_class.init();
	
	// Loop for the app to run
	while(1){
		react_class.powerOn();
		printf("react_class.powerOn();\n");
		k_msleep(1000);
		react_class.powerOff();
		printf("react_class.powerOff();\n");
		k_msleep(1000);
	}

	return 0;
}
#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <stdio.h>
#include <zephyr/zbus/zbus.h>
#include "ReadClass.h"

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

static struct gpio_callback button_cb_data;

ZBUS_CHAN_DEFINE(gpio_input_data_chan,  		/* Name */
		 uint8_t, 								/* Message type */
		 NULL,                                 	/* Validator */
		 NULL,                                	/* User data */
		 ZBUS_OBSERVERS(gpio_in_lis),     			/* observers */
		 ZBUS_MSG_INIT(0) 						/* Initial value */
);

static void gpio_in_lis_callback(const struct zbus_channel *chan)
{
	const int *inputLevel = (int*)zbus_chan_const_msg(chan);

	if(*inputLevel == 1) {
		printf("HIGH\n");
		gpio_pin_set_raw(gpio_dev, REACT_PIN_OUT , 0);
	} else {
		printf("LOW\n");
		gpio_pin_set_raw(gpio_dev, REACT_PIN_OUT , 1);
	}

}

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

	ReadClass read_class2(gpio_dev, REACT_PIN_OUT);
	read_class2.init();
	// int ret = gpio_pin_configure(gpio_dev, REACT_PIN_OUT, GPIO_OUTPUT_INACTIVE);

	// if( ret != 0) {
	// 	printf("Pins are not configured");
	// 	return 0;
	// }

	// /* Board has internal pull-up/down resistors of 45kOhm so we can use that and avout the need for 
	//    external ones for the input check*/
	// ret = gpio_pin_configure(gpio_dev, READ_PIN_IN, GPIO_INPUT | GPIO_PULL_UP);

	// // Register the interrupt callback
    // ret = gpio_pin_interrupt_configure(gpio_dev, READ_PIN_IN, GPIO_INT_EDGE_BOTH);

	// if( ret != 0) {
	// 	printf("Pins are not configured");
	// 	return 0;
	// }

	// gpio_init_callback(&button_cb_data, button_pressed,
	//   BIT(READ_PIN_IN));
    //  gpio_add_callback(gpio_dev, &button_cb_data);

	while(1){

		// ret = gpio_pin_set_raw(gpio_dev, REACT_PIN_OUT , GPIO_OUTPUT_ACTIVE);
		// printf("LED ON\n");
		//  k_msleep(1000);
		// ret = gpio_pin_set_raw(gpio_dev, REACT_PIN_OUT , GPIO_OUTPUT_INACTIVE);
		// printf("LED OFF\n");
		// k_msleep(1000);

	}

	return 0;
}

// void button_pressed(const struct device *dev,
// 		    struct gpio_callback *cb,
// 		    uint32_t pins)
// {
//  uint8_t res = gpio_pin_get(dev,READ_PIN_IN);
// 	if(res) {
// 		printf("HIGH\n");
// 		zbus_chan_pub(&gpio_input_data_chan, &res, K_SECONDS(1));
// 	} else {
// 		printf("LOW\n");
// 		zbus_chan_pub(&gpio_input_data_chan, &res, K_SECONDS(1));
// 	}
// }

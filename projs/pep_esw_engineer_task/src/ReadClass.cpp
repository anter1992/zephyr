/**
 * @file ReadClass.cpp
 * @brief Implementation of interrupt handling for GPIO configured as input
 *          and notifying subsribers about the input value read
 *
 */
#include "ReadClass.h"

extern struct zbus_channel gpio_input_data_chan; // Channel

ReadClass::ReadClass(const struct device *gpio_dev, uint8_t pin): gpio_dev(gpio_dev), pin(pin) {};

void ReadClass::init() {
    /*  Board has internal pull-up/down resistors of 45kOhm so we can use that and avout the need for 
	external ones for the input check*/
	gpio_pin_configure(gpio_dev, pin, GPIO_INPUT | GPIO_PULL_UP);
	// Register the interrupt callback, since we are checking highs and lows, GPIO_INT_EDGE_BOTH is used
    gpio_pin_interrupt_configure(gpio_dev, pin, GPIO_INT_EDGE_BOTH);
	gpio_init_callback(&cb_data, button_pressed, BIT(pin));
    gpio_add_callback(gpio_dev, &cb_data);
    // Add worker which will notify subscribers on input change
    k_work_init(&notification_work, notificationWorkHandler);
};

void ReadClass::button_pressed(const struct device *dev,
		    struct gpio_callback *cb,
		    uint32_t pins) {
    // Obtain the instance which holds the cb value
    ReadClass *obj = CONTAINER_OF(cb, ReadClass, cb_data);

    // Check that the instance was actually found
    if(obj) {
        // Submit the work item for processing later
        k_work_submit(&obj->notification_work); 
    } else{
        printf("obj not found!\n");
    }
}


void ReadClass::notificationWorkHandler(struct k_work *work) {
    uint8_t inputVal = 0;
    // Obtain the instance which holds the work value
    ReadClass *obj = CONTAINER_OF(work, ReadClass, notification_work);

    // Check that the instance was actually found
    if(obj) {
        //Read the input
        inputVal = gpio_pin_get(obj->gpio_dev, obj->pin);
        // Notify observers of input value
        zbus_chan_pub(&gpio_input_data_chan, &inputVal, K_SECONDS(1));
    } else{
        printf("obj not found!\n");
    }    
}
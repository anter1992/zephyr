#include "ReadClass.h"

/**
* @brief ReadClass - Handles GPIO input reading and triggering events.
*/
ReadClass::ReadClass(const struct device *gpio_dev, uint8_t pin): gpio_dev(gpio_dev), pin(pin) {};

/**
* @brief Set up GPIO input with pull-up and interrupt.
*/
void ReadClass::init() {
    /*  Board has internal pull-up/down resistors of 45kOhm so we can use that and avout the need for 
	external ones for the input check*/
	gpio_pin_configure(gpio_dev, pin, GPIO_INPUT | GPIO_PULL_UP);
	// Register the interrupt callback
    gpio_pin_interrupt_configure(gpio_dev, pin, GPIO_INT_EDGE_BOTH);
	gpio_init_callback(&cb_data, button_pressed, BIT(pin));
    gpio_add_callback(gpio_dev, &cb_data);
};

void ReadClass::button_pressed(const struct device *dev,
		    struct gpio_callback *cb,
		    uint32_t pins) {

    ReadClass *obj = CONTAINER_OF(cb, ReadClass, cb_data);
    uint8_t res = 0; //gpio_pin_get(dev,pin);
    printf("HIGH for pin %d\n", obj->pin);
    if(res) {
        printf("HIGH for pin %d\n", obj->pin);
        //zbus_chan_pub(&gpio_input_data_chan, &res, K_SECONDS(1));
    } else {
        printf("LOW\n");
        //zbus_chan_pub(&gpio_input_data_chan, &res, K_SECONDS(1));
    }
}
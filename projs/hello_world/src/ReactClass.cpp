#include "ReactClass.h"

/**
* @brief ReadClass - Handles GPIO input reading and triggering events.
*/
ReactClass::ReactClass(const struct device *gpio_dev, uint8_t pin): gpio_dev(gpio_dev), pin(pin) {};

/**
* @brief Set up GPIO input with pull-up and interrupt.
*/
void ReactClass::init() {
    /*  Board has internal pull-up/down resistors of 45kOhm so we can use that and avout the need for 
	external ones for the input check*/
    gpio_pin_configure(gpio_dev, pin, GPIO_OUTPUT_INACTIVE);
    // Add worker which will notify subscribers on input change
    //k_work_init(&notification_work, notificationWorkHandler);
};

void ReactClass::handleNotification(const uint8_t *inputLevel) {
    if (*inputLevel == 1) {
		printf("react_class.powerOff();\n");
        gpio_pin_set(gpio_dev, pin , 0);
    } else {
        gpio_pin_set(gpio_dev, pin , 1);
		printf("react_class.powerOn();\n");		
    }
}
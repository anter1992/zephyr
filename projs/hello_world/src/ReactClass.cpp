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
    k_work_init(&notificationWorkHigh, notificationWorkHandlerHigh);
    k_work_init(&notificationWorkLow, notificationWorkHandlerLow);
};

void ReactClass::handleNotification(const uint8_t *inputLevel) {
    if (*inputLevel == 1) {
        // Submit the work item for processing later
        k_work_submit(&notificationWorkHigh);
    } else {
        // Submit the work item for processing later
        k_work_submit(&notificationWorkLow);	
    }
}

void ReactClass::notificationWorkHandlerHigh(struct k_work *work) {
    uint8_t inputVal = 0;
    // Obtain the instance which holds the work value
    ReactClass *obj = CONTAINER_OF(work, ReactClass, notificationWorkHigh);

    // Check that the instance was actually found
    if(obj) {
		printf("react_class.powerOff();\n");
        gpio_pin_set(obj->gpio_dev, obj->pin , 0);
    } else{
        // TODO, add error msg
    }    
}

void ReactClass::notificationWorkHandlerLow(struct k_work *work) {
    uint8_t inputVal = 0;
    // Obtain the instance which holds the work value
    ReactClass *obj = CONTAINER_OF(work, ReactClass, notificationWorkLow);

    // Check that the instance was actually found
    if(obj) {
        gpio_pin_set(obj->gpio_dev, obj->pin , 1);
		printf("react_class.powerOn();\n");	
    } else{
        // TODO, add error msg
    }    
}
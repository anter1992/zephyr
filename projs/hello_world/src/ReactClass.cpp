#include "ReactClass.h"

/**
* @brief ReadClass - Handles GPIO input reading and triggering events.
*/
ReactClass::ReactClass(const struct device *gpio_dev, uint8_t pin): gpio_dev(gpio_dev), pin(pin),
                    inputLevel_(0) {};

/**
* @brief Set up GPIO input with pull-up and interrupt.
*/
void ReactClass::init() {
    /*  Board has internal pull-up/down resistors of 45kOhm so we can use that and avout the need for 
	external ones for the input check*/
    gpio_pin_configure(gpio_dev, pin, GPIO_OUTPUT_INACTIVE);
    // Add worker which will notify subscribers on input change
    k_work_init_delayable(&my_work, notificationWorkHandler);

};

void ReactClass::handleNotification(const uint8_t *inputLevel) {

    if(inputLevel) {
        inputLevel_ = *inputLevel;
        // Submit the work item for processing later
        k_work_schedule(&my_work, K_MSEC(500));
    }
}

void ReactClass::notificationWorkHandler(struct k_work *work) {
    uint8_t inputVal = 0;
    // Obtain the instance which holds the work value
    ReactClass *obj = CONTAINER_OF(work, ReactClass, my_work);

    // Check that the instance was actually found
    if(obj) {
        if(obj->inputLevel_){
        // If the state is high, blink the LED 3 times with 100ms between each blink
		printf("react_class.powerOff() start ;\n");
        for (uint8_t i = 0; i < 3; i++) {
            gpio_pin_set(obj->gpio_dev, obj->pin, 1);
            k_msleep(100);
            gpio_pin_set(obj->gpio_dev, obj->pin, 0);
            k_msleep(100);
        }
        printf("react_class.powerOff() end ;\n");

        } else {
            printf("react_class.powerOn()start ;\n");	
            gpio_pin_set(obj->gpio_dev, obj->pin , 1);
            k_msleep(500);
            gpio_pin_set(obj->gpio_dev, obj->pin, 0);
            printf("react_class.powerOn()end ;\n");	
        }

    } else{
        // TODO, add error msg
    }    
}
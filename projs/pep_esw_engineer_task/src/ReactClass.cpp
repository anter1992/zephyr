/**
 * @file ReactClass.cpp
 * @brief Implementation LED toggling based on input level.
 *
 */
#include "ReactClass.h"


ReactClass::ReactClass(const struct device *gpio_dev, uint8_t pin): gpio_dev(gpio_dev), pin(pin),
                    inputLevel_(0) {};


void ReactClass::init() {
    gpio_pin_configure(gpio_dev, pin, GPIO_OUTPUT_INACTIVE);
    // Add worker which will handle toggling of LED
    k_work_init_delayable(&my_work, notificationWorkHandler);

};

void ReactClass::handleNotification(const uint8_t *inputLevel) {

    if(inputLevel) {
        inputLevel_ = *inputLevel;
        /*Submit the work item for processing later. This sort of handles the debouncing as well
          since in case before the timeout the notification is received, it will cancel the one waiting 
          and start the new one */
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
		printf("Input high start ;\n");
        for (uint8_t i = 0; i < 3; i++) {
            gpio_pin_set(obj->gpio_dev, obj->pin, 1);
            k_msleep(100);
            gpio_pin_set(obj->gpio_dev, obj->pin, 0);
            k_msleep(100);
        }
        printf("Input high end ;\n");

        } else {
            printf("Input low start ;\n");	
            gpio_pin_set(obj->gpio_dev, obj->pin , 1);
            k_msleep(500);
            gpio_pin_set(obj->gpio_dev, obj->pin, 0);
            printf("Input low end ;\n");	
        }

    } else{
        printf("obj not found!\n");
    }    
}
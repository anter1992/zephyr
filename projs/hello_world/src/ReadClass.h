#ifndef READCLASS_H
#define READCLASS_H
#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <stdio.h>
#include <zephyr/zbus/zbus.h>

class ReadClass {
private:
    // Device object containing the GPIO configuration
    const struct device *gpio_dev;
    
    // GPIO interrupt callback data
    struct gpio_callback cb_data;
    
    // Pin number used for GPIO input
    uint8_t pin;
    
    // Worker for handling pin change notifications
    k_work notification_work;

    /**
     * @brief Callback function that is invoked when the interrupt occurs on the GPIO pin.
     */
    static void button_pressed(const struct device *dev,
                               struct gpio_callback *cb,
                               uint32_t pins);

    /**
     * @brief Work handler function that processes the GPIO input change.
     */
    static void notificationWorkHandler(struct k_work *work);

public:
    /**
     * @brief Constructor for the ReadClass object.
     */
    ReadClass(const struct device *gpio_dev, uint8_t pin);

    /**
     * @brief Initializes the GPIO input with pull-up configuration and interrupt.
     */
    void init();
};

#endif

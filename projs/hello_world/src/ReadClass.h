#ifndef MYCLASS_H
#define MYCLASS_H
#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <stdio.h>
#include <zephyr/zbus/zbus.h>

class ReadClass {
private:
    // Device containing the gpio configuration
    const struct device *gpio_dev;
    // Pin of the input
    uint8_t pin;
    // Callback data from the gpio interrupt
    struct gpio_callback cb_data;

    void button_PressedCb(const struct device *dev,
		    struct gpio_callback *cb, uint32_t pins);

     // Static callback helper to redirect the call
     void static_button_PressedCb(const struct device *dev,
                                        struct gpio_callback *cb, uint32_t pins);

public:
    /**
     * @brief ReadClass - Handles GPIO input reading and triggering events.
     */
    ReadClass(const struct device *gpio_dev, uint8_t pin);

    /**
     * @brief Set up GPIO input with pull-up and interrupt.
     */
    void init();



};

#endif
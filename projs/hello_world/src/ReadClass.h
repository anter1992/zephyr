#ifndef READCLASS_H
#define READCLASS_H
#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <stdio.h>
#include <zephyr/zbus/zbus.h>

class ReadClass {
private:
    // Device containing the gpio configuration
    const struct device *gpio_dev;
    // Callback data from the gpio interrupt
    struct gpio_callback cb_data;
    // Pin of the input
    uint8_t pin;
    // Worker for the notification of pin change
    k_work notification_work;

public:
    /**
     * @brief ReadClass - Handles GPIO input reading and triggering events.
     */
    ReadClass(const struct device *gpio_dev, uint8_t pin);

    /**
     * @brief Set up GPIO input with pull-up and interrupt.
     */
    void init();

    /**
     * @brief Callback function when the interrupt occurs.
     */
    static void button_pressed(const struct device *dev,
		    struct gpio_callback *cb,
		    uint32_t pins);
    
    /**
    * @brief Callback function when the interrupt occurs.
    */
    static void notificationWorkHandler(struct k_work *work); // Work handler function

};

#endif
#ifndef REACTCLASS_H
#define REACTCLASS_H
#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <stdio.h>
#include <zephyr/zbus/zbus.h>

class ReactClass {
private:
    // Device containing the gpio configuration
    const struct device *gpio_dev;
    // Pin of the input
    uint8_t pin;
    // input level
    uint8_t inputLevel_;
    // Worker for the notification of pin change
    k_work_delayable my_work;

    static void notificationWorkHandler(struct k_work *work); // Work handler function

public:
    /**
     * @brief ReadClass - Handles GPIO input reading and triggering events.
     */
    ReactClass(const struct device *gpio_dev, uint8_t pin);

    /**
     * @brief Set up GPIO input with pull-up and interrupt.
     */
    void init();
    void handleNotification(const uint8_t *inputLevel);
};

#endif
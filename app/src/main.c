/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/drivers/led.h>
#include <zephyr/drivers/gpio.h>

LOG_MODULE_REGISTER(main);

enum {
	LED_BLUE,
	LED_GREEN,
	LED_RED,
	LED_MAX
};

static const struct pwm_dt_spec pwm_buzzer = PWM_DT_SPEC_GET(DT_ALIAS(buzzer_pwm));
static const struct device *rgb_led = DEVICE_DT_GET(DT_NODELABEL(lp5521));
static const struct gpio_dt_spec sw0_btn = GPIO_DT_SPEC_GET(DT_ALIAS(sw0), gpios);
struct gpio_callback sw0_btn_cb;

struct btn_rgb_work_st {
	struct k_work btn_rgb_work;
	bool shld_rgb_on;
};

void btn_rgb_work_cb(struct k_work *work)
{
	int err = 0;
	struct btn_rgb_work_st *ptr = CONTAINER_OF(work, struct btn_rgb_work_st, btn_rgb_work);

	err = ptr->shld_rgb_on ? led_on(rgb_led, LED_BLUE) : led_off(rgb_led, LED_BLUE);
	if (err) {
		LOG_ERR("err in led_off, err=%d", err);
		return;
	}
}

static struct btn_rgb_work_st btn_rgb_work_s = {.btn_rgb_work = Z_WORK_INITIALIZER(btn_rgb_work_cb),
						.shld_rgb_on = false};

static void sw0_btn_cb_handle(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
	if (gpio_pin_get_dt(&sw0_btn)) {
		pwm_set_dt(&pwm_buzzer, PWM_HZ(4000), PWM_HZ(4000) / 2);
		btn_rgb_work_s.shld_rgb_on = true;
		k_work_submit(&btn_rgb_work_s.btn_rgb_work);

	} else if (!gpio_pin_get_dt(&sw0_btn)) {
		pwm_set_dt(&pwm_buzzer, PWM_HZ(4000), 0);
		btn_rgb_work_s.shld_rgb_on = false;
		k_work_submit(&btn_rgb_work_s.btn_rgb_work);
	}
}

void play_starting_seq(void)
{
	uint8_t i = 0;

	for (i = 0; i < 4; i++) {
		if (0 != pwm_set_dt(&pwm_buzzer, PWM_HZ(4000), PWM_HZ(4000) / 2)) {
			LOG_ERR("err in pwm_set_dt");
			return;
		}
		k_msleep(250);
		if (0 != pwm_set_dt(&pwm_buzzer, PWM_HZ(4000), 0)) {
			LOG_ERR("err in pwm_set_dt");
			return;
		}
		k_msleep(250);
	}
}

int main(void)
{
	if (!pwm_is_ready_dt(&pwm_buzzer)) {
		LOG_ERR("Error: PWM device %s is not ready\n", pwm_buzzer.dev->name);
		return 0;
	}
	if (!device_is_ready(rgb_led)) {
		LOG_ERR("LED device %s is not ready", rgb_led->name);
		return 0;
	}

	if (gpio_pin_configure_dt(&sw0_btn, GPIO_INPUT)) {
		return 0;
	}

	gpio_init_callback(&sw0_btn_cb, sw0_btn_cb_handle, BIT(sw0_btn.pin));

	if (gpio_add_callback(sw0_btn.port, &sw0_btn_cb) ||
	    gpio_pin_interrupt_configure_dt(&sw0_btn, GPIO_INT_EDGE_BOTH)) {
		return 0;
	}

	/* Turn off all RGB LEDs */
	for (int i = 0; i < LED_MAX; i++) {
		led_off(rgb_led, i);
	}

	play_starting_seq();

	printf("Hello World! %s\n", CONFIG_BOARD);
	return 0;
}

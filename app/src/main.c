/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/drivers/gpio.h>

LOG_MODULE_REGISTER(main);

static const struct pwm_dt_spec pwm_buzzer = PWM_DT_SPEC_GET(DT_ALIAS(buzzer_pwm));
static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(DT_ALIAS(led1), gpios);

void play_starting_seq(void)
{
	uint8_t i = 0;

	for (i = 0; i < 4; i++) {
		if (0 != pwm_set_dt(&pwm_buzzer, PWM_HZ(4000), PWM_HZ(4000) / 2)) {
			LOG_ERR("err in pwm_set_dt");
			return;
		}
		gpio_pin_set_dt(&led0, true);
		gpio_pin_set_dt(&led1, false);

		k_msleep(250);
		if (0 != pwm_set_dt(&pwm_buzzer, PWM_HZ(4000), 0)) {
			LOG_ERR("err in pwm_set_dt");
			return;
		}
		gpio_pin_set_dt(&led0, false);
		gpio_pin_set_dt(&led1, true);
		k_msleep(250);
	}
	gpio_pin_set_dt(&led1, false);
}

int main(void)
{
	if (!pwm_is_ready_dt(&pwm_buzzer)) {
		LOG_ERR("Error: PWM device %s is not ready\n", pwm_buzzer.dev->name);
		return 0;
	}
	if (!gpio_is_ready_dt(&led0) || !gpio_is_ready_dt(&led1)) {
		return 0;
	}
	if ((gpio_pin_configure_dt(&led0, GPIO_OUTPUT_ACTIVE) < 0) ||
	    gpio_pin_configure_dt(&led1, GPIO_OUTPUT_ACTIVE)) {
		return 0;
	}

	play_starting_seq();

	printf("Hello World! %s\n", CONFIG_BOARD);
	return 0;
}

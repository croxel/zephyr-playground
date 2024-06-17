/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/pwm.h>

LOG_MODULE_REGISTER(main);

static const struct pwm_dt_spec pwm_buzzer = PWM_DT_SPEC_GET(DT_ALIAS(buzzer_pwm));

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

	play_starting_seq();

	printf("Hello World! %s\n", CONFIG_BOARD);
	return 0;
}

#ifndef _USER_ALERTS_H_
#define _USER_ALERTS_H_

#include <stdint.h>
#include <stdbool.h>
#include <zephyr/kernel.h>

struct alert_step {
	uint16_t ms;
	/* extends alert info here */
};

struct buzzer_alert_step {
	uint16_t ms;
	uint16_t freq;
};

struct mono_led_alert_step {
	uint16_t ms;
	uint8_t duty;
};

struct bi_led_alert_step {
	uint16_t ms;
	uint8_t duty_1;
	uint8_t duty_2;
};

struct rgb_led_alert_step {
	uint16_t ms;
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t _rsv;
};

struct user_alerts_pattern {
	const void *steps;
	uint16_t steps_count;
	uint16_t loop_count;
};

struct user_alerts_channel;
typedef int (*user_alerts_step_exec)(const struct user_alerts_channel *ch, const void *step);

struct user_alerts_channel {
	uint8_t step_size;
	const void *io;
	user_alerts_step_exec exec;
	uint16_t cur_step_idx;
	uint16_t cur_loop_idx;
	struct k_timer timer;
	const struct user_alerts_pattern *pattern;
};

int user_alerts_channel_init_timer(struct user_alerts_channel *ch);

int user_alerts_channel_play(struct user_alerts_channel *ch,
			     const struct user_alerts_pattern *pattern,
			     bool force);

int user_alerts_channel_stop(struct user_alerts_channel *ch, bool force);

#endif /* _USER_ALERTS_H_ */

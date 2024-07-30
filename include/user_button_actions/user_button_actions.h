#pragma once

#include <stdint.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <utils/sll_listeners.h>

enum user_button_actions {
	eUSER_BUTTON_PRESS = 0x01,
	eUSER_BUTTON_CLICK = 0x11,
	eUSER_BUTTON_CLICK_AND_HOLD = 0x12,
	eUSER_BUTTON_DOUBLE_CLICK = 0x22,
	eUSER_BUTTON_DOUBLE_CLICK_AND_HOLD = 0x23,
	eUSER_BUTTON_TRIPLE_CLICK = 0x33,
	eUSER_BUTTON_TRIPLE_CLICK_AND_HOLD = 0x34,
};

union user_button_action_code {
	struct {
		uint8_t down_cnt : 4;
		uint8_t up_cnt : 4;
	};
	uint8_t code;
};

struct user_button_actions_channel {
	uint8_t id;
	union user_button_action_code _rv;
	union user_button_action_code result;
	struct k_timer timer;
	struct k_work work;
	struct gpio_dt_spec btn;
	struct gpio_callback cb;
};

typedef void (*user_button_actions_callback)(struct user_button_actions_channel *ch);

struct user_button_actions_listener {
	user_button_actions_callback handler;
};

SLL_LISTENER_DECLARE(user_button_actions, struct user_button_actions_listener);

int user_button_actions_start_monitor(struct user_button_actions_channel *ch);

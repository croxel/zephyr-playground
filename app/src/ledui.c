#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <user_alerts/user_alerts.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/led.h>
#include <zephyr/dt-bindings/led/led.h>
#include <user_button_actions/user_button_actions.h>

LOG_MODULE_REGISTER(ledui);

struct bi_led_io {
	struct gpio_dt_spec led1;
	struct gpio_dt_spec led2;
};

int bi_led_user_alerts_step_exec(const struct user_alerts_channel *ch, const void *step)
{
	int err;
	const struct bi_led_alert_step *b_step = step;
	const struct bi_led_io *io = ch->io;

	if (b_step) {
		err = gpio_pin_set_dt(&io->led1, b_step->duty_1);
		err = gpio_pin_set_dt(&io->led2, b_step->duty_2);
	} else {
		err = gpio_pin_set_dt(&io->led1, 0);
		err = gpio_pin_set_dt(&io->led2, 0);
	}
	return err;
}

const struct bi_led_alert_step _bootup_bi_led_steps[] = {
	{.ms = 250, .duty_1 = 0xFF, .duty_2 = 0x00},
	{.ms = 250, .duty_1 = 0xFF, .duty_2 = 0xFF},
	{.ms = 250, .duty_1 = 0x00, .duty_2 = 0xFF},
	{.ms = 250, .duty_1 = 0x00, .duty_2 = 0x00},
	{.ms = 250, .duty_1 = 0x00, .duty_2 = 0xFF},
	{.ms = 250, .duty_1 = 0xFF, .duty_2 = 0xFF},
	{.ms = 250, .duty_1 = 0xFF, .duty_2 = 0x00},
};
struct user_alerts_pattern _bootup_bi_led_pattern = {
	.steps = _bootup_bi_led_steps,
	.steps_count = ARRAY_SIZE(_bootup_bi_led_steps),
	.loop_count = 4,
};

static const struct bi_led_io _bi_led_io = {
	.led1 = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios),
	.led2 = GPIO_DT_SPEC_GET(DT_ALIAS(led1), gpios),
};

static struct user_alerts_channel _bi_led_ch = {
	.step_size = sizeof(struct bi_led_alert_step),
	.io = &_bi_led_io,
	.exec = bi_led_user_alerts_step_exec,
	.cur_step_idx = 0,
	.cur_loop_idx = 0,
	/*".timer" wil be init using api */
	.pattern = NULL,
};

int rgb_led_user_alerts_step_exec(const struct user_alerts_channel *ch, const void *step)
{
	int err;
	const struct rgb_led_alert_step *b_step = step;
	const struct device *const dev = ch->io;

	if (b_step) {
		err = led_set_brightness(dev, LED_COLOR_ID_RED, b_step->r);
		err = led_set_brightness(dev, LED_COLOR_ID_GREEN, b_step->g);
		err = led_set_brightness(dev, LED_COLOR_ID_BLUE, b_step->b);
	} else {
		err = led_off(dev, LED_COLOR_ID_RED);
		err = led_off(dev, LED_COLOR_ID_GREEN);
		err = led_off(dev, LED_COLOR_ID_BLUE);
	}
	return err;
}

const struct rgb_led_alert_step _btn_click_rgb_led_steps[] = {
	{.ms = 250, .r = 0x64, .g = 0x64, .b = 0x64},
	{.ms = 250, .r = 0x64, .g = 0x64, .b = 0x00},
	{.ms = 250, .r = 0x64, .g = 0x00, .b = 0x64},
	{.ms = 250, .r = 0x64, .g = 0x00, .b = 0x00},
	{.ms = 250, .r = 0x00, .g = 0x64, .b = 0x64},
	{.ms = 250, .r = 0x00, .g = 0x64, .b = 0x00},
	{.ms = 250, .r = 0x00, .g = 0x00, .b = 0x64},
	{.ms = 250, .r = 0x00, .g = 0x00, .b = 0x00},
};
struct user_alerts_pattern _btn_click_rgb_led_pattern = {
	.steps = _btn_click_rgb_led_steps,
	.steps_count = ARRAY_SIZE(_btn_click_rgb_led_steps),
	.loop_count = 2,
};

const struct device *const _rgb_led_io = DEVICE_DT_GET_ANY(ti_lp5521);
static struct user_alerts_channel _rgb_led_ch = {
	.step_size = sizeof(struct rgb_led_alert_step),
	.io = _rgb_led_io,
	.exec = rgb_led_user_alerts_step_exec,
	.cur_step_idx = 0,
	.cur_loop_idx = 0,
	/*".timer" wil be init using api */
	.pattern = NULL,

};

const struct bi_led_alert_step _btn_press_bi_led_steps[] = {
	{.ms = 50, .duty_1 = 0xFF, .duty_2 = 0x00},
	{.ms = 450, .duty_1 = 0x00, .duty_2 = 0x00},
	{.ms = 50, .duty_1 = 0x00, .duty_2 = 0xFF},
	{.ms = 450, .duty_1 = 0x00, .duty_2 = 0x00},
};
struct user_alerts_pattern _btn_press_bi_led_pattern = {
	.steps = _btn_press_bi_led_steps,
	.steps_count = ARRAY_SIZE(_btn_press_bi_led_steps),
	.loop_count = 0,
};

void led_btn_action_handler(struct user_button_actions_channel *ch)
{
	switch (ch->result.code) {
		case eUSER_BUTTON_PRESS: {
		if (_bi_led_ch.pattern == &_btn_press_bi_led_pattern) {
			user_alerts_channel_stop(&_bi_led_ch, true);
		} else {
			user_alerts_channel_play(&_bi_led_ch, &_btn_press_bi_led_pattern, true);
		}
		} break;
		case eUSER_BUTTON_CLICK: {
			user_alerts_channel_play(&_rgb_led_ch, &_btn_click_rgb_led_pattern, true);
		} break;
	}
}

SLL_LISTENER_DEFINE_NODE(user_button_actions,
			led_btn_action_listener,
			.handler = led_btn_action_handler);

int play_bootup_led_blinks(void)
{
	int err;

	SLL_LISTENER_ADD_NODE(user_button_actions, led_btn_action_listener);

	err = gpio_is_ready_dt(&_bi_led_io.led1);
	if (!err) {
		LOG_ERR("Led1 is not ready");
		return 0;
	}

	err = gpio_is_ready_dt(&_bi_led_io.led2);
	if (!err) {
		LOG_ERR("Led3 is not ready");
		return 0;
	}

	err = gpio_pin_configure_dt(&_bi_led_io.led1, GPIO_OUTPUT_ACTIVE);
	if (err) {
		LOG_ERR("Led1 configuration error %d", err);
		return 0;
	}

	err = gpio_pin_configure_dt(&_bi_led_io.led2, GPIO_OUTPUT_ACTIVE);
	if (err) {
		LOG_ERR("Led2 configuration error %d", err);
		return 0;
	}

	user_alerts_channel_init_timer(&_bi_led_ch);
	user_alerts_channel_play(&_bi_led_ch, &_bootup_bi_led_pattern, true);

	user_alerts_channel_init_timer(&_rgb_led_ch);

	return 0;
}

SYS_INIT(play_bootup_led_blinks, APPLICATION, 99);

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <user_alerts/user_alerts.h>
#include <zephyr/drivers/gpio.h>
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

const struct bi_led_alert_step _btn_click_bi_led_steps[] = {
	{.ms = 50, .duty_1 = 0xFF, .duty_2 = 0x00},
	{.ms = 450, .duty_1 = 0x00, .duty_2 = 0x00},
	{.ms = 50, .duty_1 = 0x00, .duty_2 = 0xFF},
	{.ms = 450, .duty_1 = 0x00, .duty_2 = 0x00},
};
struct user_alerts_pattern _btn_click_bi_led_pattern = {
	.steps = _btn_click_bi_led_steps,
	.steps_count = ARRAY_SIZE(_btn_click_bi_led_steps),
	.loop_count = 0,
};

void led_btn_action_handler(struct user_button_actions_channel *ch)
{
	if (ch->result.code == eUSER_BUTTON_CLICK) {
		if (_bi_led_ch.pattern == &_btn_click_bi_led_pattern) {
			user_alerts_channel_stop(&_bi_led_ch, true);
		} else {
			user_alerts_channel_play(&_bi_led_ch, &_btn_click_bi_led_pattern, true);
		}
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
	return 0;
}

SYS_INIT(play_bootup_led_blinks, APPLICATION, 99);

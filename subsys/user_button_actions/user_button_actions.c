#include <user_button_actions/user_button_actions.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/gpio.h>

LOG_MODULE_REGISTER(user_button_actions);

#define CB_NULL_CHECK(cb) (((cb)->handler) != NULL)

SLL_LISTENER_DEFINE(user_button_actions, CB_NULL_CHECK);

void user_button_actions_fire_work(struct k_work *work)
{
	struct user_button_actions_channel *ch = CONTAINER_OF(work,
							struct user_button_actions_channel,
							work);
	if ((ch->_rv.down_cnt - ch->_rv.up_cnt) <= 1) {
		LOG_INF("BTN Action %x", ch->result.code);
		SLL_LISTENER_FIRE_NODES(user_button_actions, handler, ch);
	}
}

void user_button_actions_timeout(struct k_timer *timer)
{
	struct user_button_actions_channel *ch = CONTAINER_OF(timer,
							struct user_button_actions_channel,
							timer);
	ch->result.code = ch->_rv.code;
	ch->_rv.code = 0;
	k_work_submit(&ch->work);
}

static void user_button_actions_handler(const struct device *dev,
					struct gpio_callback *cb,
					uint32_t pins)
{
	struct user_button_actions_channel *ch = CONTAINER_OF(cb,
							struct user_button_actions_channel,
							cb);
	if (gpio_pin_get_dt(&ch->btn)) {
		// pressed
		ch->_rv.down_cnt++;
		k_timer_start(&ch->timer,
			K_MSEC(CONFIG_USER_BUTTON_ACTION_HOLD_TIMEOUT_MS),
			K_NO_WAIT);
	} else {
		// releases
		ch->_rv.up_cnt++;
		k_timer_start(&ch->timer,
			K_MSEC(CONFIG_USER_BUTTON_ACTION_RE_CLICK_TIMEOUT_MS),
			K_NO_WAIT);
	}
}

int user_button_actions_start_monitor(struct user_button_actions_channel *ch)
{
	int err;

	err = gpio_is_ready_dt(&ch->btn);
	if (!err) {
		LOG_ERR("BTN is not ready");
		return 0;
	}

	err = gpio_pin_configure_dt(&ch->btn, GPIO_INPUT);
	if (err) {
		LOG_ERR("BTN is config err %d", err);
		return 0;
	}

	gpio_init_callback(&ch->cb, user_button_actions_handler, BIT(ch->btn.pin));

	err = gpio_add_callback(ch->btn.port, &ch->cb);
	if (err) {
		LOG_ERR("Callback config err %d", err);
		return 0;
	}

	err = gpio_pin_interrupt_configure_dt(&ch->btn, GPIO_INT_EDGE_BOTH);
	if (err) {
		LOG_ERR("int config err %d", err);
		return 0;
	}

	k_timer_init(&ch->timer, user_button_actions_timeout, NULL);
	k_work_init(&ch->work, user_button_actions_fire_work);

	return 0;
}

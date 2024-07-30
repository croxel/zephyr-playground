#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <user_button_actions/user_button_actions.h>

LOG_MODULE_REGISTER(user_button);

static struct user_button_actions_channel btn = {
	.btn = GPIO_DT_SPEC_GET(DT_ALIAS(sw0), gpios),
};

int user_button_setup(void)
{
	user_button_actions_start_monitor(&btn);
	return 0;
}

SYS_INIT(user_button_setup, APPLICATION, 99);

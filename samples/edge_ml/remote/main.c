#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

/* 100 msec = 1 sec */
#define ON_TIME_MS  50
#define OFF_TIME_MS 950

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led1)

/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

int main(void)
{
	int ret;

	printk("Hello world from %s\n", CONFIG_BOARD_TARGET);

	if (!gpio_is_ready_dt(&led)) {
		return 0;
	}

	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return 0;
	}

	k_msleep(OFF_TIME_MS);
	k_msleep(OFF_TIME_MS);

	while (1) {
		ret = gpio_pin_toggle_dt(&led);
		if (ret < 0) {
			return 0;
		}
		k_msleep(OFF_TIME_MS);
		ret = gpio_pin_toggle_dt(&led);
		if (ret < 0) {
			return 0;
		}
		k_msleep(ON_TIME_MS);
	}
	return 0;
}

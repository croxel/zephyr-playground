#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

int main(void)
{
	printk("EdgeML Demo %s\n", CONFIG_BOARD_TARGET);
	return 0;
}

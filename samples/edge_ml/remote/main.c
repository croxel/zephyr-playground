#include <stdio.h>
#include <zephyr/kernel.h>

int main(void)
{
	printk("Hello world from %s\n", CONFIG_BOARD_TARGET);
	return 0;
}

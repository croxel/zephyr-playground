/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main);

int main(void)
{
	LOG_INF("Hello World! %s\n", CONFIG_BOARD);
	return 0;
}

/**
 * @file listeners_utils.h
 * @author Anuj Pathak (anuj@croxel.com)
 * @brief
 * @version 0.1
 * @date 2022-09-23
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <zephyr/types.h>
#include <zephyr/sys/byteorder.h>
#include <errno.h>

/**
 * @brief Following macro will be used in header file of the module that supports listener
 * capabilities this macro defines a wrapper structure for callbacks and apis to add
 * and remove them
 */
#define SLL_LISTENER_DECLARE(name, value_type)					\
	struct sll_##name##_listener;						\
	struct sll_##name##_listener {						\
		struct sll_##name##_listener *next_node;			\
		value_type node_value;						\
	};									\
	int name##_add_listener(struct sll_##name##_listener *listener);	\
	int name##_remove_listener(struct sll_##name##_listener *listener)

/**
 * @brief Following macro will be used in source file of the module that supports listener
 * capabilities this macro replicate basic structure with corresponding type declaration,
 * it can be replaced with a function but to enforce type macro is chosen
 */
#define SLL_LISTENER_DEFINE(name, is_valid)						\
	static struct sll_##name##_listener *_##name##_root_listener_node;		\
	int name##_add_listener(struct sll_##name##_listener *listener)			\
	{										\
		struct sll_##name##_listener **node = &_##name##_root_listener_node;	\
		if (!listener || (!is_valid(&listener->node_value))) {			\
			return -EINVAL;							\
		}									\
		while (*node) {								\
			if (*node == listener) {					\
				return -EALREADY;					\
			}								\
			node = &(*node)->next_node;					\
		}									\
		*node = listener;							\
		listener->next_node = NULL;						\
		return 0;								\
	}										\
											\
	int name##_remove_listener(struct sll_##name##_listener *listener)		\
	{										\
		struct sll_##name##_listener **node = &_##name##_root_listener_node;	\
		if (!listener || (!is_valid(&listener->node_value))) {			\
			return -EINVAL;							\
		}									\
		while (*node) {								\
			if (*node == listener) {					\
				*node = listener->next_node;				\
				return 0;						\
			}								\
			node = &(*node)->next_node;					\
		}									\
		return -ENODEV;								\
	}

#define SLL_LISTENER_TEARDOWN(name) {		\
	_##name##_root_listener_node = NULL;	\
}

#define SLL_LISTENER_FIRE_NODES(name, handler_name, ...)				\
	{										\
		struct sll_##name##_listener **node = &_##name##_root_listener_node;	\
		while (*node) {								\
			if ((*node)->node_value.handler_name) {				\
				(*node)->node_value.handler_name(__VA_ARGS__);		\
			}								\
			node = &(*node)->next_node;					\
		}									\
	}

/**
 * @brief Following macro should be used by the user of the module
 */
#define SLL_LISTENER_DEFINE_NODE(name, node_name, ...)		\
	static struct sll_##name##_listener node_name = {	\
		.next_node = NULL,				\
		.node_value = {__VA_ARGS__},			\
	}

#define SLL_LISTENER_ADD_NODE(name, node_name) name##_add_listener(&node_name)

#define SLL_LISTENER_REMOVE_NODE(name, node_name) name##_remove_listener(&node_name)

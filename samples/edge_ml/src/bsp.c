#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/zbus/zbus.h>
#include <channel.h>

#define LED0_NODE DT_ALIAS(led0)
#define LED1_NODE DT_ALIAS(led1)
#define LED2_NODE DT_ALIAS(led2)
#define LED3_NODE DT_ALIAS(led3)
#define BTN0_NODE DT_ALIAS(sw0)
#define BTN1_NODE DT_ALIAS(sw1)
#define BTN2_NODE DT_ALIAS(sw2)
#define BTN3_NODE DT_ALIAS(sw3)

enum bsp_led_id {
	INFERENCE_STATE_LED = 0,
	COLLECTION_STATE_LED,
	BLE_CONNECTED_LED,
};

enum bsp_button_id {
	INFERENCE_TOGGLE_BUTTON = 0,
	COLLECTION_TOGGLE_BUTTON,
};

static const struct gpio_dt_spec leds[] = {
	GPIO_DT_SPEC_GET(LED0_NODE, gpios),
	GPIO_DT_SPEC_GET(LED1_NODE, gpios),
	GPIO_DT_SPEC_GET(LED2_NODE, gpios),
	GPIO_DT_SPEC_GET(LED3_NODE, gpios),
};
static const struct gpio_dt_spec buttons[] = {
	GPIO_DT_SPEC_GET(BTN0_NODE, gpios),
	GPIO_DT_SPEC_GET(BTN1_NODE, gpios),
	GPIO_DT_SPEC_GET(BTN2_NODE, gpios),
	GPIO_DT_SPEC_GET(BTN3_NODE, gpios),
};
static struct gpio_callback button_cb_data[4];

int bsp_led_set(uint8_t led_id, uint8_t state)
{
	return gpio_pin_set_dt(&leds[led_id], state);
}

int bsp_led_get(uint8_t led_id)
{
	return gpio_pin_get_dt(&leds[led_id]);
}

static void button_pressed_cb(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
	int err;
	struct demo_event event;

	event.arg = NULL;
	for (int i = 0; i < 4; i++) {
		if (pins & BIT(buttons[i].pin)) {
			switch (i) {
			case INFERENCE_TOGGLE_BUTTON:
				event.event_id = (bsp_led_get(INFERENCE_STATE_LED) == 1)
							 ? DEMO_EVENT_INFERENCE_STOP_ID
							 : DEMO_EVENT_INFERENCE_START_ID;
				err = zbus_chan_pub(&demo_event_chan, &event, K_NO_WAIT);
				__ASSERT_NO_MSG(err == 0);
				break;
			case COLLECTION_TOGGLE_BUTTON:
				event.event_id = (bsp_led_get(COLLECTION_STATE_LED) == 1)
							 ? DEMO_EVENT_COLLECTION_STOP_ID
							 : DEMO_EVENT_COLLECTION_START_ID;
				err = zbus_chan_pub(&demo_event_chan, &event, K_NO_WAIT);
				__ASSERT_NO_MSG(err == 0);
				break;
			}
		}
	}
}

static void demo_event_handler(const struct zbus_channel *chan)
{
	const struct demo_event *event_data = zbus_chan_const_msg(chan);

	switch (event_data->event_id) {
	case DEMO_EVENT_INFERENCE_START_ID:
		bsp_led_set(INFERENCE_STATE_LED, 1);
		break;
	case DEMO_EVENT_INFERENCE_STOP_ID:
		bsp_led_set(INFERENCE_STATE_LED, 0);
		break;
	case DEMO_EVENT_COLLECTION_START_ID:
		bsp_led_set(COLLECTION_STATE_LED, 1);
		break;
	case DEMO_EVENT_COLLECTION_STOP_ID:
		bsp_led_set(COLLECTION_STATE_LED, 0);
		break;
	case DEMO_EVENT_BLE_CONNECTED_ID:
		bsp_led_set(BLE_CONNECTED_LED, 1);
		break;
	case DEMO_EVENT_BLE_DISCONNECTED_ID:
		bsp_led_set(BLE_CONNECTED_LED, 0);
		break;
	default:
		break;
	}
}

ZBUS_LISTENER_DEFINE(demo_event_chan_listener, demo_event_handler);
ZBUS_CHAN_ADD_OBS(demo_event_chan, demo_event_chan_listener, 3);

int bsp_init(void)
{
	int ret;

	// Configure all LEDs
	for (int i = 0; i < 4; i++) {
		if (!gpio_is_ready_dt(&leds[i])) {
			return -ENODEV;
		}
		ret = gpio_pin_configure_dt(&leds[i], GPIO_OUTPUT_INACTIVE);
		if (ret < 0)
			return ret;
	}

	// Configure all buttons
	for (int i = 0; i < 4; i++) {
		if (!gpio_is_ready_dt(&buttons[i])) {
			return -ENODEV;
		}

		ret = gpio_pin_configure_dt(&buttons[i], GPIO_INPUT);
		if (ret < 0)
			return ret;

		ret = gpio_pin_interrupt_configure_dt(&buttons[i], GPIO_INT_EDGE_RISING);
		if (ret < 0)
			return ret;

		gpio_init_callback(&button_cb_data[i], button_pressed_cb, BIT(buttons[i].pin));
		gpio_add_callback(buttons[i].port, &button_cb_data[i]);
	}

	return 0;
}

SYS_INIT(bsp_init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);

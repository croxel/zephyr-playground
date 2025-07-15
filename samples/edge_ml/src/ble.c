/* main.c - Application main entry point */

/*
 * Copyright (c) 2015-2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/types.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/byteorder.h>
#include <zephyr/kernel.h>
#include <zephyr/settings/settings.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>
#include <channel.h>

/* Randomly generated service guid */
#define BT_UUID_EDGEML_SERVICE_VAL \
	BT_UUID_128_ENCODE(0x735297ee, 0x7e18, 0x4bf1, 0xacc2, 0xa1a98e6e963a)
#define BT_UUID_EDGEML_SERVICE_DATA_STREAM_CHAR_VAL \
	BT_UUID_128_ENCODE(0x735297ee, 0x7e18, 0x4bf1, 0xacc3, 0xa1a98e6e963a)
#define BT_UUID_EDGEML_SERVICE_INFERENCE_STREAM_CHAR_VAL \
	BT_UUID_128_ENCODE(0x735297ee, 0x7e18, 0x4bf1, 0xacc4, 0xa1a98e6e963a)
#define BT_UUID_EDGEML_SERVICE_INFERENCE_CTRL_VAL \
	BT_UUID_128_ENCODE(0x735297ee, 0x7e18, 0x4bf1, 0xacc5, 0xa1a98e6e963a)

static const struct bt_uuid_128 edge_ml_svc_uid = BT_UUID_INIT_128(
	BT_UUID_EDGEML_SERVICE_VAL);
static const struct bt_uuid_128 edge_ml_data_stream_uid = BT_UUID_INIT_128(
	BT_UUID_EDGEML_SERVICE_DATA_STREAM_CHAR_VAL);
static const struct bt_uuid_128 edge_ml_inference_stream_uid = BT_UUID_INIT_128(
	BT_UUID_EDGEML_SERVICE_INFERENCE_STREAM_CHAR_VAL);
static const struct bt_uuid_128 edge_ml_inference_ctrl_uid = BT_UUID_INIT_128(
	BT_UUID_EDGEML_SERVICE_INFERENCE_CTRL_VAL);

static bool data_streaming;
static bool inference_streaming;
static bool inference_running;
static uint16_t mtu_size = 23 - 3; // Default MTU size

static void edge_ml_data_stream_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
	data_streaming = (value == BT_GATT_CCC_NOTIFY);
	printk("Data streaming %s\n", data_streaming ? "enabled" : "disabled");
}

static void edge_ml_inference_stream_ccc_cfg_changed(const struct bt_gatt_attr *attr,
	uint16_t value)
{
	inference_streaming = (value == BT_GATT_CCC_NOTIFY);
	printk("Inference streaming %s\n", inference_streaming ? "enabled" : "disabled");
}

static ssize_t edge_ml_inference_ctrl_read(struct bt_conn *conn, const struct bt_gatt_attr *attr,
					   void *buf, uint16_t len, uint16_t offset)
{
	const char *value = (const char *)&inference_running;

	return bt_gatt_attr_read(conn, attr, buf, len, offset, value, sizeof(inference_running));
}

static ssize_t edge_ml_inference_ctrl_write(struct bt_conn *conn,
						const struct bt_gatt_attr *attr,
						const void *buf, uint16_t len, uint16_t offset,
						uint8_t flags)
{
	int err;
	struct demo_event event = {.arg = NULL};

	if (offset + len > sizeof(inference_running)) {
		return BT_GATT_ERR(BT_ATT_ERR_INVALID_OFFSET);
	}

	event.event_id = ((uint8_t *)buf)[offset]
				 ? DEMO_EVENT_INFERENCE_START_ID
				 : DEMO_EVENT_INFERENCE_STOP_ID,
	err = zbus_chan_pub(&demo_event_chan, &event, K_NO_WAIT);
	__ASSERT_NO_MSG(err == 0);

	return len;
}

/* Vendor Primary Service Declaration */
BT_GATT_SERVICE_DEFINE(edge_ml_svc,
			   BT_GATT_PRIMARY_SERVICE(&edge_ml_svc_uid),
			   /* data stream characterstic */
			   BT_GATT_CHARACTERISTIC(&edge_ml_data_stream_uid.uuid,
						  BT_GATT_CHRC_NOTIFY,
						  BT_GATT_PERM_NONE,
						  NULL, NULL, NULL),
			   BT_GATT_CCC(edge_ml_data_stream_ccc_cfg_changed,
				   BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
			   /* inference result stream */
			   BT_GATT_CHARACTERISTIC(&edge_ml_inference_stream_uid.uuid,
						  BT_GATT_CHRC_NOTIFY,
						  BT_GATT_PERM_NONE,
						  NULL, NULL, NULL),
			   BT_GATT_CCC(edge_ml_inference_stream_ccc_cfg_changed,
				   BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
			   /* inference start stop control */
			   BT_GATT_CHARACTERISTIC(&edge_ml_inference_ctrl_uid.uuid,
						  BT_GATT_CHRC_READ | BT_GATT_CHRC_WRITE,
						  BT_GATT_PERM_READ | BT_GATT_PERM_WRITE,
						  edge_ml_inference_ctrl_read,
						  edge_ml_inference_ctrl_write,
						  NULL),
			);

void mtu_updated(struct bt_conn *conn, uint16_t tx, uint16_t rx)
{
	printk("Updated MTU: TX: %d RX: %d bytes\n", tx, rx);
	mtu_size = tx - 3; // Subtract 3 bytes for ATT header
}

static struct bt_gatt_cb gatt_callbacks = {
	.att_mtu_updated = mtu_updated,
};

static const struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
	BT_DATA(BT_DATA_NAME_COMPLETE, CONFIG_BT_DEVICE_NAME, sizeof(CONFIG_BT_DEVICE_NAME) - 1),
	BT_DATA_BYTES(BT_DATA_UUID128_ALL, BT_UUID_EDGEML_SERVICE_VAL),
};

static void bt_advertise(void)
{
	int err;

	err = bt_le_adv_start(BT_LE_ADV_CONN_FAST_1, ad, ARRAY_SIZE(ad), NULL, 0);
	__ASSERT_NO_MSG(err == 0);
}

static void connected(struct bt_conn *conn, uint8_t reason)
{
	int err;
	struct demo_event event = {
		.event_id = reason ? DEMO_EVENT_BLE_DISCONNECTED_ID : DEMO_EVENT_BLE_CONNECTED_ID,
		.arg = NULL};

	err = zbus_chan_pub(&demo_event_chan, &event, K_NO_WAIT);
	__ASSERT_NO_MSG(err == 0);
}

static void disconnected(struct bt_conn *conn, uint8_t reason)
{
	int err;
	struct demo_event event = {.event_id = DEMO_EVENT_BLE_DISCONNECTED_ID, .arg = NULL};

	err = zbus_chan_pub(&demo_event_chan, &event, K_NO_WAIT);
	__ASSERT_NO_MSG(err == 0);
}

BT_CONN_CB_DEFINE(conn_callbacks) = {
	.connected = connected,
	.disconnected = disconnected,
};

ZBUS_SUBSCRIBER_DEFINE(ble_sub, 10);
ZBUS_CHAN_ADD_OBS(imu_sample_data_chan, ble_sub, 10);
ZBUS_CHAN_ADD_OBS(inference_result_chan, ble_sub, 10);
ZBUS_CHAN_ADD_OBS(demo_event_chan, ble_sub, 10);

void bt_gatt_notify_with_mtu(struct bt_conn *conn, const struct bt_gatt_attr *attr,
				 const uint8_t *data, uint16_t len)
{
	int err;
	uint16_t chunk_len;

	while (len > 0) {
		chunk_len = MIN(len, mtu_size);
		err = bt_gatt_notify(conn, attr, data, chunk_len);
		if (err) {
			printk("Notification failed: %d\n", err);
			return;
		}
		data += chunk_len;
		len -= chunk_len;
	}
}

static void ble_thread_func(void)
{
	int err;
	const struct zbus_channel *chan;

	err = bt_enable(NULL);
	__ASSERT_NO_MSG(err == 0);

	if (IS_ENABLED(CONFIG_SETTINGS)) {
		settings_load();
	}

	bt_advertise();

	bt_gatt_cb_register(&gatt_callbacks);

	while (1) {
		err = zbus_sub_wait(&ble_sub, &chan, K_FOREVER);
		__ASSERT_NO_MSG(!err);

		if ((chan == &imu_sample_data_chan) && data_streaming) {
			const struct imu_sample_data *data;

			err = zbus_chan_read(chan, &data, K_MSEC(10));
			__ASSERT_NO_MSG(!err);
			bt_gatt_notify_with_mtu(NULL, &edge_ml_svc.attrs[1],
						(uint8_t *)data, sizeof(*data));
		} else if ((chan == &inference_result_chan) && inference_streaming) {
			const struct gesture_inference_result *result;

			err = zbus_chan_read(chan, &result, K_MSEC(10));
			__ASSERT_NO_MSG(!err);
			bt_gatt_notify_with_mtu(NULL, &edge_ml_svc.attrs[4],
						(uint8_t *)result, sizeof(*result));
		} else if (chan == &demo_event_chan) {
			struct demo_event event;

			err = zbus_chan_read(chan, &event, K_MSEC(10));
			__ASSERT_NO_MSG(!err);
			switch (event.event_id) {
			case DEMO_EVENT_INFERENCE_START_ID:
				inference_running = true;
				break;
			case DEMO_EVENT_INFERENCE_STOP_ID:
				inference_running = false;
				break;
			case DEMO_EVENT_BLE_DISCONNECTED_ID:
				bt_advertise();
				break;
			default:
				break;
			}
		}
	}
}

K_THREAD_DEFINE(ble_thread, 1024, ble_thread_func, NULL, NULL, NULL,
		K_IDLE_PRIO - 1, 0, 0);

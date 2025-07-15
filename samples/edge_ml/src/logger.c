#include <zephyr/kernel.h>
#include <zephyr/zbus/zbus.h>
#include "channel.h"

ZBUS_SUBSCRIBER_DEFINE(logger_sub, 10);
ZBUS_CHAN_ADD_OBS(imu_sample_data_chan, logger_sub, 10);
ZBUS_CHAN_ADD_OBS(inference_result_chan, logger_sub, 10);
ZBUS_CHAN_ADD_OBS(demo_event_chan, logger_sub, 10);

static void logger_thread_func(void)
{
	const struct zbus_channel *chan;
	int err;
	bool is_inference_started = false;
	bool is_collection_started = false;

	while (1) {
		err = zbus_sub_wait(&logger_sub, &chan, K_FOREVER);
		__ASSERT_NO_MSG(!err);

		if ((chan == &imu_sample_data_chan) && is_collection_started) {
			const struct imu_sample_data *data;

			err = zbus_chan_read(chan, &data, K_MSEC(10));
			__ASSERT_NO_MSG(!err);
			printk("%d,%d,%d,%d,%d,%d,%d\n",
			       data->sequence,
			       data->acc.x, data->acc.y, data->acc.z,
			       data->gyro.x, data->gyro.y, data->gyro.z);
		} else if ((chan == &inference_result_chan) && is_inference_started) {
			const struct gesture_inference_result *result;

			err = zbus_chan_read(chan, &result, K_MSEC(10));
			__ASSERT_NO_MSG(!err);
			printk("[%d]:%d%%\n",
			       result->gesture_id_highest,
			       result->gesture_id_confidence[result->gesture_id_highest]);
		} else if (chan == &demo_event_chan) {
			struct demo_event event;

			err = zbus_chan_read(chan, &event, K_MSEC(10));
			__ASSERT_NO_MSG(!err);
			switch (event.event_id) {
			case DEMO_EVENT_INFERENCE_START_ID:
				is_inference_started = true;
				break;
			case DEMO_EVENT_INFERENCE_STOP_ID:
				is_inference_started = false;
				break;
			case DEMO_EVENT_COLLECTION_START_ID:
				is_collection_started = true;
				break;
			case DEMO_EVENT_COLLECTION_STOP_ID:
				is_collection_started = false;
				break;
			default:
				break;
			}
		}
	}
}

K_THREAD_DEFINE(logger_thread, 1024, logger_thread_func, NULL, NULL, NULL,
		K_IDLE_PRIO - 1, 0, 0);

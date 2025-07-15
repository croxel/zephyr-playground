#include <channel.h>

ZBUS_CHAN_DEFINE(imu_sample_data_chan,
		 const struct imu_sample_data *,
		 NULL,
		 NULL,
		 ZBUS_OBSERVERS_EMPTY,
		 ZBUS_MSG_INIT(0));

ZBUS_CHAN_DEFINE(inference_result_chan,
		 const struct gesture_inference_result *,
		 NULL,
		 NULL,
		 ZBUS_OBSERVERS_EMPTY,
		 ZBUS_MSG_INIT(0));

ZBUS_CHAN_DEFINE(demo_event_chan,
		 struct demo_event,
		 NULL,
		 NULL,
		 ZBUS_OBSERVERS_EMPTY,
		 ZBUS_MSG_INIT(0));

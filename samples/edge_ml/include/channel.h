#pragma once

#include <messages.h>
#include <zephyr/zbus/zbus.h>

ZBUS_CHAN_DECLARE(imu_sample_data_chan);

ZBUS_CHAN_DECLARE(inference_result_chan);

ZBUS_CHAN_DECLARE(demo_event_chan);

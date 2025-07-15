/**
 * @file inference_runner.c
 * @author Anuj Pathak (anuj@biii.in)
 * @brief
 * @version 0.1
 * @date 2025-07-11
 *
 * @copyright Copyright (c) 2025 BIII TECH LLP
 *
 */
#include <zephyr/kernel.h>
#include <neuton/neuton.h>
#include <neuton/neuton_version.h>
#include <channel.h>

ZBUS_SUBSCRIBER_DEFINE(inference_sub, 10);
ZBUS_CHAN_ADD_OBS(imu_sample_data_chan, inference_sub, 10);
ZBUS_CHAN_ADD_OBS(demo_event_chan, inference_sub, 10);

static struct gesture_inference_result inference_result;
const struct gesture_inference_result *inference_result_ptr = &inference_result;

static void inference_runner_thread_func(void)
{
	int err;
	neuton_input_t input_data[6];
	neuton_u16_t predicted_target;
	neuton_input_features_t *nai_input;
	const neuton_output_t *nai_output;
	const struct zbus_channel *ch = NULL;
	struct imu_sample_data *sample_data = NULL;
	bool is_inference_started = false;

	neuton_nn_setup();

	for (;;) {
		err = zbus_sub_wait(&inference_sub, &ch, K_FOREVER);
		__ASSERT_NO_MSG(!err);

		if ((ch == &imu_sample_data_chan) && is_inference_started) {
			err = zbus_chan_read(ch, &sample_data, K_MSEC(10));
			__ASSERT_NO_MSG(!err);
			/* input data copy not required as of now,
			 * but if data format changes it will come handy
			 */
			input_data[0] = sample_data->acc.x;
			input_data[1] = sample_data->acc.y;
			input_data[2] = sample_data->acc.z;
			input_data[3] = sample_data->gyro.x;
			input_data[4] = sample_data->gyro.y;
			input_data[5] = sample_data->gyro.z;
			/* feed input sample data */
			nai_input = neuton_nn_feed_inputs(&sample_data->acc.x, 6);
			if (nai_input) {
				inference_result.count = neuton_nn_run_inference(nai_input,
										 &predicted_target,
										 &nai_output);
				inference_result.gesture_id_highest = predicted_target;
				if (inference_result.count > 0) {
					for (neuton_u16_t i = 0; i < inference_result.count; i++) {
						inference_result.gesture_id_confidence[i] =
						    (nai_output[i] * 100) / 255;
					}
					err = zbus_chan_pub(&inference_result_chan,
							    &inference_result_ptr,
							    K_MSEC(10));
					__ASSERT_NO_MSG(!err);
				}
			} else {
				continue;
			}
		} else if (ch == &demo_event_chan) {
			struct demo_event event;

			err = zbus_chan_read(ch, &event, K_MSEC(10));
			__ASSERT_NO_MSG(!err);
			switch (event.event_id) {
			case DEMO_EVENT_INFERENCE_START_ID:
				is_inference_started = true;
				break;
			case DEMO_EVENT_INFERENCE_STOP_ID:
				is_inference_started = false;
				break;
			default:
				break;
			}
		}
	}
}

K_THREAD_DEFINE(inference_runner_thread, 1024, inference_runner_thread_func, NULL, NULL, NULL,
		K_IDLE_PRIO - 1, 0, 0);

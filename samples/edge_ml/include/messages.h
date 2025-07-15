#pragma once

#include <stdint.h>
#include <stdbool.h>

enum gesture_id {
	GESTURE_STATE_IDLE_ID = 0,
	GESTURE_STATE_RANDOM_ID,
	GESTURE_STATE_CLOCKWISE_ID,
	GESTURE_STATE_COUNTERCLOCKWISE_ID,
	GESTURE_STATE_RIGHT_SWIPE_ID,
	GESTURE_STATE_LEFT_SWIPE_ID,
	GESTURE_STATE_DOUBLE_TAP_ID,
	GESTURE_STATE_MAX_ID,
};

struct imu_sample_data {
	uint16_t sequence;
	struct {
		int16_t x;
		int16_t y;
		int16_t z;
	} acc;
	struct {
		int16_t x;
		int16_t y;
		int16_t z;
	} gyro;
};

struct demo_state {
	bool inference_active;
	bool streaming_data;
};

struct gesture_inference_result {
	uint8_t count;
	uint8_t gesture_id_highest;
	uint8_t gesture_id_confidence[GESTURE_STATE_MAX_ID];
};

enum demo_event_id {
	DEMO_EVENT_INFERENCE_START_ID,
	DEMO_EVENT_INFERENCE_STOP_ID,
	DEMO_EVENT_COLLECTION_START_ID,
	DEMO_EVENT_COLLECTION_STOP_ID,
	DEMO_EVENT_BLE_CONNECTED_ID,
	DEMO_EVENT_BLE_DISCONNECTED_ID,
};

struct demo_event {
	enum demo_event_id event_id;
	void *arg;
};

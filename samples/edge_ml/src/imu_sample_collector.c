#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <stdio.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(imu_collector, CONFIG_LOG_DEFAULT_LEVEL);

#define SLEEP_TIME_MS 20

static int sensor_value_to_int16(const struct sensor_value *val)
{
	double v = sensor_value_to_double(val) * 1000;
	// Convert sensor_value to int16, assuming val1 is the main value
	if ((((int)v) < INT16_MIN) || (((int)v) > INT16_MAX)) {
		LOG_ERR("Value out of range for int16: %d", ((int)v));
		return 0; // or handle error appropriately
	}
	return (int16_t)v;
}

static int process_icm45686(const struct device *dev)
{
	struct sensor_value temperature;
	struct sensor_value accel[3];
	struct sensor_value gyro[3];
	int rc = sensor_sample_fetch(dev);

	if (rc == 0) {
		rc = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
	}
	if (rc == 0) {
		rc = sensor_channel_get(dev, SENSOR_CHAN_GYRO_XYZ, gyro);
	}
	if (rc == 0) {
		// print int16 values for accel and gyro
		printf("%d,%d,%d,%d,%d,%d\n",
		       sensor_value_to_int16(&accel[0]),
		       sensor_value_to_int16(&accel[1]),
		       sensor_value_to_int16(&accel[2]),
		       sensor_value_to_int16(&gyro[0]),
		       sensor_value_to_int16(&gyro[1]),
		       sensor_value_to_int16(&gyro[2]));
	} else {
		printf("sample fetch/get failed: %d\n", rc);
	}

	return rc;
}

static void handle_icm45686_drdy(const struct device *dev,
				 const struct sensor_trigger *trig)
{
	int rc = process_icm45686(dev);

	if (rc != 0) {
		printf("cancelling trigger due to failure: %d\n", rc);
		(void)sensor_trigger_set(dev, trig, NULL);
		return;
	}
}

static int imu_collector_trigger(void)
{
	const struct device *const icm45686 = DEVICE_DT_GET_ONE(invensense_icm45686);
	static struct sensor_trigger data_trigger;

	if (!device_is_ready(icm45686)) {
		LOG_ERR("Device %s is not ready", icm45686->name);
		return -ENODEV;
	}

	data_trigger = (struct sensor_trigger){
	    .type = SENSOR_TRIG_DATA_READY,
	    .chan = SENSOR_CHAN_ALL,
	};

	if (sensor_trigger_set(icm45686, &data_trigger,
			       handle_icm45686_drdy) < 0) {
		printf("Cannot configure data trigger!!!\n");
		return -EIO;
	}

	return 0;
}

SYS_INIT(imu_collector_trigger, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);

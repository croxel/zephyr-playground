# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(edge_ml)

# Verify that this sample is built through sysbuild to ensure MCUboot is
# automatically included and that sample specific MCUboot configurations are
# used when building MCUboot.
test_sysbuild()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/neuton-ai/neuton/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/neuton-ai)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

target_sources(app PRIVATE
	src/ble.c
	src/bsp.c
	src/channel.c
	src/imu_sample_collector.c
	src/inference_runner.c
	src/logger.c
	src/main.c
)

zephyr_link_libraries(${CMAKE_CURRENT_LIST_DIR}/src/neuton-ai/neuton/lib/libneuton_arm_cortex-m33.a)

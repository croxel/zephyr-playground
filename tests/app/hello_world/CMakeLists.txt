cmake_minimum_required(VERSION 3.21.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(app)

target_include_directories(app PRIVATE
	.
)

target_sources(app PRIVATE
	src/main.c
)

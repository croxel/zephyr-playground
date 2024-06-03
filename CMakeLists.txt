set(ZEPHYR_PLAYGROUND_FW_ROOT ${CMAKE_CURRENT_LIST_DIR} CACHE PATH "ZEPHYR PLAYGROUND Root Directory")

zephyr_include_directories(include)

add_subdirectory(subsys)
add_subdirectory(drivers)

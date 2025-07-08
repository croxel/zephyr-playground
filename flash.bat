@echo off

if "%1"=="" (
	echo "Rebuilding ..."
	west build || goto :error
) else (
	echo "Prestine build ..."
	west build -b max32655fthr/max32655/m4 ..\zephyr\samples\drivers\i2s\echo\ %1 ^
		-- ^
		-DCONFIG_DEBUG_OPTIMIZATIONS=y ^
		-DCONFIG_LOG=y ^
		-DCONFIG_I2S_LOG_LEVEL_DBG=y ^
		-DCONFIG_DMA_LOG_LEVEL_DBG=y ^
		-DCONFIG_LOG_BUFFER_SIZE=2048 ^
		 || goto :error
)
openocd.exe -f interface/cmsis-dap.cfg -f target/max32655.cfg -c "init" -c "reset" -c "halt" -c "flash write_image erase build/zephyr/zephyr.hex" -c "reset" -c "exit"

goto :EOF

:error
echo Failed with error #%errorlevel%.
exit /b %errorlevel%

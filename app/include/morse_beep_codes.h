#include <zephyr/sys/util.h>
#include <user_alerts/user_alerts.h>

#define MORSE_DAA_FREQ	4400
#define MORSE_DIT_FREQ	3600
#define MORSE_SPACE_FREQ	0
#define MORSE_TIME(S)	((S) * 100)

/* morse beeper code array for "ZEPHYR" */
static const struct buzzer_alert_step _zephyr_morse_beep_steps[] = {
/* morse encoded "Z" Char */
	{.freq = MORSE_DAA_FREQ, .ms = MORSE_TIME(3)},
	{.freq = MORSE_SPACE_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_DAA_FREQ, .ms = MORSE_TIME(3)},
	{.freq = MORSE_SPACE_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_DIT_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_SPACE_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_DIT_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_SPACE_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_SPACE_FREQ, .ms = MORSE_TIME(3)},
/* morse encoded "E" Char */
	{.freq = MORSE_DIT_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_SPACE_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_SPACE_FREQ, .ms = MORSE_TIME(3)},
/* morse encoded "P" Char */
	{.freq = MORSE_DIT_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_SPACE_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_DAA_FREQ, .ms = MORSE_TIME(3)},
	{.freq = MORSE_SPACE_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_DAA_FREQ, .ms = MORSE_TIME(3)},
	{.freq = MORSE_SPACE_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_DIT_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_SPACE_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_SPACE_FREQ, .ms = MORSE_TIME(3)},
/* morse encoded "H" Char */
	{.freq = MORSE_DIT_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_SPACE_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_DIT_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_SPACE_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_DIT_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_SPACE_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_DIT_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_SPACE_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_SPACE_FREQ, .ms = MORSE_TIME(3)},
/* morse encoded "Y" Char */
	{.freq = MORSE_DAA_FREQ, .ms = MORSE_TIME(3)},
	{.freq = MORSE_SPACE_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_DIT_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_SPACE_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_DAA_FREQ, .ms = MORSE_TIME(3)},
	{.freq = MORSE_SPACE_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_DAA_FREQ, .ms = MORSE_TIME(3)},
	{.freq = MORSE_SPACE_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_SPACE_FREQ, .ms = MORSE_TIME(3)},
/* morse encoded "R" Char */
	{.freq = MORSE_DIT_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_SPACE_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_DAA_FREQ, .ms = MORSE_TIME(3)},
	{.freq = MORSE_SPACE_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_DIT_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_SPACE_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_SPACE_FREQ, .ms = MORSE_TIME(3)},
};

static const struct user_alerts_pattern _zephyr_morse_beeps = {
	.steps = _zephyr_morse_beep_steps,
	.steps_count = ARRAY_SIZE(_zephyr_morse_beep_steps),
	.loop_count = 1,
};
/* morse beeper code array for "CROXEL INC" */
static const struct buzzer_alert_step _croxel_inc_morse_beep_steps[] = {
/* morse encoded "C" Char */
	{.freq = MORSE_DAA_FREQ, .ms = MORSE_TIME(3)},
	{.freq = MORSE_SPACE_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_DIT_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_SPACE_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_DAA_FREQ, .ms = MORSE_TIME(3)},
	{.freq = MORSE_SPACE_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_DIT_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_SPACE_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_SPACE_FREQ, .ms = MORSE_TIME(3)},
/* morse encoded "R" Char */
	{.freq = MORSE_DIT_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_SPACE_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_DAA_FREQ, .ms = MORSE_TIME(3)},
	{.freq = MORSE_SPACE_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_DIT_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_SPACE_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_SPACE_FREQ, .ms = MORSE_TIME(3)},
/* morse encoded "O" Char */
	{.freq = MORSE_DAA_FREQ, .ms = MORSE_TIME(3)},
	{.freq = MORSE_SPACE_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_DAA_FREQ, .ms = MORSE_TIME(3)},
	{.freq = MORSE_SPACE_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_DAA_FREQ, .ms = MORSE_TIME(3)},
	{.freq = MORSE_SPACE_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_SPACE_FREQ, .ms = MORSE_TIME(3)},
/* morse encoded "X" Char */
	{.freq = MORSE_DAA_FREQ, .ms = MORSE_TIME(3)},
	{.freq = MORSE_SPACE_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_DIT_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_SPACE_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_DIT_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_SPACE_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_DAA_FREQ, .ms = MORSE_TIME(3)},
	{.freq = MORSE_SPACE_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_SPACE_FREQ, .ms = MORSE_TIME(3)},
/* morse encoded "E" Char */
	{.freq = MORSE_DIT_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_SPACE_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_SPACE_FREQ, .ms = MORSE_TIME(3)},
/* morse encoded "L" Char */
	{.freq = MORSE_DIT_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_SPACE_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_DAA_FREQ, .ms = MORSE_TIME(3)},
	{.freq = MORSE_SPACE_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_DIT_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_SPACE_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_DIT_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_SPACE_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_SPACE_FREQ, .ms = MORSE_TIME(3)},
/* morse encoded word space */
	{.freq = MORSE_SPACE_FREQ, .ms = MORSE_TIME(7)},
/* morse encoded "I" Char */
	{.freq = MORSE_DIT_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_SPACE_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_DIT_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_SPACE_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_SPACE_FREQ, .ms = MORSE_TIME(3)},
/* morse encoded "N" Char */
	{.freq = MORSE_DAA_FREQ, .ms = MORSE_TIME(3)},
	{.freq = MORSE_SPACE_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_DIT_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_SPACE_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_SPACE_FREQ, .ms = MORSE_TIME(3)},
/* morse encoded "C" Char */
	{.freq = MORSE_DAA_FREQ, .ms = MORSE_TIME(3)},
	{.freq = MORSE_SPACE_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_DIT_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_SPACE_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_DAA_FREQ, .ms = MORSE_TIME(3)},
	{.freq = MORSE_SPACE_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_DIT_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_SPACE_FREQ, .ms = MORSE_TIME(1)},
	{.freq = MORSE_SPACE_FREQ, .ms = MORSE_TIME(3)},
};

static const struct user_alerts_pattern _croxel_inc_morse_beeps = {
	.steps = _croxel_inc_morse_beep_steps,
	.steps_count = ARRAY_SIZE(_croxel_inc_morse_beep_steps),
	.loop_count = 1,
};

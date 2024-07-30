#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/pwm.h>
#include <user_alerts/user_alerts.h>
#include <morse_beep_codes.h>

LOG_MODULE_REGISTER(buzzer);

int pwm_user_alerts_step_exec(const struct user_alerts_channel *ch, const void *step)
{
	int err;
	const struct buzzer_alert_step *b_step = step;
	const struct pwm_dt_spec *pwm = ch->io;

	err = (b_step && b_step->freq)
		? pwm_set_dt(pwm, PWM_HZ(b_step->freq), PWM_HZ(b_step->freq) / 2)
		: pwm_set_dt(pwm, PWM_HZ(4000), 0);
	return err;
}

static const struct pwm_dt_spec _buzzer_ch_pwm = PWM_DT_SPEC_GET(DT_NODELABEL(buzzer));
static struct user_alerts_channel _buzzer_ch = {
	.step_size = sizeof(struct buzzer_alert_step),
	.io = &_buzzer_ch_pwm,
	.exec = pwm_user_alerts_step_exec,
	.cur_step_idx = 0,
	.cur_loop_idx = 0,
	/*".timer" wil be init using api */
	.pattern = NULL,
};

int play_bootup_buzzer_beeps(void)
{
	user_alerts_channel_init_timer(&_buzzer_ch);
	user_alerts_channel_play(&_buzzer_ch, &_zephyr_morse_beeps, true);
	return 0;
}

SYS_INIT(play_bootup_buzzer_beeps, APPLICATION, 99);

#include <zephyr/kernel.h>
#include <user_alerts/user_alerts.h>

K_MSGQ_DEFINE(user_alerts_q, sizeof(struct user_alerts_channel *),
	      CONFIG_USER_ALERTS_CHANNEL_QUEUE_SIZE, 1);

void ch_timer_period_irq(struct k_timer *timer)
{
	struct user_alerts_channel *ch = k_timer_user_data_get(timer);

	k_msgq_put(&user_alerts_q, &ch, K_NO_WAIT);
}

int user_alerts_channel_init_timer(struct user_alerts_channel *ch)
{
	if (!ch) {
		return -EINVAL;
	}
	if (k_timer_remaining_ticks(&ch->timer) > 0) {
		k_timer_stop(&ch->timer);
	}
	k_timer_init(&ch->timer, ch_timer_period_irq, NULL);
	k_timer_user_data_set(&ch->timer, ch);
	return 0;
}

int user_alerts_channel_stop(struct user_alerts_channel *ch, bool force)
{
	if (!ch) {
		return -EINVAL;
	}
	ch->pattern = NULL;
	return 0;
}

int user_alerts_channel_play(struct user_alerts_channel *ch,
			     const struct user_alerts_pattern *pattern,
			     bool force)
{
	if (!ch && !pattern && !ch->exec) {
		return -EINVAL;
	}
	if (ch->pattern && (force == false)) {
		return -EAGAIN;
	}
	ch->cur_step_idx = 0;
	ch->cur_loop_idx = 0;
	ch->pattern = pattern;
	return k_msgq_put(&user_alerts_q, &ch, K_MSEC(1));
}

static void beeper_thread_loop(void)
{
	struct user_alerts_channel *ch;

	while (true) {
		k_msgq_get(&user_alerts_q, &ch, K_FOREVER);
		if (ch->pattern) {
			if ((ch->pattern->loop_count == 0) ||
			    (ch->cur_loop_idx < ch->pattern->loop_count)) {
				const void *step = ((uint8_t *)ch->pattern->steps) +
						   (ch->step_size * ch->cur_step_idx);

				ch->exec(ch, step);
				k_timer_start(&ch->timer, K_MSEC(((struct alert_step *)step)->ms),
					      K_NO_WAIT);
				ch->cur_step_idx++;
				if (ch->cur_step_idx >= ch->pattern->steps_count) {
					ch->cur_step_idx = 0;
					ch->cur_loop_idx++;
				}
			} else {
				ch->exec(ch, NULL);
				ch->pattern = NULL;
				k_timer_stop(&ch->timer);
			}
		} else {
			ch->exec(ch, NULL);
			k_timer_stop(&ch->timer);
		}
	}
}

K_THREAD_DEFINE(beeper_thread_id, 1024, beeper_thread_loop, NULL, NULL, NULL, K_IDLE_PRIO + 1, 0,
		0);

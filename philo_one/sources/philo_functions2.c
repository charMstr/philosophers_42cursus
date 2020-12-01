/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_functions2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <charmstr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/30 21:08:16 by charmstr          #+#    #+#             */
/*   Updated: 2020/12/01 15:44:25 by charmstr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

/*
** note:	this function will copy a string into our buffer, starting at
**			start index.
**
** RETURNS:	the length of the string we copied into our buffer
*/

int philo_strcpy_in_buffer(char *dst, int start, const char *src)
{
	size_t i;

	i = 0;
	while (src[i])
	{
		dst[i + start] = src[i];
		i++;
	}
	return (i);
}

/*
** note:	this function will do the mutex_locks and the writing in a separate
**			thread.
*/

void *philo_write_in_thread(void *writer_void)
{
	t_writer *writer;
	char buffer[32];
	int len;

	writer = (t_writer*)writer_void;
	len = philo_itoa_set_buff(writer->time, buffer, 1, 0);
	//len += philo_strcpy_in_buffer(buffer, len, writer->itoa_id);
	if (writer->state == FORK)
		len += philo_strcpy_in_buffer(buffer, len, "has taken a fork\n");
	else if (writer->state == EAT)
		len += philo_strcpy_in_buffer(buffer, len, "is eating\n");
	else if (writer->state == SLEEP)
		len += philo_strcpy_in_buffer(buffer, len, "is sleeping\n");
	else if (writer->state == THINK)
		len += philo_strcpy_in_buffer(buffer, len, "is thinking\n");
	else
		len += philo_strcpy_in_buffer(buffer, len, "died\n");
	pthread_mutex_lock(writer->mutex_on_mic);
	write(1, buffer, len);
	pthread_mutex_unlock(writer->mutex_on_mic);
	free(writer);
	return (NULL);
}

/*
** note:	this function will describe on stdout the state of the philosopher.
*/

void	describe_state(t_philo *philo, t_state state, int time)
{
	pthread_t writer_thread;
	t_writer *writer;

	if (!(writer = malloc(sizeof(t_writer))))
		return ;
	writer->time = time;
	writer->state = state;
	writer->id = philo->id;
	writer->mutex_on_mic = philo->mutex_on_mic;
	pthread_create(&writer_thread, NULL, philo_write_in_thread, (void*)(writer));
	pthread_detach(writer_thread);
}

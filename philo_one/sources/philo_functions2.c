/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_functions2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <charmstr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/30 21:08:16 by charmstr          #+#    #+#             */
/*   Updated: 2020/12/01 22:58:00 by charmstr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

/*
** note:	this function will copy a string into our buffer, starting at
**			start index.
**
** RETURNS:	the length of the string we copied into our buffer
*/

unsigned int philo_strcpy_in_buffer(char *dst, unsigned int start, const char *src)
{
	unsigned int i;

	i = 0;
	while (src[i])
	{
		dst[i + start] = src[i];
		i++;
	}
	return (i);
}

void	*philo_write(void *writer_void)
{
	t_writer *writer;
	unsigned int	len;

	writer = (t_writer*)writer_void;
	len = philo_itoa_set_buff(writer->time, writer->buffer, 1, 0);
	//len += philo_strcpy_in_buffer(writer->buffer, len, philo->itoa_id);
	if (writer->state == FORK1 || writer->state == FORK2)
		len += philo_strcpy_in_buffer(writer->buffer, len, "has taken a fork\n");
	else if (writer->state == EAT)
		len += philo_strcpy_in_buffer(writer->buffer, len, "is eating\n");
	else if (writer->state == SLEEP)
		len += philo_strcpy_in_buffer(writer->buffer, len, "is sleeping\n");
	else if (writer->state == THINK)
		len += philo_strcpy_in_buffer(writer->buffer, len, "is thinking\n");
	else
		len += philo_strcpy_in_buffer(writer->buffer, len, "died\n");
	pthread_mutex_lock(writer->mutex_on_mic);
	write(1, writer->buffer, len);
	pthread_mutex_unlock(writer->mutex_on_mic);
	return (NULL);
}

/*
** note:	this function will describe on stdout the state of the philosopher.
*/

void describe_state(t_state state, unsigned int time, t_writer *writer)
{
	pthread_t writer_thread;

	writer->state = state;
	writer->time = time;
	pthread_create(&writer_thread, NULL, philo_write, (void*)writer);
	pthread_detach(writer_thread);
}

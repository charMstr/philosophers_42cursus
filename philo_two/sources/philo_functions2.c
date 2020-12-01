/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_functions2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <charmstr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/30 21:08:16 by charmstr          #+#    #+#             */
/*   Updated: 2020/12/02 00:49:15 by charmstr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

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

/*
** note:	this funcition will write first a space, then write our number
**			backwards, finally returning the index where should be placed the
**			very next character.
*/

int philo_num_to_buff(int num, char buff[], int start)
{
	int i;

	i = 1;
	buff[start] = ' ';
	if (num == 0)
	{
		buff[i + start] = 48;
		i++;
	}
	while (num > 0)
	{
		buff[i + start] = 48 + num % 10;
		num = num / 10;
		i++;
	}
	return (i + start);
}

/*
** note:	when we are done writing our spaces and the backward numbers, we
**			finally revert the string.
*/

void philo_strrev(int len, char *buff)
{
	int j;

	j = 0;
	while (--len > j)
	{
		buff[len] ^= buff[j];
		buff[j] ^= buff[len];
		buff[len] ^= buff[j];
		j++;
	}
}

/*
** note:	this function is started in a thread, it will set up the correct
**			values in a buffer, thern write the buffer to stdout with the
**			mutex's permission.
**
** note:	The writer structure contains the mutex_on_mic (write to stdout),
**			and also a dedicated buffer for one of the 6 different sentences
**			we are going to build.
*/

void	*philo_write(void *writer_void)
{
	t_writer *writer;
	unsigned int	len;

	writer = (t_writer*)writer_void;
	len = philo_num_to_buff(writer->id, writer->buffer, 0);
	len = philo_num_to_buff(writer->time, writer->buffer, len);
	philo_strrev(len, writer->buffer);
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

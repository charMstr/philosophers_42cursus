/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_functions2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <charmstr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/30 21:08:16 by charmstr          #+#    #+#             */
/*   Updated: 2020/12/01 16:40:02 by charmstr         ###   ########.fr       */
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
** note:	this function will do the mutex_locks and the writing in a separate
**			thread.
*/

void *philo_write_in_thread(void *handle_void)
{
	t_writer_handle *handle;
	int len;
	char buffer[32];

	handle = (t_writer_handle*)handle_void;
	len = philo_num_to_buff(handle->id, buffer, 0);
	len = philo_num_to_buff(handle->time, buffer, len);
	philo_strrev(len, buffer);
	if (handle->state == FORK)
		len += philo_strcpy_in_buffer(buffer, len, "has taken a fork\n");
	else if (handle->state == EAT)
		len += philo_strcpy_in_buffer(buffer, len, "is eating\n");
	else if (handle->state == SLEEP)
		len += philo_strcpy_in_buffer(buffer, len, "is sleeping\n");
	else if (handle->state == THINK)
		len += philo_strcpy_in_buffer(buffer, len, "is thinking\n");
	else
		len += philo_strcpy_in_buffer(buffer, len, "died\n");
	pthread_mutex_lock(handle->mutex_on_mic);
	write(1, buffer, len);
	pthread_mutex_unlock(handle->mutex_on_mic);
	free(handle);
	return (NULL);
}

/*
** note:	this function will describe on stdout the state of the philosopher.
*/

void	describe_state(t_philo *philo, t_state state, int time)
{
	pthread_t writer;
	t_writer_handle *handle;

	if (!(handle = malloc(sizeof(t_writer_handle))))
		return ;
	handle->state = state;
	handle->time = time;
	handle->id = philo->id;
	handle->mutex_on_mic = philo->mutex_on_mic;
	pthread_create(&writer, NULL, philo_write_in_thread, (void*)handle);
	pthread_detach(writer);
}

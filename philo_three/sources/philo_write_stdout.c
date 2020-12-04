/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_write_stdout.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <charmstr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/30 21:08:16 by charmstr          #+#    #+#             */
/*   Updated: 2020/12/03 21:17:36 by charmstr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

/*
** note:	this function will copy a string into our buffer, starting at
**			start index.
** ** RETURNS:	the length of the string we copied into our buffer
*/

unsigned int	philo_strcpy_in_buffer(char *dst, unsigned int start, \
		const char *src)
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

int				philo_num_to_buff(int num, char buff[], int start)
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

void			philo_strrev(int len, char *buff)
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
** note:	no need for a write, we use an atomic write with a memory aligned
**			on 32 bits!
**
** RETURN:	1 it was ok to write, no philo is dead.
**			0 a philo was dead, no more writes to stdout.
*/

void			write_philo_state(t_philo *philo)
{
	unsigned int	len;

	len = philo_num_to_buff(philo->id, philo->state_buff, 0);
	len = philo_num_to_buff(philo->time, philo->state_buff, len);
	philo_strrev(len, philo->state_buff);
	if (philo->state == FORK)
		len += philo_strcpy_in_buffer(philo->state_buff, len,
				"has taken a fork\n");
	else if (philo->state == EAT)
		len += philo_strcpy_in_buffer(philo->state_buff, len, "is eating\n");
	else if (philo->state == SLEEP)
		len += philo_strcpy_in_buffer(philo->state_buff, len, "is sleeping\n");
	else
		len += philo_strcpy_in_buffer(philo->state_buff, len, "is thinking\n");
	sem_wait(philo->sema_talk);
	write(1, philo->state_buff, len);
	sem_post(philo->sema_talk);
}

/*
** note:	this function is designed to write that a philosopher is dead.
**
** note:	inside the functino if a philosoper had already died, we dont use
**			stdout. otherwise we set the global variable to 0.
**
** note:	the semaphore for talking is held but never released, therefor no
**			other thread can use it (and it is common to all processes).
*/

void			write_stop_philo(t_philo *philo, int full_or_dead)
{
	unsigned int	len;

	if (full_or_dead == PHILO_DONE_EATING)
	{
		len = philo_num_to_buff(philo->id, philo->state_buff, 0);
		philo->state_buff[len++] = ' ';
		philo->state_buff[len++] = '[';
		philo_strrev(len, philo->state_buff);
		len += philo_strcpy_in_buffer(philo->state_buff, len, \
				"says no more 🍔!! ]\n");
		sem_wait(philo->sema_talk);
		write(1, philo->state_buff, len);
		sem_post(philo->sema_talk);
		return ;
	}
	len = philo_num_to_buff(philo->id, philo->death_buff, 0);
	len = philo_num_to_buff(philo->time_poll, philo->death_buff, len);
	philo_strrev(len, philo->death_buff);
	len += philo_strcpy_in_buffer(philo->death_buff, len, "died\n");
	sem_wait(philo->sema_talk);
	write(1, philo->death_buff, len);
}

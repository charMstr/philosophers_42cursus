/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_write_stdout.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <charmstr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/30 21:08:16 by charmstr          #+#    #+#             */
/*   Updated: 2020/12/02 05:00:16 by charmstr         ###   ########.fr       */
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

void write_without_lock(t_philo *philo)
{
	unsigned int	len;

	len = philo_num_to_buff(philo->id, philo->buffer, 0);
	len = philo_num_to_buff(philo->time, philo->buffer, len);
	philo_strrev(len, philo->buffer);
	if (philo->state == FORK)
		len += philo_strcpy_in_buffer(philo->buffer, len, "has taken a fork\n");
	else if (philo->state == EAT)
		len += philo_strcpy_in_buffer(philo->buffer, len, "is eating\n");
	else if (philo->state == SLEEP)
		len += philo_strcpy_in_buffer(philo->buffer, len, "is sleeping\n");
	else if (philo->state == THINK)
		len += philo_strcpy_in_buffer(philo->buffer, len, "is thinking\n");
	else
		len += philo_strcpy_in_buffer(philo->buffer, len, "died\n");
	write(1, philo->buffer, len);
}

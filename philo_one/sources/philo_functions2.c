/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_functions2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <charmstr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/30 21:08:16 by charmstr          #+#    #+#             */
/*   Updated: 2020/12/01 02:59:09 by charmstr         ###   ########.fr       */
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
** note:	this function will describe on stdout the state of the philosopher.
*/

void	describe_state(t_philo *philo, t_state state, int time)
{
	int len;
	char buffer[32];

	len = philo_itoa_set_buff(time, buffer);
	len += philo_strcpy_in_buffer(buffer, len, philo->itoa_id);
	if (state == FORK)
		len += philo_strcpy_in_buffer(buffer, len, "has taken a fork\n");
	else if (state == EAT)
		len += philo_strcpy_in_buffer(buffer, len, "is eating\n");
	else if (state == SLEEP)
		len += philo_strcpy_in_buffer(buffer, len, "is sleeping\n");
	else if (state == THINK)
		len += philo_strcpy_in_buffer(buffer, len, "is thinking\n");
	else
		len += philo_strcpy_in_buffer(buffer, len, "died\n");
	pthread_mutex_lock(philo->mutex_on_mic);
	write(1, buffer, len);
	pthread_mutex_unlock(philo->mutex_on_mic);
}

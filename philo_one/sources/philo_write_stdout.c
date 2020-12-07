/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_write_stdout.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <charmstr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/30 21:08:16 by charmstr          #+#    #+#             */
/*   Updated: 2020/12/07 01:29:47 by charmstr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

/*
** note:	This functino will write the state of the buffer to stdout. It uses
**			a mutex to have access to stdout.
**
** note:	If a philosophers is noticed to be dead, it will display it and
**			never release the mutex for stdout, therefor protecting any
**			subsequent output.
*/

void			write_with_lock(t_philo *philo, t_state state)
{
	unsigned int	len;

	len = philo_num_to_buff(philo->id, philo->state_buff, 0);
	len = philo_num_to_buff(philo->elapsed_time, philo->state_buff, len);
	philo_strrev(len, philo->state_buff);
	if (state == FORK)
		len += philo_strcpy_in_buffer(philo->state_buff, len, \
				"has taken a fork\n");
	else if (state == EAT)
		len += philo_strcpy_in_buffer(philo->state_buff, len, "is eating\n");
	else if (state == SLEEP)
		len += philo_strcpy_in_buffer(philo->state_buff, len, "is sleeping\n");
	else
		len += philo_strcpy_in_buffer(philo->state_buff, len, "is thinking\n");
	pthread_mutex_lock(philo->speaker);
	if (*(philo->nb_philo_alive) == 0)
	{
		pthread_mutex_unlock(philo->speaker);
		return ;
	}
	write(1, philo->state_buff, len);
	pthread_mutex_unlock(philo->speaker);
}

/*
** note:	this function will be called only when the philo is dead
**
** note:	in order to avoid displaying many deaths occurence, we check each
**			time if the stop condition between locking and unlocking the
**			speaker mutex.
*/

void			write_dead_philo(t_philo *philo)
{
	unsigned int	len;

	len = philo_num_to_buff(philo->id, philo->death_buff, 0);
	len = philo_num_to_buff(philo->death_time, philo->death_buff, len);
	philo_strrev(len, philo->death_buff);
	len += philo_strcpy_in_buffer(philo->death_buff, len, "died\n");
	pthread_mutex_lock(philo->speaker);
	if (!*(philo->nb_philo_alive))
	{
		pthread_mutex_unlock(philo->speaker);
		return ;
	}
	*(philo->nb_philo_alive) = 0;
	write(1, philo->death_buff, len);
	pthread_mutex_unlock(philo->speaker);
}

/*
** note:	this function will be called when a philosopher has had all his
**			dinners. It uses the stdout mutex then releases it.
**
** note:	for the philosopher that is full, its Main thread has already
**			returned null. and when done with this function the polling_philo
**			thread will return as well.
**
** note:	we will decrease the total_number of philosophers each time one
**			has stop eating. note that we also set this value directly to zero
**			if a philosopher dies.
*/

void			write_fed_up_philo(t_philo *philo)
{
	unsigned int	len;

	len = philo_num_to_buff(philo->id, philo->death_buff, 0);
	philo->death_buff[len++] = ' ';
	philo->death_buff[len++] = '[';
	philo_strrev(len, philo->death_buff);
	len += philo_strcpy_in_buffer(philo->death_buff, len, \
			"says no more 🍔!! ]\n");
	pthread_mutex_lock(philo->speaker);
	if (!*(philo->nb_philo_alive))
	{
		pthread_mutex_unlock(philo->speaker);
		return ;
	}
	(*(philo->nb_philo_alive))--;
	write(1, philo->death_buff, len);
	pthread_mutex_unlock(philo->speaker);
}

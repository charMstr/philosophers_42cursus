/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_functions1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <charmstr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/30 15:16:04 by charmstr          #+#    #+#             */
/*   Updated: 2020/12/02 03:25:31 by charmstr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

/*
** note:	This function returns the time elapsed since the last meal.
**			It takes care of the case when we go from one second to another.
**
** RETURN:	time elapsed
**			-1 failure with gettimeofday func.
*/

unsigned int	get_elapsed_time(t_philo *philo)
{
	gettimeofday(&philo->timeval_tmp, NULL);
	if (philo->timeval_last_meal.tv_sec == philo->timeval_tmp.tv_sec)
	{
		return ((philo->timeval_tmp.tv_usec - \
					philo->timeval_last_meal.tv_usec) / 1000);
	}
	else
	{
		return ((philo->timeval_tmp.tv_sec - philo->timeval_last_meal.tv_sec \
			- 1) * 1000 + (1000000 - philo->timeval_last_meal.tv_usec \
				+ philo->timeval_tmp.tv_usec) / 1000);
	}
}

/*
** note:	This function will try to see if the philosopher can eat: if it
**			cannot, it returns 0 and the stop's value common to all threads
**			will be set to 1.
**
** note:	One out of two philosophers is made left handed, the other one
**			right handed.
**
** RETURN:	0 KO
**			1 OK
*/

void philo_try_to_grab_forks(t_philo *philo)
{
	pthread_mutex_lock(&((philo->mutexes_on_forks)[philo->fork1]));
	write_without_lock(FORK, get_elapsed_time(philo), philo);
	pthread_mutex_lock(&((philo->mutexes_on_forks)[philo->fork2]));
	write_without_lock(FORK, get_elapsed_time(philo), philo);
}

void philo_starts_to_eat(t_philo *philo, unsigned int time)
{
	write_without_lock(EAT, time, philo);
	usleep(philo->time_to_eat);
	pthread_mutex_unlock(&((philo->mutexes_on_forks)[philo->fork1]));
	pthread_mutex_unlock(&((philo->mutexes_on_forks)[philo->fork2]));
	philo->timeval_last_meal = philo->timeval_tmp;
	if (philo->meals_limit)
		philo->meals_count--;
	write_without_lock(SLEEP, get_elapsed_time(philo), philo);
	usleep(philo->time_to_sleep);
	write_without_lock(THINK, get_elapsed_time(philo), philo);
}

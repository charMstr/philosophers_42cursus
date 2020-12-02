/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_functions1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <charmstr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/30 15:16:04 by charmstr          #+#    #+#             */
/*   Updated: 2020/12/01 23:27:14 by charmstr         ###   ########.fr       */
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

void philo_try_to_grab_forks(t_philo *philo, t_writer (*array_writers)[])
{
	pthread_mutex_lock(&((philo->mutexes_on_forks)[philo->fork1]));
	describe_state(FORK1, get_elapsed_time(philo), &(*array_writers)[0]);
	pthread_mutex_lock(&((philo->mutexes_on_forks)[philo->fork2]));
	describe_state(FORK2, get_elapsed_time(philo), &(*array_writers)[1]);
}

void philo_starts_to_eat(t_philo *philo, unsigned int time, t_writer (*array_writers)[])
{
	describe_state(EAT, time, &(*array_writers)[2]);
	usleep(philo->time_to_eat);
	pthread_mutex_unlock(&((philo->mutexes_on_forks)[philo->fork1]));
	pthread_mutex_unlock(&((philo->mutexes_on_forks)[philo->fork2]));
	philo->timeval_last_meal = philo->timeval_tmp;
	if (philo->meals_limit)
		philo->meals_count--;
	describe_state(SLEEP, get_elapsed_time(philo), &(*array_writers)[3]);
	usleep(philo->time_to_sleep);
	describe_state(THINK, get_elapsed_time(philo), &(*array_writers)[4]);
}

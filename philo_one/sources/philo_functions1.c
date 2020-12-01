/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_functions1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <charmstr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/30 15:16:04 by charmstr          #+#    #+#             */
/*   Updated: 2020/12/01 17:23:27 by charmstr         ###   ########.fr       */
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

int	get_elapsed_time(t_philo *philo)
{
	int res;

	if (gettimeofday(&philo->timeval_tmp, NULL))
		return (-1);
	if (philo->timeval_last_meal.tv_sec == philo->timeval_tmp.tv_sec)
	{
		return ((philo->timeval_tmp.tv_usec - \
					philo->timeval_last_meal.tv_usec) / 1000);
	}
	else
	{
		res = (philo->timeval_tmp.tv_sec - philo->timeval_last_meal.tv_sec - 1) * 1000;
		res += (1000000 - philo->timeval_last_meal.tv_usec + philo->timeval_tmp.tv_usec) / 1000;
		return (res);
	}
}

/*
** note:	This function will try to see if the philosopher can eat: if it
**			cannot, it will prompt a message as being dead, and the stop's
**			value common to all threads will be set to 1.
**
** note:	One out of two philosophers is made left handed, the other one
**			right handed.
**
** RETURN:	0 OK
**			else, the timestamp at wich philo died.
*/

int philo_try_to_eat(t_philo *philo, int time)
{
	pthread_mutex_lock(&((philo->mutexes_on_forks)[philo->fork1]));
	describe_state(philo, FORK, time);
	pthread_mutex_lock(&((philo->mutexes_on_forks)[philo->fork2]));
	if (((time = get_elapsed_time(philo)) == -1) || time > philo->time_to_die)
	{
		pthread_mutex_unlock(&((philo->mutexes_on_forks)[philo->fork1]));
		pthread_mutex_unlock(&((philo->mutexes_on_forks)[philo->fork2]));
		return (time);
	}
	describe_state(philo, FORK, time);
	describe_state(philo, EAT, time);
	usleep(philo->time_to_eat);
	pthread_mutex_unlock(&((philo->mutexes_on_forks)[philo->fork1]));
	pthread_mutex_unlock(&((philo->mutexes_on_forks)[philo->fork2]));
	philo->timeval_last_meal = philo->timeval_tmp;
	philo->meals_count++;
	time = get_elapsed_time(philo);
	describe_state(philo, SLEEP, time);
	usleep(philo->time_to_sleep);
	time = get_elapsed_time(philo);
	describe_state(philo, THINK, time);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_gettime_and_eat.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <charmstr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/30 15:16:04 by charmstr          #+#    #+#             */
/*   Updated: 2020/12/04 04:58:44 by charmstr         ###   ########.fr       */
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

void			philo_try_to_grab_forks_and_eat(t_philo *philo)
{
	pthread_mutex_lock(philo->fork1);
	philo->state = FORK;
	pthread_mutex_lock(philo->fork2);
	philo->state = FORK;
	philo->time = get_elapsed_time(philo);
	write_with_lock(philo);
	write_with_lock(philo);
	philo->time = get_elapsed_time(philo);
	philo->timeval_last_meal = philo->timeval_tmp;
	philo->state = EAT;
	write_with_lock(philo);
	usleep(philo->time_to_eat);
	pthread_mutex_unlock(philo->fork1);
	pthread_mutex_unlock(philo->fork2);
}

/*
** note:	the separation is made so that we can interrupt the printing on
**			stdout in case we are done with our meals.
*/

void			philo_try_to_sleep_and_think(t_philo *philo)
{
	philo->state = SLEEP;
	philo->time = get_elapsed_time(philo);
	write_with_lock(philo);
	usleep(philo->time_to_sleep);
	philo->state = THINK;
	philo->time = get_elapsed_time(philo);
	write_with_lock(philo);
}

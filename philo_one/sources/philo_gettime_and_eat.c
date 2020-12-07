/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_gettime_and_eat.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <charmstr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/30 15:16:04 by charmstr          #+#    #+#             */
/*   Updated: 2020/12/07 01:34:36 by charmstr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

/*
** note:	This function returns the time elapsed since the last meal.
**			It takes care of the case when we go from one second to another.
*/

void	set_elapsed_time(t_philo *philo)
{
	gettimeofday(&philo->timeval_tmp, NULL);
	if (philo->timeval_last_meal.tv_sec == philo->timeval_tmp.tv_sec)
	{
		philo->elapsed_time = ((philo->timeval_tmp.tv_usec - \
					philo->timeval_last_meal.tv_usec) / 1000);
	}
	else
	{
		philo->elapsed_time = ((philo->timeval_tmp.tv_sec \
			- philo->timeval_last_meal.tv_sec - 1) * 1000 + (1000000 \
			- philo->timeval_last_meal.tv_usec + philo->timeval_tmp.tv_usec) \
			/ 1000);
	}
}

/*
** note:	This function will update the philosoher's last meal time, and set
**			the elapsed time to zero.
*/

void	philo_update_last_meal_time(t_philo *philo)
{
	pthread_mutex_lock(philo->touch_last_meal);
	gettimeofday(&philo->timeval_last_meal, NULL);
	pthread_mutex_unlock(philo->touch_last_meal);
	philo->elapsed_time = 0;
}

/*
** note:	This function will retrieve the last meal_time in the philosopher
**			structure, and update the elapsed time to zero.
**
** return:	1 OK
**			0 KO, the philosopher died before eating.
*/

int		philo_check_last_meal_time(t_philo *philo)
{
	struct timeval tmp;

	gettimeofday(&tmp, NULL);
	if (philo->timeval_last_meal.tv_sec == tmp.tv_sec)
		philo->death_time = ((tmp.tv_usec - philo->timeval_last_meal.tv_usec) \
				/ 1000);
	else
	{
		philo->death_time = ((tmp.tv_sec - philo->timeval_last_meal.tv_sec - \
			1) * 1000 + (1000000 - philo->timeval_last_meal.tv_usec + \
				tmp.tv_usec) / 1000);
	}
	if (philo->death_time > philo->time_to_die)
		return (0);
	return (1);
}

/*
** note:	This function will try to update the philospher's last meal time in
**			time before it starves.
**
** note:	At start all the philosophers except the first one are right handed
**			and they are started in a specific order so that the odd id numbers
**			start together.
**			Then each time a philosopher is done eating and has an odd number,
**			it will go from being right handed to left handed or vice versa.
*/

void	philo_try_to_grab_forks_and_eat(t_philo *philo)
{
	pthread_mutex_t *tmp;

	pthread_mutex_lock(philo->fork1);
	set_elapsed_time(philo);
	write_with_lock(philo, FORK);
	pthread_mutex_lock(philo->fork2);
	set_elapsed_time(philo);
	write_with_lock(philo, FORK);
	philo_update_last_meal_time(philo);
	write_with_lock(philo, EAT);
	usleep(philo->time_to_eat);
	pthread_mutex_unlock(philo->fork1);
	pthread_mutex_unlock(philo->fork2);
	if (philo->id % 2)
	{
		tmp = philo->fork1;
		philo->fork1 = philo->fork2;
		philo->fork2 = tmp;
	}
}

/*
** note:	the separation is made so that we can interrupt the printing on
**			stdout in case we are done with our meals.
*/

void	philo_try_to_sleep_and_think(t_philo *philo)
{
	set_elapsed_time(philo);
	write_with_lock(philo, SLEEP);
	usleep(philo->time_to_sleep);
	set_elapsed_time(philo);
	write_with_lock(philo, THINK);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_functions1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <charmstr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/30 15:16:04 by charmstr          #+#    #+#             */
/*   Updated: 2020/12/01 03:56:09 by charmstr         ###   ########.fr       */
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

/*
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
*/

int get_timestamp(t_philo *philo)
{
	//struct timeval	timeval_tmp;

	if (gettimeofday(&philo->timeval_tmp, NULL))
		return (-1);
	return ((philo->timeval_tmp.tv_sec - philo->timeval_start.tv_sec) * 1000 \
		+ (philo->timeval_tmp.tv_usec - philo->timeval_start.tv_usec) / 1000);
}

/*
** note:	this function will decide if the first_fork_index based on the
**			philo_id, given that odd ids will be left_handed.
**
** RETURN:	first_fork_index
*/

int	first_fork_index(t_philo *philo)
{
	if (philo->id % 2)
		return (philo->id - 1);
	else
	{
		if (philo->id == philo->total_number)
			return (0);
		return (philo->id);
	}
}

/*
** note:	this function will decide if the second_fork_index based on the
**			philo_id, given that odd ids will be left_handed. so they will use
**			their right hand to reach the second fork.
**
** RETURN:	first_fork_index
*/

int	second_fork_index(t_philo *philo)
{
	if (philo->id % 2)
		return (philo->id);
	else
		return (philo->id - 1);
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

int philo_try_to_eat(t_philo *philo, int time, int id_first, int id_second)
{
	pthread_mutex_lock(&((philo->mutexes_on_forks)[id_first]));
	describe_state(philo, FORK, time);
	pthread_mutex_lock(&((philo->mutexes_on_forks)[id_second]));
	if (((time = get_timestamp(philo)) == -1) || time - philo->last_meal > philo->time_to_die)
	{
		pthread_mutex_unlock(&((philo->mutexes_on_forks)[id_second]));
		pthread_mutex_unlock(&((philo->mutexes_on_forks)[id_first]));
		return (time);
	}
	describe_state(philo, FORK, time);
	describe_state(philo, EAT, time);
	usleep(philo->time_to_eat);
	pthread_mutex_unlock(&((philo->mutexes_on_forks)[id_second]));
	pthread_mutex_unlock(&((philo->mutexes_on_forks)[id_first]));
	philo->last_meal = time;
	philo->meals_count++;
	time = get_timestamp(philo);
	describe_state(philo, SLEEP, time);
	usleep(philo->time_to_sleep);
	time = get_timestamp(philo);
	describe_state(philo, THINK, time);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_functions1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <charmstr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/30 15:16:04 by charmstr          #+#    #+#             */
/*   Updated: 2020/12/01 00:46:50 by charmstr         ###   ########.fr       */
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
** note:	We get into this function if the philosopher was not dead.
**			This function will try to take actions depending on the current
**			state of the philosopher.
**
** note:	if a philosopher dies, it sets the stops's value to one (common to
**			all	threads).
*/

void philo_take_actions(t_philo *philo, int elapsed_time)
{
	if (philo->state == THINK)
	{
		philo_try_to_eat(philo, elapsed_time);
		return ;
	}
	else if (elapsed_time > philo->time_to_eat + philo->time_to_sleep)
	{
		describe_state(philo, THINK, elapsed_time);
		philo->state = THINK;
		return ;
	}
	else if (philo->state != SLEEP && elapsed_time > philo->time_to_eat)
	{
		describe_state(philo, SLEEP, elapsed_time);
		philo->state = SLEEP;
		return ;
	}
}

/*
** note:	this function will decide if the first_fork_index based on the
**			philo_id, given that odd ids will be left_handed.
**
** RETURN:	first_fork_index
*/

int	first_fork_index_find(t_philo *philo)
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

int	second_fork_index_find(t_philo *philo)
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
*/

void philo_try_to_eat(t_philo *philo, int time)
{
	int first_fork_index;
	int second_fork_index;

	first_fork_index = first_fork_index_find(philo);
	second_fork_index = second_fork_index_find(philo);
	pthread_mutex_lock(&((philo->mutexes_on_forks)[first_fork_index]));
	describe_state(philo, FORK, time);
	pthread_mutex_lock(&((philo->mutexes_on_forks)[second_fork_index]));
	if ((time = get_elapsed_time(philo)) > philo->time_to_die)
	{
		describe_state(philo, DEAD, time);
		*(philo->stop) = 1;
		return ;
	}
	describe_state(philo, FORK, time);
	describe_state(philo, EAT, time);
	usleep(philo->time_to_eat * 1000);
	pthread_mutex_unlock(&((philo->mutexes_on_forks)[first_fork_index]));
	pthread_mutex_unlock(&((philo->mutexes_on_forks)[second_fork_index]));
	philo->state = EAT;
	philo->timeval_last_meal = philo->timeval_tmp;
	if (philo->meals_limit)
		philo->meals_count++;
}

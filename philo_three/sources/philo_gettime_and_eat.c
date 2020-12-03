/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_gettime_and_eat.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <charmstr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/30 15:16:04 by charmstr          #+#    #+#             */
/*   Updated: 2020/12/03 17:28:27 by charmstr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

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
** note:	this function will try to see if a philosopher can be seated and
**			eat. If it can be seated, the philosopher grabs two forks and eats,
**			it then realeases its forks and its 'seat' on the table.
*/

void			philo_try_to_grab_forks_and_eat(t_philo *philo)
{
	sem_wait(philo->sema_sit_down);
	sem_wait(philo->sema_forks);
	philo->time = get_elapsed_time(philo);
	philo->state = FORK;
	write_philo_state(philo);
	sem_wait(philo->sema_forks);
	philo->time = get_elapsed_time(philo);
	write_philo_state(philo);
	philo->timeval_last_meal = philo->timeval_tmp;
	philo->state = EAT;
	write_philo_state(philo);
	sem_post(philo->sema_sit_down);
	sem_post(philo->sema_forks);
	sem_post(philo->sema_forks);
	usleep(philo->time_to_eat);
}

void			philo_try_to_sleep_and_think(t_philo *philo)
{
	philo->time = get_elapsed_time(philo);
	philo->state = SLEEP;
	write_philo_state(philo);
	usleep(philo->time_to_sleep);
	philo->time = get_elapsed_time(philo);
	philo->state = THINK;
	write_philo_state(philo);
}

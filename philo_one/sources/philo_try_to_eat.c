/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_try_to_eat.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <charmstr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/30 15:16:04 by charmstr          #+#    #+#             */
/*   Updated: 2020/12/21 12:31:25 by charmstr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_one.h"

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
	wait_time(philo, philo->time_to_eat);
	pthread_mutex_unlock(philo->fork1);
	pthread_mutex_unlock(philo->fork2);
	if (philo->odd_number && philo->id % 2)
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
	wait_time(philo, philo->time_to_sleep);
	set_elapsed_time(philo);
	write_with_lock(philo, THINK);
}

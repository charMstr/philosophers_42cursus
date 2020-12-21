/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_try_to_eat.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <charmstr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/30 15:16:04 by charmstr          #+#    #+#             */
/*   Updated: 2020/12/21 13:05:00 by charmstr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

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
	sem_wait(philo->sema_sit);
	sem_wait(philo->sema_forks);
	set_elapsed_time(philo);
	write_philo_state(philo, FORK);
	sem_wait(philo->sema_forks);
	set_elapsed_time(philo);
	write_philo_state(philo, FORK);
	philo_update_last_meal_time(philo);
	sem_post(philo->sema_sit);
	write_philo_state(philo, EAT);
	wait_time(philo, philo->time_to_eat);
	sem_post(philo->sema_forks);
	sem_post(philo->sema_forks);
}

/*
** note:	the separation is made so that we can interrupt the printing on
**			stdout in case we are done with our meals.
*/

void	philo_try_to_sleep_and_think(t_philo *philo)
{
	set_elapsed_time(philo);
	write_philo_state(philo, SLEEP);
	wait_time(philo, philo->time_to_sleep);
	set_elapsed_time(philo);
	write_philo_state(philo, THINK);
}

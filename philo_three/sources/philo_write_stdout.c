/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_write_stdout.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <charmstr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/30 21:08:16 by charmstr          #+#    #+#             */
/*   Updated: 2020/12/07 05:34:49 by charmstr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

/*
** note:	This function will write the state of the philo to stdout. It uses
**			a semaphore to have access to stdout.
*/

void	write_philo_state(t_philo *philo, t_state state)
{
	unsigned int	len;

	len = philo_num_to_buff(philo->id, philo->state_buff, 0);
	len = philo_num_to_buff(philo->elapsed_time, philo->state_buff, len);
	philo_strrev(len, philo->state_buff);
	if (state == FORK)
		len += philo_strcpy_in_buffer(philo->state_buff, len,
				"has taken a fork\n");
	else if (state == EAT)
		len += philo_strcpy_in_buffer(philo->state_buff, len, "is eating\n");
	else if (state == SLEEP)
		len += philo_strcpy_in_buffer(philo->state_buff, len, "is sleeping\n");
	else
		len += philo_strcpy_in_buffer(philo->state_buff, len, "is thinking\n");
	sem_wait(philo->sema_speaker);
	write(1, philo->state_buff, len);
	sem_post(philo->sema_speaker);
}

/*
** note:	this function is designed to write that a philosopher is dead.
**
** note:	inside the function if a philosoper had already died, we dont use
**			stdout. otherwise we set the global variable to 0.
*/

void	write_dead_philo(t_philo *philo)
{
	unsigned int	len;

	len = philo_num_to_buff(philo->id, philo->death_buff, 0);
	len = philo_num_to_buff(philo->death_time, philo->death_buff, len);
	philo_strrev(len, philo->death_buff);
	len += philo_strcpy_in_buffer(philo->death_buff, len, "died\n");
	sem_wait(philo->sema_speaker);
	write(1, philo->death_buff, len);
}

/*
** note:	This function is designed to write that a philosopher has enough
**			food.
**
** note:	Inside the function if a philosoper had already died, we dont use
**			stdout.
*/

void	write_fed_up_philo(t_philo *philo)
{
	unsigned int	len;

	len = philo_num_to_buff(philo->id, philo->state_buff, 0);
	philo->state_buff[len++] = ' ';
	philo->state_buff[len++] = '[';
	philo_strrev(len, philo->state_buff);
	len += philo_strcpy_in_buffer(philo->state_buff, len, \
			"says no more 🍔!! ]\n");
	sem_wait(philo->sema_speaker);
	write(1, philo->state_buff, len);
	sem_post(philo->sema_speaker);
}

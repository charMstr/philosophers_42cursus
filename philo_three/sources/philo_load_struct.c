/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_load_struct.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <charmstr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 06:26:45 by charmstr          #+#    #+#             */
/*   Updated: 2020/12/02 08:44:24 by charmstr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

/*
** note:	this function will load the yielded values and creat semaphores.
**
** RETURN:	1 OK
**			0 KO
*/

int	philo_load_struct(t_parser_input *parser, t_philo *philo)
{
	philo->total_number = (unsigned int)parser->number_philo;
	philo->time_to_eat = (unsigned int)(parser->time_to_eat * 1000);
	philo->time_to_sleep = (unsigned int)(parser->time_to_sleep * 1000);
	philo->time_to_die = (unsigned int)parser->time_to_die;
	if (parser->total_meals_each != -1)
	{
		philo->meals_count = (unsigned int)parser->total_meals_each;
		philo->meals_limit = 1;
	}
	else
	{
		philo->meals_count = 1;
		philo->meals_limit = 0;
	}
	if (!philo_init_semaphores(philo, parser->number_philo))
		return (0);
	if (gettimeofday(&(philo->timeval_last_meal), NULL))
		return (0);
	return (1);
}

/*
** note:	this function will init the semaphores in our structure.
**
** RETURN:	1 OK
**			0 KO
*/

int	philo_init_semaphores(t_philo *philo, int number_philo)
{
	sem_t	*sema_forks;
	sem_t	*sema_sit_down;

	sema_forks = sem_open("semaph_philo_forks", O_CREAT, 0644, \
			number_philo);
	if (sema_forks == SEM_FAILED)
		return (0);
	sema_sit_down = sem_open("semaph_philo_sit_down", O_CREAT, 0644, \
			number_philo - 1);
	if (sema_sit_down == SEM_FAILED)
	{
		sem_unlink("semaph_philo_forks");
		return (0);
	}
	sem_unlink("semaph_philo_sit_down");
	sem_unlink("semaph_philo_forks");
	philo->sema_forks = sema_forks;
	philo->sema_sit_down = sema_sit_down;
	return (1);
}

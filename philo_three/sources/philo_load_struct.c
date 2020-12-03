/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_load_struct.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <charmstr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 06:26:45 by charmstr          #+#    #+#             */
/*   Updated: 2020/12/03 21:10:12 by charmstr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

/*
** note:	this function will load the yielded values and creat semaphores.
**
** RETURN:	1 OK
**			0 KO
*/

int		philo_load_struct(t_parser_input *parser, t_philo *philo)
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
	{
		philo_destroy_all_semaphores(philo);
		return (0);
	}
	return (1);
}

/*
** note:	this function will simply create the semaphors and place them in
**			the philo structure.
**
** note:	we have three sets of semaphores
**			- sema_forks: total number of forks.
**			- sema_sit_down: max number of philosophers that can sit down and
**				try to grab a pair of forks at the same time.
**			-sema_talk: the token for using stdout.
**
** RETURN:	1 OK
**			0 KO, some semaphore creation failed.
*/

int		philo_init_semaphores(t_philo *philo, int num)
{
	sem_t	*sem_forks;
	sem_t	*sem_sit;
	sem_t	*sem_talk;

	if ((sem_forks = sem_open("philo_fork", O_CREAT | O_EXCL, 0644, num)) \
			== SEM_FAILED)
		return (0);
	sem_unlink("philo_fork");
	if ((sem_sit = sem_open("philo_sit", O_CREAT | O_EXCL, 0644, num / 2)) \
			== SEM_FAILED)
		return (philo_destroy_sem(sem_forks));
	sem_unlink("philo_sit");
	if ((sem_talk = sem_open("philo_talk", O_CREAT | O_EXCL, 0644, 1)) \
			== SEM_FAILED)
	{
		sem_close(sem_forks);
		return (philo_destroy_sem(sem_forks));
	}
	sem_unlink("philo_talk");
	philo->sema_forks = sem_forks;
	philo->sema_talk = sem_talk;
	philo->sema_sit_down = sem_sit;
	return (1);
}

/*
** note:	this function will close all three semaphores created within our
**			philo_structure.
*/

void	philo_destroy_all_semaphores(t_philo *philo)
{
	sem_close(philo->sema_forks);
	sem_close(philo->sema_sit_down);
	sem_close(philo->sema_talk);
}

/*
** note:	42 norm function. just destroy a semaphore and returns 0.
*/

int		philo_destroy_sem(sem_t *destroy_me)
{
	sem_close(destroy_me);
	return (0);
}

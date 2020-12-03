/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_manage_semaphores.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <charmstr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/03 14:11:04 by charmstr          #+#    #+#             */
/*   Updated: 2020/12/03 15:50:59 by charmstr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

/*
** note:	this function will simply creat the semaphors and place them in
**			each philo structure.
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

int		philo_array_init_semaphores(t_philo **philo_array, int num)
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
	philo_array_init_semaphore_assist(philo_array, sem_forks, sem_sit, \
			sem_talk);
	return (1);
}

/*
** note:	42 norm function. just destroy a semaphore and returns 0.
*/

int		philo_destroy_sem(sem_t *destroy_me)
{
	sem_close(destroy_me);
	return (0);
}

/*
** note:	this function just places the created semaphores into each philo
**			structure
*/

void	philo_array_init_semaphore_assist(t_philo **philo_array, \
		sem_t *sem_forks, sem_t *sem_sit, sem_t *sem_talk)
{
	unsigned int	number_philo;
	unsigned int	i;

	i = 0;
	number_philo = (philo_array[0])->total_number;
	while (i < number_philo)
	{
		philo_array[i]->sema_forks = sem_forks;
		philo_array[i]->sema_talk = sem_talk;
		philo_array[i]->sema_sit_down = sem_sit;
		i++;
	}
}

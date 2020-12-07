/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_manage_semaphores.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <charmstr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/03 14:11:04 by charmstr          #+#    #+#             */
/*   Updated: 2020/12/07 05:35:33 by charmstr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

/*
** note:	this function will simply creat the semaphors and place them in
**			each philo structure.
**
** note:	we have four sets of semaphores
**			- sema_forks: total number of forks.
**			- sema_sit_down: max number of philosophers that can sit down and
**				try to grab a pair of forks at the same time.
**			- sema_talk: the token for using stdout.
**			- sema_touch_last_meal: to grant access to the last meal time stamp
**
** RETURN:	1 OK
**			0 KO, some semaphore creation failed.
*/

int	philo_init_semaphores(t_philo **philo_array, int number_philo)
{
	if (!philo_set_sema_forks(philo_array, number_philo))
		return (0);
	if (!philo_set_sema_speaker(philo_array, number_philo))
	{
		sem_close((philo_array[0])->sema_forks);
		return (0);
	}
	if (!philo_set_sema_sit(philo_array, number_philo))
	{
		sem_close((philo_array[0])->sema_speaker);
		sem_close((philo_array[0])->sema_forks);
		return (0);
	}
	if (!philo_set_sema_touch_last_meal(philo_array, number_philo))
	{
		sem_close((philo_array[0])->sema_speaker);
		sem_close((philo_array[0])->sema_sit);
		sem_close((philo_array[0])->sema_forks);
		return (0);
	}
	return (1);
}

/*
** note:	this function will creat and place in each philosopher the
**			semaphore for the number of forks
**
** return:	1 OK
**			O KO, sem_open failed
*/

int	philo_set_sema_forks(t_philo **philo_array, int number_philo)
{
	sem_t	*new;
	int		i;

	i = 0;
	if ((new = sem_open("philo_fork", O_CREAT | O_EXCL, 0644, number_philo)) \
			== SEM_FAILED)
		return (0);
	sem_unlink("philo_fork");
	while (i < number_philo)
	{
		(philo_array[i])->sema_forks = new;
		i++;
	}
	return (1);
}

/*
** note:	this function will creat and place in each philosopher the
**			semaphore for speaker, access granter to stdout
**
** return:	1 OK
**			O KO, sem_open failed
*/

int	philo_set_sema_speaker(t_philo **philo_array, int number_philo)
{
	sem_t	*new;
	int		i;

	i = 0;
	if ((new = sem_open("philo_speaker", O_CREAT | O_EXCL, 0644, 1)) \
			== SEM_FAILED)
		return (0);
	sem_unlink("philo_speaker");
	while (i < number_philo)
	{
		(philo_array[i])->sema_speaker = new;
		i++;
	}
	return (1);
}

/*
** note:	this function will creat and place in each philosopher the
**			semaphore for being sitted
**
** return:	1 OK
**			O KO, sem_open failed
*/

int	philo_set_sema_sit(t_philo **philo_array, int number_philo)
{
	sem_t	*new;
	int		i;

	i = 0;
	if ((new = sem_open("philo_sit_down", O_CREAT | O_EXCL, 0644, \
					number_philo / 2)) == SEM_FAILED)
		return (0);
	sem_unlink("philo_sit_down");
	while (i < number_philo)
	{
		(philo_array[i])->sema_sit = new;
		i++;
	}
	return (1);
}

/*
** note:	this function will creat and place in each philosopher the
**			semaphore for touching the last meal time. This semaphore is
**			specific to each philosopher's structure.
**
** return:	1 OK
**			O KO, sem_open failed
*/

int	philo_set_sema_touch_last_meal(t_philo **philo_array, int number_philo)
{
	sem_t	*new;
	int		i;
	char	buff[32];
	int		len;

	i = 0;
	while (i < number_philo)
	{
		len = philo_num_to_buff((philo_array[i])->id, buff, 0);
		philo_strrev(len, buff);
		len += philo_strcpy_in_buffer(buff, len, "philo_touch_meal");
		buff[len] = 0;
		if ((new = sem_open(buff, O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED)
		{
			philo_del_sema_touch_last_meal(philo_array, i - 1);
			return (0);
		}
		sem_unlink(buff);
		(philo_array[i])->sema_touch_last_meal = new;
		i++;
	}
	return (1);
}

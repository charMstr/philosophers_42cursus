/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_mutexes_set2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <charmstr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/06 15:58:04 by charmstr          #+#    #+#             */
/*   Updated: 2020/12/21 12:30:08 by charmstr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_one.h"

/*
** note:	this function will malloc and initialise a mutex for the speaker,
**			it will then place it in every philo struct.
** note:	Speaker is the mutex required to write to stdout.
**
** return:	1 OK
**			0, KO
*/

int		philo_init_mutex_speaker(int number_philo, t_philo **philo_array)
{
	int				i;
	pthread_mutex_t	*speaker;

	i = 0;
	if (!(speaker = malloc(sizeof(pthread_mutex_t))))
		return (0);
	if (pthread_mutex_init(speaker, NULL) != 0)
	{
		free(speaker);
		return (0);
	}
	while (i < number_philo)
	{
		(philo_array[i])->speaker = speaker;
		i++;
	}
	return (1);
}

/*
** note:	this function will, for EACH philosopher: malloc a mutex,
**			ititialize it, then place it in the philosopher's structure.
**
** return:	1 OK
**			0, KO, some malloc or phtread_mutex_init failed.
*/

int		philo_init_mutex_touch_last_meal(int number_philo, \
		t_philo **philo_array)
{
	int				i;
	pthread_mutex_t	*touch_last_meal;

	i = 0;
	while (i < number_philo)
	{
		if (!(touch_last_meal = malloc(sizeof(pthread_mutex_t))))
		{
			destroy_mutexes_touch_last_meal(philo_array, i - 1);
			return (0);
		}
		if (pthread_mutex_init(touch_last_meal, NULL) != 0)
		{
			free(touch_last_meal);
			destroy_mutexes_touch_last_meal(philo_array, i - 1);
			return (0);
		}
		(philo_array[i])->touch_last_meal = touch_last_meal;
		i++;
	}
	return (1);
}

/*
** note:	This fuction will loop over the philo_array structures backwards,
**			and destroy then free the touch_last_meal mutexes.
*/

void	destroy_mutexes_touch_last_meal(t_philo **philo_array, int index)
{
	while (index >= 0)
	{
		pthread_mutex_destroy((philo_array[index])->touch_last_meal);
		free((philo_array[index])->touch_last_meal);
		index--;
	}
}

/*
** note:	this function will pass over the mutexe array and destroy all the
**			the initialised ones, then it will free the array itself.
**
** inputs:	num: the number of mutexes initialised so far
**			mutexes_on_forks: an array of mutexes.
*/

void	destroy_mutexes_on_forks(pthread_mutex_t *mutexes_on_forks, int num)
{
	while (num >= 0)
	{
		pthread_mutex_destroy(&(mutexes_on_forks[num]));
		num--;
	}
	free(mutexes_on_forks);
}

/*
** note:	this functino will only be called when all the mutexes have
**			already been malloced/initialized successfully, then need to be
**			destroyed
*/

void	mutex_destroy_all(t_philo **philo_array, int number_philo, \
			pthread_mutex_t *mutexes_on_forks)
{
	pthread_mutex_destroy((philo_array[0])->speaker);
	free((philo_array[0])->speaker);
	destroy_mutexes_on_forks(mutexes_on_forks, number_philo - 1);
	destroy_mutexes_touch_last_meal(philo_array, number_philo - 1);
}

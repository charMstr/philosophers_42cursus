/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_array_mutexes_manage.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <charmstr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/30 13:58:35 by charmstr          #+#    #+#             */
/*   Updated: 2020/12/04 05:13:25 by charmstr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

/*
** note:	this function will initialise the mutex related values in our array
**			of philosophers structures.
**			ie: fork1: address of the mutex matching the first fork to grab.
**				fork2: address of the mutex matching the 2nd fork to grab.
**				speaker: the mutex needed to write to stdout.
**
** RETURN:	1 OK
**			0 KO when creating/initialising the speaker
*/

int				philo_array_init_mutexes(t_philo **philo_array, \
		int number_philo, pthread_mutex_t *speaker)
{
	pthread_mutex_t	*mutexes_on_forks;

	if (!(mutexes_on_forks = philo_init_mutexes_on_forks(number_philo)))
		return (0);
	if (pthread_mutex_init(speaker, NULL))
	{
		destroy_mutexes_on_forks(mutexes_on_forks, number_philo - 1);
		return (0);
	}
	philo_array_place_mutex(philo_array, number_philo, mutexes_on_forks, \
		speaker);
	return (1);
}

/*
** note:	This function will place correctly the mutexes we created in our
**			array of philosophers's structures.
** note:	We keep the mutexes_on_forks pointer in the structure for
**			convenience, but we wont use it until we destroy each mutex it
**			contains and free it. We directly place in each philosoper a
**			reference to the mutex to its left and righ.
*/

void			philo_array_place_mutex(t_philo **philo_array, \
		int number_philo, pthread_mutex_t *mutexes_on_forks, \
		pthread_mutex_t *speaker)
{
	int				i;
	int				first_fork_index;
	int				second_fork_index;

	i = 0;
	while (i < number_philo)
	{
		(philo_array[i])->mutexes_on_forks = mutexes_on_forks;
		(philo_array[i])->speaker = speaker;
		first_fork_index = set_fork_index(i + 1, number_philo, 1);
		(philo_array[i])->fork1 = &(mutexes_on_forks[first_fork_index]);
		second_fork_index = set_fork_index(i + 1, number_philo, 2);
		(philo_array[i])->fork2 = &(mutexes_on_forks[second_fork_index]);
		i++;
	}
}

/*
** note:	this function will creat an array of mutexes according to the
**			number of philosophers. Each mutex represents a fork.
**
** RETURN:	NULL KO
**			pointer to an array containing mutexes that are initialized.
*/

pthread_mutex_t	*philo_init_mutexes_on_forks(int number_philo)
{
	pthread_mutex_t	*mutexes_on_forks;
	int				i;

	i = 0;
	if (!(mutexes_on_forks = malloc(sizeof(pthread_mutex_t) * number_philo)))
		return (NULL);
	while (i < number_philo)
	{
		if (pthread_mutex_init(&(mutexes_on_forks[i]), NULL) != 0)
		{
			destroy_mutexes_on_forks(mutexes_on_forks, i - 1);
			return (NULL);
		}
		i++;
	}
	return (mutexes_on_forks);
}

/*
** note:	this function will pass over the mutexe array and destroy all the
**			the initialised ones, then it will free the array itself.
**
** inputs:	num: the number of mutexes initialised so far
**			mutexes_on_forks: an array of mutexes.
*/

void			destroy_mutexes_on_forks(pthread_mutex_t *mutexes_on_forks, \
		int num)
{
	while (num >= 0)
	{
		pthread_mutex_destroy(&(mutexes_on_forks[num]));
		num--;
	}
	free(mutexes_on_forks);
}

/*
** note:	this function will help in setting the index of the first and
**			second fork a philosopher will grab.
**
** note:	the last param, tell us either if we are setting the first (==1) or
**			the second (==2) fork's index.
**
** RETURN:	the index of the fork a philosopher should grab (its the first fork
**			or the second fork depending on 'which').
*/

int				set_fork_index(int id, int total_number, int which)
{
	if (which == 1)
	{
		if (id % 2)
			return (id - 1);
		else
		{
			if (id == total_number)
				return (0);
			return (id);
		}
	}
	else
	{
		if (id % 2)
			return (id);
		else
			return (id - 1);
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_start_threads.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <charmstr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/30 14:28:44 by charmstr          #+#    #+#             */
/*   Updated: 2020/12/01 22:58:43 by charmstr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"
#include <stdio.h>

/*
** note:	This function will launch our threads.
**
** note:	another way to wait our threads would have been to explicitely use
**			pthread_exit() in our main function. But we cannot, so we will
**			have to use the pthread_join() functions to wait for our threads to
**			terminate. last argument is NULL because we do not care about
**			fetching the result of each thread.
**
** note:	If a philosopher dies in a thread, the common int pointer stop will
**			be dereferenced and set to one. all philosophers will know they
**			gotta die.
*/

void start_and_join_threads(unsigned int number_philo, pthread_t *pthreads_array, t_philo **philo_array)
{
	unsigned int		i;

	i = 0;
	while (i < number_philo)
	{
		pthread_create(&pthreads_array[i], NULL, start_philo, (void*)philo_array[i]);
		i++;
	}
	i = 0;
	while (i < number_philo)
	{
		pthread_join(pthreads_array[i], NULL);
		i++;
	}
}

/*
** note:	This function will initialise each t_writer structure in the array.
**			Keep in mind that each structure contains a buffer that will be
**			dedicated to a specific line to print on screen (first fork taken,
**			second, is eating, is sleeping, ...). Each line we will be writen
**			in a thread, which will be detached, therefore the arrays need to
**			be existant at all time (cannot be deleted when a function
**			finishes) and need to be acompagnied by the	mutex_on_mic.
*/

void	init_array_writers(t_writer (*array_writers)[], t_philo *philo)
{
	unsigned int i;

	i = 0;
	while (i < 6)
	{
		(*array_writers)[i].mutex_on_mic = philo->mutex_on_mic;
		i++;
	}
}

/*
** note:	this function will be called for each philosopher.
*/

void	*start_philo(void *philo_void)
{
	t_philo *philo;
	unsigned int time;
	t_writer array_writers[6];

	philo = (t_philo *)philo_void;
	init_array_writers(&array_writers, philo);
	while (*(philo->stop) == 0)
	{
		if (!philo->meals_count)
			break;
		philo_try_to_eat1(philo, &array_writers);
		if ((time = get_elapsed_time(philo)) > philo->time_to_die)
		{
			pthread_mutex_unlock(&((philo->mutexes_on_forks)[philo->fork1]));
			pthread_mutex_unlock(&((philo->mutexes_on_forks)[philo->fork2]));
			describe_state(DEAD, time, &(array_writers[5]));
			*(philo->stop) = 1;
			return (NULL);
		}
		philo_try_to_eat2(philo, time, &array_writers);
	}
	return (NULL);
}

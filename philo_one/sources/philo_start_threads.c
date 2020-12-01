/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_start_threads.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <charmstr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/30 14:28:44 by charmstr          #+#    #+#             */
/*   Updated: 2020/12/01 17:12:08 by charmstr         ###   ########.fr       */
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

void start_and_join_threads(int number_philo, pthread_t *pthreads_array, t_philo **philo_array)
{
	int		i;

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
** note:	this function will be called for each philosopher.
*/

void	*start_philo(void *philo_void)
{
	t_philo *philo;
	int time;

	philo = (t_philo *)philo_void;
	while (*(philo->stop) == 0)
	{
		if (philo->meals_limit && (philo->meals_count >= philo->meals_target))
			break;
		if (((time = get_elapsed_time(philo)) == -1) \
				|| time > philo->time_to_die)
		{
			describe_state(philo, DEAD, time);
			*(philo->stop) = 1;
			return (NULL);
		}
		if ((time = philo_try_to_eat(philo, time)))
		{
			describe_state(philo, DEAD, time);
			*(philo->stop) = 1;
			return (NULL);
		}
	}
	return (NULL);
}

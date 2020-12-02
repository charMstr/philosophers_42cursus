/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_start_threads.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <charmstr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/30 14:28:44 by charmstr          #+#    #+#             */
/*   Updated: 2020/12/02 08:34:34 by charmstr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

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

void	start_and_join_threads(unsigned int number_philo, \
		pthread_t *pthreads_array, t_philo **philo_array)
{
	unsigned int		i;

	i = 0;
	while (i < number_philo)
	{
		pthread_create(&pthreads_array[i], NULL, start_philo, \
				(void*)philo_array[i]);
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
**
** note:	try_to_eat1 tries to grab the two forks.
**			then we check if the philosopher is still alive.
**			then we can start to eat(try to eat).
**
** note:	the very last usleep when exiting the function is because we want
**			to wait for the writes (in a detached thread) to be done withe our
**			t_writer's elements, especially the buffer it uses in the write
**			syscall.
*/

void	*start_philo(void *philo_void)
{
	t_philo *philo;

	philo = (t_philo *)philo_void;
	while (*(philo->stop) == 0)
	{
		if (!philo->meals_count)
			break ;
		philo_try_to_grab_forks(philo);
		if ((philo->time = get_elapsed_time(philo)) > philo->time_to_die)
		{
			pthread_mutex_unlock(&((philo->mutexes_on_forks)[philo->fork1]));
			pthread_mutex_unlock(&((philo->mutexes_on_forks)[philo->fork2]));
			philo->state = DEAD;
			write_without_lock(philo);
			*(philo->stop) = 1;
			return (NULL);
		}
		philo_starts_to_eat(philo);
	}
	return (NULL);
}

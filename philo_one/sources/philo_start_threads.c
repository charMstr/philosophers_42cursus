/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_start_threads.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <charmstr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/30 14:28:44 by charmstr          #+#    #+#             */
/*   Updated: 2020/12/04 05:15:37 by charmstr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

/*
** note:	This function will launch our threads.
**			we do not care about the id of the thread since we will be using a
**			very global mutex to wait until a philo died in our Main.
**
** note:	other ways to wait our threads would have been to explicitely use
**			pthread_exit() in our main function. But we cannot, or through the
**			use of the pthread_join() functions to wait for our threads to
**			terminate. last argument is NULL because we do not care about
**			fetching the result of each thread.
**
** note:	we launch first the philosophers that are even.
*/

void	start_and_join_threads(unsigned int number_philo, \
		pthread_t *pthreads_array, t_philo **philo_array)
{
	unsigned int	i;
	pthread_t		thread;

	i = 1;
	while (i < number_philo)
	{
		pthread_create(&thread, NULL, start_philo, (void*)philo_array[i]);
		pthread_create(&pthreads_array[i], NULL, polling_philo, \
				(void*)philo_array[i]);
		i += 2;
	}
	i = 0;
	while (i < number_philo)
	{
		pthread_create(&thread, NULL, start_philo, (void*)philo_array[i]);
		pthread_create(&pthreads_array[i], NULL, polling_philo, \
				(void*)philo_array[i]);
		i += 2;
	}
	i = -1;
	while (++i < number_philo)
		pthread_join(pthreads_array[i], NULL);
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
	if (!philo->meals_count)
		return (NULL);
	while (*(philo->total_number))
	{
		philo_try_to_grab_forks_and_eat(philo);
		if (philo->meals_limit)
			(philo->meals_count)--;
		if (philo->meals_count == 0)
			return (NULL);
		philo_try_to_sleep_and_think(philo);
	}
	return (NULL);
}

/*
** note:	This function will be polling for the life of the philosophers it
**			is attached to, they use the exact same structure.
**			If a philosopher is dead, then we keep the write mutex in an locked
**			state, and we unlock the break_free mutex, so that the Main can
**			free memory and quit for us.
*/

void	*polling_philo(void *philo_void)
{
	t_philo *philo;

	philo = (t_philo *)philo_void;
	while (1)
	{
		usleep(500);
		if (!philo->meals_count)
		{
			write_fed_up_philo(philo);
			return (NULL);
		}
		if ((philo->time_poll = get_elapsed_time(philo)) > philo->time_to_die)
		{
			write_dead_philo(philo);
			return (NULL);
		}
	}
	return (NULL);
}

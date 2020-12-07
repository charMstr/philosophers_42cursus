/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_start_threads.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <charmstr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/30 14:28:44 by charmstr          #+#    #+#             */
/*   Updated: 2020/12/07 05:01:43 by charmstr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

/*
** note:	This function will launch our threads.
**
** note:	the array of pthread contains twice as much fields as there is
**			philosophers: because for each philosopher, we laucnch two threads
**			one for the philo and one for polling on his death.
**
** note:	we have to join both versions of the threads in the philo,
**			otherwise if we detach one of them, when freeing memory in the main
**			there is always a risk a thread would be using that exact same
**			memory, justg good practice an in case someone puts 0 in some
**			program inputs. (ex time_to_sleep and time_to_think = 0.)
**
** note:	another way to wait our threads would have been to explicitely use
**			pthread_exit() in our main function. But we cannot, so we will
**			have to use the pthread_join() functions to wait for our threads to
**			terminate. last argument is NULL because we do not care about
**			fetching the result of each thread.
**
** return:	1 OK
**			0 KO, some thread_creation failed
*/

void	start_and_join_threads(t_philo **philo_array, \
		unsigned int number_philo, pthread_t *pthreads_array)
{
	unsigned int	j;
	unsigned int	index_philo;

	j = 0;
	index_philo = 0;
	while (index_philo < number_philo)
	{
		pthread_create(&pthreads_array[j++], NULL, life, \
				(void*)philo_array[index_philo]);
		pthread_create(&pthreads_array[j++], NULL, monitor, \
				(void*)philo_array[index_philo]);
		index_philo++;
	}
	j = 0;
	while (j < number_philo * 2)
	{
		pthread_join(pthreads_array[j], NULL);
		j++;
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

void	*life(void *philo_void)
{
	t_philo *philo;

	philo = (t_philo *)philo_void;
	if (!philo->meals_count)
		return (NULL);
	while (!*(philo->stop))
	{
		philo_try_to_grab_forks_and_eat(philo);
		if (philo->meals_limit)
			--(philo->meals_count);
		if (!(philo->meals_count))
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

void	*monitor(void *philo_void)
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
		sem_wait(philo->sema_touch_last_meal);
		if (!philo_check_last_meal_time(philo))
		{
			write_dead_philo(philo);
			sem_post(philo->sema_touch_last_meal);
			return (NULL);
		}
		sem_post(philo->sema_touch_last_meal);
	}
	return (NULL);
}

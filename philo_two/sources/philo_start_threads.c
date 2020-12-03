/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_start_threads.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <charmstr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/30 14:28:44 by charmstr          #+#    #+#             */
/*   Updated: 2020/12/03 21:39:41 by charmstr         ###   ########.fr       */
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
** note:	we have to join bothe versions of the threads in the philo,
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
** note:	If a philosopher dies in a thread, the common int pointer stop will
**			be dereferenced and set to one. all philosophers will know:
**			- they cannot write anymore
**			- they gotta return.
*/

void	start_and_join_threads(unsigned int number_philo, \
		pthread_t *pthreads_array, t_philo **philo_array)
{
	unsigned int	index_pthreads;
	unsigned int	index_philo;

	index_pthreads = 0;
	index_philo = 0;
	while (index_philo < number_philo)
	{
		pthread_create(&pthreads_array[index_pthreads], NULL, start_philo, \
				(void*)philo_array[index_philo]);
		pthread_create(&pthreads_array[index_pthreads + 1], NULL, \
				polling_philo, (void*)philo_array[index_philo]);
		index_pthreads += 2;
		index_philo++;
	}
	index_pthreads = 0;
	while (index_pthreads < number_philo * 2)
	{
		pthread_join(pthreads_array[index_pthreads], NULL);
		pthread_join(pthreads_array[index_pthreads + 1], NULL);
		index_pthreads += 2;
	}
}

/*
** note:	this function will be called for each philosopher.
**
** note:	If the number of meals was set and is reached, we then Return.
**			Also we Return if another philosopher died. known from the stop
**			field in the structure (pointer common to all philosophers.)
*/

void	*start_philo(void *philo_void)
{
	t_philo			*philo;

	philo = (t_philo *)philo_void;
	if (!philo->meals_count)
		return (NULL);
	while (*(philo->stop) == 0)
	{
		philo_try_to_grab_forks_and_eat(philo);
		if (philo->meals_limit)
			philo->meals_count--;
		if (!philo->meals_count)
			break ;
		philo_try_to_sleep_and_think(philo);
	}
	return (NULL);
}

/*
** note:	This function will be launched in parallel for each philo and check
**			on its death. If the elapsed time since last meal is bigger than
**			philo->time_to_die, the philo is dead, and we advertised through
**			a shared memory: a pointer on stop.
**
** note:	If the count of meals for a philosopher is down to zero, then we
**			know this thread is of no more use, we can Return.
*/

void	*polling_philo(void *philo_void)
{
	t_philo			*philo;

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

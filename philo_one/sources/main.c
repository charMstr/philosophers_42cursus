/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <charmstr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/29 21:01:32 by charmstr          #+#    #+#             */
/*   Updated: 2020/12/07 01:59:43 by charmstr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

/*
** note:	in this philosopher version there is as many forks as there is
**			philosopher, and each fork is placed between two philosopher.
**			Therefor each philosopher can grab only two fork, to its left and
**			to its right. We will start one thread per philosopher and we use
**			mutexes not semaphores.
**
** specs:	- there is a mutex per fork (subject imposed it), and each mutex
**			has an index in a table.
**			- when a philosopher wants to eat it need to grab two forks, it has
**			to lock the mutex for the fork virtually to its left and to its
**			right.
**			- the philosophers are started in a certain order (odds IDs first),
**			some of them are "right handed" others "left handed", and this vary
**			during the execution for optimisation.
**			- each time we write on stdout we use a mutex: speaker.
**			- each time we modify/check the time_last meal, we use a mutex:
**				touch_last_meal.
**			- there is two threads for each philosopher: one for the philo
**			doing its life: trying to grab forks, eat, sleep, think, and one
**			for monitoring on the death of the philosopher.
**			- if the last parameter is given as input: number_of_meals_each
**			then when a philosopher is done eating we stop its threads, and
**			decrease nb_philo_alive, common pointer.
**			- if a philospher dies: the total nb_philo_alive is set to 0, so
**			that all the other ones cannot write no more then stop their own
**			threads.
**			- the two threads per philosophers use the exact same structure, so
**			memory swap are avoided to the maximum. Therefore, before the main
**			can finally free allocated memory and quit, we make sure we joined
**			all	the threads, avoiding using some memory that already got freed
**			in threads left behind.
*/

int		main(int argc, char **argv)
{
	t_philo			**philo_array;
	pthread_t		*pthread_array;
	t_parser_input	parser;

	if (!philo_parser_root(&parser, argc, argv))
		return (EXIT_FAILURE);
	if (!(pthread_array = malloc(sizeof(pthread_t) * parser.number_philo * 2)))
		return (EXIT_FAILURE);
	if (!(philo_array = philo_array_init(&parser, parser.number_philo)))
	{
		free(pthread_array);
		return (EXIT_FAILURE);
	}
	if (!(set_mem_protections_and_thread(philo_array, parser.number_philo, \
			pthread_array)))
	{
		philo_array_destroy(philo_array, parser.number_philo);
		free(pthread_array);
		return (EXIT_FAILURE);
	}
	philo_array_destroy(philo_array, parser.number_philo);
	free(pthread_array);
	return (EXIT_SUCCESS);
}

/*
** note:	This function will take care of seting the memory protections for
**			accessing same memory in different threads. Then it will set in all
**			the philosphers the same start time. Then it will start our threads
**
** RETURN:	0, KO, some malloc or gettimeofday or pthread_create func failed.
**			1 O
*/

int		set_mem_protections_and_thread(t_philo **philo_array, \
		int number_philo, pthread_t *pthread_array)
{
	pthread_mutex_t	*mutexes_on_forks;
	int				i;

	i = 0;
	if (!philo_mutexes_set(philo_array, number_philo, &mutexes_on_forks))
		return (0);
	if (!philo_set_start_time(philo_array, number_philo))
	{
		mutex_destroy_all(philo_array, number_philo, mutexes_on_forks);
		return (0);
	}
	if (!start_threads(philo_array, number_philo, pthread_array))
	{
		mutex_destroy_all(philo_array, number_philo, mutexes_on_forks);
		return (0);
	}
	while (i < number_philo * 2)
	{
		pthread_join(pthread_array[i], NULL);
		i++;
	}
	mutex_destroy_all(philo_array, number_philo, mutexes_on_forks);
	return (1);
}

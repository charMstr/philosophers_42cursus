/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <charmstr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/29 21:01:32 by charmstr          #+#    #+#             */
/*   Updated: 2020/12/07 04:24:17 by charmstr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

/*
** note:	in this philosopher version all the forks are in the middle of the
**			table without any index. All we know is that we have one fork
**			philosopher. Also we start a thread per philosopher, and we use
**			semaphores, not mutexes.
**
** specs:	- there is a semaphore per fork (subject imposed it).
**			- when a philosopher wants to eat it need to check another
**			semaphore value that allows him to seat or not. no more than half
**			of the philosophers can seat at the same time, therefore they can
**			directly grab two forks and eat.
**			- each time we write on stdout we use a semopher: sema_talk.
**			- there is two threads for each philosopher: one for the philo
**			doing its life: trying to grab forks, eat, sleep, think, and one
**			for pollling on the death of the philosopher.
**			- if the last parameter is given as input: number_of_meals_each
**			then when a philosopher is done eating we stop its threads.
**			- if a philospher dies: a boolean common to all philosophers is
**			set, sot that all the other ones cannot write no more then stop
**			their own threads.
**			- the two threads per philosophers use the exact same structure, so
**			memory swap are avoided to the maximum. Therefore before the main
**			can finally free allocated mem and quit, we make sure we joined all
**			the threads, avoiding using some memory that already got freed in
**			threads left behind.
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
	int				i;

	i = 0;
	if (!philo_init_semaphores(philo_array, number_philo))
		return (0);
	if (!philo_set_start_time(philo_array, number_philo))
	{
		semaphores_close_all(philo_array, number_philo);
		return (0);
	}
	start_and_join_threads(philo_array, number_philo, pthread_array);
	semaphores_close_all(philo_array, number_philo);
	return (1);
}

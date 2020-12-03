/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <charmstr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/29 21:01:32 by charmstr          #+#    #+#             */
/*   Updated: 2020/12/03 21:39:04 by charmstr         ###   ########.fr       */
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

int	main(int argc, char **argv)
{
	t_philo			**philo_array;
	pthread_t		*pthreads_array;
	t_parser_input	parser;
	unsigned int	stop;

	stop = 0;
	if (!philo_parser_root(&parser, argc, argv))
		return (EXIT_FAILURE);
	if (!(pthreads_array = malloc(sizeof(pthread_t) * parser.number_philo * 2)))
		return (EXIT_FAILURE);
	if (!(philo_array = philo_array_init_root(&parser, parser.number_philo, \
					&stop)))
	{
		free(pthreads_array);
		return (EXIT_FAILURE);
	}
	start_and_join_threads(parser.number_philo, pthreads_array, philo_array);
	philo_array_destroy(philo_array, parser.number_philo);
	free(pthreads_array);
	return (EXIT_SUCCESS);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <charmstr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/29 21:01:32 by charmstr          #+#    #+#             */
/*   Updated: 2020/12/04 13:43:43 by charmstr         ###   ########.fr       */
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
**			- I alternated left handed and right handed philosophers so that
**			two adjacent philosophers compete for the first same fork.
**			- each time we write on stdout we use a mutex: speaker.
**			- there is two threads for each philosopher: one for the philo
**			doing its life: trying to grab forks, eat, sleep, think, and one
**			for pollling on the death of the philosopher.
**			- if the last parameter is given as input: number_of_meals_each
**			then when a philosopher is done eating we stop its threads.
**			- if a philospher dies: a boolean common to all philosophers is
**			set, so that all the other ones cannot write no more then stop
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
	pthread_t		*pthread_array;
	t_parser_input	parser;
	unsigned int	total_number;
	pthread_mutex_t	speaker;

	if (!philo_parser_root(&parser, argc, argv))
		return (EXIT_FAILURE);
	if (!(pthread_array = malloc(sizeof(pthread_t) * parser.number_philo * 2)))
		return (EXIT_FAILURE);
	if (!(philo_array = philo_array_init_root(&parser, parser.number_philo, \
					&total_number, &speaker)))
	{
		free(pthread_array);
		return (EXIT_FAILURE);
	}
	start_and_join_threads(parser.number_philo, pthread_array, philo_array);
	philo_array_destroy(philo_array, parser.number_philo, DEL_MUTEX);
	free(pthread_array);
	return (EXIT_SUCCESS);
}

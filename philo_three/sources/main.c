/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <charmstr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/29 21:01:32 by charmstr          #+#    #+#             */
/*   Updated: 2020/12/07 05:38:00 by charmstr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

/*
** note:	in this philosopher version all the forks are in the middle of the
**			table without any index. All we know is that we have one fork
**			philosopher, and we fork a new process for each philosopher id.
**			we use semaphores.
**
** specs:	- there is a semaphore per fork (subject imposed it).
**			- when a philosopher wants to eat it need to check another
**			semaphore value that allows him to seat or not. no more than half
**			of the philosophers can seat at the same time, therefore they can
**			directly grab two forks and eat.
**			- each time we write on stdout we use a semopher: sema_talk.
**			- there is two threads for each philosopher process: one for the
**			philo doing its life: trying to grab forks, eat, sleep, think, and
**			one for pollling on the death of the philosopher.
**			- if a philospher dies: the Main will kill all the other processes.
**			then it should close the semaphores we opened (good practice only).
*/

int		main(int argc, char **argv)
{
	t_philo			**philo_array;
	t_parser_input	parser;

	if (!philo_parser_root(&parser, argc, argv))
		return (EXIT_FAILURE);
	if (!(philo_array = philo_array_init(&parser, parser.number_philo)))
		return (EXIT_FAILURE);
	if (!philo_init_semaphores(philo_array, parser.number_philo))
	{
		philo_array_destroy(philo_array, parser.number_philo);
		return (EXIT_FAILURE);
	}
	if (!philo_set_start_time(philo_array, parser.number_philo))
	{
		semaphores_close_all(philo_array, parser.number_philo);
		philo_array_destroy(philo_array, parser.number_philo);
		return (EXIT_FAILURE);
	}
	philo_fork_and_start_processes(parser.number_philo, philo_array);
	semaphores_close_all(philo_array, parser.number_philo);
	philo_array_destroy(philo_array, parser.number_philo);
	return (EXIT_SUCCESS);
}

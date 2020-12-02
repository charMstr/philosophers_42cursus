/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <charmstr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/29 21:01:32 by charmstr          #+#    #+#             */
/*   Updated: 2020/12/02 03:45:58 by charmstr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

int	main(int argc, char **argv)
{
	t_philo			**philo_array;
	pthread_t		*pthreads_array;
	t_parser_input	parser;
	unsigned int	stop;

	stop = 0;
	if (!philo_parser_root(&parser, argc, argv))
		return (1);
	if (!(pthreads_array = malloc(sizeof(pthread_t) * parser.number_philo)))
		return (1);
	if (!(philo_array = philo_array_init_root(&parser, parser.number_philo, \
					&stop)))
	{
		free(pthreads_array);
		return (1);
	}
	start_and_join_threads(parser.number_philo, pthreads_array, philo_array);
	philo_array_destroy(philo_array, parser.number_philo, DEL_MUTEXES);
	free(pthreads_array);
	return (0);
}

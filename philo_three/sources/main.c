/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <charmstr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/29 21:01:32 by charmstr          #+#    #+#             */
/*   Updated: 2020/12/02 07:35:16 by charmstr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

int	main(int argc, char **argv)
{
	t_philo			philo;
	t_parser_input	parser;
	int				res;

	if (!philo_parser_root(&parser, argc, argv))
		return (EXIT_FAILURE);
	if (!philo_load_struct(&parser, &philo))
		return (EXIT_FAILURE);
	res = philo_fork_and_start(parser.number_philo, philo);
	return (res);
}

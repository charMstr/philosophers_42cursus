/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_parser_root.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <charmstr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/29 21:00:06 by charmstr          #+#    #+#             */
/*   Updated: 2020/12/02 07:14:56 by charmstr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

/*
** note:	this the root function for parsing, it will first check the correct
**			input has been provided. then it will fill in a structure with the
**			parameters. Bad inputs calls the usage function.
**
** RETURN:	1 OK inputs.
**			0 KO inputs.
*/

int philo_parser_root(t_parser_input *parser, int argc, char *argv[])
{
	if (!philo_parser_check_input(argc, argv))
	{
		display_usage(argv[0]);
		return (0);
	}
	philo_parser_get_input(parser, argc, argv);
	return (1);
}

/*
** display the usage at the start of the program if inputs are wrong
*/

void	display_usage(char *argv0)
{
	ft_putstr("usage: ");
	ft_putstr(argv0);
	ft_putstr(" + \n  1 - number_of_philosopher\n"\
			"  2 - time_to_die\n"\
			"  3 - time_to_eat\n"\
			"  4 - time_to_sleep\n"\
			"[ 5 ] - number_of_time_each_philosophers_must_eat\n");
}

/*
** note:	this function will parse the input against correct number of
**			arguments, and dumb inputs like negative numbers.
**
** RETURN:	0, failure
**			1, OK
*/

int	philo_parser_check_input(int argc, char **argv)
{
	int i;

	i = 1;
	if (argc < 5 || argc > 6)
	{
		ft_putstr("error: bad number of arguments\n");
		return (0);
	}
	while (i < argc)
	{
		if (!str_isdigit(argv[i]))
		{
			ft_putstr("warning: only put numeric characters in parameters!\n");
			return (0);
		}
		i++;
	}
	if (ft_atoi(argv[1]) < 2)
	{
		ft_putstr("warning: cannot have less than two philosophers!\n");
		return (0);
	}
	return (1);
}

/*
** note:	this function will fill the stucture that collects the input of the
**			program.
**
** note:	if there is no sixth parameter, total_meals_each is set to -1
*/

void	philo_parser_get_input(t_parser_input *parser, int argc, char **argv)
{
	parser->number_philo = ft_atoi(argv[1]);
	parser->time_to_die = ft_atoi(argv[2]);
	parser->time_to_eat = ft_atoi(argv[3]);
	parser->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		parser->total_meals_each = ft_atoi(argv[5]);
	else
		parser->total_meals_each = -1;
}

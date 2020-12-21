/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_array_init_root.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <charmstr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/29 21:35:52 by charmstr          #+#    #+#             */
/*   Updated: 2020/12/21 12:27:09 by charmstr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_one.h"

/*
** note:	This function is the basis for building an array of structures,
**			each representing a philosopher, whith a unique id.
**
** note:	A common variable representing the total number of philosophers
**			alive is made common to all philosophers structures.
**
** note:	At the very end when everything is ready we will come back and
**			finally put the	same time_stamp in all the philosophers structures.
**
** INPUTS:	parser: the structure containing the valid user inputs
**			number_philo: the total number of philo struct we will create
**
** RETURN:	NULL,				KO, malloc failed
**			array of pointers,	OK
*/

t_philo	**philo_array_init(t_parser_input *parser, int number_philo)
{
	t_philo			**philo_array;
	unsigned int	*nb_philo_alive;
	int				i;

	i = 0;
	if (!(nb_philo_alive = malloc(sizeof(unsigned int))))
		return (NULL);
	*nb_philo_alive = parser->number_philo;
	if (!(philo_array = malloc(sizeof(t_philo*) * number_philo)))
	{
		free(nb_philo_alive);
		return (NULL);
	}
	while (i < number_philo)
	{
		if (!(philo_array[i] = philo_struct_init(parser, i + 1, \
						nb_philo_alive)))
			return (philo_array_destroy(philo_array, i));
		i++;
	}
	return (philo_array);
}

/*
** note:	this funciton will malloc a philosopher struct and sets its fields
**			to the correct value. The timeval_last_meal will be set in a
**			subsequent loop at the very end.
**
** note:	philo->forks is set to null so that it will be set later.
**
** note:	we multiply time_to[eat/sleep] by a 1000, so we dont have to do it
**			later.
**
** RETURN:	NULL,				KO
**			pointer to t_philo *
*/

t_philo	*philo_struct_init(t_parser_input *parser, int id, \
		unsigned int *nb_philo_alive)
{
	t_philo *philo;

	if (!(philo = malloc(sizeof(t_philo))))
		return (NULL);
	philo->nb_philo_alive = nb_philo_alive;
	philo->odd_number = (*nb_philo_alive % 2) ? 1 : 0;
	*(philo->nb_philo_alive) = (unsigned int)parser->number_philo;
	philo->id = (unsigned int)id;
	philo->time_to_eat = (unsigned int)(parser->time_to_eat * 1000);
	philo->time_to_sleep = (unsigned int)(parser->time_to_sleep * 1000);
	philo->time_to_die = (unsigned int)parser->time_to_die;
	if (parser->total_meals_each != -1)
	{
		philo->meals_count = (unsigned int)parser->total_meals_each;
		philo->meals_limit = 1;
	}
	else
	{
		philo->meals_count = 1;
		philo->meals_limit = 0;
	}
	return (philo);
}

/*
** note:	this function will be used to destroy an array of t_philo struct
**			pointers, and we destroy the common pointer to the nb_philo_alive
**			if we had at least one philo structure malloced.
**
** RETURN: NULL, always
*/

void	*philo_array_destroy(t_philo **philo_array, int size)
{
	if (size > 0)
		free((philo_array[0])->nb_philo_alive);
	while (--size >= 0)
	{
		free(philo_array[size]);
	}
	free(philo_array);
	return (NULL);
}

/*
** note:	this function will do a last pass and put in each philosophers the
**			same timestamp. Therefore we will be ready to start the thread
**			functions with the earliest one.
**
** inputs:	number_philo:	the number of philosophers
**			philo_array:	array of structures containing philosophers.
**
** RETURN:	0, KO: gettimeofday failed
**			1, OK
*/

int		philo_set_start_time(t_philo **philo_array, int number_philo)
{
	struct timeval	time_val;
	int				i;

	i = 0;
	if (gettimeofday(&time_val, NULL))
		return (0);
	while (i < number_philo)
	{
		(philo_array[i])->timeval_last_meal = time_val;
		i++;
	}
	return (1);
}

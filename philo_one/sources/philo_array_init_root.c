/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_array_init_root.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <charmstr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/29 21:35:52 by charmstr          #+#    #+#             */
/*   Updated: 2020/12/04 05:25:25 by charmstr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

/*
** note:	this function is the basis for building an array of structures,
**			each representing a philosopher. malloc is used.
**			Each philo structure is malloced in a subfunction, and given a
**			specific id.
**
** note:	when it fails the whole array is deleted, but wether we created/
**			initialised mutexes or not we will destroy them.
**
** note:	at the very end when all the malloc is done, we finally put the
**			same time_stamp in all the philosophers structures.
**
** INPUTS:	parser: the structure containing the valid user inputs
**			number_philo: the total number of philo struct we will create
**			total_number: a pointer to a stack variable from the main func.
**				it will help us decide when all philosophers are done eating
**				or when a philosopher is dead.
**
** RETURN:	NULL,				KO, malloc or gettimeofday or mutex_init failed
**			array of pointers,	OK
*/

t_philo	**philo_array_init_root(t_parser_input *parser, int number_philo, \
		unsigned int *total_number, pthread_mutex_t *speaker)
{
	t_philo			**philo_array;

	if (!(philo_array = philo_array_init(parser, total_number)))
		return (NULL);
	if (!(philo_array_init_mutexes(philo_array, number_philo, speaker)))
	{
		return (philo_array_destroy(philo_array, parser->number_philo, \
			NODEL_MUTEX));
	}
	if (!philo_array_set_time(philo_array, parser->number_philo))
	{
		return (philo_array_destroy(philo_array, parser->number_philo, \
			DEL_MUTEX));
	}
	return (philo_array);
}

/*
** note:	this function will init the array of philo structures, and the
**			unsigned int* that store a common value for the total number of
**			philosophers as well.
**
** RETURN:	pointer to an array of t_philo* structures.
**			NULL KO
*/

t_philo	**philo_array_init(t_parser_input *parser, unsigned int *total_number)
{
	t_philo	**philo_array;
	int		i;

	i = 0;
	if (!(philo_array = malloc(sizeof(t_philo*) * parser->number_philo)))
		return (NULL);
	while (i < parser->number_philo)
	{
		if (!(philo_array[i] = philo_struct_init(parser, i + 1, total_number)))
			return (philo_array_destroy(philo_array, i, NODEL_MUTEX));
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

t_philo	*philo_struct_init(t_parser_input *parser, int id, unsigned int *total)
{
	t_philo *philo;

	if (!(philo = malloc(sizeof(t_philo))))
		return (NULL);
	philo->total_number = total;
	*(philo->total_number) = (unsigned int)parser->number_philo;
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
**			pointers.
**
** RETURN: NULL, always
*/

void	*philo_array_destroy(t_philo **array, int size, int del_mutexes)
{
	if (size > 0 && del_mutexes)
	{
		destroy_mutexes_on_forks((array[0])->mutexes_on_forks, size - 1);
		pthread_mutex_destroy((array[0])->speaker);
	}
	while (--size >= 0)
	{
		free(array[size]);
	}
	free(array);
	return (NULL);
}

/*
** note:	this function will do a last pass and put in each philosophers the
**			same timestamp. Therefore we will be ready to start the thread
**			functions with the earliest one.
**
** inputs:	total_philo:	the number of philosophers
**			philo_array:	array of structures containing philosophers.
**
** RETURN:	0, KO: gettimeofday failed
**			1, OK
*/

int		philo_array_set_time(t_philo **philo_array, int total_philo)
{
	struct timeval	time_val;
	int				i;

	i = 0;
	if (gettimeofday(&time_val, NULL))
		return (0);
	while (i < total_philo)
	{
		philo_array[i]->timeval_last_meal = time_val;
		i++;
	}
	return (1);
}

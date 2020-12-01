/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_array_init_root.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <charmstr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/29 21:35:52 by charmstr          #+#    #+#             */
/*   Updated: 2020/12/02 00:46:42 by charmstr         ###   ########.fr       */
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
**			stop: a pointer to a stack variable from the main func. a boolean.
**
** RETURN:	NULL,				KO, malloc or gettimeofday or mutex_init failed
**			array of pointers,	OK
*/

t_philo **philo_array_init_root(t_parser_input *parser, int number_philo, \
		unsigned int *stop)
{
	int		i;
	t_philo	**philo_array;
	sem_t	*semaphor;

	i = 0;
	if (!(philo_array = malloc(sizeof(t_philo*) * number_philo)))
		return (NULL);
	while (i < number_philo)
	{
		if (!(philo_array[i] = philo_struct_init(parser, i + 1, stop)))
			return (philo_array_destroy(philo_array, i, NODEL_SEMAPH));
		i++;
	}
	if (!(philo_array_init_semaphore(philo_array, number_philo))
		return (philo_array_destroy(philo_array, number_philo, NODEL_SEMAPH));
	if (!philo_array_set_time(philo_array, number_philo))
		return (philo_array_destroy(philo_array, number_philo, DEL_SEMAPH));
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

t_philo *philo_struct_init(t_parser_input *parser, int id, unsigned int *stop)
{
	t_philo *philo;

	if (!(philo = malloc(sizeof(t_philo))))
		return (NULL);
	philo->semaphor = NULL;
	philo->total_number = (unsigned int)parser->number_philo;
	philo->id = (unsigned int)id;
	philo->stop = stop;
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
** note:	this function will simply creat the semaphor and place it in each
**			philo structure.
**
** RETURN:	1 OK
**			0 KO
*/

int philo_array_init_semaphore(t_philo **philo_array, int number_philo)
{
	sem_t	*semaphore;
	int		i;

	i = 0;
	semaphore = sem_open("semaph_philo", O_CREAT | O_EXCL, 0644, number_philo);
	if (semaphore == SEM_FAILED)
		return (0);
	while (i < number_philo)
	{
		philo_array[i]->semaphor = semaphore;
		i++;
	}
	return (1);
}

/*
** note:	this function will be used to destroy an array of t_philo struct
**			pointers.
**
** note:	this function can be called at different stages while creating the
**			array, setting up the mutexes, and setting up the start time.
**			Therefore the mutexes might not be created/initialized yet.
**
** note:	if mutexes were created: inside the very first t_philo struct
**			pointer, we free the common	data only once: the array of
**			initialized mutexes for forks, and the mic mutex.
**
** RETURN: NULL, always
*/

void *philo_array_destroy(t_philo **array, int size, int semaphor_created_yet)
{
	if (size > 0 && semaphor_created_yet)
	{
		sem_close(array[0]->semaphor);
		sem_unlink("semaph_philo");
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

int philo_array_set_time(t_philo **philo_array, int total_philo)
{
	struct	timeval time_val;
	int		i;

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

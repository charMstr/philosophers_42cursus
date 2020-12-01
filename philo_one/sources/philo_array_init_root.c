/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_array_init_root.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <charmstr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/29 21:35:52 by charmstr          #+#    #+#             */
/*   Updated: 2020/12/01 01:13:49 by charmstr         ###   ########.fr       */
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
		int *stop, pthread_mutex_t *mutex_on_mic)
{
	int		i;
	t_philo	**philo_array;

	i = 0;
	if (!(philo_array = malloc(sizeof(t_philo*) * number_philo)))
		return (NULL);
	while (i < number_philo)
	{
		if (!(philo_array[i] = philo_struct_init(parser, i + 1, stop)))
			return (philo_array_destroy(philo_array, i, NODEL_MUTEXES));
		i++;
	}
	if (!(philo_array_init_mutexes(philo_array, number_philo, mutex_on_mic)))
		return (philo_array_destroy(philo_array, number_philo, NODEL_MUTEXES));
	if (!philo_array_set_time(philo_array, number_philo))
		return (philo_array_destroy(philo_array, number_philo, DEL_MUTEXES));
	return (philo_array);
}

/*
** note:	this funciton will malloc a philosopher struct and sets its fields
**			to the correct value. The timeval_last_meal will be set in a
**			subsequent loop at the very end.
**
** note:	philo->forks is set to null so that it will be set later.
**
** RETURN:	NULL,				KO
**			pointer to t_philo *
*/

t_philo *philo_struct_init(t_parser_input *parser, int id, int *stop)
{
	t_philo *philo;

	if (!(philo = malloc(sizeof(t_philo))))
		return (NULL);
	philo->mutexes_on_forks = NULL;
	philo->total_number = parser->number_philo;
	philo->id = id;
	philo_itoa_set_buff(id, philo->itoa_id);
	philo->stop = stop;
	philo->time_to_eat = parser->time_to_eat;
	philo->time_to_sleep = parser->time_to_sleep;
	philo->time_to_die = parser->time_to_die;
	philo->meals_count = 0;
	philo->meals_done = 0;
	philo->meals_target = parser->total_meals_each;
	if (parser->total_meals_each != -1)
		philo->meals_limit = 1;
	else
		philo->meals_limit = 0;
	return (philo);
}

/*
** note:	this function will fill the buff string.
**
**			It pre-adds a space at the end for us for the future concatenation
**			of strings.
**
** RETURN:	the length of the itoa we just wrote.
*/

int philo_itoa_set_buff(int num, char buff[])
{
	int i;
	int j;
	int len;

	i = 0;
	j = 0;
	if (num == 0)
	{
		buff[i] = 48;
		len = 1;
	}
	else
	{
		while (num > 0)
		{
			buff[i] = 48 + num % 10;
			num = num / 10;
			i++;
		}
		len = i;
	}
	while (--i > j)
	{
		buff[i] ^= buff[j];
		buff[j] ^= buff[i];
		buff[i] ^= buff[j];
		j++;
	}
	buff[len] = ' ';
	buff[len + 1] = '\0';
	return (len + 1);
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

void *philo_array_destroy(t_philo **array, int size, int mutexes_created_yet)
{
	if (size > 0 && mutexes_created_yet)
	{
		destroy_and_free_mutexes_on_forks(array[0]->mutexes_on_forks, size);
		pthread_mutex_destroy(array[0]->mutex_on_mic);
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

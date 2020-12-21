/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_mutexes_set1.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <charmstr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/06 15:24:25 by charmstr          #+#    #+#             */
/*   Updated: 2020/12/21 12:29:28 by charmstr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_one.h"

/*
** note:	This function will take care of setting all the mutexes:
**			- speaker: needed by all threads to access the stdout.
**			- mutexes_on_forks: a common array representing the forks, note
**			that the philosophers wont take the array with them in their
**			structure but instead directly two indexes from that array
**			representing their respective left and right forks.
**			- set_last_meal: a mutex that is specific one philosopher.
**			It will make sure its different thread dont access his last meal
**			time at the same time.
**
** inputs:	1) philo_array: the array containing each philo struct
**			2) number_philo: the size of the array
**			3) mutexes_on_forks: a double pointer that we will updated for the
**			calling	function.
**
** return:	0 KO, some malloc or mutex_create failed.
**			1 OK.
*/

int				philo_mutexes_set(t_philo **philo_array, int number_philo, \
		pthread_mutex_t **mutexes_on_forks)
{
	if (!(*mutexes_on_forks = philo_init_mutexes_on_forks(number_philo)))
		return (0);
	philo_set_fork1_and_fork2(number_philo, philo_array, *mutexes_on_forks);
	if (!philo_init_mutex_speaker(number_philo, philo_array))
	{
		destroy_mutexes_on_forks(*mutexes_on_forks, number_philo - 1);
		return (0);
	}
	if (!(philo_init_mutex_touch_last_meal(number_philo, philo_array)))
	{
		pthread_mutex_destroy((philo_array[0])->speaker);
		free((philo_array[0])->speaker);
		destroy_mutexes_on_forks(*mutexes_on_forks, number_philo - 1);
		return (0);
	}
	return (1);
}

/*
** note:	this function will create an array of mutexes according to the
**			number of philosophers. Each mutex represents a fork.
**
** RETURN:	NULL KO
**			pointer to an array containing mutexes that are initialized.
*/

pthread_mutex_t	*philo_init_mutexes_on_forks(int number_philo)
{
	pthread_mutex_t	*mutexes_on_forks;
	int				i;

	i = 0;
	if (!(mutexes_on_forks = malloc(sizeof(pthread_mutex_t) * number_philo)))
		return (NULL);
	while (i < number_philo)
	{
		if (pthread_mutex_init(&(mutexes_on_forks[i]), NULL) != 0)
		{
			destroy_mutexes_on_forks(mutexes_on_forks, --i);
			return (NULL);
		}
		i++;
	}
	return (mutexes_on_forks);
}

/*
** note:	This function will place in each philosopher's structure the
**			address of two mutexes, both representing a fork.
**			Those pointers all come	from the same array, so that a pointer to
**			a mutex will be shared by two adjacent philosophers.
**
** note:	The first while will ditstibute the forks in the case we have an
** 			odd number of philosophers.
** 			The second while will distribute the forks in a different way, for
** 			a different algorithm.
*/

void			philo_set_fork1_and_fork2(int number_philo, \
		t_philo **philo_array, pthread_mutex_t *mutexes_on_forks)
{
	int				i;
	int				first_fork_index;
	int				second_fork_index;

	i = 0;
	while (number_philo % 2 && i < number_philo)
	{
		first_fork_index = set_fork_index_odd_case(i + 1, number_philo, 1);
		(philo_array[i])->fork1 = &(mutexes_on_forks[first_fork_index]);
		second_fork_index = set_fork_index_odd_case(i + 1, number_philo, 2);
		(philo_array[i])->fork2 = &(mutexes_on_forks[second_fork_index]);
		i++;
	}
	while (!(number_philo % 2) && i < number_philo)
	{
		first_fork_index = set_fork_index_even_case(i + 1, number_philo, 1);
		(philo_array[i])->fork1 = &(mutexes_on_forks[first_fork_index]);
		second_fork_index = set_fork_index_even_case(i + 1, number_philo, 2);
		(philo_array[i])->fork2 = &(mutexes_on_forks[second_fork_index]);
		i++;
	}
}

/*
** note:	This setup up is designed for an odd number of philosophers.
**			The very first philosopher is left handed, while all the other ones
**			are right handed. Then during the program, each time a philosopher
**			with an odd number finishes eating, it will swap from right handed
**			to left handed. This is reflected in the code by swaping the
**			addresses contained in fork1 and fork2 each time a philosopher with
**			an odd number is done eating.
**
** return:	the index of fork1 or fork2, depending on the last parameter, and
**			upon the fact that we have an odd or even number of philosophers.
*/

int				set_fork_index_odd_case(int id, int total_number, int which)
{
	if (which == FIRST_FORK)
	{
		if (id == total_number)
			return (0);
		if (id == 1)
			return (0);
		return (id);
	}
	else
	{
		if (id == 1)
			return (id);
		return (id - 1);
	}
}

/*
** note:	This setup up is designed for an even number of philosophers. The
** 			very first philosopher is left handed, then his neighbor is right
** 			handed and so on.
**
** return:	the index of fork1 or fork2, depending on the last parameter, and
**			upon the fact that we have an odd or even number of philosophers.
*/

int				set_fork_index_even_case(int id, int total_number, int which)
{
	if (which == FIRST_FORK)
	{
		if (id % 2)
			return (id - 1);
		else
		{
			if (id == total_number)
				return (0);
			return (id);
		}
	}
	else
	{
		if (id % 2)
			return (id);
		else
			return (id - 1);
	}
}

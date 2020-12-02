/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_array_mutexes_manage.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <charmstr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/30 13:58:35 by charmstr          #+#    #+#             */
/*   Updated: 2020/12/02 03:35:31 by charmstr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

/*
** note:	this function will make sure we create the appropriate mutexes and
**			place them in each philosopher structure.
**			We create a mutex for each fork (all in an array: locks_on_forks).
**
** RETURN:	1 OK
**			0 KO. some malloc or pthread_mutex_init failed.
*/

int philo_array_init_mutexes(t_philo **philo_array, int number_philo)
{
	pthread_mutex_t *mutexes_on_forks;
	int i;

	i = 0;
	if (!(mutexes_on_forks = create_mutexes_on_forks_array(number_philo)))
		return (0);
	while (i < number_philo)
	{
		philo_array[i]->mutexes_on_forks = mutexes_on_forks;
		i++;
	}
	return (1);
}

/*
** note:	this function will creat the locks required for each forks, in the
**			form of an array of mutexes.
**
** note:	The array of mutexes is malloced so we only need to pass its
**			pointer to all the different philosophers.
**			But the mutex_on_mic is declared in the main, so we use its address
**
** RETURN:	NULL KO, mutex initialisation or malloc failed.
**			array of mutexes OK.
*/

pthread_mutex_t	*create_mutexes_on_forks_array(int number_philo)
{
	pthread_mutex_t *mutexes_on_forks;
	int i;

	i = 0;
	if (!(mutexes_on_forks = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t) * number_philo)))
		return (NULL);
	while (i < number_philo)
	{
		if (pthread_mutex_init(&(mutexes_on_forks[i]), NULL) != 0)
		{
			destroy_and_free_mutexes_on_forks(mutexes_on_forks, i - 1);
			return (NULL);
		}
		i++;
	}
	return (mutexes_on_forks);
}

/*
** note:	this function will pass over the mutexe array and destroy all the
**			the initialised ones, then it will free the array itself.
**
** inputs:	num: the number of mutexes initialised so far
**			mutexes_on_forks: an array of mutexes.
*/

void destroy_and_free_mutexes_on_forks(pthread_mutex_t *mutexes_on_forks, \
		int num)
{
	while (num >= 0)
	{
		pthread_mutex_destroy(&(mutexes_on_forks[num]));
		num--;
	}
	free(mutexes_on_forks);
}

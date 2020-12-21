/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_start_threads.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <charmstr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/30 14:28:44 by charmstr          #+#    #+#             */
/*   Updated: 2020/12/21 11:45:43 by charmstr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_one.h"

/*
** note:	This function will create our different threads for each
**			philosophers:
**			1) life: just representing the philosopher's life
**			2) monitor: checking on the philosopher's death.
**
** note:	creating for each philosopher, then joining for all threads just to
**			be sure we don't try to access memory that is being freed in the
**			Main thread when we exit the program.
**
** return:	1 OK
**			0 KO, some thread_creation failed
*/

int		start_threads(t_philo **phi_array, int number_philo, \
		pthread_t *pth_array)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < number_philo)
	{
		pthread_create(&pth_array[j++], NULL, life, (void*)phi_array[i]);
		pthread_create(&pth_array[j++], NULL, monitor, (void*)phi_array[i]);
		i += 2;
	}
	usleep(1000);
	i = 1;
	while (i < number_philo)
	{
		pthread_create(&pth_array[j++], NULL, life, (void*)phi_array[i]);
		pthread_create(&pth_array[j++], NULL, monitor, (void*)phi_array[i]);
		i += 2;
	}
	return (1);
}

/*
** note:	This function will be called for each philosopher, when the total
** 			number of philosophers is odd.
**
** note:	try_to_eat1 tries to grab the two forks.
**			then we check if the philosopher is still alive.
**			then we can start to eat(try to eat).
**
** note:	the very last usleep when exiting the function is because we want
**			to wait for the writes (in a detached thread) to be done withe our
**			t_writer's elements, especially the buffer it uses in the write
**			syscall.
*/

void	*life(void *philo_void)
{
	t_philo *philo;

	philo = (t_philo *)philo_void;
	if (!philo->meals_count)
		return (NULL);
	while (*(philo->nb_philo_alive))
	{
		philo_try_to_grab_forks_and_eat(philo);
		if (philo->meals_limit)
			--(philo->meals_count);
		if (!(philo->meals_count))
			return (NULL);
		philo_try_to_sleep_and_think(philo);
	}
	return (NULL);
}

/*
** note:	This function will be polling for the life of the philosophers it
**			is attached to, they use the exact same structure.
**			If a philosopher is dead, then we keep the write mutex in an locked
**			state, and we unlock the break_free mutex, so that the Main can
**			free memory and quit for us.
*/

void	*monitor(void *philo_void)
{
	t_philo *philo;

	philo = (t_philo *)philo_void;
	while (1)
	{
		usleep(100);
		if (!philo->meals_count)
		{
			write_fed_up_philo(philo);
			return (NULL);
		}
		pthread_mutex_lock(philo->touch_last_meal);
		if (!philo_check_last_meal_time(philo))
		{
			write_dead_philo(philo);
			pthread_mutex_unlock(philo->touch_last_meal);
			return (NULL);
		}
		pthread_mutex_unlock(philo->touch_last_meal);
	}
	return (NULL);
}

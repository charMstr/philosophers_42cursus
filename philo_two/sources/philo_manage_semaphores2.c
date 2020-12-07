/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_manage_semaphores2.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <charmstr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/07 03:14:47 by charmstr          #+#    #+#             */
/*   Updated: 2020/12/07 03:30:43 by charmstr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

/*
**	note:	This function can be called when the initialization fucked up half
**			way or at the end when closing properly our semaphores.
*/

void	philo_del_sema_touch_last_meal(t_philo **philo_array, int index)
{
	while (index >= 0)
	{
		sem_close((philo_array[index])->sema_touch_last_meal);
		index--;
	}
}

/*
** note:	this function will be called only once all the semaphores have
**			been succefully build, and we finally need to close then all so
**			that we can quit properly the program.
*/

void	semaphores_close_all(t_philo **philo_array, int number_philo)
{
	sem_close((philo_array[0])->sema_speaker);
	sem_close((philo_array[0])->sema_sit);
	sem_close((philo_array[0])->sema_forks);
	philo_del_sema_touch_last_meal(philo_array, number_philo - 1);
}

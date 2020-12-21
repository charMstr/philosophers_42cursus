/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_get_time.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <charmstr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/21 12:12:39 by charmstr          #+#    #+#             */
/*   Updated: 2020/12/21 12:28:05 by charmstr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_one.h"

void	wait_time(t_philo *philo, unsigned int x)
{
	unsigned int elapsed_time;

	while (1)
	{
		usleep(100);
		gettimeofday(&philo->timeval_tmp, NULL);
		if (philo->timeval_last_meal.tv_sec == philo->timeval_tmp.tv_sec)
		{
			elapsed_time = (philo->timeval_tmp.tv_usec \
					- philo->timeval_last_meal.tv_usec);
		}
		else
		{
			elapsed_time = ((philo->timeval_tmp.tv_sec \
				- philo->timeval_last_meal.tv_sec - 1) * 1000 + (1000000 \
				- philo->timeval_last_meal.tv_usec \
				+ philo->timeval_tmp.tv_usec));
		}
		if (elapsed_time > x)
			break ;
	}
}

/*
** note:	This function returns the time elapsed since the last meal.
**			It takes care of the case when we go from one second to another.
*/

void	set_elapsed_time(t_philo *philo)
{
	gettimeofday(&philo->timeval_tmp, NULL);
	if (philo->timeval_last_meal.tv_sec == philo->timeval_tmp.tv_sec)
	{
		philo->elapsed_time = ((philo->timeval_tmp.tv_usec - \
					philo->timeval_last_meal.tv_usec) / 1000);
	}
	else
	{
		philo->elapsed_time = ((philo->timeval_tmp.tv_sec \
			- philo->timeval_last_meal.tv_sec - 1) * 1000 + (1000000 \
			- philo->timeval_last_meal.tv_usec + philo->timeval_tmp.tv_usec) \
			/ 1000);
	}
}

/*
** note:	This function will update the philosoher's last meal time, and set
**			the elapsed time to zero.
*/

void	philo_update_last_meal_time(t_philo *philo)
{
	pthread_mutex_lock(philo->touch_last_meal);
	gettimeofday(&philo->timeval_last_meal, NULL);
	pthread_mutex_unlock(philo->touch_last_meal);
	philo->elapsed_time = 0;
}

/*
** note:	This function will retrieve the last meal_time in the philosopher
**			structure, and update the elapsed time to zero.
**
** return:	1 OK
**			0 KO, the philosopher died before eating.
*/

int		philo_check_last_meal_time(t_philo *philo)
{
	struct timeval tmp;

	gettimeofday(&tmp, NULL);
	if (philo->timeval_last_meal.tv_sec == tmp.tv_sec)
		philo->death_time = ((tmp.tv_usec - philo->timeval_last_meal.tv_usec) \
				/ 1000);
	else
	{
		philo->death_time = ((tmp.tv_sec - philo->timeval_last_meal.tv_sec - \
			1) * 1000 + (1000000 - philo->timeval_last_meal.tv_usec + \
				tmp.tv_usec) / 1000);
	}
	if (philo->death_time > philo->time_to_die)
		return (0);
	return (1);
}

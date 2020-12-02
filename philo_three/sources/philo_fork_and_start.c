/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_fork_and_start.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <charmstr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 06:54:19 by charmstr          #+#    #+#             */
/*   Updated: 2020/12/02 08:45:00 by charmstr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

/*
** note:	This function will fork for each philosopher.
**
** note:	The returned value from a process does not matter, if one returns
**			it means a philosopher died, we then kill all the other childs.
**
** RETURN:	It does not really matter, yet we keep track of an eventual failure
**			from fork function.
*/

int		philo_fork_and_start(int number_philo, t_philo philo)
{
	int		i;
	pid_t	pid;

	i = 0;
	while (i < number_philo)
	{
		if (-1 == (pid = fork()))
		{
			if (i)
				kill(0, SIGKILL);
			return (EXIT_FAILURE);
		}
		if (!pid)
		{
			philo.id = i;
			start_philo(philo);
			return (EXIT_SUCCESS);
		}
		i++;
	}
	waitpid(-1, NULL, WUNTRACED);
	kill(0, SIGKILL);
	return (EXIT_SUCCESS);
}

/*
** note:	this function will be called for each philosopher.
**
** note:	first we try to grab the two forks.
**			then we check if the philosopher is still alive.
**			then we can start to eat.
*/

void	start_philo(t_philo philo)
{
	while (1)
	{
		if (!philo.meals_count)
			break ;
		philo_try_to_grab_forks(&philo);
		if ((philo.time = get_elapsed_time(&philo)) > philo.time_to_die)
		{
			sem_post(philo.sema_sit_down);
			sem_post(philo.sema_forks);
			sem_post(philo.sema_forks);
			philo.state = DEAD;
			write_without_lock(&philo);
			exit(1);
		}
		philo_starts_to_eat(&philo);
	}
	exit(1);
}

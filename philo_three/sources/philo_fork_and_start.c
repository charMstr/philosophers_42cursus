/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_fork_and_start.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <charmstr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 06:54:19 by charmstr          #+#    #+#             */
/*   Updated: 2020/12/07 05:39:09 by charmstr         ###   ########.fr       */
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

int		philo_fork_and_start_processes(int number_philo, t_philo **philo_array)
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
			philo_start_process(*(philo_array[i]));
			return (EXIT_SUCCESS);
		}
		i++;
	}
	philo_wait_for_pids(number_philo);
	return (EXIT_SUCCESS);
}

/*
** note:	this function will wait for the child processes to Return. They all
**			start two threads and one of them is waited for with the
**			pthread_join() func. This specific thread (philo_polling) is either
**			exiting with PHILO_DEAD or PHILO_DONE_EATING status.
**
**			Depending on the exit status we keep on waiting for another pid to
**			terminate or we kill all of the remaining ones straight away.
*/

void	philo_wait_for_pids(int number_philo)
{
	int philo_done_eating;
	int	status;

	philo_done_eating = 0;
	while (waitpid(-1, &status, 0))
	{
		if (WIFEXITED(status))
		{
			if (WEXITSTATUS(status) == PHILO_DEAD)
				kill(0, SIGINT);
			else if ((WEXITSTATUS(status) == PHILO_DONE_EATING))
				philo_done_eating++;
		}
		if (philo_done_eating == number_philo)
			break ;
	}
}

/*
** note:	this function is called when a new process is started. it is given
**			a copy of a philo structure, in which we can access the semaphores'
**			addresses.
**
** note:	Thoses processes will be killed if one of all of them returned in
**			the parent process (using wait_pid()) the specific exit status
**			PHILO_DEAD, or if all processes used the exit status
**			PHILO_DONE_EATING (using wait_pid()) as well.
**
** note:	each process should start two threads, one for a philosopher, and
**			the other one will be polling for its dead.
** 			We join the second one so that we can wait for it to exit.
*/

void	philo_start_process(t_philo philo)
{
	pthread_t	philo_thread;
	pthread_t	polling_thread;

	pthread_create(&philo_thread, NULL, life, (void*)&philo);
	pthread_create(&polling_thread, NULL, monitor, (void*)&philo);
	pthread_detach(philo_thread);
	pthread_join(polling_thread, NULL);
}

/*
** note:	this function will be called for each philosopher.
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
	while (1)
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
		usleep(500);
		if (!philo->meals_count)
		{
			write_fed_up_philo(philo);
			exit(PHILO_DONE_EATING);
		}
		sem_wait(philo->sema_touch_last_meal);
		if (!philo_check_last_meal_time(philo))
		{
			write_dead_philo(philo);
			sem_post(philo->sema_touch_last_meal);
			exit(PHILO_DEAD);
		}
		sem_post(philo->sema_touch_last_meal);
	}
	return (NULL);
}

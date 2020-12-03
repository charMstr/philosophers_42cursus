/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_fork_and_start.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <charmstr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 06:54:19 by charmstr          #+#    #+#             */
/*   Updated: 2020/12/03 21:11:31 by charmstr         ###   ########.fr       */
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

int		philo_fork_and_start_processes(int number_philo, t_philo philo)
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
			philo.id = i + 1;
			philo_start_process(philo);
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

	pthread_create(&philo_thread, NULL, start_philo, (void*)&philo);
	pthread_create(&polling_thread, NULL, polling_philo, (void*)&philo);
	pthread_detach(philo_thread);
	pthread_join(polling_thread, NULL);
}

/*
** note:	this function will be called for each philosopher.
**
** note:	If the number of meals was set and is reached, we then Return.
**			Also we Return if another philosopher died. known from the stop
**			field in the structure (pointer common to all philosophers.)
*/

void	*start_philo(void *philo_void)
{
	t_philo	*philo;

	philo = (t_philo *)philo_void;
	if (!philo->meals_count)
		return (NULL);
	while (1)
	{
		philo_try_to_grab_forks_and_eat(philo);
		if (philo->meals_limit)
			philo->meals_count--;
		if (!philo->meals_count)
			return (NULL);
		philo_try_to_sleep_and_think(philo);
	}
	return (NULL);
}

/*
** note:	This function will be launched in parallel for each philo and check
**			on its death. If the elapsed time since last meal is bigger than
**			philo->time_to_die, the philo is dead. we will display a message
**			and never release the semaphore required to use stdout, then exit
**			a specific message.
**
** note:	If the count of meals for a philosopher is down to zero, then we
**			know this thread is of no more use, we can exit a specific status.
*/

void	*polling_philo(void *philo_void)
{
	t_philo			*philo;

	philo = (t_philo *)philo_void;
	while (1)
	{
		usleep(500);
		if (!philo->meals_count)
		{
			write_stop_philo(philo, PHILO_DONE_EATING);
			exit(PHILO_DONE_EATING);
		}
		if ((philo->time_poll = get_elapsed_time(philo)) > philo->time_to_die)
		{
			write_stop_philo(philo, PHILO_DEAD);
			exit(PHILO_DEAD);
		}
	}
	return (NULL);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_three.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <charmstr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 00:49:51 by charmstr          #+#    #+#             */
/*   Updated: 2020/12/04 05:34:59 by charmstr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_THREE_H
# define PHILO_THREE_H

# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>
# include <pthread.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <semaphore.h>
# include <signal.h>

# define PHILO_DEAD 42
# define PHILO_DONE_EATING 43

typedef struct	s_parser_input
{
	int	number_philo;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	total_meals_each;
}				t_parser_input;

typedef enum	e_state
{
	FORK, EAT, SLEEP, THINK, DEAD
}				t_state;

/*
**	total_number: the total number of philosophers.
**	id:	philosopher_id
**	time_to_eat: the time it takes to eat.
**	time_to_sleep: the time it takes to sleep.
**	time_to_die: the time it takes to die since the last meal's start.
**
**	meals_limit: a boolean, if true, we need to count meals
**	meals_count: counter that will decrement until zero.
**
** sem_forks: semaphore on the number of forks
** sem_sit_down: there is total_numbers_of_philo - 1 seats so that we avoid
**		dead locks.
**
**	timeval_last_meal: contains the timestamp of last meal
**	timeval_tmp: the gettimeofday struct. always access timeval_tmp.tv_usec
*/

typedef struct	s_philo
{
	char			state_buff[32];
	char			death_buff[32];
	unsigned int	total_number;
	unsigned int	id;
	unsigned int	time_to_eat;
	unsigned int	time_to_sleep;
	unsigned int	time_to_die;
	unsigned int	meals_limit;
	unsigned int	meals_count;
	unsigned int	time;
	unsigned int	time_poll;
	t_state			state;
	sem_t			*sema_forks;
	sem_t			*sema_sit_down;
	sem_t			*sema_talk;
	struct timeval	timeval_last_meal;
	struct timeval	timeval_tmp;
}				t_philo;

int				ft_atoi(const char *str);
size_t			ft_strlen(char *str);
void			ft_putstr(char *str);
int				ft_isdigit(int c);
int				str_isdigit(char *str);

int				philo_parser_root(t_parser_input *parser, int argc, \
		char *argv[]);
void			display_usage(char *argv0);
int				philo_parser_check_input(int argc, char **argv);
void			philo_parser_get_input(t_parser_input *parser, int argc, \
		char **argv);

int				philo_load_struct(t_parser_input *parser, t_philo *philo);
int				philo_init_semaphores(t_philo *philo, int number_philo);
void			philo_destroy_all_semaphores(t_philo *philo);
int				philo_destroy_sem(sem_t *destroy_me);

int				philo_fork_and_start_processes(int number_philo, \
		t_philo philo);
void			philo_start_process(t_philo philo);
void			*start_philo(void *philo_void);
void			*polling_philo(void *philo_void);
void			philo_wait_for_pids(int number_philo);

unsigned int	get_elapsed_time(t_philo *philo);
void			philo_try_to_sleep_and_think(t_philo *philo);
void			philo_try_to_grab_forks_and_eat(t_philo *philo);

void			write_stop_philo(t_philo *philo, int full_or_dead);
void			write_philo_state(t_philo *philo);
unsigned int	philo_strcpy_in_buffer(char *dst, unsigned int start, \
		const char *src);
void			philo_strrev(int len, char *buff);
int				philo_num_to_buff(int num, char buff[], int start);

#endif

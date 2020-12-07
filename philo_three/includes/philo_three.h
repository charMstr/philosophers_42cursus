/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_three.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <charmstr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 00:49:51 by charmstr          #+#    #+#             */
/*   Updated: 2020/12/07 05:26:57 by charmstr         ###   ########.fr       */
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
	FORK, EAT, SLEEP, THINK
}				t_state;

/*
**	state_buff: buffer we use in our life threads to describe.
**	death_buff:	buffer used in the polling philo, so that it is not modified
**	elapsed_time: the time used in the life thread, time since last_meal
**	death_time: the time used in monitor thread, checking against time_to_die
**
**	id:	philosopher's id.
**	time_to_eat: the time it takes to eat.
**	time_to_sleep: the time it takes to sleep.
**	time_to_die: the time it takes to die since the last meal's start.
**
**	meals_limit: a boolean, if true, we need to count meals
**	meals_count: counter that will decrement until zero.
**
**	stop: the adress of a boolean in Main stack if set to one, all philo will
**		stop.
**	sema_forks: semaphore on the number of forks
**	sema_sit: number of philosophers divided by two, is the number of
**		philosophers that can grab two forks and eat.
**	sema_speaker: token to use stdout
**	sema_touch_last_meal: semaphore to be allowed to update/get last meal time.
**
**	timeval_last_meal: contains the timestamp of last meal
**	timeval_tmp: the gettimeofday struct. always access timeval_tmp.tv_usec
*/

typedef struct	s_philo
{
	char			state_buff[32];
	char			death_buff[32];
	unsigned int	elapsed_time;
	unsigned int	death_time;
	unsigned int	id;
	unsigned int	time_to_eat;
	unsigned int	time_to_sleep;
	unsigned int	time_to_die;
	unsigned int	meals_limit;
	unsigned int	meals_count;
	unsigned int	*stop;
	sem_t			*sema_forks;
	sem_t			*sema_sit;
	sem_t			*sema_speaker;
	sem_t			*sema_touch_last_meal;
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

t_philo			**philo_array_init(t_parser_input *parser, int number_philo);
t_philo			*philo_struct_init(t_parser_input *parser, int id, \
		unsigned int *stop);
int				philo_set_start_time(t_philo **philo_array, int number_philo);
void			*philo_array_destroy(t_philo **array, int size);

int				philo_init_semaphores(t_philo **philo_array, int num);
int				philo_set_sema_forks(t_philo **philo_array, int number_philo);
int				philo_set_sema_sit(t_philo **philo_array, int number_philo);
int				philo_set_sema_speaker(t_philo **philo_array, int number_phil);
int				philo_set_sema_touch_last_meal(t_philo **philo_array, \
		int number_philo);

void			semaphores_close_all(t_philo **philo_array, int number_philo);
void			philo_del_sema_touch_last_meal(t_philo **philo_array, \
		int index);

int				philo_fork_and_start_processes(int number_philo, \
		t_philo **philo_array);
void			philo_start_process(t_philo philo);
void			*monitor(void *philo_void);
void			*life(void *philo_void);
void			philo_wait_for_pids(int number_philo);

void			set_elapsed_time(t_philo *philo);
void			philo_update_last_meal_time(t_philo *philo);
int				philo_check_last_meal_time(t_philo *philo);
void			philo_try_to_sleep_and_think(t_philo *philo);
void			philo_try_to_grab_forks_and_eat(t_philo *philo);

void			write_philo_state(t_philo *philo, t_state state);
void			write_dead_philo(t_philo *philo);
void			write_fed_up_philo(t_philo *philo);

unsigned int	philo_strcpy_in_buffer(char *dst, unsigned int start, \
		const char *src);
void			philo_strrev(int len, char *buff);
int				philo_num_to_buff(int num, char buff[], int start);

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_one.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <charmstr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 00:51:40 by charmstr          #+#    #+#             */
/*   Updated: 2020/12/04 04:58:37 by charmstr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_ONE_H
# define PHILO_ONE_H

# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>
# include <pthread.h>

# define DEL_MUTEX 1
# define NODEL_MUTEX 0

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
**	state_buff: buffer we use in our start_philo threads to describe.
**	death_buff:	buffer used in the polling philo, so that it is not modified
**	time: the time used in the start_philo thread.
**	time_poll: the time used in polling phio_thread.
**
**	total_number: the total number of philosophers, it is a pointer so that we
**					also can decrement it when a philo is done eating x times
**					or set it to zero when a philosopher died.
**	id:	philosopher_id
**	itoa_id: string that stores an itoa version of the philo's ID.
**	time_to_eat: the time it takes to eat.
**	time_to_sleep: the time it takes to sleep.
**	time_to_die: the time it takes to die since the last meal's start.
**
**	meals_limit: a boolean, if true, we need to count meals
**	meals_count: counter that will decrement until zero.
**
**	state: the current state of the philosopher. put in the state_buff
**
**	fork1: address of a mutexe from an array, each representing a fork.
**	fork2: as above. the second fork we will try to catch.
**	speaker: pointer to a mutex for using stdoutj
**
**	timeval_last_meal: contains the timestamp of last meal
**	timeval_tmp: the gettimeofday struct. always access timeval_tmp.tv_usec
*/

typedef struct	s_philo
{
	char			state_buff[32];
	char			death_buff[32];
	unsigned int	time;
	unsigned int	time_poll;
	unsigned int	id;
	unsigned int	time_to_eat;
	unsigned int	time_to_sleep;
	unsigned int	time_to_die;
	unsigned int	meals_limit;
	unsigned int	meals_count;
	unsigned int	*total_number;
	t_state			state;
	pthread_mutex_t *fork1;
	pthread_mutex_t *fork2;
	pthread_mutex_t *speaker;
	pthread_mutex_t *mutexes_on_forks;
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

t_philo			**philo_array_init_root(t_parser_input *parser, \
			int number_philo, unsigned int *total_number, \
			pthread_mutex_t *speaker);
t_philo			**philo_array_init(t_parser_input *parser, \
			unsigned int *total_number);
t_philo			*philo_struct_init(t_parser_input *parser, int id, \
			unsigned int *total);
void			*philo_array_destroy(t_philo **array, int size, \
			int del_mutexes);
int				philo_array_set_time(t_philo **philo_array, int total_philo);

pthread_mutex_t *philo_init_mutexes_on_forks(int number_philo);
void			philo_array_place_mutex(t_philo **philo_array, \
			int number_philo, pthread_mutex_t *mutexes_on_forks, \
			pthread_mutex_t *speaker);
int				philo_array_init_mutexes(t_philo **philo_array, \
			int number_philo, pthread_mutex_t *speaker);
void			destroy_mutexes_on_forks(pthread_mutex_t *mutexes_on_forks, \
			int num);
int				set_fork_index(int id, int total_number, int which);

void			start_and_join_threads(unsigned int number_philo, \
		pthread_t *pthreads_array, t_philo **philo_array);
void			*start_philo(void *philo_void);
void			*polling_philo(void *philo_void);

unsigned int	get_elapsed_time(t_philo *philo);
void			philo_try_to_grab_forks_and_eat(t_philo *philo);
void			philo_try_to_sleep_and_think(t_philo *philo);

void			write_with_lock(t_philo *philo);
void			write_dead_philo(t_philo *philo);
void			write_fed_up_philo(t_philo *philo);

unsigned int	philo_strcpy_in_buffer(char *dst, unsigned int start, \
			const char *src);
void			philo_strrev(int len, char *buff);
int				philo_num_to_buff(int num, char buff[], int start);

#endif

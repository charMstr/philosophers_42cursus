/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_two.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <charmstr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 00:49:51 by charmstr          #+#    #+#             */
/*   Updated: 2020/12/02 08:37:21 by charmstr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_TWO_H
# define PHILO_TWO_H

# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>
# include <pthread.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <semaphore.h>

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
**	itoa_id: string that stores an itoa version of the philo's ID.
**	time_to_eat: the time it takes to eat.
**	time_to_sleep: the time it takes to sleep.
**	time_to_die: the time it takes to die since the last meal's start.
**
**	meals_limit: a boolean, if true, we need to count meals
**	meals_count: counter that will decrement until zero.
**
**	stop: the adress of a boolean in Main stack if set to one, all philo will
**		stop.
** sem_forks: semaphore on the number of forks
** sem_sit_down: there is total_numbers_of_philo - 1 seats so that we avoid
**		dead locks.
**
**	timeval_last_meal: contains the timestamp of last meal
**	timeval_tmp: the gettimeofday struct. always access timeval_tmp.tv_usec
*/

typedef struct	s_philo
{
	char			buffer[32];
	unsigned int	total_number;
	unsigned int	id;
	unsigned int	time_to_eat;
	unsigned int	time_to_sleep;
	unsigned int	time_to_die;
	unsigned int	meals_limit;
	unsigned int	meals_count;
	unsigned int	*stop;
	unsigned int	time;
	t_state			state;
	sem_t			*sema_forks;
	sem_t			*sema_sit_down;
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
		int number_philo, unsigned int *stop);
t_philo			*philo_struct_init(t_parser_input *parser, int id, \
		unsigned int *stop);
int				philo_array_init_semaphore(t_philo **philo_array, \
		int number_philo);
int				philo_array_set_time(t_philo **philo_array, int total_philo);
void			*philo_array_destroy(t_philo **array, int size);

void			start_and_join_threads(unsigned int number_philo, \
		pthread_t *pthreads_array, t_philo **philo_array);
void			*start_philo(void *philo_void);

unsigned int	get_elapsed_time(t_philo *philo);
void			philo_starts_to_eat(t_philo *philo);
void			philo_try_to_grab_forks(t_philo *philo);

void			write_without_lock(t_philo *philo);
unsigned int	philo_strcpy_in_buffer(char *dst, unsigned int start, \
		const char *src);
void			philo_strrev(int len, char *buff);
int				philo_num_to_buff(int num, char buff[], int start);

#endif

#ifndef PHILO_ONE_H
# define PHILO_ONE_H

# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>
# include <pthread.h>

# define DEL_MUTEXES 1
# define NODEL_MUTEXES 0

typedef struct s_parser_input
{
	int	number_philo;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	total_meals_each;
}				t_parser_input;

typedef enum e_state
{
		FORK, EAT, SLEEP, THINK, DEAD
}			t_state;

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
**	mutexes_on_forks: array of mutexes that represent each forks(eating).
**	mutex_on_mic: pointer to a mutex for using stdout.
**
**	timeval_last_meal: contains the timestamp of last meal
**	timeval_tmp: the gettimeofday struct. always access timeval_tmp.tv_usec
*/

typedef struct	s_philo
{
	char				itoa_id[32];
	unsigned int		total_number;
	unsigned int		id;
	unsigned int		fork1;
	unsigned int		fork2;
	unsigned int		time_to_eat;
	unsigned int		time_to_sleep;
	unsigned int		time_to_die;
	unsigned int		meals_limit;
	unsigned int		meals_count;
	unsigned int		*stop;
	pthread_mutex_t *mutexes_on_forks;
	pthread_mutex_t *mutex_on_mic;
	struct timeval	timeval_last_meal;
	struct timeval	timeval_tmp;
}				t_philo;

int		ft_atoi(const char *str);
size_t	ft_strlen(char *str);
void	ft_putstr(char *str);
int		ft_isdigit(int c);
int		str_isdigit(char *str);

int				philo_parser_root(t_parser_input *parser, int argc, char *argv[]);
void			display_usage(char *argv0);
int				philo_parser_check_input(int argc, char **argv);
void			philo_parser_get_input(t_parser_input *parser, int argc, char **argv);

t_philo			**philo_array_init_root(t_parser_input *parser, int philo_number, unsigned int *stop, pthread_mutex_t *mutex_on_mic);
t_philo			*philo_struct_init(t_parser_input *parser, int id, unsigned int *stop);
unsigned int	philo_itoa_set_buff(unsigned int num, char buff[], unsigned int i, unsigned int j);
int				set_fork_index(int id, int total_number, int which);
int				philo_array_set_time(t_philo **philo_array, int total_philo);
void			*philo_array_destroy(t_philo **array, int size, int mutexes_created_yet);

int				philo_array_init_mutexes(t_philo **philo_array, int number_philo, pthread_mutex_t *mutex_on_mic);
pthread_mutex_t	*create_mutexes_on_forks_array(int number_philo);
void			destroy_and_free_mutexes_on_forks(pthread_mutex_t *mutexes_on_forks, int num);

void			start_and_join_threads(unsigned int number_philo, pthread_t *pthreads_array, t_philo **philo_array);
void			*start_philo(void *philo_void);

unsigned int	get_elapsed_time(t_philo *philo);
void philo_try_to_eat2(t_philo *philo, unsigned int time);
void philo_try_to_eat1(t_philo *philo);

void			describe_state(t_philo *philo, t_state state, unsigned int time);
unsigned int	philo_strcpy_in_buffer(char *dst, unsigned int start, const char *src);
#endif

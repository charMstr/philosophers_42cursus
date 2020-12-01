#ifndef PHILO_ONE_H
# define PHILO_ONE_H

# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>
# include <pthread.h>

# define DEL_SEMAPH 1
# define NODEL_SEMAPH 0

typedef struct		s_parser_input
{
	int	number_philo;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	total_meals_each;
}					t_parser_input;

typedef enum		e_state
{
	FORK1, FORK2, EAT, SLEEP, THINK, DEAD
}					t_state;

/*
** note:	This structure will be used when threading for our writes.
**			Each time we want to write and lock mutexes for it (both very
**			costly operations), we will start a new thread and detach it so
**			that we can keep doing our businesses.
**			The problem is that we cannot use a single buffer, the folowing
**			writes will update it while we are writing, or we wait for the end
**			of the write (we do not detach the process) but there is no point.
**			The solution is to have one buffer per sentence we will write.
**			6 in total (2 for the forks).
**
** note:	This structure will be in an array containing six of them.
*/

typedef struct	s_writer
{
	unsigned int	time;
	t_state			state;
	unsigned int	id;
	pthread_mutex_t *mutex_on_mic;
	char			buffer[32];
}				t_writer;

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

typedef struct		s_philo
{
	char			itoa_id[32];
	unsigned int	total_number;
	unsigned int	id;
	unsigned int	fork1;
	unsigned int	fork2;
	unsigned int	time_to_eat;
	unsigned int	time_to_sleep;
	unsigned int	time_to_die;
	unsigned int	meals_limit;
	unsigned int	meals_count;
	unsigned int	*stop;
	pthread_mutex_t *mutexes_on_forks;
	pthread_mutex_t *mutex_on_mic;
	struct timeval	timeval_last_meal;
	struct timeval	timeval_tmp;
}					t_philo;

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
int				philo_array_init_semaphore(t_philo **philo_array, int number_philo);
int				philo_array_set_time(t_philo **philo_array, int total_philo);
void			*philo_array_destroy(t_philo **array, int size, int semaphor_created_yet);

void			start_and_join_threads(unsigned int number_philo, \
		pthread_t *pthreads_array, t_philo **philo_array);
void			*start_philo(void *philo_void);
void			init_array_writers(t_writer (*array_writers)[], \
		t_philo *philo);

unsigned int	get_elapsed_time(t_philo *philo);
void			philo_starts_to_eat(t_philo *philo, unsigned int time, \
		t_writer (*array_writers)[]);
void			philo_try_to_grab_forks(t_philo *philo, t_writer (*array_writers)[]);

void			describe_state(t_state state, unsigned int time, \
		t_writer *writer);
unsigned int	philo_strcpy_in_buffer(char *dst, unsigned int start, \
		const char *src);
void			*philo_write(void *writer_void);
void			philo_strrev(int len, char *buff);
int				philo_num_to_buff(int num, char buff[], int start);
#endif

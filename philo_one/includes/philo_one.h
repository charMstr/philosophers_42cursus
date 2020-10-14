#ifndef PHILO_ONE_H
# define PHILO_ONE_H

# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>
# include <pthread.h>

typedef struct s_parser_input
{
	int number_philo;
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
**	time_start: contains the same time in each philosophers
**	time_last_meal: contains the timestamp of last meal
**	id:	philosopher_id
**	current_state: enum t_state value;
**	s_timeval:		the gettimeofday struct. always access s_timeval.tv_usec
**	meals_limit: a boolean, if true, we need to count meals
**	meals_count: counter
**	meals_target: the number of meals before activating meals_done to true.
**	meals_done: set to true when meals_count reaches meals_target.
*/

typedef struct	s_philo
{
	t_state	state;
	int		total_number;
	int		id;
	int		time_to_eat;
	int		time_to_sleep;
	int		time_to_die;
	int		meals_limit;
	int		meals_count;
	int		meals_target;
	int		meals_done;
	char	*forks;
	struct timeval	timeval_last_meal;
	struct timeval	timeval_tmp;
}				t_philo;

int		ft_atoi(const char *str);
void	ft_putchar(char c);
void	ft_putnbr(int n);
size_t	ft_strlen(char *str);
void	ft_putstr(char *str);
int		ft_isdigit(int c);
int		str_isdigit(char *str);

#endif

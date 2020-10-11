#include "philo_one.h"

size_t	ft_strlen(char *str)
{
	int i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void ft_putstr(char *str)
{
	write(1, str, ft_strlen(str));
}

void	display_usage(char *argv0)
{
	ft_putstr("error: bad number of arguments\n");
	ft_putstr("usage: ");
	ft_putstr(argv0);
	ft_putstr(" + \n  1 - number_of_philosopher\n"\
			"  2 - time_to_die\n"\
			"  3 - time_to_eat\n"\
			"  4 - time_to_sleep\n"\
			"[ 5 ] - number_of_time_each_philosophers_must_eat\n");
}

int	main(int argc __attribute__((unused)), char **argv __attribute__((unused)))
{
	if (argc < 5 || argc > 6)
	{
		display_usage(argv[0]);
		return (0);
	}
	return (0);
}

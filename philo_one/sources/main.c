#include "philo_one.h"
//remoev me
#include <stdio.h>


int	display_usage(char *argv0)
{
	ft_putstr("usage: ");
	ft_putstr(argv0);
	ft_putstr(" + \n  1 - number_of_philosopher\n"\
			"  2 - time_to_die\n"\
			"  3 - time_to_eat\n"\
			"  4 - time_to_sleep\n"\
			"[ 5 ] - number_of_time_each_philosophers_must_eat\n");
	return (0);
}

/*
** note:	this function will set the state of the philosopher according to
**			the	time fetched with the get_elapsed_time func
**
**	RETURN:	a t_state enum (SLEEP, THINK, DEAD) if change in state has occured.
**
**			0 otherwise.
*/

int	set_philo_state(t_philo *philo, int elapsed_time) //OK
{
	if (elapsed_time > philo->time_to_die)
	{
		philo->state = DEAD;
		return (DEAD);
	}
	else if (elapsed_time > philo->time_to_eat + philo->time_to_sleep)
	{
		if (philo->state == THINK)
			return (0);
		philo->state = THINK;
		return (THINK);
	}
	else if (elapsed_time > philo->time_to_eat)
	{
		if (philo->state >= SLEEP)
			return (0);
		philo->state = SLEEP;
		return (SLEEP);
	}
	return (0);
}

/*
** note:	this function will describe on stdout the state of the philosopher.
*/

void	describe_state(int id, t_state state, int time)
{

	ft_putnbr(time);
	ft_putchar(' ');
	ft_putnbr(id);
	ft_putchar(' ');
	if (state == FORK)
		ft_putstr("has taken a fork\n");
	else if (state == EAT)
		ft_putstr("is eating\n");
	else if (state == SLEEP)
		ft_putstr("is sleeping\n");
	else if (state == THINK)
		ft_putstr("is thinking\n");
	else
		ft_putstr("died\n");
}

/*
** note:	This function returns the time elapsed since the last meal.
**			It takes care of the case when we go from one second to another.
**
** RETURN:	time elapsed
**			-1 failure with gettimeofday func.
*/

int	get_elapsed_time(t_philo *philo)
{
	int res;

	if (gettimeofday(&philo->timeval_tmp, NULL))
		return (-1);
	if (philo->timeval_last_meal.tv_sec == philo->timeval_tmp.tv_sec)
	{
		return ((philo->timeval_tmp.tv_usec - \
					philo->timeval_last_meal.tv_usec) / 1000);
	}
	else
	{
		res = (philo->timeval_tmp.tv_sec - philo->timeval_last_meal.tv_sec - 1) * 1000;
		res +=	(1000000 - philo->timeval_last_meal.tv_usec + philo->timeval_tmp.tv_usec) / 1000;
		return (res);
	}
}

/*
** note:	this function will RETURN true only if eating was possible for our
**			philosopher.
*/

int	try_to_eat(t_philo *philo)
{
	(void)philo;
	return (0);
}

/*
** note:	this function will be called for each philosopher.
*/

void	*start_philo(void *philo_void)
{
	t_philo *philo;
	int time;
	int state;

	philo = (t_philo *)philo_void;
	printf("got here(start_philo)\n");
	while (1)
	{
		if (try_to_eat(philo))
			describe_state(philo->id, EAT, time);
		else
		{
			if ((time = get_elapsed_time(philo)) == -1)
				return (NULL);
			if ((state = set_philo_state(philo, time)))
				describe_state(philo->id, state, time);
			if (state == DEAD)
				//do dome sort of shit to kill all the others here.
				return (0);
		}
		usleep(100);
	}
	return ((void*)philo);
}



/*
** note:	this function will be used to destroy an array of t_philo struct
**			pointers
**
** note:	inside the very first t_philo struct pointer, we free the common
**			data (forks array) only once.
**
** RETURN: NULL, always
*/

void *philo_array_destroy(t_philo **array, int size)
{
	if (size > 0)
		free(array[0]->forks);
	while (--size >= 0)
	{
		free(array[size]);
	}
	free(array);
	return (NULL);
}


/*
** note:	this funciton will malloc a philosopher struct and sets its fields
**			to the correct value. The timeval_last_meal will be set in a
**			subsequent loop just before threadind.
**
** note:	the initial state is set to THINK. so that the philosophers are
**			ready to try to take forks.
**
** RETURN:	NULL,				KO
**			pointer to t_philo *
*/

t_philo *philo_struct_init(t_parser_input *parser, int id)
{
	t_philo *philo;

	if (!(philo = malloc(sizeof(t_philo))))
		return (NULL);
	philo->forks = NULL;
	philo->total_number = parser->number_philo;
	philo->id = id;
	philo->state = THINK;
	philo->time_to_eat = parser->time_to_eat;
	philo->time_to_sleep = parser->time_to_sleep;
	philo->time_to_die = parser->time_to_die;
	philo->meals_count = 0;
	philo->meals_done = 0;
	philo->meals_target = parser->total_meals_each;
	if (parser->total_meals_each != -1)
		philo->meals_limit = 1;
	else
		philo->meals_limit = 0;
	return (philo);
}

/*
** note:	this function will malloc an array conataining all our philosophers
**			structures.
**			each philo structure is malloced in a subfunction, and given a
**			specific id.
**
** note:	at the very end when all the malloc is done, we finally put the
**			same time_stamp in all the philosophers structures, and we put in
**			common the same forks array (array of char, each index representing
**			a fork for eating pastas).
**
** RETURN:	NULL,				KO, malloc failure, or gettimeofday failed
**			array of pointers,	OK
*/

t_philo **philo_array_init(t_parser_input *parser)
{
	int		i;
	t_philo	**philo_array;

	i = 0;
	if (!(philo_array = malloc(sizeof(t_philo*) * parser->number_philo)))
		return (NULL);
	while (i < parser->number_philo)
	{
		if (!(philo_array[i] = philo_struct_init(parser,i + 1)))
			return (philo_array_destroy(philo_array, i));
		i++;
	}
	return (philo_array);
}

/*
** note:	this function will call the subroutine that will malloc and init
**			all the philosophers structures.  ** **			then it will do a last pass and put in each the same timestamp and **			fork_array. (after all the mallocs are done). therefore we will **			have the same timestamp and we will be ready to start the thread
**			functions with the earliest timestamp.
**
** RETURN:	NULL,				KO, malloc failure, or gettimeofday failed
**			array of pointers,	OK
*/

t_philo **philo_array_init_and_set_time(t_parser_input *parser)
{
	char	*forks_array; t_philo	**philo_array;
	struct	timeval time_val;
	int		i;

	i = 0;
	if (!(forks_array = malloc(sizeof(*forks_array) * parser->number_philo)))
		return (NULL);
	if (!(philo_array = philo_array_init(parser)))
	{
		free(forks_array);
		return (NULL);
	}
	if (gettimeofday(&time_val, NULL))
	{
		free(forks_array);
		return (philo_array_destroy(philo_array, parser->number_philo));
	}
	while (i < parser->number_philo)
	{
		philo_array[i]->forks = forks_array;
		philo_array[i]->timeval_last_meal = time_val;
		i++;
	}
	return (philo_array);
}

/*
** note:	this function will parse the input against correct number of
**			arguments, and dumb inputs like negative numbers.
**			It will fit each parameters into a structure.
**
** RETURN:	0, failure
**			1, OK
*/

int	parse_input(t_parser_input *parser, int argc, char **argv, int i)
{
	if (argc < 5 || argc > 6)
	{
		ft_putstr("error: bad number of arguments\n");
		return (0);
	}
	while (i < argc)
	{
		if (!str_isdigit(argv[i]))
		{
			ft_putstr("warning: only put numeric characters in parameters!\n");
			return (0);
		}
		i++;
	}
	if ((parser->number_philo = ft_atoi(argv[1])) < 2)
	{
		ft_putstr("warning: cannot have less than two philosophers!\n");
		return (0);
	}
	parser->time_to_die = ft_atoi(argv[2]);
	parser->time_to_eat = ft_atoi(argv[3]);
	parser->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		parser->total_meals_each = ft_atoi(argv[5]);
	else
		parser->total_meals_each = -1;
	return (1);
}

/*
**	la fonction pthread_create prend en param un void *
**	il faut donc creer une structure au tout debut, et cette structure va contenir
**	les specificites de chaque philosopher avec notamment:
**	-	start_time_ref
**	-	id: de 1 a number of philosophers
**	-	last_meal_time: soustraction avec le time actuel, et on compare avec time_to_die
**	-	current_state: maybe?
**	-	une zone de memoire qui est malloc, donc commune, possible de mettre la
**	-	struct_time_val: so that we dont have to creat one
**		on the stack each time.
**		les fouchettes etc?
*/


//TODO:
/*
   need to have a mutex for writing in the stdout.
*/

int	main(int argc, char **argv)
{
	t_philo			*t_philo_res;
	t_parser_input	parser;
	t_philo			**array_philo;
	pthread_t		*array_pthreads;
	int i;

	i = 0;
	if (!parse_input(&parser, argc, argv, 1))
		return (display_usage(argv[0]));
	if (!((array_philo = philo_array_init(&parser))))
		return (0);
	if (!(array_pthreads = malloc(sizeof(pthread_t) * parser.number_philo)))
	{
		philo_array_destroy(array_philo, parser.number_philo);
		return (0);
	}
	while (i < parser.number_philo)
	{
		pthread_create(&array_pthreads[i], NULL, start_philo, (void*)array_philo[i]);
		i++;
	}
	i = 0;
	while (i < parser.number_philo)
	{
		pthread_join(array_pthreads[i], (void **)&t_philo_res);
		i++;
	}
	philo_array_destroy(array_philo, parser.number_philo);
	return (0);
}

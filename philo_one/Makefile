# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: charmstr <charmstr@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/01/27 17:46:05 by charmstr          #+#    #+#              #
#    Updated: 2020/12/21 12:15:24 by charmstr         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

############################ INCLUDE OTHER MAKEFILE ###########################
###############################################################################
#-include <file_name>.mk
# '-' so if the file does not exist --> silent
###############################################################################
###############################################################################


############################ SETTINGS #########################################
###############################################################################
CFLAGS = -Wall -Wextra -Werror
CC = gcc
NAME = philo_one
IFLAGS = -I $(INCLUDE_PATH_PROJECT)
LDFLAGS = -lpthread

DEPS = $(INCLUDES)
#DEPS_BONUS = $(INCLUDES_BONUS)
###############################################################################
###############################################################################

################## FOR COMPILATION WITH DEBUG #################################
###############################################################################
ifeq ($(f), n)
CFLAGS 		=
VALGRIND 	=
else ifeq ($(f), f)
CFLAGS 		+=  -fsanitize=address,undefined -g3
VALGRIND 	=
else ifeq ($(f), v)
CFLAGS 		+= -g3
SHOW_LEAK 	= --show-leak-kinds=definite
VALGRIND 	= valgrind --track-origins=yes --leak-check=full $(SHOW_LEAK)
endif
###############################################################################
###############################################################################


############################ PATHS ############################################
###############################################################################
OBJ_PATH = ./objects/
SRC_PATH = ./sources/
INCLUDE_PATH_PROJECT = ./includes/
###############################################################################
###############################################################################


########################### HEADER FILES ######################################
###############################################################################
H_FILES =	philo_one

#H_FILES_BONUS =

INCLUDES := $(patsubst %, $(INCLUDE_PATH_PROJECT)%.h, $(H_FILES))
#ADD EXTRANEOUS LIBRARY INCLUDES HERE
# INCLUDES := $(INCLUDES) ./libft/libft.h

#INCLUDES_BONUS := $(patsubst %, $(INCLUDE_PATH_PROJECT)%.h, $(H_FILES_BONUS))
#INCLUDES_BONUS := $(INCLUDES_BONUS) ./libft/libft.h
###############################################################################
###############################################################################


########################### SRC/OBJ & BONUS FILES #############################
###############################################################################
SRC_FILES =	main\
			philo_parser_root\
			philo_array_init_root\
			philo_mutexes_set1\
			philo_mutexes_set2\
			philo_start_threads\
			philo_get_time\
			philo_try_to_eat\
			philo_write_stdout\
			philo_write_stdout_assist\
			libft_funcs1
	
#BONUS_FILES =

SRC = $(patsubst %, $(SRC_PATH)%.c, $(SRC_FILES))
OBJ = $(patsubst %, $(OBJ_PATH)%.o, $(basename $(notdir $(SRC))))

#SRC_BONUS = $(patsubst %, $(SRC_PATH)%.c, $(BONUS_FILES))
#OBJ_BONUS = $(patsubst %, $(OBJ_PATH)%.o, $(basename $(notdir $(SRC_BONUS))))
###############################################################################
###############################################################################

###############################################################################
############################### COLOR CODE ####################################
REMOVE_FG = \033[38;5;196m
CREAT_FG = \033[38;5;46m
BLACK_FG = \033[38;5;0m
BLACK_BG = \033[48;5;0m
CLEAR_COLOR = \033[m
NAME_BG = \033[48;5;39m
OBJECTS_BG = \033[48;5;11m
###############################################################################
###############################################################################

all: $(NAME)

#| silent

silent:
	@:

.PHONY: all clean fclean re break_implicit_r

$(OBJ_PATH):
	@mkdir -p $(OBJ_PATH)
	@echo "\t\t$(CREAT_FG)created the $(OBJECTS_BG)$(BLACK_FG) $@ $(BLACK_BG)$(CREAT_FG) repository for $(NAME_BG)$(BLACK_FG) $(NAME) $(BLACK_BG)$(CREAT_FG)$(CLEAR_COLOR)"

$(NAME): $(INCLUDES) $(OBJ)
	@$(CC) $(CFLAGS) $(LIB_PATH) $(OBJ) -o $@ $(LDFLAGS)
	@echo "\t\t$(CREAT_FG)Binary $(NAME_BG)$(BLACK_FG) $(NAME) $(BLACK_BG)$(CREAT_FG) has been created$(CLEAR_COLOR)"
	@echo "\t\t$(CREAT_FG)We used the flags: $(CFLAGS)$(CLEAR_COLOR)\n"

$(OBJ): $(OBJ_PATH)%.o: $(SRC_PATH)%.c $(DEPS) | $(OBJ_PATH)
	@$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

bonus:

break_implicit_r:

clean:
	@echo "\t\t$(REMOVE_FG)deleting $(OBJECTS_BG)$(BLACK_FG) $(OBJ_PATH) $(BLACK_BG)$(REMOVE_FG) containing all the .o files for $(NAME_BG)$(BLACK_FG) $(NAME) $(CLEAR_COLOR)"
	@rm -rf $(OBJ_PATH)

fclean: clean
	@echo "\t\t$(REMOVE_FG)deleting $(NAME_BG)$(BLACK_FG) $(NAME) $(BLACK_BG)$(REMOVE_FG)...$(CLEAR_COLOR)"
	@rm -rf $(NAME)

re: fclean all

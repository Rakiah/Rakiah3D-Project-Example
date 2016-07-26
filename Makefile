# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bkabbas <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/01/18 21:30:01 by bkabbas           #+#    #+#              #
#    Updated: 2016/07/26 19:24:35 by Rakiah           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = example
PATH_SRC = ./

PATH_PORNYGONZ = libs/pornygonz
PATH_RCONTAINERS = libs/rcontainers
PATH_RSTD = libs/rstd
PATH_RMATH = libs/rmath
PATH_SDL2 = libs/SDL2
PATH_R3D = libs/Rakiah3D

OPTIMIZE = yes
DEBUG = no

PATH_HEADERS = -I $(PATH_RSTD)/includes/ \
			   -I $(PATH_R3D)/includes/ \
			   -I $(PATH_RCONTAINERS)/includes/ \
			   -I $(PATH_PORNYGONZ)/includes/ \
			   -I $(PATH_RMATH)/includes/ \
			   -I $(PATH_SDL2)/includes/

SRC += $(PATH_SRC)main.c

CC = gcc
CFLAGS = -Wall -Wextra -Werror

ifeq ($(OPTIMIZE), yes)
	CFLAGS += -O3
endif

ifeq ($(DEBUG), yes)
	CFLAGS += -O0 -g
endif

CLIBS_PATH =	-L $(PATH_R3D)/ \
				-L $(PATH_PORNYGONZ)/ \
				-L $(PATH_RCONTAINERS)/ \
				-L $(PATH_RMATH)/ \
				-L $(PATH_RSTD)/

CLIBS =	-l r3d \
		-l pornygonz \
		-l rstd \
		-l rcontainers \
		-l rmath \
		-l m \
		-l SDL2


OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	make -C $(PATH_PORNYGONZ)/
	make -C $(PATH_RSTD)/
	make -C $(PATH_RCONTAINERS)/
	make -C $(PATH_RMATH)/
	make -C $(PATH_R3D)/
	$(CC) $(CFLAGS) $(CLIBS_PATH) -o $(NAME) $(OBJ) $(CLIBS)

%.o: %.c
	$(CC) $(CFLAGS) $(PATH_HEADERS) -o $@ -c $<

clean:
	@rm -f $(OBJ)
	make -C $(PATH_PORNYGONZ)/ clean
	make -C $(PATH_RSTD)/ clean
	make -C $(PATH_RCONTAINERS)/ clean
	make -C $(PATH_RMATH)/ clean
	make -C $(PATH_R3D)/ clean
	@echo removed binary files

fclean: clean
	@rm -f $(NAME)
	make -C $(PATH_PORNYGONZ)/ fclean
	make -C $(PATH_RSTD)/ fclean
	make -C $(PATH_RCONTAINERS)/ fclean
	make -C $(PATH_RMATH)/ fclean
	make -C $(PATH_R3D)/ fclean
	@echo removed library

re: fclean all

.PHONY: all clean fclean re

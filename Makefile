# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bkabbas <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/01/18 21:30:01 by bkabbas           #+#    #+#              #
#    Updated: 2016/01/21 18:06:19 by bkabbas          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = wolf3d
PATH_SRC = src/
PATH_HEADERS = -I includes/ -I libft/includes/ -I rakiah3D/includes/ -I rlists/includes/ -I SDL2/includes/
SRC = $(PATH_SRC)main.c $(PATH_SRC)w3d_loader.c
CC = gcc
CFLAGS = -Wall -Wextra -Werror -O3
CLIBS_PATH = -L rakiah3D/ \
	     -L libft/ \
	     -L SDL2/ \
	     -L rlists/
CLIBS = -l r3d \
	-l ft \
	-l rlists \
	-l SDL2 \
	-l SDL2_ttf \
	-l m
OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	make -C rakiah3D/
	make -C libft/
	make -C rlists/
	$(CC) $(CFLAGS) $(CLIBS_PATH) -o $(NAME) $(OBJ) $(CLIBS)

%.o: %.c
	$(CC) $(CFLAGS) $(PATH_HEADERS) -o $@ -c $<

nolib: $(NAME)
	$(CC) $(CFLAGS) $(CLIBS_PATH) -o $(NAME) $(OBJ) $(CLIBS)

clean:
	@rm -f $(OBJ)
	make -C libft/ clean
	make -C rakiah3D/ clean
	make -C rlists/ clean
	@echo removed binary files

fclean: clean
	@rm -f $(NAME)
	make -C libft/ fclean
	make -C rakiah3D/ fclean
	make -C rlists/ fclean
	@echo removed library

re: fclean all

.PHONY: all clean fclean re nolib

# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: imehdid <ismaelmehdid@student.42.fr>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/06 15:07:05 by ismaelmehdi       #+#    #+#              #
#    Updated: 2024/05/04 15:59:19 by imehdid          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = game_of_life

SRCS = main.c

OBJDIR = ./objs
OBJS = ${SRCS:%.c=$(OBJDIR)/%.o}

CC = cc -g
CFLAGS = -Wall -Wextra -Werror

RM = rm -rf

all: ${NAME}

$(OBJDIR)/%.o: %.c
		@mkdir -p $(@D)
		@${CC} ${CFLAGS} -c $< -o $@

$(NAME): ${OBJS}
		@${CC} ${CFLAGS} ${OBJS} -lSDL2 -o  ${NAME}

clean:
		@${RM} ${OBJS}

fclean: clean
		@${RM} ${OBJS} ${NAME} ${OBJDIR}

re: fclean all

.PHONY: all clean fclean re
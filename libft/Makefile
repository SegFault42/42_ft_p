# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rabougue <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/12/10 19:16:51 by rabougue          #+#    #+#              #
#    Updated: 2017/04/06 21:42:08 by rabougue         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

RED = \033[48;5;9m
GREEN = \033[48;5;10m
LOL = \033[48;5;106m
LIL = \033[48;5;129m
END = \033[0m
X = 0
Y = 0

PRE = ./ft_dprintf/sources/

NAME = libft.a

SRC = ft_memset.c ft_bzero.c ft_memcpy.c ft_memccpy.c ft_memmove.c ft_memchr.c \
	  ft_memcmp.c ft_strlen.c ft_strdup.c ft_strcpy.c ft_strncpy.c ft_strcat.c \
	  ft_strncat.c ft_strchr.c ft_atoi.c ft_strrchr.c ft_strstr.c ft_strnstr.c \
	  ft_strcmp.c ft_strncmp.c ft_isalpha.c ft_isdigit.c ft_isalnum.c \
	  ft_isascii.c ft_isprint.c ft_toupper.c ft_tolower.c ft_memalloc.c \
	  ft_memdel.c ft_strnew.c ft_strdel.c ft_strclr.c ft_striter.c \
	  ft_striteri.c ft_strmap.c ft_strmapi.c ft_strequ.c ft_strnequ.c \
	  ft_strsub.c ft_strjoin.c ft_strtrim.c ft_putchar.c ft_putstr.c \
	  ft_putendl.c ft_putnbr.c ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c \
	  ft_putnbr_fd.c ft_strsplit.c ft_strlcat.c ft_itoa.c ft_lstnew.c \
	  ft_lstdelone.c ft_lstdel.c ft_lstadd.c ft_lstiter.c ft_lstmap.c \
	  ft_swap.c ft_strrev.c ft_crypt.c ft_decrypt.c ft_debug.c get_next_line.c \
	  ft_2d_tab_free.c ft_tab_len.c ft_count_line_in_file.c ft_sqrt.c ft_atol.c \
	  ft_count_2d_tab.c ft_itoa_base.c ft_strclen.c ft_hexa_itoa.c \
	  ft_put_long.c ft_ltoa.c ft_put_ulong.c ft_ltoa_base.c ft_ultoa.c \
	  ft_hexa_ltoa.c ft_ultoa_base.c ft_size_ttoa.c ft_strndup.c\
	  $(PRE)ft_dprintf.c $(PRE)percent_c.c \
	  $(PRE)percent_d.c $(PRE)percent_s.c $(PRE)percent_p.c\
	  $(PRE)tools.c $(PRE)percent_x.c $(PRE)percent_o.c\
	  ft_print_2d_tab.c ft_check_int_overflow.c ft_count_char.c ft_strccmp.c \
	  ft_strcdup.c ft_strclen.c ft_strccat.c ft_isspace.c \
	  ft_trymalloc.c ft_error.c ft_set_2d_tab.c ft_strxcat.c \

OBJS = $(SRC:.c=.o)

CC = clang

FLAG = -Wall -Wextra -Werror -O3

HEAD = ./includes/ ./ft_dprintf/includes/ft_dprintf.h

all: $(NAME)

$(NAME): $(OBJS) ./includes/libft.h
	@$(CC) $(FLAG) -c $(SRC) -I $(HEAD)
	@ar rc $(NAME) $(OBJS)
	@ranlib $(NAME)

%.o : %.c $(HEAD)
	@printf " ✅                                                              \r"
	@printf "✅  $(notdir $<)\r"
	$(CC) -c $(FLAG) $< -o $@

mc: fclean all clean

clean:
	@rm -f $(OBJS) ./percent_c.o ./percent_d.o ./percent_s.o ./ft_dprintf.o \
	./percent_p.o ./percent_x.o ./tools.o ./percent_o.o

fclean: clean
	@rm -f $(NAME)

re: fclean all

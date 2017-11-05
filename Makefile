# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rabougue <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/11/05 00:34:51 by rabougue          #+#    #+#              #
#    Updated: 2017/11/05 01:01:57 by rabougue         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

all:
	@make -f ./Makefile.serveur
	@make -f ./Makefile.client

serveur:
	@make -f ./Makefile.serveur

client:
	@make -f Makefile.client

clean:
	@make clean -f ./Makefile.serveur
	@make clean -f ./Makefile.client

fclean:
	@make fclean -f ./Makefile.serveur
	@make fclean -f ./Makefile.client

re: fclean all

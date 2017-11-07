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
	@make -sf ./Makefile.serveur
	@make -sf ./Makefile.client

serveur:
	@make -sf ./Makefile.serveur

client:
	@make -sf Makefile.client

clean:
	@make clean -sf ./Makefile.serveur
	@make clean -sf ./Makefile.client

fclean:
	@make fclean -sf ./Makefile.serveur
	@make fclean -sf ./Makefile.client

re: fclean all

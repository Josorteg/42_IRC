# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: josorteg <josorteg@student.42barcel>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/16 18:17:17 by josorteg          #+#    #+#              #
#    Updated: 2024/05/06 18:03:25 by josorteg         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #




NAME = IRC

#########
RM = rm -f
CC = c++
CFLAGS = -Werror -Wextra -Wall -std=c++98
##$-g -fsanitize=address
#########

#########
FILES = main Server Client Nick User Join Channel Whonames Mode Privmsg Invite ModeFlagI ModeFlagK ModeFlagT ModeFlagO ModeFlagL Topic Kick Ping Part

SRC = $(addsuffix .cpp, $(FILES))


##vpath %.cpp
#########

#########
OBJ = $(SRC:.cpp=.o)
DEP = $(addsuffix .d, $(basename $(OBJ)))
#########

#########
%.o: %.cpp
	@${CC} $(CFLAGS) -MMD -c $< -o $@

all:
	@$(MAKE) $(NAME) --no-print-directory

$(NAME):: $(OBJ) Makefile
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)


clean:
	@$(RM) $(OBJ) $(DEP) --no-print-directory


fclean: clean
	@$(RM) $(NAME) --no-print-directory


re:	fclean all

.PHONY: all clean fclean re

-include $(DEP)

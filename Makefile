# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: josorteg <josorteg@student.42barcel>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/16 18:17:17 by josorteg          #+#    #+#              #
#    Updated: 2024/05/10 12:17:59 by josorteg         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #




NAME = IRC

#########
RM = rm -f
CC = c++
CFLAGS = -Werror -Wextra -Wall -std=c++98
##$-g -fsanitize=address
#########

#----------------------------SOURCES-------------------------------------------#

FILES = main Server Client Channel
MODE_FILES = ModeFlagI ModeFlagK ModeFlagT ModeFlagO ModeFlagL
COMMAND_FILES = Nick User Join Whonames Mode Privmsg Invite Topic Kick Ping Part

#----------------------------PATHS---------------------------------------------#
INC = -I ./inc


SRC = $(addsuffix .cpp, $(FILES))\
	$(addsuffix .cpp, $(MODE_FILES))\
	$(addsuffix .cpp, $(COMMAND_FILES))

#----------------------------OBJECTS FOLDER------------------------------------#

vpath %.cpp files/:mode_files/:command_files

F_OBJ = obj/
OBJ = $(addprefix $(F_OBJ), $(SRC:.cpp=.o))
DEP = $(addprefix $(F_OBJ), $(SRC:.cpp=.d))



#########

#----------------------------OLD OBJECT DEEPS----------------------------------#
#OBJ = $(SRC:.cpp=.o)
#DEP = $(addsuffix .d, $(basename $(OBJ)))
#-------------------------------ALL--------------------------------------------#

all:  dir $(NAME)

dir:
	mkdir -p $(F_OBJ)


$(F_OBJ)%.o: %.cpp
	@${CC} $(CFLAGS) -I ./inc  -MMD -c $< -o $@


$(NAME):: $(OBJ) Makefile
	@$(CC) $(CFLAGS) -I ./inc $(OBJ) -o $(NAME)


clean:
	@$(RM) $(OBJ) $(DEP)
	$(RM) -R obj


fclean: clean
	@$(RM) $(NAME) --no-print-directory


re:	fclean all

.PHONY: all clean fclean re

-include $(DEP)

#-----------------------------VPATH TO SEARCH IN ALL THE FOLDERS---------------#


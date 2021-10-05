NAME		= webserv

SRCS_DIR	= ./srcs/
SRCS		= main.cpp \
			  Socket.cpp \
			  PassiveSocket.cpp \
			  Parser.cpp \
			  utils.cpp \
			  Request.cpp \
			  Webserver.cpp \
			  Connexion.cpp \
			  Website.cpp \
			  Route.cpp \
			  Response.cpp

OBJS_DIR	= ./objs/
OBJS		= $(addprefix $(OBJS_DIR), $(SRCS:.cpp=.o))

RM			= rm -rf

FLAGS		= -Wall -Wextra -Werror -g -I ./includes/Webserv.hpp -std=c++98

$(OBJS_DIR)%.o : $(SRCS_DIR)%.cpp
			@mkdir -p objs
			@echo "Compiling $<"
			@clang++ $(FLAGS) -c $< -o $@

$(NAME):	$(OBJS)
			@clang++ ${FLAGS} ${OBJS} -o $(NAME)
			@echo "Compilation complete"
all:		$(NAME)

clean:
			@$(RM) $(OBJS_DIR)
			@echo "Cleaning..."

fclean:		clean
			@$(RM) $(NAME)

re:			fclean all

.PHONY:		all clean fclean re
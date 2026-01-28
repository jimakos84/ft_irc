
NAME = ircserv
CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++17 -g

GREEN = \033[0;32m
RESET = \033[0m

.SILENT:

SRC = main.cpp \
	input_validation.cpp \
	Server.cpp \
	Client.cpp \
	CmdCenter.cpp \
	Channel.cpp \
	./Commands/ParentCommand.cpp \
	./Commands/Ping.cpp \
	./Commands/Pass.cpp \
	./Commands/Nick.cpp \
	./Commands/User.cpp \
	./Commands/Mode.cpp \
	./Commands/Join.cpp \
	./Commands/Kick.cpp \
	./Commands/Privmsg.cpp \
	./Commands/Part.cpp \
	./Commands/Invite.cpp \
	./Commands/Topic.cpp \
	./Commands/Quit.cpp \

OBJ = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
	@echo "$(GREEN)✔ Build successful$(RESET)"

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

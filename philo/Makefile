NAME	:= philo

CC		:= cc
CFLAGS	:= -Wall -Wextra -Werror -pthread
CPPFLAGS := -Iincludes

SRCDIR	:= src
OBJDIR	:= obj

SRCS	:= $(addprefix $(SRCDIR)/, \
			main.c \
			init.c \
			input_parsing.c \
			start.c \
			monitor.c \
			routine.c \
			action_routines.c \
			write.c \
			safe_handlers.c \
			safe_helpers.c \
			utils.c \
			cleanup.c \
			)

OBJS	:= $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))

.PHONY: all
all: $(NAME)

$(NAME): $(OBJS)
	@echo "Linking $(NAME)..."
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "$(NAME) has been built successfully."

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(OBJDIR):
	@mkdir -p $(OBJDIR)

.PHONY: clean
clean:
	@echo "Cleaning object files..."
	@rm -rf $(OBJDIR)

.PHONY: fclean
fclean: clean
	@echo "Cleaning executable..."
	@rm -f $(NAME)

.PHONY: re
re: fclean all

.PHONY: valgrind
valgrind: all
	@echo "Running Valgrind with args: $(ARGS)"
	valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes ./$(NAME) $(ARGS)

.PHONY: help
help:
	@echo "Usage: make [target]"
	@echo "Targets:"
	@echo "  all	   - Build the project (default)"
	@echo "  clean	 - Remove object files"
	@echo "  fclean	- Remove object files and the executable"
	@echo "  re		- Rebuild the project (fclean + all)"
	@echo "  valgrind  - Run Valgrind. Use ARGS to pass arguments, e.g., make valgrind ARGS=\"4 800 200 200\""

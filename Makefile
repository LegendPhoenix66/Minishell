# Compiler and flags
CC = cc
FLAGS = -Wall -Wextra -Werror
AR = ar rcs
RM = /bin/rm -f

# Directories
SRC = src
INC = include
LIBFT = libft
OBJ = obj

# Files
SOURCES = $(wildcard $(SRC)/*.c)
OBJECTS = $(SOURCES:$(SRC)/%.c=$(OBJ)/%.o)
CFLAGS = -I $(INC) -I $(LIBFT)
LDFLAGS = -L $(LIBFT) -lft -lreadline

# Executable name
NAME = minishell

# Main rule
$(NAME): $(OBJECTS) $(LIBFT)/libft.a
	$(CC) $(FLAGS) $(OBJECTS) $(LDFLAGS) -o $(NAME)

# Compile .c files to .o
$(OBJ)/%.o: $(SRC)/%.c | $(OBJ)
	$(CC) $(FLAGS) $(CFLAGS) -c $< -o $@

# Create obj directory if it doesn't exist
$(OBJ):
	mkdir -p $(OBJ)

# Compile libft
$(LIBFT)/libft.a:
	$(MAKE) -C $(LIBFT) bonus

# Clean object files and executable
clean:
	$(RM) $(OBJECTS)
	$(MAKE) -C $(LIBFT) clean

fclean: clean
	$(RM) $(NAME)
	$(RM) -r $(OBJ)
	$(MAKE) -C $(LIBFT) fclean

# Complete reset
re: fclean $(NAME)

# Ignore temporary files
.PHONY: all clean fclean re

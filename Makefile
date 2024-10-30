# Compilateur et flags
CC = cc
FLAGS = -Wall -Wextra -Werror
AR = ar rcs
RM = /bin/rm -f

# Dossiers
SRC = src
INC = include
LIBFT = libft
OBJ = obj

# Fichiers
SOURCES = $(wildcard $(SRC)/*.c)
OBJECTS = $(SOURCES:$(SRC)/%.c=$(OBJ)/%.o)
CFLAGS = -I $(INC) -I $(LIBFT)
LDFLAGS = -L $(LIBFT) -lft -lreadline

# Nom de l'exécutable
NAME = minishell

# Règle principale
$(NAME): $(OBJECTS) $(LIBFT)/libft.a
	$(CC) $(FLAGS) $(OBJECTS) $(LDFLAGS) -o $(NAME)

# Compilation des fichiers .c en .o
$(OBJ)/%.o: $(SRC)/%.c | $(OBJ)
	$(CC) $(FLAGS) $(CFLAGS) -c $< -o $@

# Création du dossier obj s'il n'existe pas
$(OBJ):
	mkdir -p $(OBJ)

# Compilation de libft
$(LIBFT)/libft.a:
	$(MAKE) -C $(LIBFT)

# Nettoyage des fichiers objets et de l'exécutable
clean:
	$(RM) $(OBJECTS)
	$(MAKE) -C $(LIBFT) clean

fclean: clean
	$(RM) $(NAME)
	$(RM) -r $(OBJ)
	$(MAKE) -C $(LIBFT) fclean

# Réinitialisation complète
re: fclean $(NAME)

# Ignore les fichiers temporaires
.PHONY: all clean fclean re

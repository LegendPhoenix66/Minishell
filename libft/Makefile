# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -Werror

# Header file
HEADER = libft.h

# Source files for the mandatory part
SRCS = ft_atoi.c ft_bzero.c ft_calloc.c ft_isalnum.c ft_isalpha.c ft_isascii.c \
ft_isdigit.c ft_isprint.c ft_itoa.c ft_memchr.c ft_memcmp.c ft_memcpy.c \
ft_memmove.c ft_memset.c ft_putchar_fd.c ft_putstr_fd.c ft_putnbr_fd.c\
ft_split.c ft_strchr.c ft_strdup.c ft_striteri.c ft_strjoin.c \
ft_strlcat.c ft_strlcpy.c ft_strlen.c ft_strmapi.c ft_strncmp.c ft_strnstr.c \
ft_strrchr.c ft_strtrim.c ft_substr.c ft_tolower.c ft_toupper.c ft_putendl_fd.c

# Source files for the bonus part
BONUS_SRCS = ft_lstadd_back_bonus.c ft_lstadd_front_bonus.c ft_lstclear_bonus.c \
ft_lstdelone_bonus.c ft_lstiter_bonus.c ft_lstlast_bonus.c ft_lstmap_bonus.c \
ft_lstnew_bonus.c ft_lstsize_bonus.c

# Subfolder for object files
OBJDIR = obj/

# Object files for the mandatory part
OBJS = $(addprefix $(OBJDIR),$(notdir $(SRCS:.c=.o)))

# Object files for the bonus part
BONUS_OBJS = $(addprefix $(OBJDIR),$(notdir $(BONUS_SRCS:.c=.o)))

# libft library
LIBFT = libft.a

$(LIBFT): $(OBJS)
	ar rcs $(LIBFT) $(OBJS)

$(NAME): $(OBJS)
	ar rcs $(NAME) $(OBJS)

$(OBJDIR)%.o: %.c
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

all: $(LIBFT) $(NAME)

bonus: $(OBJS) $(BONUS_OBJS)
	ar rcs $(LIBFT) $(OBJS) $(BONUS_OBJS)

clean:
	rm -f $(OBJDIR)*.o

fclean: clean
	rm -f $(LIBFT)

re: fclean all

.PHONY: all clean fclean remake

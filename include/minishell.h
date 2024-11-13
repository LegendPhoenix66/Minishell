/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.42luxembourg.lu>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 19:30:38 by lhopp             #+#    #+#             */
/*   Updated: 2024/09/30 16:52:46 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include "../libft/libft.h"

//for manipulation of environment variable
typedef struct s_node
{
    char *env;
    struct s_node *next;
} t_node;

typedef struct s_args
{
	t_node *env;
	int exit;
	char *current_directory;
} t_args;

void	parse_input(char *input, t_args **args);
void	execute_command(char **command, t_args **args);
t_node  *init_lst(void);
void    free_lst(t_node *top); //free linked list
void    print_lst(t_node **top);//make env function in execute command
int     ft_unsetenv(t_node **env_list, const char *var);//unset function

char	*get_next_line(int fd);

#endif //MINISHELL_H

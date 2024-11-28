/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 19:30:38 by lhopp             #+#    #+#             */
/*   Updated: 2024/11/27 12:21:56 by lhopp            ###   ########.fr       */
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
# include <fcntl.h>
# include "../libft/libft.h"

// for manipulation of environment variable
typedef struct s_node
{
	char *content;
	int in_single;
	int in_double;
	int no_quotes;
	int is_cmd;
	struct s_node *next;
} t_node;

typedef struct s_args
{
	t_node			*env;
	int				exit;
	char			*current_directory;
	t_node			*tokens;
}					t_args;

void				parse_input(char *input, t_args **args);
void				execute_command(t_args **args);
t_node				*init_lst(void);
void				free_lst(t_node *top);
void				print_lst(t_node **top);
void				print_list_debug(t_node **top);
int					ft_unsetenv(t_node **env_list, const char *var);
char				*get_next_line(int fd);
void				sort_lst(t_node **top);
t_node				*copy_list(t_node *original);
void				add_node(t_node **top, const char *env);
int					find_equal(const char *var);
void				remove_if(t_node **top, const char *var_name);
int					ft_export(const char *var, t_args **args);
void				tokenize_input(const char *input, t_args **args);
void				name_token(t_node **top);
void				no_quotes(t_node **top);

#endif // MINISHELL_H

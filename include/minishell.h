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

typedef enum e_dir
{
	STDOUT, //>
	APPEND, //>>
	STDIN,  //<
	HEREDOC, //<<
	NO_DIR
} t_dir;

// for manipulation of environment variable
typedef struct s_node
{
	char *content;
	int in_single;
	int in_double;
	int no_quotes;
	int is_cmd;
	int is_out;
	t_dir type;
	struct s_node *next;
} t_node;

typedef struct s_shell
{
	char			**environ;
	t_node			*env;
	int				exit;
	char			*current_directory;
	t_list			*tokens;
	t_node			*export;
}					t_shell;

void				parse_input(char *input, t_shell *args);
void				execute_command(t_shell *args);
t_node				*init_lst(void);
void				free_lst(t_node *top);
void				print_lst(t_node **top);
void				print_list_debug(t_list **top);
char				*get_next_line(int fd);
void				sort_lst(t_node **top);
t_node				*copy_list(t_node *original);
void				add_node(t_node **top, const char *env);
int					find_equal(const char *var);
void				remove_if(t_node **top, const char *var_name);
//void				remove_if(t_list **top, const char *var_name);
int					ft_export(const char *var, t_shell *args);
void				tokenize_input(const char *input, t_shell *args);
void				debug_list(t_node **head);
char				*find_command_in_path(char *cmd);
void 				is_cmd(t_node **top);
void				parse_redirections(t_node **top);
void				tokenize_input1(t_shell *args);

// builtins
void				builtin_echo(const t_list *tokens);
int					builtin_unset(t_node **env_list, const char *var);
int					builtin_export(t_shell *args);

#endif // MINISHELL_H

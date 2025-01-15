/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 19:30:38 by lhopp             #+#    #+#             */
/*   Updated: 2025/01/15 13:50:12 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>
# define COLOR_RESET "\033[0m"
# define COLOR_GREEN "\033[0;32m"

typedef enum e_dir
{
	STDOUT,
	APPEND,
	STDIN,
	HEREDOC,
	NO_DIR
}					t_dir;

typedef struct s_redir
{
	t_dir			type;
	char			*file;
	int				fd;
}					t_redir;

typedef struct s_cmd
{
	char			**args;
	char			*input_file;
	char			*output_file;
	int				input_fd;
	int				output_fd;
	int				input_mode; // 0: stdin, 1: file <, 2: heredoc <<
	int				output_mode; // 0: stdout, 1: file >, 2: file >>
}					t_cmd;

typedef enum e_builtin_type
{
	BUILTIN_EXIT,
	BUILTIN_CD,
	BUILTIN_PWD,
	BUILTIN_ECHO,
	BUILTIN_ENV,
	BUILTIN_UNSET,
	BUILTIN_EXPORT,
	BUILTIN_NONE
}					t_builtin_type;

// for manipulation of environment variable
typedef struct s_node
{
	char			*content;
	int				in_single;
	int				in_double;
	int				no_quotes;
	int				is_cmd;
	int				is_out;
	t_dir			type;
	struct s_node	*next;
}					t_node;

typedef struct s_shell
{
	char			**environ;
	t_node			*env;
	int				exit;
	char			*current_directory;
	t_list			*tmp_tokens;
	t_list			*tokens;
	t_list			*tokens1;
	t_node			*export;
	int				saved_stdin;
	int				saved_stdout;
	t_redir			*redirections;
	int				last_status;
}					t_shell;

typedef struct s_context
{
	char			**new_content;
	int				*output_index;
	int				last_status;
}					t_context;

typedef struct s_p
{
	int				pipefd[2];
	pid_t			pid;
	int				status;
	int				input_fd;
	int				is_last_command;
	int				saved_stdin;
	int				fd;
	char			*exec_path;
	int				ret;
}					t_p;

// main
char				*get_input(void);
void				set_upsignals(void);
t_shell				*initialize_shell(char **env);

// handle heredoc
t_cmd				*init_cmd(void);
void				free_cmd(t_cmd *cmd);
int					is_builtin(const char *token);
void				add_arg(t_cmd *cmd, char *arg);
void				handle_heredoc(t_cmd *cmd, char *delimiter);
int					ft_strcmp(char *str1, char *str2);
int					setup_heredoc_pipe(int pipe_fd[2]);
void				process_heredoc_input(int fd, char *delimiter);
void				write_heredoc_line(int fd, const char *line);
int					process_heredoc_line(char *line, char *delimiter, int fd);

// parse red and pipe
t_cmd				*parse_command(t_list *tokens);
int					handle_input(t_cmd *cmd, t_list **tokens);
int					handle_output(t_cmd *cmd, t_list **tokens);
int					handle_append(t_cmd *cmd, t_list **tokens);
int					handle_heredoc1(t_cmd *cmd, t_list **tokens);

// find executable
char				*find_executable(const char *command);

// execute_simple_command
void				execute_simple_command(t_cmd *cmd, t_shell *shell);

// execute_pipeline
void				execute_pipeline(t_shell *shell);
void				handle_child_process(t_p *data, t_cmd *cmd, t_shell *shell);
void				initialize_pipeline_data(t_p *data);
void				process_tokens1(t_list **tokens, t_p *data);
int					create_pipe(t_p *data, t_cmd *cmd);
void				handle_parent_process(t_p *data, t_shell *shell);

void				parse_input(char *input, t_shell *shell);
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
t_list				*tokenize_input(const char *input, int last_status);
void				debug_list(t_node **head);
char				*find_command_in_path(char *cmd);
t_list				*remove_quotes_and_substitue_variables1(const char *input,
						t_shell *args);
t_list				*add_token(t_list **list, const char *token, int length);
void				error(const char *msg);
t_list				*is_a_redirecton(t_shell *args);
void				save_std_fds(int *saved_stdin, int *saved_stdout);
int					handle_output_redir(char *file, int flags);
void				restore_std_fds(int saved_stdin, int saved_stdout);
void				execute_external_command1(t_shell *args);
int					is_redir(char *node_content);
int					handle_redirection(t_list *token);
int					count_node(t_list **top);
void				execute_command1(t_shell *shell);
int					execute_builtin(t_cmd *cmd, t_shell *shell);

// builtins
int					builtin_echo(t_cmd *cmd);
int					builtin_unset(t_node **env_list, const char *var);
int					builtin_export(t_shell *shell, char **args);
int					builtin_exit(t_shell *shell, t_cmd *cmd);
int					builtin_pwd(void);
int					builtin_cd(t_shell *shell, t_cmd *cmd);

#endif

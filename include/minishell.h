/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 19:30:38 by lhopp             #+#    #+#             */
/*   Updated: 2025/01/30 21:27:07 by lhopp            ###   ########.fr       */
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
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
# define COLOR_RESET "\033[0m"
# define COLOR_GREEN "\033[0;32m"
# define ERR_INVALID_IDENTIFIER "minishell: export: `%s': not valid\n"
# define MAX_FD 1024

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
	pid_t			pid;
	char			*delimiter;
	int				fd[2];
}					t_redir;

typedef struct s_heredoc
{
	int				pipe_fd[2];
	pid_t			process_pid;
	char			*delimiter;
}					t_heredoc;

typedef struct s_cmd
{
	char			**args;
	char			*input_file;
	char			*output_file;
	char			*delimiter;
	int				input_fd;
	int				output_fd;
	int				input_mode;
	int				output_mode;
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
	t_list			*tokens;
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
	t_shell			*shell;
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

typedef struct s_pipeline_ctx
{
	t_shell			*shell;
	t_p				data;
	t_list			*tokens;
	int				nb_pipe;
	int				nb_child;
	int				**pipes;
	pid_t			*pids;
}					t_pipeline_ctx;

// main
char				*get_input(void);
void				set_upsignals(void);
t_shell				*initialize_shell(char **env);
size_t				calculate_size(char **elements);
int					ft_isspace(int c);

// handle heredoc
t_cmd				*init_cmd(void);
void				free_cmd(t_cmd *cmd);
int					is_builtin(char *token);
void				add_arg(t_cmd *cmd, char *arg, t_shell *shell);
void				handle_heredoc(t_cmd *cmd, char *delimiter);
int					ft_strcmp(char *str1, char *str2);
int					setup_heredoc_pipe(int pipe_fd[2]);
void				process_heredoc_input(int fd, char *delimiter);
void				write_heredoc_line(int fd, const char *line);
int					process_heredoc_line(char *line, char *delimiter, int fd);
int					check_for_heredoc(t_list *tokens);
void				execute_heredoc(t_heredoc *data, t_cmd *cmd,
						t_shell *shell);
t_list				*cmd_befor_heredoc(t_list **top);
t_list				*cmd_after_heredoc(t_list **top);
t_list				*last_token(t_list **top);

// parse red and pipe
t_cmd				*parse_command(t_shell *shell, t_list *tokens);
int					handle_input(t_cmd *cmd, t_list **tokens);
int					handle_output(t_cmd *cmd, t_list **tokens);
int					handle_append(t_cmd *cmd, t_list **tokens);
int					handle_heredoc1(t_cmd *cmd, t_list **tokens);

// find executable
char				*find_executable(t_shell *shell, const char *command);

// execute_simple_command and simple_cmd_utils
int					execute_simple_command(t_cmd *cmd, t_shell *shell);
void				wait_for_child(pid_t pid, int *status, t_shell *shell);
void				handle_io_redirection(t_cmd *cmd);
void				execute_command(t_cmd *cmd, t_shell *shell);
int					setup_redirection(t_cmd *cmd, int mode, int target_fd);

// execute_pipeline
void				execute_pipeline(t_shell *shell);
void				handle_child_process(t_p *data, t_cmd *cmd, t_shell *shell);
void				initialize_pipeline_data(t_p *data);
void				process_tokens1(t_list **tokens, t_p *data);
int					initialize_pipes_and_pids(t_pipeline_ctx *ctx);
t_cmd				*process_command1(t_p *data, t_list **tokens,
						t_shell *shell);
char				*get_input1(void);
int					process_child(t_pipeline_ctx *ctx, int i);
void				initialize_context(t_pipeline_ctx *ctx, t_shell *shell);
void				handle_additional_input(t_pipeline_ctx *ctx);

// finalize_pipeline
void				close_pipes(int **pipes, int nb_pipe);
void				free_pipe_array(int **pipe_array, int pipe_count);
void				free_pid_array(pid_t *pid_array);
void				finalize_pipeline(t_pipeline_ctx *ctx);

// cd_utils
char				*get_cd_path(t_shell *shell, t_cmd *cmd);
int					execute_cd(char *path, t_shell *shell);
void				update_directory(t_shell *shell);
int					set_error(t_shell *shell, int status);

// export_utils
int					check_in(char *var, t_shell *shell);
void				print_export(t_shell **args);

// get_input and get_input_utils
void				trim_and_remove_whitespace(char *str);

// get_next_line and gnl_utils.c
char				*get_next_line(int fd);
void				append_line(char **line, const char *buffer);

// linked_utils and linked_utils1
int					count_node(t_list **top);
t_node				*init_lst(void);
void				print_lst(t_node **top);
t_node				*copy_list(t_node *original);
void				sort_lst(t_node **top);
void				remove_if(t_node **top, const char *var_name);
void				free_lst(t_node *top);
void				add_node(t_node **top, const char *env);
char				*get_node(t_node **top, const char *var_name);

// tokenize fonctions
void				error(const char *msg);
t_list				*add_token(t_list **list, const char *token, int length);
t_list				*tokenize_input(const char *input);
void				ft_realloc(void **ptr, size_t old_size, size_t new_size);
void				append_to_new_content(char **new_content, int *output_index,
						const char *str);
void				append_variable_value(t_context *ctx, const char *value);
void				process_variable_substitution(const char *content,
						int *index, t_context *ctx);
void				process_quoted_content(const char *content, int *index,
						t_context *ctx, char quote);
void				correct_pipes_and_redirects(t_list **parsed_tokens);
t_list				*split_by_spaces(const char *input);
char				*clean_arg(char *token, t_shell *shell);

// parse_input
void				parse_input(char *input, t_shell *shell);
char				*find_command_in_path(t_shell *shell, char *cmd);

// execute_cmd
void				execute_command1(t_shell *shell);
int					execute_builtin(t_cmd *cmd, t_shell *shell);

// print_export
void				print_export(t_shell **args);

// builtins
int					builtin_echo(t_cmd *cmd);
int					builtin_unset(t_node **env_list, const char *var);
int					builtin_export(t_shell *shell, char **args);
int					builtin_exit(t_shell *shell, t_cmd *cmd);
int					builtin_pwd(t_shell *shell);
int					builtin_cd(t_shell *shell, t_cmd *cmd);

#endif

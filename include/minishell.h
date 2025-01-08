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
# define COLOR_RESET "\033[0m"
# define COLOR_GREEN "\033[0;32m"

typedef enum e_dir
{
	STDOUT, //>
	APPEND, //>>
	STDIN,  //<
	HEREDOC, //<<
	NO_DIR
} t_dir;

typedef struct s_redir
{
    t_dir type;
    char *file;
    int fd;
} t_redir;

typedef struct s_cmd 
{
	char **args; //tableau de chaine pour les arg de la cmd
	char *input_file; //Nom  du fichier pour <
	char *output_file; //Nom du fichier pour > ou >>
	int input_fd; //descripteur de fichier pour l' entrer
	int output_fd; //descripteur de fichier pour la sortie
	int input_mode; // 0: stdin, 1: file <, 2: heredoc <<
	int output_mode; // 0: stdout, 1: file >, 2: file >>
} t_cmd;


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
} t_builtin_type;

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
	t_list			*tmp_tokens; //to jump from split by space to pipe... and remove quote...whithout leaks
	t_list			*tokens; //the final token_list a the end of your fonctions
	t_list			*tokens1; //to jump from tokens you did to split_var_and_varname without leaks(now is new final linked list)
	t_node			*export;
	int				saved_stdin;
	int				saved_stdout;
	t_redir			*redirections;
	int				last_status;
}					t_shell;

char				*get_input(void);
void				set_upsignals(void);
t_shell				*initialize_shell(char **env);


void				execute_command(t_shell *args);
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
int					ft_export(const char *var, t_shell *args);
t_list				*tokenize_input(const char *input, int last_status);
void				debug_list(t_node **head);
char				*find_command_in_path(char *cmd);
void 				is_cmd(t_node **top);
void				parse_redirections(t_node **top);
t_list 				*remove_quotes_and_substitue_variables1(const char *input, t_shell *args);
t_list				*add_token(t_list **list, const char *token, int length);
void				error(const char *msg);
t_list				*split_var_and_varname(const char *input, t_shell *args);
t_list              *is_a_redirecton(t_shell *args);
void				save_std_fds(int *saved_stdin, int *saved_stdout);
int					handle_output_redir(char *file, int flags);
void				restore_std_fds(int saved_stdin, int saved_stdout);
void				execute_external_command1(t_shell *args);
int					is_redir(char *node_content);
int					handle_redirection(t_list *token);
int					count_node(t_list **top);
void				execute_command1(t_shell *shell);
int				execute_builtin(t_cmd *cmd, t_shell *shell);

// builtins
//void				builtin_echo(const t_list *tokens);
int					builtin_echo(t_cmd *cmd);
//void				builtin_exit(t_shell *args);
int					builtin_unset(t_node **env_list, const char *var);
//int					builtin_export(t_shell *args);
int					builtin_export(t_shell *shell, char **args);
int					builtin_exit(t_shell *shell, t_cmd *cmd);
int					builtin_pwd();
int					builtin_cd(t_shell *shell, t_cmd *cmd);

#endif // MINISHELL_H

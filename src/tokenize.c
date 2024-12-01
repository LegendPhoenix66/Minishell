/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 15:49:31 by lhopp             #+#    #+#             */
/*   Updated: 2024/11/27 11:31:23 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void init_node(t_node *node)
{
    if (!node)
        return;
    node->content = NULL;
    node->is_cmd = 0;
    node->is_out = 0;
    node->in_single = 0;
    node->in_double = 0;
    node->no_quotes = 0;
    node->type = NO_DIR;
}
void	add_token(t_list **list, const char *token, int length)
{
	t_list	*new_node;

	new_node = ft_lstnew(strndup(token, length));
	ft_lstadd_back(list, new_node);
}

void error(const char *msg)
{
	fprintf(stderr, "Error: %s\n", msg);
	exit(EXIT_FAILURE);
}

void handle_quotes(const char *input, int *i, t_list **tokens) {
	char quote = input[*i];
	int start = *i + 1;
	(*i)++;
	while (input[*i] && input[*i] != quote) {
		(*i)++;
	}
	if (input[*i] != quote) {
		error("Unmatched quote");
	}
	add_token(tokens, input + start, *i - start);
	(*i)++;
}

void tokenize_input(const char *input, t_args **args)
{
	int i = 0;
	int j = 0;
	(*args)->tokens = NULL;

	while (input[i])
	{
		if (input[i] == '\'' || input[i] == '\"')
		{
			if (j > 0)
			{
				add_token(&(*args)->tokens, input + i - j, j);
				j = 0;
			}
			handle_quotes(input, &i, &(*args)->tokens);
		}
		else if (input[i] == ' ' || input[i] == '|' || input[i] == '<' || input[i] == '>')
		{
			if (j > 0)
			{
				add_token(&(*args)->tokens, input + i - j, j);
				j = 0;
			}
			if (input[i] != ' ')
			{
				add_token(&(*args)->tokens, input + i, 1);
			}
			i++;
		}
		else
		{
			j++;
			i++;
		}
	}
	if (j > 0)
	{
		add_token(&(*args)->tokens, input + i - j, j);
	}
}

void tokenize_input2(const char *input, t_args **args)
{
	args = args;
	t_list *parsed_tokens = NULL;
	int i = 0;
	while (input[i]) {
		if (input[i] == ' ') {
			add_token(&parsed_tokens, input, i);
			input += i + 1;
			i = 0;
		} else if (input[i] == '\"' || input[i] == '\'') {
			char quote = input[i];
			int j = i + 1;
			while (input[j] && input[j] != quote) {
				j++;
			}
			if (!input[j]) {
				error("Unmatched quote");
			}
			i = j + 1;
		} else {
			i++;
		}
	}
	add_token(&parsed_tokens, input, i);

	// check every token for pipes or redirects (not in quotes)
	t_list *tokens_with_pipes = NULL;
	t_list *current = parsed_tokens;
	while (current) {
		i = 0;
		int pipe_pos = -1;
		char *content = current->content;
		while (content[i]) {
			if (content[i] == '\"' || content[i] == '\'') {
				char quote = content[i];
				i++;
				while (content[i] != quote) {
					i++;
				}
				i++;
			}
			if (content[i] == '|') {
				if (ft_strlen(current->content) == 1) {
					break;
				}
				add_token(&tokens_with_pipes, current->content, i);
				add_token(&tokens_with_pipes, current->content + i, 1);
				pipe_pos = i;
			}
			if (content[i] == '<' || content[i] == '>') {
				if (content[i + 1] == content[i]) {
					if (ft_strlen(content) == 2) {
						break;
					}
					add_token(&tokens_with_pipes, content, i);
					add_token(&tokens_with_pipes, content + i, 2);
					pipe_pos = i + 1;
					i++;
				} else {
					if (ft_strlen(content) == 1) {
						break;
					}
					add_token(&tokens_with_pipes, content, i);
					add_token(&tokens_with_pipes, content + i, 1);
					pipe_pos = i;
				}
			}
			i++;
		}
		add_token(&tokens_with_pipes, content + pipe_pos + 1, ft_strlen(content) - pipe_pos - 1);
		// add last token
		current = current->next;
	}

	// clean-up quotes and interpret $
	current = tokens_with_pipes;
	while (current) {
		char *new_content = NULL;
		i = 0;
		int j = 0;
		char *content = current->content;
		while (content[i]) {
			if (content[i] == '\"') {
				i++;
				while (content[i] && content[i] != '\"') {
					if (content[i] == '$') {
						// Handle variable substitution
						i++;
						int var_start = i;
						while (content[i] && content[i] != ' ' && content[i] != '\"') {
							i++;
						}
						char var_name[i - var_start + 1];
						strncpy(var_name, &content[var_start], i - var_start);
						var_name[i - var_start] = '\0';
						char *var_value = getenv(var_name);
						if (var_value) {
							new_content = realloc(new_content, j + strlen(var_value) + 1);
							strcpy(&new_content[j], var_value);
							j += strlen(var_value);
						}
					} else {
						new_content = realloc(new_content, j + 2);
						new_content[j++] = content[i++];
					}
				}
				i++;
			} else if (content[i] == '\'') {
				i++;
				while (content[i] && content[i] != '\'') {
					new_content = realloc(new_content, j + 2);
					new_content[j++] = content[i++];
				}
				i++;
			} else if (content[i] == '$') {
				// Handle variable substitution
				i++;
				int var_start = i;
				while (content[i] && content[i] != ' ' && content[i] != '\"' && content[i] != '\'') {
					i++;
				}
				char var_name[i - var_start + 1];
				strncpy(var_name, &content[var_start], i - var_start);
				var_name[i - var_start] = '\0';
				char *var_value = getenv(var_name);
				if (var_value) {
					new_content = realloc(new_content, j + strlen(var_value) + 1);
					strcpy(&new_content[j], var_value);
					j += strlen(var_value);
				}
			} else {
				new_content = realloc(new_content, j + 2);
				new_content[j++] = content[i++];
			}
		}
		if (new_content) {
			new_content[j] = '\0';
			free(current->content);
			current->content = new_content;
		}
		current = current->next;
	}

	(*args)->tokens = tokens_with_pipes;
}

//this fonction just tell me if is a shell cmd or not
//and if it is put the good variable at 1.
void is_cmd(t_node **top)
{
	t_node *current;
	char *is_cmd;

	current = *top;
	while (current != NULL)
	{
		is_cmd = find_command_in_path(current->content);
		if (is_cmd == NULL)
			current = current->next;
		else
		{
			current->is_cmd = 1;
			current = current->next;
		}
	}
}

// void parse_redirections(t_node **top)
// {
//     t_node *current;
//
// 	current = *top;
//     while (current != NULL)
//     {
//         if (strcmp(current->content, ">") == 0)
//             current->type = STDOUT;
//         else if (strcmp(current->content, ">>") == 0)
//             current->type = APPEND;
//         else if (strcmp(current->content, "<") == 0)
//             current->type = STDIN;
//         else if (strcmp(current->content, "<<") == 0)
//             current->type = HEREDOC;
//         else
//             current->type = NO_DIR;
//         current = current->next;
//     }
// }

//give back a pointer to the last element
char *find_output(t_node **top)
{
    t_node *current;
    char *out;

    current = *top;
    while (current != NULL)
    {
        if(current->type != NO_DIR && current->next != NULL)
        {
            current->next->is_out = 1;
             out = current->next->content;
        }
                current = current->next;
    }
    return (out);
}

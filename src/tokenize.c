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
void error(const char *msg)
{
	fprintf(stderr, "Error: %s\n", msg);
	exit(EXIT_FAILURE);
}

t_list	*add_token(t_list **list, const char *token, int length)
{
	t_list	*new_node;

	new_node = ft_lstnew(strndup(token, length));
	if( new_node == NULL)
	{
		error("malloc token");
		return (NULL);
	}
	ft_lstadd_back(list, new_node);
	return (new_node);
}

void tokenize_input(const char *input, t_shell *args)
{
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
		while (content[i]) 
		{
			if (content[i] == '\"') 
			{
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
			} 
			else if (content[i] == '\'')
			{
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

	args->tokens = tokens_with_pipes;
}

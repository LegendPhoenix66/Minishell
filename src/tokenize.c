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

void	add_token(t_node **list, const char *token, int length)
{
	t_node	*new_node;
	t_node	*current;

	new_node = malloc(sizeof(t_node));
	if (!new_node) {
		perror("malloc error");
		exit(EXIT_FAILURE);
	}
	new_node->content = strndup(token, length);
	new_node->next = NULL;
	if (*list == NULL)
	{
		*list = new_node;
	}
	else
	{
		current = *list;
		while (current->next != NULL)
		{
			current = current->next;
		}
		current->next = new_node;
	}
}

void error(const char *msg)
{
	fprintf(stderr, "Error: %s\n", msg);
	exit(EXIT_FAILURE);
}

void handle_quotes(const char *input, int *i, t_node **tokens) {
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
	t_node *parsed_tokens = NULL;
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
	t_node *tokens_with_pipes = NULL;
	t_node *current = parsed_tokens;
	while (current) {
		i = 0;
		int pipe_pos = -1;
		while (current->content[i]) {
			if (current->content[i] == '\"' || current->content[i] == '\'') {
				char quote = current->content[i];
				i++;
				while (current->content[i] != quote) {
					i++;
				}
				i++;
			}
			if (current->content[i] == '|') {
				if (ft_strlen(current->content) == 1) {
					break;
				}
				add_token(&tokens_with_pipes, current->content, i);
				add_token(&tokens_with_pipes, current->content + i, 1);
				pipe_pos = i;
			}
			if (current->content[i] == '<' || current->content[i] == '>') {
				if (current->content[i + 1] == current->content[i]) {
					if (ft_strlen(current->content) == 2) {
						break;
					}
					add_token(&tokens_with_pipes, current->content, i);
					add_token(&tokens_with_pipes, current->content + i, 2);
					pipe_pos = i + 1;
					i++;
				} else {
					if (ft_strlen(current->content) == 1) {
						break;
					}
					add_token(&tokens_with_pipes, current->content, i);
					add_token(&tokens_with_pipes, current->content + i, 1);
					pipe_pos = i;
				}
			}
			i++;
		}
		add_token(&tokens_with_pipes, current->content + pipe_pos + 1, ft_strlen(current->content) - pipe_pos - 1);
		// add last token
		current = current->next;
	}

	// clean-up quotes and interpret $
	current = tokens_with_pipes;
	while (current) {
		char *new_content = NULL;
		i = 0;
		int j = 0;
		while (current->content[i]) {
			if (current->content[i] == '\"') {
				i++;
				while (current->content[i] && current->content[i] != '\"') {
					if (current->content[i] == '$') {
						// Handle variable substitution
						i++;
						int var_start = i;
						while (current->content[i] && current->content[i] != ' ' && current->content[i] != '\"') {
							i++;
						}
						char var_name[i - var_start + 1];
						strncpy(var_name, &current->content[var_start], i - var_start);
						var_name[i - var_start] = '\0';
						char *var_value = getenv(var_name);
						if (var_value) {
							new_content = realloc(new_content, j + strlen(var_value) + 1);
							strcpy(&new_content[j], var_value);
							j += strlen(var_value);
						}
					} else {
						new_content = realloc(new_content, j + 2);
						new_content[j++] = current->content[i++];
					}
				}
				i++;
			} else if (current->content[i] == '\'') {
				i++;
				while (current->content[i] && current->content[i] != '\'') {
					new_content = realloc(new_content, j + 2);
					new_content[j++] = current->content[i++];
				}
				i++;
			} else if (current->content[i] == '$') {
				// Handle variable substitution
				i++;
				int var_start = i;
				while (current->content[i] && current->content[i] != ' ' && current->content[i] != '\"' && current->content[i] != '\'') {
					i++;
				}
				char var_name[i - var_start + 1];
				strncpy(var_name, &current->content[var_start], i - var_start);
				var_name[i - var_start] = '\0';
				char *var_value = getenv(var_name);
				if (var_value) {
					new_content = realloc(new_content, j + strlen(var_value) + 1);
					strcpy(&new_content[j], var_value);
					j += strlen(var_value);
				}
			} else {
				new_content = realloc(new_content, j + 2);
				new_content[j++] = current->content[i++];
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

//tell if in single or double quotes
void name_token(t_node **top)
{
	t_node *current;

	current = *top;
	while (current != NULL)
	{
		if(current->content[0] == '\"' && current->content[ft_strlen(current->content) - 1] == '\"')
		{
			current->in_double = 1;
			printf("variable in double quotes\n");
		}
		if(current->content[0] == '\'' && current->content[ft_strlen(current->content) - 1] == '\'')
		{
			current->in_single = 1;
			printf("variable in single quotes\n");
		}
		else
		{
			current->no_quotes = 1;
			printf("no quotes around\n");
		}
		current = current->next;	
	}
}
void no_quotes(t_node **top)
{
    t_node *current;
	char *new_content;

    current = *top;
    while (current != NULL)
    {
        if (current->content[0] == '\"' && current->content[strlen(current->content) - 1] == '\"')
        {
            new_content = ft_strdup(current->content + 1);
            new_content[strlen(new_content) - 1] = '\0'; 
            free(current->content);
            current->content = new_content; 
        }
        else if (current->content[0] == '\'' && current->content[strlen(current->content) - 1] == '\'')
        {
            new_content = ft_strdup(current->content + 1); 
            new_content[strlen(new_content) - 1] = '\0';      
            free(current->content);
            current->content = new_content;
        }
        current = current->next;
    }
}
//don' work for the moment
void is_cmd(t_node **top)
{
	t_node *current;
	char *is_cmd;

	current = *top;
	while (current != NULL)
	{
		is_cmd = find_commande_in_path(current->content);
		if (is_cmd == NULL)
			current = current->next;
		else
		{
			current->is_cmd = 1;
			current = current->next;
		}
	}
}

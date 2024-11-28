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
	t_node *temp_tokens = NULL;
	int i = 0;
	while (input[i]) {
		if (input[i] == ' ') {
			add_token(&temp_tokens, input, i);
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
	add_token(&temp_tokens, input, i);
	(*args)->tokens = temp_tokens;
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

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

void tokenize_input(const char *input, t_args **args)
{
	char quote;
	int i, j, start;

	i = 0;
	j = 0;
	quote = 0;
	(*args)->tokens = NULL;
	while (input[i])
	{
		if (input[i] == '\'' || input[i] == '\"')
		{
			quote = input[i];
			start = i;
			i++;
			while (input[i] && input[i] != quote)
				i++;
			if (input[i] != quote)
			{
				error("Unmatched quote");
				return;
			}
			add_token(&(*args)->tokens, input + start, i - start + 1);
			i++;
			j = 0;
		}
		else if ((input[i] == ' ' && quote == 0) || input[i] == '|' || input[i] == '<' || input[i] == '>')
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
//tell if in single or double quotes or no quotes
// and them put good variable at 1 in structure
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
//now we know wath is in quotes or not them we can remove quotes
//for tokenize the rest
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
void parse_redirections(t_node **top)
{
    t_node *current;

	current = *top;
    while (current != NULL)
    {
        if (strcmp(current->content, ">") == 0)
            current->type = STDOUT;
        else if (strcmp(current->content, ">>") == 0)
            current->type = APPEND;
        else if (strcmp(current->content, "<") == 0)
            current->type = STDIN;
        else if (strcmp(current->content, "<<") == 0)
            current->type = HEREDOC;
        else
            current->type = NO_DIR;
        current = current->next;
    }
}


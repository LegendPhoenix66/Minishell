/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drenquin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 11:49:50 by drenquin          #+#    #+#             */
/*   Updated: 2024/12/09 11:49:50 by drenquin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

//that fonction manage space and unmatched quote
t_list *split_by_spaces1(const char *input, t_shell *args)
{
    t_list *parsed_tokens;
    int i;
    char quote;
    int j;

    parsed_tokens = NULL;
    i = 0;
    j = 0;
    quote = 0;

    while (input[i])
    {
        if(input[i] == ' ')
        {
            //printf("malloc space\n");
            add_token(&parsed_tokens, input, i);
            input += i + 1;
            i = 0;
        }
        else if (input[i] == '\"' || input[i] == '\'')
        {
            quote = input[i];
            j = i + 1;
            while (input[j] && input[j] != quote)
                j++;
            if(!input[j])
                error("unmatched quote");
            i = j + 1;
        }
        else
            i++;
    }
    //printf("add at end 1\n");
    add_token(&parsed_tokens, input, i);
    args->tmp_tokens = parsed_tokens;
    return (args->tmp_tokens);
}
//check every token for pipe or redirects (not in quotes)
t_list *correct_pipes_and_redirects1(const char *input, t_shell *args)
{
    t_list *current;
    int pipe_pos;
    char *content;
    int i;
    char quote;

    current = split_by_spaces1(input, args);
    quote = 0;
    while (current)
    {
        i = 0;
        pipe_pos = -1;
        content = current->content;
        while (content[i])
        {
            if(content[i] == '\"' || content[i] == '\'')
            {
                quote = content[i];
                i++;
                while (content[i] != quote)
                    i++;
                i++;
            }
            if(content[i] == '|')
            {
                if(ft_strlen(current->content) == 1)
                    break;
                //printf("malloc pipe\n");
                add_token(&(args->tokens), current->content, i);
                add_token(&(args->tokens), current->content + i, 1);
                pipe_pos = i;
            }
            if(content[i] == '<' || content[i] == '>')
            {
                if(content[i + 1] == content[i])
                {
                    if(ft_strlen(content) == 2)
                        break;
                    printf("malloc rederirection if\n");
                    add_token(&(args->tokens), content, i);
                    add_token(&(args->tokens), content + i, 2);
					pipe_pos = i + 1;
					i++;
                }
                else
                {
                    if(ft_strlen(content) == 1)
                        break;
                    printf("malloc redirection else\n");
                    add_token(&(args->tokens), content, i);
                    add_token(&(args->tokens), content + i, 1);
				    pipe_pos = i;
                }
            }
            i++;
        }
        printf("add at end 2\n");
        add_token(&(args->tokens), content + pipe_pos + 1, ft_strlen(content) - pipe_pos - 1);
        current = current->next;
    }
    return (args->tokens);
}
// Function to remove quotes and substitute variables
t_list *remove_quotes_and_substitue_variables1(const char *input, t_shell *args)
{
    t_list *current;
    char *new_content;
    char *content;
    int i;
    int j;
    int var_start;
    char var_name[256];
    char *var_value;

    current = correct_pipes_and_redirects1(input, args);
    while (current)
    {
        new_content = NULL;
        i = 0; 
        j = 0; 
        content = current->content;
        while (content[i])
        {
            if (content[i] == '\"') 
            {
                i++;
                while (content[i] && content[i] != '\"') 
                {
                    if (content[i] == '$')
                    {
                        i++;
                        var_start = i;
                        while (content[i] && content[i] != ' ' && content[i] != '\"')
                            i++;
                        strncpy(var_name, &content[var_start], i - var_start);
                        var_name[i - var_start] = '\0';
                        var_value = getenv(var_name);
                        if (var_value) 
                        {
                            new_content = realloc(new_content, j + ft_strlen(var_value) + 1);
                            strcpy(&new_content[j], var_value);
                            j += ft_strlen(var_value);
                        }
                    }
                    else 
                    {
                        new_content = realloc(new_content, j + 2);
                        new_content[j++] = content[i++];
                    }
                }
                i++;
            }
            else if (content[i] == '\'')
            {
                i++;
                while (content[i] && content[i] != '\'') 
                {
                    new_content = realloc(new_content, j + 2);
                    new_content[j++] = content[i++];
                }
                i++;
            }
            else if (content[i] == '$')
            {
                i++;
                var_start = i;
                while (content[i] && content[i] != ' ' && content[i] != '\"' && content[i] != '\'')
                    i++;
                strncpy(var_name, &content[var_start], i - var_start);
                var_name[i - var_start] = '\0';
                var_value = getenv(var_name);
                if (var_value) 
                {
                    new_content = realloc(new_content, j + ft_strlen(var_value) + 1);
                    strcpy(&new_content[j], var_value);
                    j += ft_strlen(var_value);
                }
            }
            else
            {
                new_content = realloc(new_content, j + 2);
                new_content[j++] = content[i++];
            }
        }
        if (new_content) 
        {
            new_content[j] = '\0';
            free(current->content);
            current->content = new_content;
        }
        current = current->next;
    }
    return (args->tokens);
}
//tokenize_input tokenize export salut=hello in 2 nodes
//tokenize_input1 tokenize that in 4 nodes
//need to take care tokenize make leaks ans tokenize1 also for the moment 
t_list *split_var_and_varname(const char *input, t_shell *args)  
{
    t_list *current;
    int equal_pos;

    current = remove_quotes_and_substitue_variables1(input, args);
    while (current != NULL)
	{
        equal_pos = find_equal(current->content);
        if (equal_pos != -1)
		{
            if (equal_pos > 0)
            {
                //printf("pas de valeur\n");
                add_token(&args->tokens1, current->content, equal_pos);
            }
            //printf("ajout du egale\n");
            add_token(&args->tokens1, "=", 1);
            if ((size_t)equal_pos + 1 < ft_strlen(current->content))
			{
                //printf("rajout de la valeur");
                add_token(&args->tokens1, current->content + equal_pos + 1,
                          ft_strlen(current->content) - equal_pos - 1);
            }
        }	
		else 
        {
           //printf("simple ajout d' un token");
            add_token(&args->tokens1, current->content, ft_strlen(current->content));
        }
        current = current->next;
    }
    return(args->tokens1);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 15:06:36 by drenquin          #+#    #+#             */
/*   Updated: 2025/01/30 21:01:26 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	count_pipe(t_list **top)
{
	t_list	*current;
	int		nb_pipe;

	nb_pipe = 0;
	if (top == NULL || *top == NULL)
		return (0);
	current = *top;
	while (current != NULL)
	{
		if (ft_strcmp(current->content, "|") == 0)
			nb_pipe++;
		current = current->next;
	}
	return (nb_pipe);
}

int	count_processes(t_list **tokens)
{
	t_list	*current;
	int		process_count;
	int		has_command_after_pipe;

	if (!tokens)
		return (0);
	process_count = 1;
	has_command_after_pipe = 0;
	current = *tokens;
	while (current)
	{
		if (ft_strcmp(current->content, "|") == 0)
		{
			process_count++;
			has_command_after_pipe = 0;
			if (current->next)
				has_command_after_pipe = 1;
		}
		else if (current->content)
			has_command_after_pipe = 1;
		current = current->next;
	}
	if (!has_command_after_pipe)
		process_count--;
	return (process_count);
}

int	last_is_pipe(t_list **top)
{
	t_list	*current;

	if (top == NULL || *top == NULL)
		return (0);
	current = *top;
	while (current != NULL)
	{
		if (ft_strcmp(current->content, "|") == 0 && current->next == NULL)
			return (1);
		current = current->next;
	}
	return (0);
}

void	handle_additional_input(t_pipeline_ctx *ctx)
{
	const char	*cmd_end;
	t_list		*new_tokens_end;
	t_list		*current_token;

	while (ctx->nb_pipe == ctx->nb_child && last_is_pipe(&ctx->tokens))
	{
		cmd_end = get_input1();
		if (cmd_end == NULL)
			return ;
		new_tokens_end = tokenize_input(cmd_end);
		free((char *)cmd_end);
		current_token = new_tokens_end;
		while (current_token != NULL)
		{
			add_token(&ctx->tokens, current_token->content,
				ft_strlen(current_token->content));
			current_token = current_token->next;
		}
		ft_lstclear(&new_tokens_end, free);
		ctx->nb_pipe = count_pipe(&ctx->tokens);
		ctx->nb_child = count_processes(&ctx->tokens);
	}
}

void	initialize_context(t_pipeline_ctx *ctx, t_shell *shell)
{
	ctx->shell = shell;
	ctx->tokens = shell->tokens;
	initialize_pipeline_data(&ctx->data);
	ctx->nb_pipe = count_pipe(&ctx->tokens);
	ctx->nb_child = count_processes(&ctx->tokens);
}

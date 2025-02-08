/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 15:26:11 by drenquin          #+#    #+#             */
/*   Updated: 2025/02/01 10:53:53 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	**expand_variables(char **tokens, t_context *ctx)
{
	int		i;
	char	*variable;

	i = 0;
	while (tokens[i] != NULL)
	{
		if (tokens[i][0] == '$' && tokens[i][1] != '\0' && tokens[i][1] != '$')
		{
			variable = get_node(&ctx->shell->env, tokens[i] + 1);
			free(tokens[i]);
			tokens[i] = NULL;
			if (variable != NULL)
				tokens[i] = ft_strdup(variable);
			else
				tokens[i] = ft_strdup("");
		}
		i++;
	}
	return (tokens);
}

char	*process_line(char *input, t_context *ctx)
{
	char	**tokens;
	char	*result;

	tokens = tokenize_input_test(input);
	free(input);
	if (!tokens)
		return (NULL);
	tokens = expand_variables(tokens, ctx);
	result = create_string_from_array(tokens);
	free_tokens(tokens);
	return (result);
}

t_context	*init_context(t_shell *shell)
{
	t_context	*ctx;

	ctx = (t_context *)malloc(sizeof(t_context));
	if (!ctx)
		return (NULL);
	ctx->new_content = (char **)malloc(sizeof(char *));
	if (!ctx->new_content)
	{
		free(ctx);
		return (NULL);
	}
	*(ctx->new_content) = NULL;
	ctx->output_index = (int *)malloc(sizeof(int));
	if (!ctx->output_index)
	{
		free(ctx->new_content);
		free(ctx);
		return (NULL);
	}
	*(ctx->output_index) = 0;
	ctx->shell = shell;
	return (ctx);
}

void	cleanup(t_heredoc *data)
{
	if (data)
	{
		close(data->pipe_fd[0]);
		close(data->pipe_fd[1]);
		free(data);
	}
}

void	execute_heredoc(t_heredoc *data, t_cmd *cmd, t_shell *shell)
{
	int	process;

	data = init_pipe_data();
	if (!data)
	{
		perror("failled initialization\n");
		return ;
	}
	process = start_process(data, cmd, shell);
	if (process == 1)
	{
		write_nothing(shell);
		cleanup(data);
		return ;
	}
	if (process == 0)
	{
		write_and_wait(data, shell);
		cleanup(data);
		return ;
	}
}

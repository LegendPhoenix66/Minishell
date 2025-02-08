/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_msg_array.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.1337.ma>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 10:57:12 by lhopp             #+#    #+#             */
/*   Updated: 2025/02/01 10:57:23 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	*cleanup_and_exit(char **messages, t_context *ctx)
{
	if (messages)
		free_message_array(messages);
	if (ctx)
		free_context(ctx);
	return (NULL);
}

void	*cleanup_and_exit1(char **messages, t_context *ctx, char *str)
{
	if (str)
		free(str);
	if (messages)
		free_message_array(messages);
	if (ctx)
		free_context(ctx);
	return (NULL);
}

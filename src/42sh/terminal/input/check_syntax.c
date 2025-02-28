/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 20:45:33 by vzurera-          #+#    #+#             */
/*   Updated: 2025/02/28 18:34:01 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "libft.h"
	#include "terminal/input.h"

#pragma endregion

// //	Set the error message for SOPAR syntax error
// static char	*set_sopar_msg(t_data *data) {
// 	char	*line;
// 	int		i;
// 	int		n;
// 	bool	ignore;

// 	i = -1;
// 	n = 2;
// 	ignore = true;
// 	while (data->input && data->input[++i])
// 	{
// 		if (data->input[i] == '\n' && ignore)
// 			ignore = false;
// 		else if (data->input[i] == '\n')
// 			n++;
// 	}
// 	line = ft_itoa(n);
// 	if (!line)
// 		exit_error(data, NO_MEMORY, 1, NULL);
// 	line = ft_strjoin("-c: line ", line, 2);
// 	line = ft_strjoin(line, ": syntax error: unexpected end of file\n", 1);
// 	return (line);
// }

// //	Print a message if there are syntax errors
// static int	syntax_msg(t_token *token, int error, char *c) {
// 	char	*s;

// 	if (error == PTK)
// 		get_token_type(token->next, &s);
// 	else
// 		get_token_type(token, &s);
// 	print(token->data, 2, "minishell: ", R);
// 	if (error == NL)
// 		print(token->data, 2, \
// 			"syntax error near unexpected token `newline'\n", P);
// 	else if (error == TK || error == PTK)
// 	{
// 		print(token->data, 2, "syntax error near unexpected token `", J);
// 		print(token->data, 2, ft_strjoin(s, "'\n", 0), FP);
// 	}
// 	else if (error == SCPAR)
// 	{
// 		print(token->data, 2, "syntax error near unexpected token `", J);
// 		print(token->data, 2, ft_strjoin(c, "'\n", 0), FP);
// 	}
// 	else if (error == SOPAR)
// 		print(token->data, 2, set_sopar_msg(token->data), FP);
// 	token->data->last_status = 2;
// 	return (1);
// }

// //	Check the syntax of the full command
// static int	check_syntax_2(t_data *data, t_token *current, bool cmd, int paren) {
// 	if (current->type > DGT && current->next && current->next->type > OPAR
// 		&& !(current->type == CPAR && current->next->type > OPAR))
// 		return (syntax_msg(current, PTK, NULL));
// 	else if (!data->extra && current->type > DGT && !current->next
// 		&& !(current->type > OR || current->type == CPAR))
// 		return (syntax_msg(current, TK, NULL));
// 	else if (current->type > OPAR && cmd && current == data->tokens)
// 		return (syntax_msg(current, TK, NULL));
// 	else if (current->type == CMD && current->next
// 		&& current->next->type == OPAR)
// 		return (syntax_msg(current, PTK, NULL));
// 	else if (current->type == CPAR && !paren)
// 		return (syntax_msg(current, TK, NULL));
// 	else if (current->type == CPAR && current->next
// 		&& current->next->type == CMD)
// 		return (syntax_msg(current, SCPAR, current->next->args_lst->arg));
// 	return (0);
// }

// //	Check the syntax of the full command
// static int	check_syntax_1(t_token *current, bool cmd) {
// 	if (current->type > 8 && !current->prev && current->next
// 		&& current->next->type > DGT)
// 		return (syntax_msg(current, TK, NULL));
// 	else if (current->type == PIPE && !current->prev && current->next
// 		&& current->next->type > OR)
// 		return (syntax_msg(current, SCPAR, "|"));
// 	else if (current->type > 0 && current->type < CPAR && cmd && !current->next)
// 		return (syntax_msg(current, NL, NULL));
// 	else if (current->type > CMD && current->type < OPAR
// 		&& current->next && current->next->type == OPAR)
// 		return (syntax_msg(current, PTK, NULL));
// 	else if (current->type > CMD && cmd && current->type < OPAR
// 		&& current->next && current->next->type > CMD)
// 		return (syntax_msg(current, PTK, NULL));
// 	else if (current->type == CMD && current->next
// 		&& current->next->type == OPAR && current->next->next
// 		&& current->next->next->args_lst)
// 		return (syntax_msg(current, SCPAR, current->next->next->args_lst->arg));
// 	return (0);
// }

// //	Check the syntax of the full command
// int	check_syntax() {
// 	t_token	*current;
// 	bool	cmd;
// 	int		paren;

// 	paren = 0;
// 	current = data->tokens;
// 	while (current)
// 	{
// 		cmd = (!current->cmd || !*current->cmd);
// 		if (check_syntax_1(current, cmd))
// 			return (1);
// 		if (check_syntax_2(data, current, cmd, paren))
// 			return (1);
// 		if (current->type == OPAR)
// 			paren++;
// 		if (current->type == CPAR)
// 			paren--;
// 		current = current->next;
// 	}
// 	if (!data->extra && paren)
// 		return (syntax_msg(data->tokens, SOPAR, NULL));
// 	return (0);
// }

int	check_syntax(const char *input, t_context *context) {
	if (!input || !*input) return (0);
	(void) input;
	(void) context;
	int invalid = 0;

	if (invalid) {
		write(1, "Error chiquitin\n", 16);
		return (1);
	}

	return (0);
}

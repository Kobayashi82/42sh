/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 13:31:33 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/28 13:36:16 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region "Includes"


#pragma endregion

#pragma region "Variables"

	#pragma region "Enumerators"

		typedef enum {
			PIPE,		//	|
			GT,			//	>
			DGT,		//	>>
			LT,			//	<
			DLT,		//	<<
			TLT			//	<<<
		} e_redir;

	#pragma endregion

	#pragma region "Structures"

		#pragma region "Tokens"

			typedef struct s_token	t_token;
			typedef struct s_redir	t_redir;

			typedef struct s_redir {
				char	*value;
				t_token	*token;
				e_redir	type;
				t_redir	*prev;
				t_redir	*next;
			}	t_redir;

		#pragma endregion

	#pragma endregion

#pragma endregion

#pragma region "Methods"


#pragma endregion

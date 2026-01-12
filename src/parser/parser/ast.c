/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 13:27:05 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/12 13:15:17 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "parser/parser.h"
	#include "utils/utils.h"

#pragma endregion

#pragma region "Print (DEBUG)"

	const char *type_to_string(int type) {
		switch (type) {
			case TOKEN_WORD:				return ("WORD");
			case TOKEN_CMD_SUB:				return ("$(");
			case TOKEN_ARITH_SUB:			return ("$((");
			case TOKEN_PARAM_EXP:			return ("${");
			case TOKEN_BRACE:				return ("{");
			case TOKEN_GROUP:				return ("{ ");
			case TOKEN_SUBSHELL:			return ("(");
			case TOKEN_ARITH:				return ("((");

			case TOKEN_REDIRECT_IN:			return ("<");
			case TOKEN_REDIRECT_HEREDOC:	return ("<<");
			case TOKEN_REDIRECT_HERESTRING:	return ("<<<");
			case TOKEN_REDIRECT_IN_OUT:		return ("<>");
			case TOKEN_REDIRECT_DUP_IN:		return ("<&");
			case TOKEN_REDIRECT_OUT:		return (">");
			case TOKEN_REDIRECT_APPEND:		return (">>");
			case TOKEN_REDIRECT_FORCE_OUT:	return (">|");
			case TOKEN_REDIRECT_DUP_OUT:	return (">&");
			case TOKEN_REDIRECT_OUT_ALL:	return ("&>");
			case TOKEN_REDIRECT_APPEND_ALL:	return ("&>>");

			case TOKEN_PIPE:				return ("|");
			case TOKEN_PIPE_ALL:			return ("|&");
			case TOKEN_BACKGROUND:			return ("&");
			case TOKEN_AND:					return ("&&");
			case TOKEN_OR:					return ("||");
			case TOKEN_SEMICOLON:			return (";");

			case TOKEN_IF:					return ("if");
			case TOKEN_THEN:				return ("then");
			case TOKEN_ELSE:				return ("else");
			case TOKEN_ELIF:				return ("elif");
			case TOKEN_FI:					return ("fi");
			
			case TOKEN_CASE:				return ("case");
			case TOKEN_ESAC:				return ("esac");
			case TOKEN_IN:					return ("in");
			case TOKEN_TERMINATOR:			return (";;");
			case TOKEN_FALLTHROUGH:			return (";&");
			case TOKEN_CONTINUE:			return (";;&");
			
			case TOKEN_FUNCTION:			return ("function");
			case TOKEN_WHILE:				return ("while");
			case TOKEN_UNTIL:				return ("until");
			case TOKEN_DO:					return ("do");
			case TOKEN_DONE:				return ("done");
			case TOKEN_FOR:					return ("for");
			case TOKEN_NEGATE:				return ("!");
			case TOKEN_LBRACKET2:			return ("[[");
			case TOKEN_RBRACKET2:			return ("]]");

			case TOKEN_NEWLINE:				return ("NEWLINE");
			case TOKEN_EOF:					return ("EOF");

			default: return ("?");
		}
	}

	void ast_print_indent(int level) {
		for (int i = 0; i < level; i++) printf("  ");
	}

	void ast_print_recursive(t_ast *node, int level) {
		if (!node) return;

		ast_print_indent(level);

		switch (node->type) {
			case TOKEN_PIPE:
				printf("PIPE (|)\n");
				ast_print_recursive(node->left, level + 1);
				ast_print_recursive(node->right, level + 1);
				break;
			case TOKEN_PIPE_ALL:
				printf("PIPE ALL (|&)\n");
				ast_print_recursive(node->left, level + 1);
				ast_print_recursive(node->right, level + 1);
				break;
			case TOKEN_BACKGROUND:
				printf("BACKGROUND (&)\n");
				ast_print_recursive(node->left, level + 1);
				ast_print_recursive(node->right, level + 1);
				break;
			case TOKEN_AND:
				printf("AND (&&)\n");
				ast_print_recursive(node->left, level + 1);
				ast_print_recursive(node->right, level + 1);
				break;

			case TOKEN_OR:
				printf("OR (||)\n");
				ast_print_recursive(node->left, level + 1);
				ast_print_recursive(node->right, level + 1);
				break;
			case TOKEN_SEMICOLON:
				printf("SEQUENCE (;)\n");
				ast_print_recursive(node->left, level + 1);
				ast_print_recursive(node->right, level + 1);
				break;

			case TOKEN_BACKTICK:
				printf("BACKTICK ``\n");
				ast_print_recursive(node->child, level + 1);
				break;
			case TOKEN_ARITH_SUB:
				printf("ARITHMETIC EXPANSION $(())\n");
				ast_print_recursive(node->child, level + 1);
				break;
			case TOKEN_CMD_SUB:
				printf("COMMAND SUBSTITUTION $()\n");
				ast_print_recursive(node->child, level + 1);
				break;
			case TOKEN_PARAM_EXP:
				printf("PARAMETER EXPANSION ${}\n");
				ast_print_recursive(node->child, level + 1);
				break;

			case TOKEN_ARITH:
				printf("ARITHMETIC EXPRESSION (())\n");
				ast_print_recursive(node->child, level + 1);
				break;
			case TOKEN_SUBSHELL:
				printf("SUBSHELL ()\n");
				ast_print_recursive(node->child, level + 1);
				break;
			case TOKEN_GROUP:
				printf("COMMAND GROUP {  ; }\n");
				ast_print_recursive(node->child, level + 1);
				break;
			case TOKEN_CONDITIONAL:
				printf("CONDITIONAL EXPRESSION [[]]\n");
				ast_print_recursive(node->child, level + 1);
				break;

			case TOKEN_NEWLINE:
				printf("NEWLINE \\n\n");
				ast_print_recursive(node->child, level + 1);
				break;

			case TOKEN_WORD:
				printf("COMMAND\n");

				// Assign
				if (node->assign) {
					ast_print_indent(level + 1);
					printf("assign:\n");
					t_assign *assign = node->assign;
					while (assign) {
						ast_print_indent(level + 2);
						char *value = segment_flatten(assign->segment);
						printf("- %c%s%c\n", assign->segment->quoted, value, assign->segment->quoted);
						free(value);
						assign = assign->next;
					}
				}

				// Args
				if (node->args) {
					ast_print_indent(level + 1);
					printf("args:\n");
					t_args *arg = node->args;
					while (arg) {
						t_segment *curr = arg->segment;
						while (curr) {
							if (curr->string.value) {
								ast_print_indent(level + 2);
								printf("- %c%s%c\n", curr->quoted, curr->string.value, curr->quoted);
							}
							curr = curr->next;
						}
						if (arg->right_space) {
							ast_print_indent(level + 2);
							printf("[space]\n");
						}
						arg = arg->next;
					}
				}

				// Redirs
				if (node->redirs) {
					ast_print_indent(level + 1);
					printf("redirs:\n");
					t_redir *redir = node->redirs;
					while (redir) {
						ast_print_indent(level + 2);
						printf("%s (fd: %d)\n", type_to_string(redir->type), redir->fd);
						ast_print_indent(level + 2);
						printf("file:\n");
						t_segment *curr = redir->file;
						while (curr) {
							if (curr->string.value) {
								ast_print_indent(level + 3);
								printf("- %c%s%c\n", curr->quoted, curr->string.value, curr->quoted);
							}
							curr = curr->next;
						}
						redir = redir->next;
					}
				}
				break;

			default:
				printf("UNKNOWN (%d)\n", node->type);
				break;
		}
	}

	void ast_print(t_ast *ast) {
		if (!ast) return;
		ast_print_recursive(ast, 0);
	}

#pragma endregion

#pragma region "Free"

	void ast_free(t_ast **ast) {
		if (!ast || !*ast) return;

		t_ast *node = *ast;

		if (node->left)		ast_free(&node->left);
		if (node->right)	ast_free(&node->right);
		if (node->child)	ast_free(&node->child);

		// Assign
		if (node->assign) {
			t_assign *assign = node->assign;
			while (assign) {
				t_assign *next = assign->next;
				segment_free(assign->segment);
				free(assign);
				assign = next;
			}
		}

		// Args
		if (node->args) {
			t_args *arg = node->args;
			while (arg) {
				t_args *next = arg->next;
				segment_free(arg->segment);
				free(arg);
				arg = next;
			}
		}

		// Redirs
		if (node->redirs) {
			t_redir *redir = node->redirs;
			while (redir) {
				t_redir *next = redir->next;
				segment_free(redir->file);
				free(redir);
				redir = next;
			}
		}

		if (node->value) free(node->value);

		free(node);
		*ast = NULL;
	}

#pragma endregion

#pragma region "Create"

	t_ast *ast_create(int type) {
		t_ast *node = calloc(1, sizeof(t_ast));
		node->type = type;

		return (node);
	}

#pragma endregion

#pragma region "AST to Argv"

	static t_ast *find_first_command(t_ast *node) {
		if (!node)		return (NULL);
		if (node->args)	return (node);									// If it's a command node (has args), return it

		if (node->left) {												// If it's an operator, search left first
			t_ast *found = find_first_command(node->left);
			if (found)		return (found);
		}
		if (node->right)	return (find_first_command(node->right));	// Then right
		if (node->child)	return (find_first_command(node->child));	// If it's a subshell, search in child

		return (NULL);
	}

	static char *join_segments(t_segment *seg) {
		if (!seg) return (NULL);

		t_segment *current;

		char *result = ft_strdup(seg->string.value);
		current = seg->next;
		while (current) {
			result = ft_strjoin(result, current->string.value, J_FREE_VAL1);
			if (!result) return (NULL);
			current = current->next;
		}

		return (result);
	}

	char **get_argv_from_ast(t_ast *ast, int *argc) {
		if (!ast) { *argc = 0; return (NULL); }

		t_ast	*cmd_node;
		t_args	*current;
		char	**argv;
		int		i = 0, count = 0;

		cmd_node = find_first_command(ast);
		if (!cmd_node || !cmd_node->args) return (NULL);

		// Count args
		current = cmd_node->args;
		while (current) {
			count++;
			current = current->next;
		}

		// Allocate array
		argv = malloc((count + 1) * sizeof(char *));
		if (!argv) return (NULL);

		// Fill array
		current = cmd_node->args;
		while (current) {
			argv[i] = join_segments(current->segment);
			if (!argv[i]) {
				array_free(argv);
				*argc = 0;
				return (NULL);
			}
			i++;
			current = current->next;
		}
		argv[count] = NULL;
		*argc = count;

		return (argv);
	}

#pragma endregion

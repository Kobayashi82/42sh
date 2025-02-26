/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_alias.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 20:58:15 by vzurera-          #+#    #+#             */
/*   Updated: 2025/02/26 23:18:03 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// falta expansion recursiva
// Maximum alias expansion no funciona
// (), &, ;, etc... dentro de (()) no vale, solo subshell

#pragma region "Includes"

	#include "libft.h"
	#include "terminal/input.h"
	#include "hashes/alias.h"
	#include "main/options.h"

#pragma endregion

#pragma region "Expand"


#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_ALIAS_EXPANSIONS 50

typedef enum {
    CTX_NORMAL,
    CTX_ARITHMETIC,
    CTX_SUBSHELL,
    CTX_BACKTICK
} ParserContext;

typedef struct ContextNode {
    ParserContext type;
    int nest_level;
    struct ContextNode* prev;
} ContextNode;

// Prototipos de funciones auxiliares
static ContextNode* push_context(ContextNode** top, ParserContext type);
static void pop_context(ContextNode** top);
static bool is_command_separator(char c);
static bool should_expand_aliases(ContextNode* ctx);
static char* replace_substring(char* original, size_t start, size_t len, const char* replacement);

static char* replace_substring(char* original, size_t start, size_t len, const char* replacement) {
    size_t orig_len = strlen(original);
    size_t repl_len = strlen(replacement);
    
    if (start > orig_len) return NULL;
    if (start + len > orig_len) len = orig_len - start;
    
    char* new_str = malloc(orig_len - len + repl_len + 1);
    if (!new_str) return NULL;
    
    // Parte inicial
    strncpy(new_str, original, start);
    new_str[start] = '\0';
    
    // Reemplazo
    strcat(new_str, replacement);
    
    // Parte final
    if (start + len < orig_len) {
        strcat(new_str, original + start + len);
    }
    
    return new_str;
}

static ContextNode* push_context(ContextNode** top, ParserContext type) {
    ContextNode* new_node = malloc(sizeof(ContextNode));
    if (!new_node) return NULL;
    
    new_node->type = type;
    new_node->nest_level = (*top) ? (*top)->nest_level + 1 : 1;
    new_node->prev = *top;
    *top = new_node;
    return new_node;
}

static void pop_context(ContextNode** top) {
    if (*top) {
        ContextNode* temp = *top;
        *top = (*top)->prev;
        free(temp);
    }
}

static bool is_command_separator(char c) {
    return c == ';' || c == '|' || c == '&' || c == '\n' || c == '(' || c == '`';
}

static bool should_expand_aliases(ContextNode* ctx) {
    if (!ctx) return true;
    return ctx->type == CTX_SUBSHELL || ctx->type == CTX_BACKTICK;
}

void expand_aliases(char** input) {
    ContextNode* ctx_stack = NULL;
    size_t i = 0;
    bool in_quotes = false, in_dquotes = false, escape = false;
    bool command_start = true;
    int expansion_count = 0;

    while ((*input)[i]) {
        if (expansion_count > MAX_ALIAS_EXPANSIONS) {
            fprintf(stderr, "Maximum alias expansion depth reached\n");
            break;
        }

        if (escape) {
            escape = false;
            i++;
            continue;
        }

        // Manejar escapes
        if ((*input)[i] == '\\' && !in_quotes) {
            escape = true;
            i++;
            continue;
        }

        // Manejar comillas
        if ((*input)[i] == '\'' && !in_dquotes) {
            in_quotes = !in_quotes;
            i++;
            continue;
        }
        if ((*input)[i] == '"' && !in_quotes) {
            in_dquotes = !in_dquotes;
            i++;
            continue;
        }

        // Solo procesar fuera de comillas
        if (!in_quotes && !in_dquotes) {
            // Detectar nuevos contextos
            if (strncmp(&(*input)[i], "((", 2) == 0 || 
                strncmp(&(*input)[i], "$((", 3) == 0) {
                push_context(&ctx_stack, CTX_ARITHMETIC);
                i += (strncmp(&(*input)[i], "$((", 3) == 0) ? 3 : 2;
                command_start = false;
                continue;
            }
			else if (strncmp(&(*input)[i], "$(", 2) == 0) {
                push_context(&ctx_stack, CTX_SUBSHELL);
                i += 2;
                command_start = true;
                continue;
            }
            else if ((*input)[i] == '(') {
                push_context(&ctx_stack, CTX_SUBSHELL);
                i += 1;
                command_start = true;
                continue;
            }
            else if ((*input)[i] == '`') {
                push_context(&ctx_stack, CTX_BACKTICK);
                i++;
                command_start = true;
                continue;
            }

            // Manejar cierres de contexto
            if (ctx_stack) {
                if (ctx_stack->type == CTX_ARITHMETIC && 
                    i < strlen(*input)-1 && 
                    strncmp(&(*input)[i], "))", 2) == 0) {
                    pop_context(&ctx_stack);
                    i += 2;
                    command_start = false;
                    continue;
                }
                else if (ctx_stack->type == CTX_SUBSHELL && (*input)[i] == ')') {
                    pop_context(&ctx_stack);
                    i++;
                    command_start = false;
                    continue;
                }
                else if (ctx_stack->type == CTX_BACKTICK && (*input)[i] == '`') {
                    pop_context(&ctx_stack);
                    i++;
                    command_start = false;
                    continue;
                }
            }
        }

        // Detectar separadores de comandos
		if (ft_isspace((*input)[i])) { i++; continue; }
        if (is_command_separator((*input)[i])) {
            command_start = true;
            i++;
            continue;
        }

        // Expandir aliases cuando corresponde
        if (command_start && should_expand_aliases(ctx_stack)) {
            size_t alias_start = i;
            size_t alias_end = alias_start;
            
            // Encontrar fin del posible alias
            while ((*input)[alias_end] && !is_command_separator((*input)[alias_end]) && (*input)[alias_end] != ')' && !ft_isspace((*input)[alias_end])) {
                alias_end++;
            }

            char* alias_name = strndup(*input + alias_start, alias_end - alias_start);
            char* alias_value = alias_find_value(alias_name);
            
			ft_printf(1, "%s\n", alias_name);

			command_start = false;

            if (alias_value) {
				if (*alias_value && ft_isspace(alias_value[ft_strlen(alias_value) - 1])) command_start = true;
                char* new_input = replace_substring(*input, alias_start, alias_end - alias_start, alias_value);
                if (!new_input) {
                    free(alias_name);
                    i++;
                    continue;
                }
                
                free(*input);
                *input = new_input;
                expansion_count++;
                
                // Reiniciar el procesado desde la nueva posición
                i = alias_start + strlen(alias_value);
                free(alias_name);

				ft_printf(1, "%s\n", &input[i]);
                continue;
            }
            
            free(alias_name);
            i = alias_end;
        }

        command_start = false;
        i++;
    }

    // Limpiar contexto
    while (ctx_stack) pop_context(&ctx_stack);
}

// Función de reemplazo de subcadenas (ejemplo)



// #include <stdbool.h>
// #include <string.h>
// // Función para detectar subshells dentro de expresiones aritméticas
// size_t find_subshell(const char *str, size_t *index) {
//     size_t start = *index;
//     int nest = 1;
//     bool in_quotes = false, in_dquotes = false, escape = false;

//     for (; str[*index] && nest > 0; (*index)++) {
//         if (escape) {
//             escape = false;
//             continue;
//         }
        
//         if (str[*index] == '\\') {
//             escape = true;
//             continue;
//         }
        
//         if (str[*index] == '\'' && !in_dquotes) in_quotes = !in_quotes;
//         if (str[*index] == '"' && !in_quotes) in_dquotes = !in_dquotes;
        
//         if (in_quotes || in_dquotes) continue;
        
//         if (str[*index] == '(') nest++;
//         if (str[*index] == ')') nest--;
//     }
    
//     return (nest == 0) ? *index - start : 0;
// }

// size_t find_arithmetic_expression(const char *str, size_t *index) {
//     size_t original_pos = *index;
//     bool in_quotes = false, in_dquotes = false, escape = false;
//     int arith_nesting = 0;

//     // Detectar inicio de expresión aritmética
//     if (str[*index] == '(' && str[*index + 1] == '(') {
//         arith_nesting = 1;
//         *index += 2;
//     } else if (str[*index] == '$' && 
//                str[*index + 1] == '(' && 
//                str[*index + 2] == '(') {
//         arith_nesting = 1;
//         *index += 3;
//     } else {
//         (*index)++;
//         return 0;
//     }

//     // Buscar cierre de expresión aritmética
//     while (str[*index]) {
//         if (escape) {
//             escape = false;
//             (*index)++;
//             continue;
//         }

//         if (str[*index] == '\\') {
//             escape = true;
//             (*index)++;
//             continue;
//         }

//         if (str[*index] == '\'' && !in_dquotes) in_quotes = !in_quotes;
//         if (str[*index] == '"' && !in_quotes) in_dquotes = !in_dquotes;

//         if (in_quotes || in_dquotes) {
//             (*index)++;
//             continue;
//         }

//         // Detectar subshells para posible expansión de alias
//         if (str[*index] == '$' && str[*index + 1] == '(') {
//             size_t subshell_start = *index;
//             size_t subshell_len = find_subshell(str, index);
            
//             if (subshell_len > 0) {
//                 // Procesar el subshell para expansión de alias
//                 char *subshell = ft_substr(str, subshell_start + 2, subshell_len - 2);
// 				ft_printf(1, "expande: %s\n", subshell);
//                 //expand_alias(&subshell, 0);  // 0 = no partial mode
//                 // Reemplazar el subshell en la cadena original
//                 // (Necesitarías implementar lógica de reemplazo)
//                 free(subshell);
//             }
//             continue;
//         }

//         // Detectar cierre de expresión aritmética
//         if (str[*index] == ')' && str[*index + 1] == ')') {
//             arith_nesting--;
//             if (arith_nesting == 0) {
//                 *index += 2;
//                 return *index - original_pos;
//             }
//             (*index)++;
//         }
        
//         (*index)++;
//     }

//     *index = original_pos + 1;
//     return 0;
// }


// #include <stdbool.h>
// #include <string.h>

// size_t find_arithmetic_expression(const char *str, size_t *index) {
//     size_t original_pos = *index;
//     bool in_quotes = false;
//     bool in_dquotes = false;
//     bool escape_next = false;
//     size_t start = original_pos;

//     // Verificar si estamos en el inicio de una expresión aritmética
//     if (str[start] == '(' && str[start + 1] == '(') {
//         start += 2; // Avanzar sobre '(('
//     } else if (str[start] == '$' && str[start + 1] == '(' && str[start + 2] == '(') {
//         start += 3; // Avanzar sobre '$ (('
//     } else {
//         // No es una expresión aritmética: incrementar índice en 1
//         *index += 1;
//         return 0;
//     }

//     // Buscar el cierre '))' correspondiente
//     for (size_t i = start; str[i] != '\0'; ++i) {
//         if (escape_next) {
//             escape_next = false;
//             continue;
//         }

//         // Manejar escapes
//         if (str[i] == '\\' && !in_quotes) {
//             escape_next = true;
//             continue;
//         }

//         // Actualizar estado de comillas
//         if (str[i] == '\'' && !in_dquotes) in_quotes = !in_quotes;
//         if (str[i] == '"' && !in_quotes) in_dquotes = !in_dquotes;

//         // Saltar procesamiento dentro de comillas
//         if (in_quotes || in_dquotes) continue;

//         // Verificar cierre '))'
//         if (str[i] == ')' && str[i + 1] == ')') {
//             *index = i + 2; // Posición después del cierre
//             return *index;
//         }
//     }

//     // No se encontró el cierre: reiniciar índice y retornar 0
//     *index = original_pos + 1;
//     return 0;
// }




// typedef struct s_alias_chain {
//     char *alias;
//     struct s_alias_chain *next;
// } t_alias_chain;

// static bool is_metacharacter(char c) {
//     return (c == ';' || c == '|' || c == '&' || c == '\n' || c == '(' || c == '`');
// }

// static bool is_separator(char c) {
//     return (is_metacharacter(c) || ft_isspace(c));
// }

// static void add_alias_to_chain(t_alias_chain **chain, const char *alias) {
//     t_alias_chain *new_node = malloc(sizeof(t_alias_chain));
//     new_node->alias = ft_strdup(alias);
//     new_node->next = *chain;
//     *chain = new_node;
// }

// static bool is_alias_in_chain(t_alias_chain *chain, const char *alias) {
//     while (chain) {
//         if (ft_strcmp(chain->alias, alias) == 0)
//             return true;
//         chain = chain->next;
//     }
//     return false;
// }

// static void free_alias_chain(t_alias_chain *chain) {
//     while (chain) {
//         t_alias_chain *tmp = chain;
//         chain = chain->next;
//         free(tmp->alias);
//         free(tmp);
//     }
// }

// static char *replace_str(char *str, size_t start, size_t len, const char *repl) {
//     char *new = malloc(ft_strlen(str) - len + ft_strlen(repl) + 1);
//     if (!new) return NULL;
    
//     ft_strncpy(new, str, start);
//     strcpy(new + start, repl);
//     strcpy(new + start + ft_strlen(repl), str + start + len);
//     return new;
// }

// static size_t find_command_end(char *str, size_t start, bool *in_quotes, bool *in_dquotes, int partial_mode) {
//     size_t i = start;
//     bool escape = false;
    
//     *in_quotes = (partial_mode == 1);
//     *in_dquotes = (partial_mode == 2);
    
//     while (str[i] && (!is_separator(str[i]) || *in_quotes || *in_dquotes || escape)) {
//         if (escape) {
//             escape = false;
//             i++;
//             continue;
//         }
        
//         if (str[i] == '\\' && !*in_quotes) {
//             escape = true;
//             i++;
//             continue;
//         }
        
//         if (str[i] == '\'' && !*in_dquotes) {
//             *in_quotes = !*in_quotes;
//         } else if (str[i] == '"' && !*in_quotes) {
//             *in_dquotes = !*in_dquotes;
//         }
        
//         i++;
//     }
//     return i;
// }

// void expand_alias_recursive(char **input, int partial_mode, t_alias_chain *chain) {
//     if (!input || !*input) return;

//     char *value = *input;
//     size_t length = ft_strlen(value);
//     bool changes = false;
//     bool global_quotes = (partial_mode == 1);
//     bool global_dquotes = (partial_mode == 2);
//     size_t i = 0;

//     while (i < length) {
//         bool in_quotes = global_quotes;
//         bool in_dquotes = global_dquotes;
//         bool escape = false;
//         size_t cmd_start = i;
//         size_t cmd_end;

//         // Skip metacharacters and whitespace
//         while (i < length && (is_metacharacter(value[i]) || ft_isspace(value[i]))) i++;
//         if (i >= length) break;
//         cmd_start = i;

//         // Find command end considering quoting
//         cmd_end = find_command_end(value, cmd_start, &in_quotes, &in_dquotes, partial_mode);
        
//         // Extract command name
//         char *cmd = ft_strndup(value + cmd_start, cmd_end - cmd_start);
//         char *alias_val = alias_find_value(cmd);
        
//         if (alias_val && !is_alias_in_chain(chain, cmd)) {
//             add_alias_to_chain(&chain, cmd);
            
//             // Check if alias ends with space
//             bool trailing_space = alias_val[ft_strlen(alias_val)-1] == ' ';
            
//             // Replace command
//             char *new_value = replace_str(value, cmd_start, cmd_end - cmd_start, alias_val);
//             if (new_value) {
//                 free(*input);
//                 *input = new_value;
//                 value = new_value;
//                 length = ft_strlen(value);
//                 changes = true;
                
//                 // If trailing space, process next word immediately
//                 if (trailing_space) {
//                     size_t next_start = cmd_start + ft_strlen(alias_val);
//                     while (next_start < length && ft_isspace(value[next_start])) next_start++;
//                     if (next_start < length) {
//                         expand_alias_recursive(input, partial_mode, chain);
//                     }
//                 }
                
//                 // Restart processing after expansion
//                 free(cmd);
//                 free_alias_chain(chain);
//                 expand_alias_recursive(input, partial_mode, NULL);
//                 return;
//             }
//             free_alias_chain(chain);
//         }
//         free(cmd);
//         i = cmd_end;
//     }
    
//     if (changes) {
//         expand_alias_recursive(input, partial_mode, chain);
//     }
// }

// void expand_alias(char **input, int partial_mode) {
//     if (!options.expand_aliases || !input || !*input) return;
//     if (partial_mode == 1 || partial_mode == 2) {
//         // Check if quotes are balanced
//         bool in_quotes = (partial_mode == 1);
//         bool in_dquotes = (partial_mode == 2);
//         size_t i = 0;
//         while ((*input)[i]) {
//             // ... (verificación de comillas balanceadas aquí)
//             // Si las comillas se cierran, actualizar partial_mode a 0
//         }
//     }
//     expand_alias_recursive(input, partial_mode, NULL);
// }

	// void expand_alias(char **input, int partial_mode) {
	// 	if (!options.expand_aliases || !input || !*input) return;

	// 	char *value = *input;
	// 	size_t i = 0;
	// 	size_t length = ft_strlen(value);
	// 	bool in_quotes = (partial_mode == 2);
	// 	bool in_dquotes = (partial_mode == 3);
	// 	bool changes = false;

	// 	while (i < length) {
	// 		if (value[i] == '\\' && !in_quotes) {
	// 			i += 2; continue;
	// 		}
	// 		if (value[i] == '\'' && !in_dquotes) {
	// 			in_quotes = !in_quotes;
	// 		}
	// 		if (value[i] == '"' && !in_quotes) {
	// 			in_dquotes = !in_dquotes;
	// 		}

	// 		if (value[i] == '!' && i + 1 < length && !ft_isspace(value[i + 1]) && value[i + 1] != '"' && !in_quotes) {
	// 			int start = i;
	// 			size_t end = i + 1;
	// 			char *replacement = NULL;

	// 			if (value[end] == '!') {																						// !!
	// 				replacement = expand_position(value, start, &end, length, true);
	// 				if (!replacement) return;
	// 			} else if (value[end] == '-' && ft_isdigit(value[end + 1])) {													// !-[n]
	// 				replacement = expand_position(value, start, &end, length, false);
	// 				if (!replacement) return;
	// 			} else if (ft_isdigit(value[end])) {																			// ![n]
	// 				replacement = expand_event(value, start, &end, length);
	// 				if (!replacement) return;
	// 			} else if (value[end] == '?' && value[end + 1] != '?' && end + 1 < length && !ft_isspace(value[end + 1]) && value[i + 1] != '"') {		// !?[str], !?[str]?
	// 				replacement = expand_value(value, start, &end, length, false);
	// 				if (!replacement) return;
	// 			} else {																										// ![str]
	// 				replacement = expand_value(value, start, &end, length, true);
	// 				if (!replacement) return;
	// 			}

	// 			if (replacement) {
	// 				value = replace(value, &start, end - start, replacement);
	// 				i = start;
	// 				changes = true;
	// 			}
	// 		}
	// 		i++;
	// 	}

	// 	if (changes) {
	// 		char *new_value = ft_strdup(value);
	// 		sfree(value);
	// 		*input = new_value;
	// 	}
	// }

#pragma endregion

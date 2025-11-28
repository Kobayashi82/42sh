/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.no.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 15:15:32 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/28 20:40:32 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/lexer.h"
#include "utils/libft.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

// Estado global del lexer
static lexer_state_t g_lexer = {0};

// === FUNCIONES DEL STACK ===

static void init_stack(void) {
    g_lexer.stack_capacity = 64;
    g_lexer.stack_size = 0;
    g_lexer.delim_stack = malloc(g_lexer.stack_capacity);
}

static void push_delim(char delim) {
    if (g_lexer.stack_size >= g_lexer.stack_capacity) {
        g_lexer.stack_capacity *= 2;
        g_lexer.delim_stack = realloc(g_lexer.delim_stack, g_lexer.stack_capacity);
    }
    g_lexer.delim_stack[g_lexer.stack_size++] = delim;
}

static char pop_delim(void) {
    if (g_lexer.stack_size == 0)
        return '\0';
    return g_lexer.delim_stack[--g_lexer.stack_size];
}

static char top_delim(void) {
    if (g_lexer.stack_size == 0)
        return '\0';
    return g_lexer.delim_stack[g_lexer.stack_size - 1];
}

static int in_single_quote(void) {
    return top_delim() == '\'';
}

static int in_double_quote(void) {
    // Buscar en el stack si hay alguna " abierta
    for (size_t i = 0; i < g_lexer.stack_size; i++) {
        if (g_lexer.delim_stack[i] == '"')
            return true;
    }
    return false;
}

static int in_any_quote(void) {
    return in_single_quote() || in_double_quote();
}

static int in_any_context(void) {
    return g_lexer.stack_size > 0;
}

// === FUNCIONES DEL LEXER ===

void lexer_init(char *input) {
    if (g_lexer.delim_stack)
        free(g_lexer.delim_stack);
    if (g_lexer.input)
        free(g_lexer.input);
    
    g_lexer.input = ft_strdup(input);
    g_lexer.pos = 0;
    g_lexer.len = strlen(input);
    g_lexer.needs_more_input = false;
    
    init_stack();
}

void lexer_append_input(char *more_input) {
    size_t old_len = g_lexer.len;
    size_t new_len = strlen(more_input);
    
    // Realoca y añade con newline
    g_lexer.input = realloc(g_lexer.input, old_len + new_len + 2);
    g_lexer.input[old_len] = '\n';
    strcpy(g_lexer.input + old_len + 1, more_input);
    g_lexer.len = old_len + new_len + 1;
    
    // CRÍTICO: NO resetear pos ni stack
    g_lexer.needs_more_input = false;
}

int lexer_needs_continuation(void) {
    return g_lexer.needs_more_input;
}

void lexer_cleanup(void) {
    free(g_lexer.input);
    free(g_lexer.delim_stack);
    g_lexer.input = NULL;
    g_lexer.delim_stack = NULL;
}

// === FUNCIONES DE NAVEGACIÓN ===

static char peek(void) {
    if (g_lexer.pos >= g_lexer.len)
        return '\0';
    return g_lexer.input[g_lexer.pos];
}

static char peek_ahead(size_t n) {
    if (g_lexer.pos + n >= g_lexer.len)
        return '\0';
    return g_lexer.input[g_lexer.pos + n];
}

static void advance(void) {
    if (g_lexer.pos < g_lexer.len)
        g_lexer.pos++;
}

static int skip_whitespace(void) {
    int had_space = false;
    // Solo skipear espacios si NO estamos dentro de quotes
    if (in_any_quote())
        return false;
        
    while (peek() == ' ' || peek() == '\t') {
        had_space = true;
        advance();
    }
    return had_space;
}

// === FUNCIONES DE TOKENS ===

static token_t *token_create(token_type_t type, char *value, int left_space, int is_quoted) {
    token_t *tok = malloc(sizeof(token_t));
    tok->type = type;
    tok->value = value ? ft_strdup(value) : NULL;
    tok->had_left_space = left_space;
    tok->had_right_space = false;
    tok->quoted = is_quoted;
    return tok;
}

void token_free(token_t *tok) {
    if (!tok) return;
    free(tok->value);
    free(tok);
}

// === LECTURA DE OPERADORES ===

static token_t *read_operator(int had_left_space) {
    char c = peek();
    
    // Solo reconocer operadores si NO estamos en quotes
    if (in_any_quote())
        return NULL;
    
    if (c == '|') {
        if (peek_ahead(1) == '|') {
            advance(); advance();
            return token_create(TOKEN_OR, "||", had_left_space, false);
        }
        advance();
        return token_create(TOKEN_PIPE, "|", had_left_space, false);
    }
    
    if (c == '&' && peek_ahead(1) == '&') {
        advance(); advance();
        return token_create(TOKEN_AND, "&&", had_left_space, false);
    }
    
    if (c == '>') {
        if (peek_ahead(1) == '>') {
            advance(); advance();
            return token_create(TOKEN_REDIRECT_APPEND, ">>", had_left_space, false);
        }
        advance();
        return token_create(TOKEN_REDIRECT_OUT, ">", had_left_space, false);
    }
    
    if (c == '<') {
        advance();
        return token_create(TOKEN_REDIRECT_IN, "<", had_left_space, false);
    }
    
    if (c == ';') {
        advance();
        return token_create(TOKEN_SEMICOLON, ";", had_left_space, false);
    }
    
    if (c == '(') {
        advance();
        return token_create(TOKEN_LPAREN, "(", had_left_space, false);
    }
    
    if (c == ')') {
        // Solo si NO es cierre de subshell/param_exp
        if (top_delim() != ')') {
            advance();
            return token_create(TOKEN_RPAREN, ")", had_left_space, false);
        }
    }
    
    return NULL;
}

// === LECTURA DE PALABRAS ===

static int is_word_boundary(char c) {
    // Dentro de quotes, nada es boundary (excepto la quote que cierra)
    if (in_any_quote())
        return false;
    
    // En contexto normal, estos terminan una palabra
    return (c == '\0' || c == '\n' || c == ' ' || c == '\t' ||
            c == '|' || c == '&' || c == ';' || c == '>' || c == '<' ||
            c == '(' || c == ')');
}

static token_t *read_word(int had_left_space) {
    size_t capacity = 64;
    size_t len = 0;
    char *buffer = malloc(capacity);
    int was_quoted = in_double_quote() || in_single_quote();
    
    while (true) {
        char c = peek();
        char next = peek_ahead(1);
        
        // Detectar fin de palabra
        if (is_word_boundary(c))
            break;
        
        // Expandir buffer si es necesario
        if (len >= capacity - 1) {
            capacity *= 2;
            buffer = realloc(buffer, capacity);
        }
        
        // === MANEJO DE SINGLE QUOTES ===
        if (in_single_quote()) {
            if (c == '\'') {
                // Cerrar single quote
                pop_delim();
                advance();
                continue;  // NO añadir la comilla al buffer
            }
            // Dentro de single quote, TODO es literal
            buffer[len++] = c;
            advance();
            continue;
        }
        
        // === ABRIR SINGLE QUOTE ===
        if (c == '\'') {
            push_delim('\'');
            advance();
            was_quoted = true;
            continue;  // NO añadir la comilla al buffer
        }
        
        // === MANEJO DE DOUBLE QUOTES ===
        if (c == '"') {
            if (top_delim() == '"') {
                // Cerrar double quote del top
                pop_delim();
            } else {
                // Abrir double quote
                push_delim('"');
                was_quoted = true;
            }
            advance();
            continue;  // NO añadir la comilla al buffer
        }
        
        // === DETECCIÓN DE $( y ${ ===
        // Si encontramos estos, terminamos la palabra aquí
        // El siguiente token será TOKEN_SUBSHELL_START o TOKEN_PARAM_EXP_START
        if (c == '$' && (next == '(' || next == '{')) {
            if (len > 0) {
                // Ya tenemos contenido, devolver palabra hasta aquí
                break;
            }
            // Si no hay contenido, dejar que lexer_next_token lo maneje
            break;
        }
        
        // === MANEJO DE BACKSLASH (escape) ===
        if (c == '\\' && !in_single_quote() && next != '\0') {
            // El backslash escapa el siguiente caracter
            buffer[len++] = c;
            advance();
            c = peek();
            if (c != '\0') {
                buffer[len++] = c;
                advance();
            }
            continue;
        }
        
        // === CARACTER NORMAL ===
        buffer[len++] = c;
        advance();
        
        // === CHECK EOF CON CONTEXTOS ABIERTOS ===
        if (peek() == '\0' && in_any_context()) {
            buffer[len] = '\0';
            g_lexer.needs_more_input = true;
            free(buffer);
            return NULL;
        }
    }
    
    if (len == 0) {
        free(buffer);
        return NULL;
    }
    
    buffer[len] = '\0';
    token_t *tok = token_create(TOKEN_WORD, buffer, had_left_space, was_quoted);
    free(buffer);
    return tok;
}

// === FUNCIÓN PRINCIPAL ===

token_t *lexer_next_token(void) {
    // Skip espacios y marca si había
    int had_left_space = skip_whitespace();
    
    char c = peek();
    char next = peek_ahead(1);
    
    // EOF o newline sin contextos abiertos
    if (c == '\0' || c == '\n') {
        if (in_any_context()) {
            g_lexer.needs_more_input = true;
            return NULL;
        }
        if (c == '\n')
            advance();
        return token_create(TOKEN_EOF, NULL, had_left_space, false);
    }
    
    // === MANEJO DE COMILLAS (sin devolver tokens) ===
    // Las comillas se gestionan internamente en read_word
    
    // === SUBSHELL START $( ===
    if (c == '$' && next == '(') {
        int was_quoted = in_double_quote();
        advance(); advance();
        
        // Check si es arithmetic $(( 
        if (peek() == '(') {
            // Por ahora tratarlo como subshell normal
            // En una implementación completa, necesitarías TOKEN_ARITHMETIC_START
            advance();
            push_delim(')');
            push_delim(')');
        } else {
            push_delim(')');
        }
        
        token_t *tok = token_create(TOKEN_SUBSHELL_START, "$(", had_left_space, was_quoted);
        tok->had_right_space = (peek() == ' ' || peek() == '\t');
        return tok;
    }
    
    // === SUBSHELL END ) ===
    if (c == ')' && top_delim() == ')') {
        int was_quoted = in_double_quote();
        pop_delim();
        advance();
        token_t *tok = token_create(TOKEN_SUBSHELL_END, ")", had_left_space, was_quoted);
        tok->had_right_space = (peek() == ' ' || peek() == '\t');
        return tok;
    }
    
    // === PARAMETER EXPANSION START ${ ===
    if (c == '$' && next == '{') {
        int was_quoted = in_double_quote();
        advance(); advance();
        push_delim('}');
        token_t *tok = token_create(TOKEN_PARAM_EXP_START, "${", had_left_space, was_quoted);
        tok->had_right_space = (peek() == ' ' || peek() == '\t');
        return tok;
    }
    
    // === PARAMETER EXPANSION END } ===
    if (c == '}' && top_delim() == '}') {
        int was_quoted = in_double_quote();
        pop_delim();
        advance();
        token_t *tok = token_create(TOKEN_PARAM_EXP_END, "}", had_left_space, was_quoted);
        tok->had_right_space = (peek() == ' ' || peek() == '\t');
        return tok;
    }
    
    // === OPERADORES (solo fuera de quotes) ===
    token_t *tok = read_operator(had_left_space);
    if (tok) {
        tok->had_right_space = (peek() == ' ' || peek() == '\t');
        return tok;
    }
    
    // === PALABRA ===
    tok = read_word(had_left_space);
    if (tok) {
        tok->had_right_space = (peek() == ' ' || peek() == '\t');
        return tok;
    }
    
    // Si necesita más input
    if (g_lexer.needs_more_input)
        return NULL;
    
    // Error: caracter inesperado
    return NULL;
}

// void lexer_print() {
// 	const char *type_names[] = { "WORD", "PIPE", "BACKGROND", "AND", "OR", "BRACE", "BRACE_GROUP", "VAR", "CMD_SHELL", "SHELL", "CMD_ARIT", "ARIT", "SEMICOLON", "REDIR_HEREDOC", "REDIR_IN", "REDIR_OUT", "REDIR_APPEND", "REDIR_HERESTRING" };
// 	t_lx_token *token = lx_tokens;

// 	while (token) {
// 		printf("[%s] %s\n", type_names[token->type], token->token);
// 		token = token->next;
// 	}
// }

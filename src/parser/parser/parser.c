/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 11:38:21 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/24 12:56:05 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Asumimos que tienes el lexer con sus tipos
// Aquí defino los tipos del AST

static parser_state_t g_parser = {0};

// === FUNCIONES AUXILIARES ===

static ast_node_t *create_node(ast_type_t type) {
    ast_node_t *node = calloc(1, sizeof(ast_node_t));
    node->type = type;
    return node;
}

static void consume_token(void) {
    if (g_parser.current_token) {
        token_free(g_parser.current_token);
        g_parser.current_token = NULL;
    }
    g_parser.current_token = lexer_next_token();
}

static bool check_token(token_type_t type) {
    return g_parser.current_token && g_parser.current_token->type == type;
}

static bool match_token(token_type_t type) {
    if (check_token(type)) {
        consume_token();
        return true;
    }
    return false;
}

static token_t *expect_token(token_type_t type) {
    if (!check_token(type)) {
        fprintf(stderr, "syntax error: expected token type %d\n", type);
        g_parser.has_error = true;
        return NULL;
    }
    token_t *tok = g_parser.current_token;
    g_parser.current_token = NULL;
    consume_token();
    return tok;
}

// === LIBERACIÓN DE MEMORIA ===

void ast_free(ast_node_t *node) {
    if (!node) return;
    
    free(node->value);
    
    if (node->argv) {
        for (int i = 0; i < node->argc; i++)
            free(node->argv[i]);
        free(node->argv);
    }
    
    ast_free(node->left);
    ast_free(node->right);
    ast_free(node->child);
    ast_free(node->cmd);
    ast_free(node->file);
    
    free(node);
}

// === FORWARD DECLARATIONS ===

static ast_node_t *parse_pipeline(void);
static ast_node_t *parse_command(void);
static ast_node_t *parse_simple_command(void);
static ast_node_t *parse_word(void);
static ast_node_t *parse_redirects(ast_node_t *cmd);

// === PARSING DE EXPRESIONES ===

// Parsea: cmd1 || cmd2 || cmd3
static ast_node_t *parse_or(void) {
    ast_node_t *left = parse_pipeline();
    
    while (match_token(TOKEN_OR)) {
        ast_node_t *node = create_node(AST_OR);
        node->left = left;
        node->right = parse_pipeline();
        left = node;
    }
    
    return left;
}

// Parsea: cmd1 && cmd2 && cmd3
static ast_node_t *parse_and(void) {
    ast_node_t *left = parse_or();
    
    while (match_token(TOKEN_AND)) {
        ast_node_t *node = create_node(AST_AND);
        node->left = left;
        node->right = parse_or();
        left = node;
    }
    
    return left;
}

// Parsea: cmd1 | cmd2 | cmd3
static ast_node_t *parse_pipeline(void) {
    ast_node_t *left = parse_command();
    
    while (match_token(TOKEN_PIPE)) {
        ast_node_t *node = create_node(AST_PIPE);
        node->left = left;
        node->right = parse_command();
        left = node;
    }
    
    return left;
}

// Parsea un comando con posibles redirecciones
static ast_node_t *parse_command(void) {
    ast_node_t *cmd = parse_simple_command();
    if (!cmd)
        return NULL;
    
    // Parsear redirecciones
    return parse_redirects(cmd);
}

// Parsea redirecciones: cmd < in > out >> append
static ast_node_t *parse_redirects(ast_node_t *cmd) {
    while (true) {
        if (check_token(TOKEN_REDIRECT_IN)) {
            consume_token();
            ast_node_t *node = create_node(AST_REDIRECT_IN);
            node->cmd = cmd;
            node->file = parse_word();
            cmd = node;
        }
        else if (check_token(TOKEN_REDIRECT_OUT)) {
            consume_token();
            ast_node_t *node = create_node(AST_REDIRECT_OUT);
            node->cmd = cmd;
            node->file = parse_word();
            cmd = node;
        }
        else if (check_token(TOKEN_REDIRECT_APPEND)) {
            consume_token();
            ast_node_t *node = create_node(AST_REDIRECT_APPEND);
            node->cmd = cmd;
            node->file = parse_word();
            cmd = node;
        }
        else {
            break;
        }
    }
    
    return cmd;
}

// Parsea un comando simple: echo hello world
static ast_node_t *parse_simple_command(void) {
    // Subshell como comando standalone: (cmd)
    if (check_token(TOKEN_LPAREN)) {
        consume_token();
        ast_node_t *node = create_node(AST_SUBSHELL);
        node->child = parse_and();  // Parsear contenido completo
        expect_token(TOKEN_RPAREN);
        return node;
    }
    
    ast_node_t *cmd = create_node(AST_COMMAND);
    cmd->argv = malloc(sizeof(char*) * 16);
    cmd->argc = 0;
    int capacity = 16;
    
    // Leer argumentos (palabras, subshells, expansiones)
    while (true) {
        if (check_token(TOKEN_EOF) || check_token(TOKEN_PIPE) || 
            check_token(TOKEN_AND) || check_token(TOKEN_OR) ||
            check_token(TOKEN_SEMICOLON) || check_token(TOKEN_RPAREN) ||
            check_token(TOKEN_REDIRECT_IN) || check_token(TOKEN_REDIRECT_OUT) ||
            check_token(TOKEN_REDIRECT_APPEND)) {
            break;
        }
        
        ast_node_t *arg = parse_word();
        if (!arg)
            break;
        
        // Expandir el argumento a strings
        // Por ahora, simplificado: solo añadir el value
        if (cmd->argc >= capacity - 1) {
            capacity *= 2;
            cmd->argv = realloc(cmd->argv, sizeof(char*) * capacity);
        }
        
        // Aquí deberías expandir subshells y expansiones
        // Por ahora, placeholder:
        if (arg->type == AST_WORD) {
            cmd->argv[cmd->argc++] = strdup(arg->value);
        }
        else if (arg->type == AST_SUBSHELL) {
            // Ejecutar subshell y añadir resultado
            // Por ahora, solo un placeholder
            cmd->argv[cmd->argc++] = strdup("$(subshell)");
        }
        else if (arg->type == AST_PARAM_EXP) {
            // Expandir variable y añadir resultado
            cmd->argv[cmd->argc++] = strdup("${var}");
        }
        
        ast_free(arg);
    }
    
    cmd->argv[cmd->argc] = NULL;
    
    if (cmd->argc == 0) {
        ast_free(cmd);
        return NULL;
    }
    
    return cmd;
}

// Parsea una palabra, subshell o expansión
static ast_node_t *parse_word(void) {
    // Palabra literal
    if (check_token(TOKEN_WORD)) {
        token_t *tok = expect_token(TOKEN_WORD);
        ast_node_t *node = create_node(AST_WORD);
        node->value = strdup(tok->value);
        node->quoted = tok->quoted;
        token_free(tok);
        return node;
    }
    
    // Subshell: $(...)
    if (check_token(TOKEN_SUBSHELL_START)) {
        token_t *tok = g_parser.current_token;
        bool was_quoted = tok->quoted;
        consume_token();
        
        ast_node_t *node = create_node(AST_SUBSHELL);
        node->quoted = was_quoted;
        
        // Parsear el contenido del subshell recursivamente
        node->child = parse_and();
        
        expect_token(TOKEN_SUBSHELL_END);
        return node;
    }
    
    // Parameter expansion: ${...}
    if (check_token(TOKEN_PARAM_EXP_START)) {
        token_t *tok = g_parser.current_token;
        bool was_quoted = tok->quoted;
        consume_token();
        
        ast_node_t *node = create_node(AST_PARAM_EXP);
        node->quoted = was_quoted;
        
        // Dentro de ${...} debería haber una palabra (nombre de variable)
        token_t *var_tok = expect_token(TOKEN_WORD);
        if (var_tok) {
            node->value = strdup(var_tok->value);
            token_free(var_tok);
        }
        
        expect_token(TOKEN_PARAM_EXP_END);
        return node;
    }
    
    return NULL;
}

// === FUNCIÓN PRINCIPAL DE PARSING ===

ast_node_t *parse(void) {
    // Inicializar parser
    g_parser.has_error = false;
    g_parser.current_token = lexer_next_token();
    
    // Si no hay token (necesita continuación), devolver NULL
    if (!g_parser.current_token) {
        if (lexer_needs_continuation()) {
            return NULL;
        }
        // EOF vacío
        return NULL;
    }
    
    // EOF inmediato
    if (check_token(TOKEN_EOF)) {
        token_free(g_parser.current_token);
        g_parser.current_token = NULL;
        return NULL;
    }
    
    // Parsear expresión completa (con &&, ||, pipes, etc)
    ast_node_t *ast = parse_and();
    
    // Consumir semicolon opcional al final
    match_token(TOKEN_SEMICOLON);
    
    // Verificar que llegamos a EOF
    if (!check_token(TOKEN_EOF) && !lexer_needs_continuation()) {
        fprintf(stderr, "syntax error: unexpected token\n");
        ast_free(ast);
        return NULL;
    }
    
    if (g_parser.current_token) {
        token_free(g_parser.current_token);
        g_parser.current_token = NULL;
    }
    
    if (g_parser.has_error) {
        ast_free(ast);
        return NULL;
    }
    
    return ast;
}

// === FUNCIÓN PARA IMPRIMIR EL AST (debug) ===

static void print_ast_indent(ast_node_t *node, int indent) {
    if (!node) return;
    
    for (int i = 0; i < indent; i++) printf("  ");
    
    switch (node->type) {
        case AST_COMMAND:
            printf("COMMAND [");
            for (int i = 0; i < node->argc; i++) {
                printf("%s%s", node->argv[i], i < node->argc - 1 ? ", " : "");
            }
            printf("]\n");
            break;
            
        case AST_PIPE:
            printf("PIPE\n");
            print_ast_indent(node->left, indent + 1);
            print_ast_indent(node->right, indent + 1);
            break;
            
        case AST_AND:
            printf("AND (&&)\n");
            print_ast_indent(node->left, indent + 1);
            print_ast_indent(node->right, indent + 1);
            break;
            
        case AST_OR:
            printf("OR (||)\n");
            print_ast_indent(node->left, indent + 1);
            print_ast_indent(node->right, indent + 1);
            break;
            
        case AST_REDIRECT_IN:
            printf("REDIRECT_IN <\n");
            print_ast_indent(node->cmd, indent + 1);
            print_ast_indent(node->file, indent + 1);
            break;
            
        case AST_REDIRECT_OUT:
            printf("REDIRECT_OUT >\n");
            print_ast_indent(node->cmd, indent + 1);
            print_ast_indent(node->file, indent + 1);
            break;
            
        case AST_REDIRECT_APPEND:
            printf("REDIRECT_APPEND >>\n");
            print_ast_indent(node->cmd, indent + 1);
            print_ast_indent(node->file, indent + 1);
            break;
            
        case AST_SUBSHELL:
            printf("SUBSHELL $(...)%s\n", node->quoted ? " [quoted]" : "");
            print_ast_indent(node->child, indent + 1);
            break;
            
        case AST_PARAM_EXP:
            printf("PARAM_EXP ${%s}%s\n", node->value, node->quoted ? " [quoted]" : "");
            break;
            
        case AST_WORD:
            printf("WORD \"%s\"%s\n", node->value, node->quoted ? " [quoted]" : "");
            break;
    }
}

void print_ast(ast_node_t *node) {
    print_ast_indent(node, 0);
}

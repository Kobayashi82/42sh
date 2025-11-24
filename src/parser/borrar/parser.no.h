
#pragma once

#include "parser/lexer.h"

typedef enum {
    AST_COMMAND,        // comando simple: echo hello
    AST_PIPE,           // pipe: cmd1 | cmd2
    AST_AND,            // and: cmd1 && cmd2
    AST_OR,             // or: cmd1 || cmd2
    AST_REDIRECT_IN,    // redirect input: cmd < file
    AST_REDIRECT_OUT,   // redirect output: cmd > file
    AST_REDIRECT_APPEND,// append: cmd >> file
    AST_SUBSHELL,       // subshell: $(cmd)
    AST_PARAM_EXP,      // parameter expansion: ${var}
    AST_WORD,           // palabra literal
} ast_type_t;

typedef struct ast_node {
    ast_type_t type;
    
    // Para palabras y redirects
    char *value;
    int quoted;        // Si estaba entre comillas
    
    // Para comandos - almacenan nodos AST, no strings
    // La expansión ocurre en el executor, no en el parser
    struct ast_node **argv;  // Array de nodos AST (palabras, subshells, expansiones)
    int argc;
    
    // Para operadores binarios (pipe, &&, ||)
    struct ast_node *left;
    struct ast_node *right;
    
    // Para subshells y expansiones
    struct ast_node *child;
    
    // Para redirecciones
    struct ast_node *cmd;       // El comando a ejecutar
    struct ast_node *file;      // El archivo para la redirección
} ast_node_t;

// Estado del parser
typedef struct {
    token_t *current_token;
    int has_error;
} parser_state_t;

ast_node_t *parse(void);
void ast_free(ast_node_t *node);
void print_ast(ast_node_t *node);

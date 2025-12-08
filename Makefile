# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/16 12:54:20 by vzurera-          #+#    #+#              #
#    Updated: 2025/12/08 16:58:39 by vzurera-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ──────────── #
# ── COLORS ── #
# ──────────── #

NC    				= \033[0m
RED     			= \033[0;31m
GREEN   			= \033[0;32m
YELLOW  			= \033[0;33m
BLUE    			= \033[0;34m
MAGENTA 			= \033[0;35m
CYAN    			= \033[0;36m
WHITE   			= \033[0;37m
INV_RED  			= \033[7;31m
INV_GREEN	  		= \033[7;32m
INV_YELLOW  		= \033[7;33m
INV_BLUE  			= \033[7;34m
INV_MAGENTA			= \033[7;35m
INV_CYAN			= \033[7;36m
INV_WHITE			= \033[7;37m
BG_CYAN				= \033[40m
FG_YELLOW			= \033[89m
COUNTER 			= 0

# ────────── #
# ── NAME ── #
# ────────── #

NAME				=	42sh

# ─────────── #
# ── FLAGS ── #
# ─────────── #

CC					= clang
# FLAGS				= -Wall -Wextra -Werror -O2							# (for production)
FLAGS				= -Wall -Wextra -Werror -g -O0 -gdwarf-4			# (for debugging)
# FLAGS				= -Wall -Wextra -Werror -g -O0 -fsanitize=thread	# (for debugging with sanitize)
EXTRA_FLAGS			= -ltermcap
EXTRA_FLAGS_OBJ		=
LDFLAGS				= -Wl,--wrap=malloc -Wl,--wrap=calloc -Wl,--wrap=realloc -Wl,--wrap=free -Wl,--wrap=open -Wl,--wrap=close -Wl,--wrap=dup -Wl,--wrap=dup2 -Wl,--wrap=pipe -Wl,--wrap=execve -Wl,--wrap,exit	# (for safe functions)
TESTING				= 1

# ───────────────── #
# ── DIRECTORIES ── #
# ───────────────── #

INC_DIR				= $(shell find inc -type d | sed 's/^/-I/')
BLD_DIR				= build/
OBJ_DIR				= $(BLD_DIR)obj/
SRC_DIR				= src/

# ─────────── #
# ── FILES ── #
# ─────────── #

SRCS	=	main/main.c										\
			main/options.c									\
			main/shell.c									\
			main/error.c									\
															\
															\
			terminal/signal.c								\
			terminal/input.c								\
															\
			terminal/readinput/readinput.c					\
			terminal/readinput/modes/dumb.c					\
			terminal/readinput/modes/readline.c				\
			terminal/readinput/modes/vi.c					\
			terminal/readinput/common/auto_complete.c		\
			terminal/readinput/common/history_expansion.c	\
			terminal/readinput/common/history_search.c		\
			terminal/readinput/common/history.c				\
			terminal/readinput/common/prompt.c				\
			terminal/readinput/common/termcaps.c			\
			terminal/readinput/common/undo.c				\
															\
															\
			parser/lexer/container.c						\
			parser/lexer/expansion.c						\
			parser/lexer/keyword.c							\
			parser/lexer/lexer.c							\
			parser/lexer/operator.c							\
			parser/lexer/redirection.c						\
			parser/lexer/token.c							\
			parser/lexer/utils.c							\
			parser/lexer/word.c								\
															\
			parser/parser/ast.c								\
			parser/parser/command.c							\
			parser/parser/parser.c							\
															\
															\
			expansion/globbing/brackets.c					\
			expansion/globbing/globbing.c					\
			expansion/globbing/match.c						\
			expansion/globbing/pattern.c					\
			expansion/arithmetic.c							\
			expansion/brace.c								\
			expansion/group.c								\
			expansion/parameter.c							\
			expansion/process.c								\
			expansion/tilde.c								\
			expansion/word_split.c							\
															\
															\
			hashes/alias.c									\
			hashes/builtin.c								\
			hashes/cmdp.c									\
			hashes/key_value.c								\
			hashes/variables.c								\
															\
															\
			builtins/alias/alias.c							\
			builtins/alias/unalias.c						\
			builtins/env/declare.c							\
			builtins/env/export.c							\
			builtins/env/readonly.c							\
			builtins/env/set.c								\
			builtins/env/shopt.c							\
			builtins/env/unset.c							\
			builtins/filesystem/cd.c						\
			builtins/filesystem/hash.c						\
			builtins/filesystem/pwd.c						\
			builtins/history/fc.c							\
			builtins/history/history.c						\
			builtins/jobs/bg.c								\
			builtins/jobs/fg.c								\
			builtins/jobs/jobs.c							\
			builtins/jobs/kill.c							\
			builtins/jobs/wait.c							\
			builtins/math/let.c								\
			builtins/scripting/dot.c						\
			builtins/scripting/eval.c						\
			builtins/scripting/exec.c						\
			builtins/scripting/source.c						\
			builtins/scripting/test.c						\
			builtins/shell/banner.c							\
			builtins/shell/builtin.c						\
			builtins/shell/command.c						\
			builtins/shell/echo.c							\
			builtins/shell/enable.c							\
			builtins/shell/exit.c							\
			builtins/shell/help.c							\
			builtins/shell/type.c							\
			builtins/utils/executer.c						\
			builtins/utils/options.c						\
															\
															\
			utils/array.c									\
			utils/safe_execve.c								\
			utils/safe_fd.c									\
			utils/gnl.c										\
			utils/safe_mem.c								\
			utils/safe_xmem.c								\
			utils/num.c										\
			utils/paths.c									\
			utils/print.c									\
			utils/str.c										\
			utils/temp.c									\
			utils/times.c									\
			utils/users.c									\
															\
															\
			tests/args.c									\
			tests/builtin.c									\
			tests/create_args.c								\
			tests/internal.c								\
			tests/tests.c									\
			tests/untests.c									\

# ───────────────────────────────────────────────────────────── #
# ─────────────────────────── RULES ─────────────────────────── #
# ───────────────────────────────────────────────────────────── #

all: _show_title
$(NAME): _show_title

SRC_PATHS	= $(addprefix $(SRC_DIR), $(SRCS))
OBJS		= $(SRCS:%.c=$(OBJ_DIR)%.o)
DEPS		= $(OBJS:.o=.d)
-include $(DEPS)

_compile: $(OBJS)
	@$(MAKE) -s _hide_cursor
#	Create folder
	@mkdir -p $(BLD_DIR)

#	Compile
	@printf "\r%50s\r\t$(CYAN)Compiling... $(YELLOW)$(NAME)$(NC)"
	@$(CC) $(FLAGS) $(LDFLAGS) $(INC_DIR) $(OBJS) $(EXTRA_FLAGS) -o $(NAME)
	@printf "\r%50s\r\t$(CYAN)Compiled    $(GREEN)✓ $(YELLOW)$(NAME)$(NC)\n"

	@$(MAKE) -s _progress; printf "\n"
#	----- REMOVE TO DISABLED TEST -----
	@if [ "$(TESTING)" -ne 0 ]; then \
		printf "\033[A" && ./leaks test || true; \
		printf "\n"; \
	fi
#	----- REMOVE TO DISABLED TEST -----
	@$(MAKE) -s _show_cursor

# ───────────── #
# ── OBJECTS ── #
# ───────────── #

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@$(MAKE) -s _hide_cursor
#	Create folder
	@mkdir -p $(@D)
#	Compile objects
	@filename=$$(basename $<); filename=$${filename%.*}; \
	BAR=$$(printf "/ — \\ |" | cut -d" " -f$$(($(COUNTER) % 4 + 1))); \
	printf "\r%50s\r\t$(CYAN)Compiling... $(GREEN)$$BAR  $(YELLOW)$$filename$(NC)"; \
	$(eval COUNTER=$(shell echo $$(($(COUNTER)+1))))
	@$(CC) $(FLAGS) $(INC_DIR) $(EXTRA_FLAGS_OBJ) -MMD -o $@ -c $<

# ───────────────── #
# ── EXTRA RULES ── #
# ───────────────── #

_show_title:
	@$(MAKE) -s _hide_cursor
	@$(MAKE) -s _title

#	Check if source exists and needs recompiling
	@if  [ ! -n "$(NAME)" ] || [ ! -n "$(SRCS)" ] || [ ! -d "$(SRC_DIR)" ]; then \
        printf "\t$(CYAN)Source files don't exist$(NC)"; \
		printf "\n\t$(WHITE)────────────────────────$(NC)"; \
		$(MAKE) -s _progress; printf "\n" \
		$(MAKE) -s _show_cursor; \
	elif [ -f "$(NAME)" ] && \
		[ -z "$$(find $(SRC_PATHS) -newer "$(NAME)" 2>/dev/null; find inc -name '*.h' -newer "$(NAME)" 2>/dev/null)" ] && \
		[ $$(find $(OBJS) 2>/dev/null | wc -l) -eq $$(echo "$(OBJS)" | wc -w) ]; then \
        printf "\t$(GREEN)✓ $(YELLOW)$(NAME)$(CYAN) is up to date$(NC)"; \
		printf "\n\t$(WHITE)────────────────────────$(NC)"; \
		$(MAKE) -s _progress; printf "\n" \
		$(MAKE) -s _show_cursor; \
	else \
		printf "\n\t$(WHITE)────────────────────────$(NC)\033[1A\r"; \
		$(MAKE) -s _compile; \
	fi
	@$(MAKE) -s _show_cursor

# ───────────────────────────────────────────────────────────── #
# ────────────────────────── RE-MAKE ────────────────────────── #
# ───────────────────────────────────────────────────────────── #

# ───────────── #
# ── RE-MAKE ── #
# ───────────── #

re:
	@$(MAKE) -s _hide_cursor
	@$(MAKE) -s _title

#	Check if source exists and needs recompiling
	@if  [ ! -n "$(NAME)" ] || [ ! -n "$(SRCS)" ] || [ ! -d "$(SRC_DIR)" ]; then \
        printf "\t$(CYAN)Source files don't exist$(NC)"; \
		printf "\n\t$(WHITE)────────────────────────$(NC)"; \
		$(MAKE) -s _progress; \
		$(MAKE) -s _show_cursor; \
	fi

#	Delete objects
	@$(MAKE) -s _delete_objects
	@if [ -f $(NAME) ]; then \
		printf "\t$(CYAN)Deleting... $(YELLOW)$(NAME)$(NC)"; \
		rm -f $(NAME); \
	fi
	@printf "\r%50s\r\t$(CYAN)Deleted     $(GREEN)✓ $(YELLOW)$(NAME)$(NC)\n"
	@$(MAKE) -s _progress; printf "\n"
	@-find $(BLD_DIR) -type d -empty -delete >/dev/null 2>&1 || true
	@printf "\t$(WHITE)────────────────────────\n$(NC)"
	@printf "\033[1A\033[1A\r"

#	Compile
	@$(MAKE) -s _compile

# ─────────── #
# ── CLEAN ── #
# ─────────── #

clean:
	@$(MAKE) -s _hide_cursor
	@$(MAKE) -s _title

	@$(MAKE) -s _delete_objects
	@printf "\r%50s\r\t$(CYAN)Deleted     $(GREEN)✓ $(YELLOW)objects$(NC)\n"

	@$(MAKE) -s _progress; printf "\n"
	@$(MAKE) -s _show_cursor

# ──────────── #
# ── FCLEAN ── #
# ──────────── #

fclean:
	@$(MAKE) -s _hide_cursor
	@$(MAKE) -s _title

	@$(MAKE) -s _delete_objects
	@if [ -f $(NAME) ]; then \
		printf "\t$(CYAN)Deleting... $(YELLOW)$(NAME)$(NC)"; \
		rm -f $(NAME); \
	fi
	@printf "\r%50s\r\t$(CYAN)Deleted     $(GREEN)✓ $(YELLOW)$(NAME)$(NC)\n"
	@find $(BLD_DIR) -type d -empty -delete >/dev/null 2>&1 || true
	@find  -type d -empty -delete >/dev/null 2>&1 || true

	@$(MAKE) -s _progress; printf "\n"
	@$(MAKE) -s _show_cursor

# ───────────────────────────────────────────────────────────── #
# ───────────────────────── FUNCTIONS ───────────────────────── #
# ───────────────────────────────────────────────────────────── #

# ─────────── #
# ── TITLE ── #
# ─────────── #

_title:
	@clear;
	@printf "\n$(NC)\t$(INV_CYAN) $(BG_CYAN)$(FG_YELLOW)★$(INV_CYAN) $(BG_CYAN)$(FG_YELLOW)★$(INV_CYAN) $(BG_CYAN)$(FG_YELLOW)★\
	$(INV_CYAN)    $(NC)$(INV_CYAN)$(shell echo $(NAME) | tr a-z A-Z | tr '_' ' ')$(INV_CYAN)    \
	$(BG_CYAN)$(FG_YELLOW)★$(INV_CYAN) $(BG_CYAN)$(FG_YELLOW)★$(INV_CYAN) $(BG_CYAN)$(FG_YELLOW)★$(INV_CYAN) $(NC)\n"
	@printf "\t$(WHITE)────────────────────────\n$(NC)"

# ───────────── #
# ── CURSORS ── #
# ───────────── #

_hide_cursor:
	@printf "\e[?25l"
 
_show_cursor:
	@printf "\e[?25h"

# ──────────────────── #
# ── DELETE OBJECTS ── #
# ──────────────────── #

_delete_objects:
	@printf "\n\t$(WHITE)────────────────────────$(NC)\033[1A\r"
	@if [ -n "$(shell find $(OBJ_DIR) -type f -name '*.o' 2>/dev/null)" ]; then \
		COUNTER=0; \
		find $(OBJ_DIR) -type f -name '*.o' | while read -r file; do \
			BAR=$$(printf "/ — \\ |" | cut -d" " -f$$((COUNTER % 4 + 1))); \
			filename=$$(basename $$file); \
			srcpath=$$(find $(SRC_DIR) -type f -name "$${filename%.o}.*" 2>/dev/null); \
			if [ -n "$$srcpath" ]; then \
				rm -f $$file $$(dirname $$file)/$${filename%.o}.d; \
				filename=$${filename%.*}; \
				printf "\r%50s\r\t$(CYAN)Deleting... $(GREEN)$$BAR $(YELLOW)$$filename$(NC)"; sleep 0.05; \
				COUNTER=$$((COUNTER+1)); \
			fi; \
		done; \
	fi; printf "\r%50s\r"
	@-find $(BLD_DIR) -type d -empty -delete >/dev/null 2>&1 || true

wipe:
	@rm -rf $(BLD_DIR)
	@rm -rf $(NAME)

# ─────────────────── #
# ── PROGRESS LINE ── #
# ─────────────────── #

_progress:
	@total=24; printf "\r\t"; for i in $$(seq 1 $$total); do printf "$(RED)─"; sleep 0.01; done; printf "$(NC)"
	@total=24; printf "\r\t"; for i in $$(seq 1 $$total); do printf "─"; sleep 0.01; done; printf "\n$(NC)"

# ─────────── #
# ── PHONY ── #
# ─────────── #

.PHONY: all clean fclean re wipe _show_title _title _hide_cursor _show_cursor _delete_objects _progress

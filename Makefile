# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/29 13:41:50 by vzurera-          #+#    #+#              #
#    Updated: 2024/11/29 14:20:48 by vzurera-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ───────────────────────────────────────────────────────────── #
# ─────────────────────── CONFIGURATION ─────────────────────── #
# ───────────────────────────────────────────────────────────── #

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

# ─────────── #
# ── FLAGS ── #
# ─────────── #

CC					= clang
FLAGS				= -Wall -Wextra -Werror -g # -O0 -fsanitize=thread

# ───────────────── #
# ── DIRECTORIES ── #
# ───────────────── #

SRC_DIR				= src/
INC_DIR				= -I./inc -I./inc/main -I./inc/terminal
OBJ_DIR				= build/obj/

# ────────── #
# ── NAME ── #
# ────────── #

NAME	=	42sh
SRCS	=	main/main.c																													\
			terminal/terminal.c

SRCS      := $(addprefix $(SRC_DIR), $(SRCS))

# ────────────────────────────────────────────────────────── #
# ───────────────────────── NORMAL ───────────────────────── #
# ────────────────────────────────────────────────────────── #

all: $(NAME)

OBJS      = $(SRCS:$(SRC_DIR)%.c=$(OBJ_DIR)%.o)
DEPS		= $(OBJS:.o=.d)
-include $(DEPS)

#	Compile if the executable does not exist or there are changes in the .c files.
#	This avoids re-linking and shows a more appropriate message.
#	If you run "make -n", it should not show any compilation commands if it is already compiled.
#	But it will show the "make -s" calls. This is to check if compilation is necessary.
$(NAME): normal
	@if [ ! -f $(NAME) ]; then \
        $(MAKE) -s _compile; \
	elif $(MAKE) -s -q $(OBJS); then \
		$(MAKE) -s _uptodate; \
	else \
		$(MAKE) -s _compile; \
	fi

#	Show "up to date" message
_uptodate:
	@printf "\r%50s\r\t$(CYAN)Up to date    $(GREEN)✓ $(YELLOW)$(NAME)$(NC)\n"; \
	$(MAKE) -s _progress; printf "\n"; \
	$(MAKE) -s _show_cursor; \

#	Compile program
_compile: $(OBJS)
	@printf "\r%50s\r\t$(CYAN)Compiling... $(YELLOW)$(NAME)$(NC)"
	@$(CC) $(FLAGS) $(INC_DIR) $(OBJS) -o $(NAME);
	@printf "\r%50s\r\t$(CYAN)Compiled    $(GREEN)✓ $(YELLOW)$(NAME)$(NC)\n"
#	_Progress line
	@$(MAKE) -s _progress; printf "\n"
#	Restore cursor
	@$(MAKE) -s _show_cursor

# ───────────── #
# ── OBJECTS ── #
# ───────────── #

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
#	Create folder
	@mkdir -p $(@D)
#	Compile objects
	@filename=$$(basename $<); filename=$${filename%.*}; \
	BAR=$$(printf "/ ─ \\ |" | cut -d" " -f$$(($(COUNTER) % 4 + 1))); \
	printf "\r%50s\r\t$(CYAN)Compiling... $(GREEN)$$BAR  $(YELLOW)$$filename$(NC)"; \
	$(eval COUNTER=$(shell echo $$(($(COUNTER)+1))))
	@$(CC) $(FLAGS) $(INC_DIR) -MMD -o $@ -c $<

# ───────────────── #
# ── EXTRA RULES ── #
# ───────────────── #

normal:
	@if [ ! -d "$(SRC_DIR)" ]; then \
		printf "\n\t$(CYAN)source files doesn't exist\n\n$(NC)"; \
		rm -f .is_re; \
		exit 1; \
	fi
#	Hide cursor
	@$(MAKE) -s _hide_cursor
#	Title
	@if [ ! -f .is_re ]; then \
		clear; $(MAKE) -s _title; \
	else \
		printf "\t$(WHITE)────────────────────────\n$(NC)"; \
		printf "\n\t────────────────────────$(NC)\033[1A\r"; \
	fi; rm -f .is_re

# ───────────────────────────────────────────────────────────── #
# ────────────────────────── RE-MAKE ────────────────────────── #
# ───────────────────────────────────────────────────────────── #

re:
	@rm -f .is_re
	@if [ ! -d "$(SRC_DIR)" ]; then \
		printf "\n\t$(CYAN)source files doesn't exist\n\n$(NC)"; \
		exit 1; \
	fi
#	Hide cursor
	@$(MAKE) -s _hide_cursor
	@$(MAKE) -s fclean
#	Create files
	@touch .is_re
	@printf "\033[1A\033[1A\r"
#	Execute $(NAME)
	@$(MAKE) -s $(NAME)

# ───────────────────────────────────────────────────────────── #
# ─────────────────────────── CLEAN ─────────────────────────── #
# ───────────────────────────────────────────────────────────── #

clean:
	@rm -f .is_re
#	Hide cursor
	@$(MAKE) -s _hide_cursor
#	Title
	@clear
	@$(MAKE) -s _title
#	Delete objects
	@$(MAKE) -s _delete_objects
	@printf "\r%50s\r\t$(CYAN)Deleted     $(GREEN)✓ $(YELLOW)objects$(NC)\n"
#	Progress line
	@$(MAKE) -s _progress; printf "\n"
#	Restore cursor
	@$(MAKE) -s _show_cursor

# ───────────────────────────────────────────────────────────── #
# ────────────────────────── F-CLEAN ────────────────────────── #
# ───────────────────────────────────────────────────────────── #

fclean:
	@rm -f .is_re webserv
#	Hide cursor
	@$(MAKE) -s _hide_cursor
#	Title
	@clear
	@$(MAKE) -s _title
#	Delete objects
	@$(MAKE) -s _delete_objects
#	Delete $(NAME)
	@if [ -f $(NAME) ]; then \
		printf "\t$(CYAN)Deleting... $(YELLOW)$(NAME)$(NC)"; \
		rm -f $(NAME); \
	fi
	@printf "\r%50s\r\t$(CYAN)Deleted     $(GREEN)✓ $(YELLOW)$(NAME)$(NC)\n"
	@$(MAKE) -s _progress
	@printf "\n"
#	Delete folder and files
	@-find build -type d -empty -delete >/dev/null 2>&1 || true
#	Restore cursor
	@$(MAKE) -s _show_cursor

# ───────────────────────────────────────────────────────────── #
# ───────────────────────── FUNCTIONS ───────────────────────── #
# ───────────────────────────────────────────────────────────── #

# ─────────── #
# ── TITLE ── #
# ─────────── #

_title:
	@printf "\n$(NC)\t$(INV_CYAN) $(BG_CYAN)$(FG_YELLOW)★$(INV_CYAN) $(BG_CYAN)$(FG_YELLOW)★$(INV_CYAN) $(BG_CYAN)$(FG_YELLOW)★\
	$(INV_CYAN)    $(NC)$(INV_CYAN)$(shell echo $(NAME) | tr a-z A-Z | tr '_' ' ')$(INV_CYAN)    \
	$(BG_CYAN)$(FG_YELLOW)★$(INV_CYAN) $(BG_CYAN)$(FG_YELLOW)★$(INV_CYAN) $(BG_CYAN)$(FG_YELLOW)★$(INV_CYAN) $(NC)\n"
	@printf "\t$(WHITE)────────────────────────\n$(NC)";
	@printf "\n\t────────────────────────$(NC)\033[1A\r"

# ──────────── #
# ── CURSOR ── #
# ──────────── #

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
			BAR=$$(printf "/ ─ \\ |" | cut -d" " -f$$((COUNTER % 4 + 1))); \
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
	@-find $(OBJ_DIR) -type d -empty -delete >/dev/null 2>&1 || true

# ─────────────────── #
# ── PROGRESS LINE ── #
# ─────────────────── #

_progress:
	@total=24; printf "\r\t"; for i in $$(seq 1 $$total); do printf "$(RED)─"; sleep 0.01; done; printf "$(NC)"
	@total=24; printf "\r\t"; for i in $$(seq 1 $$total); do printf "─"; sleep 0.01; done; printf "\n$(NC)"

# ─────────── #
# ── PHONY ── #
# ─────────── #

wipe:
	@rm -rf build; rm -f .is_re; rm -f $(NAME); rm -f $(NAME);

.PHONY: all clean fclean re normal _delete_objects _title _hide_cursor _show_cursor _progress _compile _uptodate wipe
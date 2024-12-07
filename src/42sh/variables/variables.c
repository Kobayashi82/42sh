/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variables.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 17:39:40 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/07 22:23:51 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "42sh.h"

t_var *main_table[HASH_SIZE];

unsigned int hash_index(const char *key) {
	unsigned int hash = 0;

	//	31 is a commonly used prime number
	while (*key) hash = (hash * 31) + *key++;
	return (hash % HASH_SIZE);
}

void variable_add(t_var **table, const char *name, const char *value, int exported, int readonly, int integer) {
	if (!name) return;

	t_var *new_var = variable_find(table, name);
	if (new_var) {
		free(new_var->value);
		new_var->value = ft_strdup(value);
		new_var->readonly = readonly;
		new_var->exported = exported;
		new_var->integer = integer;
		return;
	}

	unsigned int index = hash_index(name);
	new_var = safe_malloc(sizeof(t_var));

	new_var->name = ft_strdup(name);
	new_var->value = NULL;
	if (value) new_var->value = ft_strdup(value);
	if (!new_var->name || (value && !new_var->value)) {
		free(new_var->name); free(new_var->value);
		exit_error(NO_MEMORY, 1, NULL, true);
	}

	new_var->readonly = readonly;
	new_var->exported = exported;
	new_var->integer = integer;
	new_var->next = table[index];
	table[index] = new_var;
}

t_var *variable_find(t_var **table, const char *name) {
	if (!name) return (NULL);

	unsigned int index = hash_index(name);
	t_var *var = table[index];

	while (var) {
		if (!ft_strcmp(var->name, name)) return (var);
		var = var->next;
	}

	return (NULL);
}

int variable_delete(t_var **table, const char *name) {
	if (!name) return (1);

	unsigned int index = hash_index(name);
	t_var *var = table[index];
	t_var *prev = NULL;

	while (var) {
		if (!ft_strcmp(var->name, name)) {
			if (prev)	prev->next = var->next;
			else		table[index] = var->next;
			free(var->name); free(var->value); free(var);
			return (0);
		}
		prev = var;
		var = var->next;
	}
	
	return (1);
}

void variable_clear(t_var **table) {
	for (unsigned int index = 0; index < HASH_SIZE; index++) {
		if (table[index]) {
			t_var *var = table[index];
			while (var) {
				t_var *next = var->next;
				free(var->name);
				free(var->value);
				free(var);
				var = next;
			}
			table[index] = NULL;
		}
	}
}

void variables_add_array(t_var **table, char **array) {
    if (!array) return;

    for (size_t i = 0; array[i]; i++) {
        char *key = NULL, *value = NULL;
        get_key_value(array[i], &key, &value, '=');
		if (!key) continue;
        variable_add(table, key, value, 1, 0, 0);
        free(key); free(value);
    }
	fflush(stdout);
}


char **variables_to_array(t_var **table, int type) {
    size_t i = 0;

    for (unsigned int index = 0; index < HASH_SIZE; index++) {
        t_var *var = table[index];
        while (var) {
			if ((type == EXPORTED_LIST && var->exported) || (type == EXPORTED  && var->exported && var->value)) i++;
			if (type == INTERNAL && !var->exported && var->value) i++;
			if (type == READONLY && var->readonly && var->value) i++;
			var = var->next;
		}
    }

	if (i == 0) return (NULL);
	char **array = safe_malloc((i + 1) * sizeof(char *));

    i = 0;
    for (unsigned int index = 0; index < HASH_SIZE; index++) {
        t_var *var = table[index];
        while (var) {
			if (var->value || (!var->value && type == EXPORTED_LIST)) {
				if ((type == EXPORTED_LIST || type == EXPORTED) && !var->exported)	{ var = var->next; continue; }
				if (type == INTERNAL && var->exported) 								{ var = var->next; continue; }
				if (type == READONLY && !var->readonly)								{ var = var->next; continue; }
				size_t len = ft_strlen(var->name) + ft_strlen(var->value) + 2;
				array[i] = malloc(len);
				if (!array[i]) {
					for (size_t j = 0; j < i; j++) free(array[j]);
					free(array); exit_error(NO_MEMORY, 1, NULL, true);
				}

				ft_strcpy(array[i], var->name);
				if (var->value) {
					ft_strcat(array[i], "=");
					ft_strcat(array[i], var->value);
				}

            	i++;
			}
			var = var->next;
        }
    } array[i] = NULL;

	sort_array(array);
    return (array);
}

void variables_join(t_var **table_1, t_var **table_2) {
    for (int index = 0; index < HASH_SIZE; index++) {
        t_var *var = table_2[index];
        while (var) {
            variable_add(table_1, var->name, var->value, var->exported, var->readonly, var->integer);
            var = var->next;
        }
    }
}


// void variable_list(t_var **table, int type) {
// 	for (unsigned int index = 0; index < HASH_SIZE; index++) {
// 		if (table[index]) {
// 			t_var *var = table[index];
// 			while (var) {
// 				t_var *next = var->next;
// 				free(var->name);
// 				free(var->value);
// 				free(var);
// 				var = next;
// 			}
// 			table[index] = NULL;
// 		}
// 	}
// }


void sort_array(char **array) {
    if (!array) return;

    size_t count = 0;
    while (array[count]) count++;

    for (size_t i = 0; i < count - 1; i++) {
        for (size_t j = 0; j < count - i - 1; j++) {
            if (ft_strcmp(array[j], array[j + 1]) > 0) {
                char *temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }
}












// Readonly	La variable es de solo lectura. No se puede modificar ni eliminar.
// Exported	La variable está exportada al entorno y será visible para procesos hijo.
// Integer	La variable solo almacena valores numéricos (realiza cálculos automáticamente).

// declare [-rxi] nombre="valor" or [-p] [name]
//
// -r: Marca la variable como readonly.
// -x: Exporta la variable (como export).
// -i: Marca la variable como un entero (solo números).

// readonly [-p] name[=value]

//	compgen -v | while read var; do printf "%s=%q\n" "$var" "${!var}"; done

//	export c	// el value es NULL y por lo tanto no se exportará

// BASH=/usr/bin/bash
// BASHOPTS=checkwinsize:cmdhist:complete_fullquote:expand_aliases:extglob:extquote:force_fignore:globasciiranges:globskipdots:histappend:interactive_comments:patsub_replacement:progcomp:promptvars:sourcepath
// BASHPID=84016
// BASH_ALIASES=''
// BASH_ARGC=0
// BASH_ARGV=''
// BASH_ARGV0=bash
// BASH_CMDS=''
// BASH_COMMAND=printf\ \"%s=%q\\n\"\ \"\$var\"\ \"\$\{\!var\}\"
// BASH_COMPLETION_VERSINFO=2
// BASH_LINENO=''
// BASH_LOADABLES_PATH=/usr/local/lib/bash:/usr/lib/bash:/opt/local/lib/bash:/usr/pkg/lib/bash:/opt/pkg/lib/bash:.
// BASH_SOURCE=''
// BASH_SUBSHELL=1
// BASH_VERSINFO=5
// BASH_VERSION=5.2.21\(1\)-release
// COLUMNS=133
// COMP_WORDBREAKS=$' \t\n"\'><=;|&(:'
// DIRSTACK=/mnt/d/Programas/Drivers/Mis\ Programas/C/Active/42sh
// EPOCHREALTIME=1733590531.932608
// EPOCHSECONDS=1733590531
// EUID=1000
// GROUPS=1000
// HISTCMD=2000
// HISTCONTROL=ignoreboth
// HISTFILE=/home/kobayashi/.bash_history
// HISTFILESIZE=2000
// HISTSIZE=1000
// HOSTNAME=PADRE
// HOSTTYPE=x86_64
// IFS=$' \t\n'
// LESSCLOSE=/usr/bin/lesspipe\ %s\ %s
// LESSOPEN=\|\ /usr/bin/lesspipe\ %s
// LINENO=1
// LINES=19
// LS_COLORS=''
// MACHTYPE=x86_64-pc-linux-gnu
// MAILCHECK=60
// NVM_DIR=/.nvm
// OLDPWD=/mnt/d/Programas/Drivers/Mis\ Programas/C/Active/42sh
// OPTERR=1
// OPTIND=1
// OSTYPE=linux-gnu
// PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
// PIPESTATUS=0
// PPID=77847
// PS1=\$\{debian_chroot:+\(\$debian_chroot\)\}\\u@\\h:\\w\\\$\ .
// PS2=\>\ .
// PS4=+\ .
// PWD=/mnt/d/Programas/Drivers/Mis\ Programas/C/Active/42sh
// RANDOM=26355
// SECONDS=9
// SHELL=/bin/bash
// SHELLOPTS=braceexpand:emacs:hashall:histexpand:history:interactive-comments:monitor
// SHLVL=1
// SRANDOM=253357727
// TERM=dumb
// UID=1000
// _=var
// __git_printf_supports_v=yes
// _backup_glob=@\(#\*#\|\*@\(~\|.@\(bak\|orig\|rej\|swp\|dpkg\*\|rpm@\(orig\|new\|save\)\)\)\)
// _xspecs=''
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 16:51:51 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/22 20:03:42 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region "Variables"

	#pragma region "Enumerators"

		enum e_option {
			O_SET,
			O_SHOPT
		};

	#pragma endregion

	#pragma region "Structures"

		typedef struct s_options {

			//	=== SHELL ===
			int	allexport;					//	SET -a		1 = Auto-export all modified/created variables			0 = Manual export
			int	errexit;					//	SET -e		1 = Exit on command failure								0 = Continue
			int	errtrace;					//	SET -E		1 = Inherit ERR trap in functions/subshells				0 = Don't inherit
			int	functrace;					//	SET -T		1 = Inherit DEBUG/RETURN traps in functions				0 = Don't inherit
			int	nounset;					//	SET -u		1 = Error on undefined variable expansion				0 = Expand to empty
			int	noclobber;					//	SET -C		1 = Prevent overwriting files with >					0 = Allow overwrite
			int	noexec;						//	SET -n		1 = Read commands but don't execute (syntax check)		0 = Execute normally
			int	notify;						//	SET -b		1 = Report background job status immediately			0 = Report on prompt
			int	monitor;					//	SET -m		1 = Enable job control									0 = Disable job control
			int	privileged;					//	SET -p		1 = Privileged/restricted mode							0 = Normal mode
			int	pipefail;					//	SET			1 = Pipeline returns status of last failed command		0 = Returns last command
			int	hashall;					//	SET -h		1 = Hash command locations when found					0 = Don't hash
			int	ignoreeof;					//	SET			1 = Don't exit on EOF (require exit/logout)				0 = Exit on EOF
			int	execfail;					//	SHOPT		1 = Shell doesn't exit if exec fails					0 = Exit on exec failure
			int	checkjobs;					//	SHOPT		1 = List stopped/background jobs before exit			0 = Don't list
			int	huponexit;					//	SHOPT		1 = Send SIGHUP to all jobs on interactive login exit	0 = Don't send
			int	inherit_errexit;			//	SHOPT		1 = Subshells inherit errexit state						0 = Don't inherit
			int	login_shell;				//	SHOPT		1 = Shell is a login shell (READ-ONLY)					0 = Not login shell
			int	restricted_shell;			//	SHOPT		1 = Shell is in restricted mode (READ-ONLY)				0 = Not restricted
			
			//	=== READINPUT ===
			int	emacs;						//	SET			1 = Emacs editing mode									0 = Disabled
			int	vi;							//	SET			1 = Vi editing mode										0 = Disabled
			int	hide_ctrl_chars;			//	O_SET		1 = Hide control characters (^C)						0 = Show them
			int	multiwidth_chars;			//	O_SET		1 = Enable multi-width character support				0 = ASCII only
			
			//	=== HISTORY ===
			int	history;					//	SET			1 = Command history enabled								0 = Disabled
			int	histexpand;					//	SET -H		1 = Enable history expansion (!!)						0 = Disabled
			int	histappend;					//	SHOPT		1 = Append to history file								0 = Overwrite file
			int	histreedit;					//	SHOPT		1 = Allow reedit of failed history substitution			0 = Disabled
			int	histverify;					//	SHOPT		1 = Load expansion result into buffer for review		0 = Execute directly
			
			//	=== EXPANSION ===
			int	expand_aliases;				//	SHOPT		1 = Expand aliases										0 = Disabled
			int	braceexpand;				//	SET -B		1 = Enable brace expansion {a,b,c}						0 = Disabled
			int	noglob;						//	SET -f		1 = Disable pathname expansion (globbing)				0 = Enable globbing
			int	dotglob;					//	SHOPT		1 = Include hidden files in glob patterns				0 = Exclude dot files
			int	nullglob;					//	SHOPT		1 = Non-matching patterns expand to empty string		0 = Keep literal pattern
			int	failglob;					//	SHOPT		1 = Non-matching patterns cause error					0 = No error
			int	nocaseglob;					//	SHOPT		1 = Case-insensitive pathname expansion					0 = Case-sensitive
			int	nocasematch;				//	SHOPT		1 = Case-insensitive pattern matching in case/[[		0 = Case-sensitive
			int	extglob;					//	SHOPT		1 = Enable extended glob patterns (?()|*()|+()|!())		0 = Basic patterns only
			int	globstar;					//	SHOPT		1 = ** matches directories recursively					0 = ** is literal
			int	globasciiranges;			//	SHOPT		1 = Use ASCII order for [a-z] ranges					0 = Use locale order
			int	globskipdots;				//	SHOPT		1 = Glob patterns never match . and ..					0 = Can match dots
			int	extquote;					//	SHOPT		1 = Enable $'string' and $"string" in param expansion	0 = Disabled
			int	patsub_replacement;			//	SHOPT		1 = & in ${var//pat/repl} references matched text		0 = & is literal
			
			//	=== DIRECTORY ===
			int	physical;					//	SET -P		1 = Don't follow symlinks (physical paths)				0 = Follow symlinks
			int	cdable_vars;				//	SHOPT		1 = Try cd argument as variable name if path fails		0 = Disabled
			int	autocd;						//	SHOPT		1 = Directory name alone acts as 'cd directory'			0 = Require 'cd' command
			int	cdspell;					//	SHOPT		1 = Auto-correct minor spelling errors in cd paths		0 = No correction
			int	dirspell;					//	SHOPT		1 = Auto-correct directory spelling during completion	0 = No correction
			int	direxpand;					//	SHOPT		1 = Expand directory names during line completion		0 = Don't expand
			int	checkwinsize;				//	SHOPT		1 = Update LINES and COLUMNS after each command			0 = Don't update
			
			//	=== COMPLETION ===
			int	progcomp;					//	SHOPT		1 = Enable programmable completion facilities			0 = Disabled
			int	progcomp_alias;				//	SHOPT		1 = Attempt programmable completion for aliases			0 = Disabled
			int	hostcomplete;				//	SHOPT		1 = Attempt hostname completion when @ present			0 = Disabled
			int	no_empty_cmd_completion;	//	SHOPT		1 = Don't complete on empty line						0 = Attempt completion
			int	complete_fullquote;			//	SHOPT		1 = Quote all metacharacters in filename completion		0 = Minimal quoting
			int	force_fignore;				//	SHOPT		1 = Force use of FIGNORE suffixes in word completion	0 = Don't force
			
			//	=== EXECUTION ===
			int	checkhash;					//	SHOPT		1 = Check hashed commands exist before execution		0 = Don't check
			int	sourcepath;					//	SHOPT		1 = Use PATH to search for source command argument		0 = Don't use PATH
			int	lastpipe;					//	SHOPT		1 = Run last pipeline command in current shell			0 = Run in subshell
			
			//	=== VARIABLE ===
			int	assoc_expand_once;			//	SHOPT		1 = Expand associative arrays only once in assignments	0 = Multiple expansion
			int	localvar_inherit;			//	SHOPT		1 = Local vars inherit value from same-named var		0 = Don't inherit
			int	localvar_unset;				//	SHOPT		1 = Local vars inherit unset state from upper scope		0 = Don't inherit
			
			//	=== PROMPT ===
			int	promptvars;					//	SHOPT		1 = Expand variables/command sub in prompt strings		0 = Don't expand
			
			//	=== DEBUG ===
			int	verbose;					//	SET -v		1 = Print shell input lines as read						0 = Silent
			int	xtrace;						//	SET -x		1 = Print commands and arguments before execution		0 = Silent
			int	extdebug;					//	SHOPT		1 = Enable extended debugging behavior for debugger		0 = Normal behavior
			int	shift_verbose;				//	SHOPT		1 = shift prints error if count exceeds positionals		0 = Silent error
			int	gnu_errfmt;					//	SHOPT		1 = Error messages in standard GNU format				0 = Default format
			int	xpg_echo;					//	SHOPT		1 = echo expands backslash escapes without -e			0 = Require -e flag
			int	interactive_comments;		//	O_SET		1 = Allow # comments in interactive commands			0 = No comments

		} t_options;

	#pragma endregion

#pragma endregion

#pragma region "Methods"

	int		option_set(const char *name, int value, int type);
	int		option_set_char(char c, int value);
	int		option_get(const char *name);
	int		option_get_char(char c);
	int		option_print(const char *name, int type, int reusable, int suppress);
	int		options_print(int type, int reusable, int suppress);
	void	options_initialize();

#pragma endregion

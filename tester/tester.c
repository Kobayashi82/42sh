/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tester.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 21:06:44 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/30 16:47:10 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <unistd.h>
	#include <sys/wait.h>
	#include <ctype.h>

	#define MAX_CMD_LEN 1024
	#define OUTPUT_FILE "output"

	#define G	"\033[1;32m"
	#define RD	"\033[1;31m"
	#define C	"\033[1;36m"
	#define Y	"\033[1;33m"
	#define NC	"\033[0m"

#pragma endregion

#pragma region "Variables"

	static FILE *output;

#pragma endregion

#pragma region "Execute"

	int execute(const char *cmd, char *output, char *error_output, const char *shell) {
		int stdout_pipe[2], stderr_pipe[2];

		if (pipe(stdout_pipe) == -1 || pipe(stderr_pipe) == -1) return (42);

		pid_t pid = fork();
		if (pid == 0) {
			close(stdout_pipe[0]);
			close(stderr_pipe[0]);

			if (dup2(stdout_pipe[1], STDOUT_FILENO) == -1 || dup2(stderr_pipe[1], STDERR_FILENO) == -1) exit(42);
			execlp(shell, shell, "-c", cmd, (char *)NULL);
			exit(42);
		} else if (pid > 0) {
			close(stdout_pipe[1]);
			close(stderr_pipe[1]);

			int status;
			waitpid(pid, &status, 0);

			if (WIFEXITED(status))		status = WEXITSTATUS(status);
			if (WIFSIGNALED(status))	status = WTERMSIG(status);

			read(stdout_pipe[0], output, MAX_CMD_LEN);
			read(stderr_pipe[0], error_output, MAX_CMD_LEN);

			close(stdout_pipe[0]);
			close(stderr_pipe[0]);

			return (status);
		} else return (42);
	}

#pragma endregion

#pragma region "Is Spaces"

	int isspaces(const char *line) {
		while (*line) {
			if (!isspace((unsigned char)*line)) return (0);
			line++;
		} return (1);
	}

#pragma endregion

#pragma region "Compare Outputs"

	void normalize_output(char *error_output) {
		if (!error_output) return;
		char *end_of_line = strstr(error_output, "42sh: ");
		if (end_of_line) memmove(error_output, end_of_line + 6, strlen(end_of_line + 6) + 1);
		end_of_line = strstr(error_output, "bash: ");
		if (end_of_line) memmove(error_output, end_of_line + 6, strlen(end_of_line + 6) + 1);
		end_of_line = strstr(error_output, "line");
		if (end_of_line) end_of_line = strstr(end_of_line, ": ");
		if (end_of_line) memmove(error_output, end_of_line + 2, strlen(end_of_line + 2) + 1);
	}

	int compare_outputs(const char *output1, const char *error1, const char *output2, const char *error2) {
		normalize_output((char *)error1);
		normalize_output((char *)error2);
		return (strcmp(output1, output2) || strcmp(error1, error2));
	}

#pragma endregion

#pragma region "Reset Output"

	void reset_output() {
		FILE *output = fopen(OUTPUT_FILE, "w");
		fclose(output);
	}

#pragma endregion

#pragma region "Run Tests"

	void run_tests(const char *filename) {
		FILE *file = fopen(filename, "r");
		if (!file) {
			fprintf(output, "Error opening file '%s'\n", filename);
			printf(RD"%s\n\n"NC, filename);
			return;
		} else {
			fprintf(output, "FILE: %s\n", filename);
			fprintf(output, "======================\n");
			printf(C"%s"NC"\n", filename);
		}

		char cmd[MAX_CMD_LEN];
		char output_42sh[MAX_CMD_LEN], error_42sh[MAX_CMD_LEN];
		char output_bash[MAX_CMD_LEN], error_bash[MAX_CMD_LEN];
		int test_number = 0, noprint = 0;

		while (fgets(cmd, MAX_CMD_LEN, file)) {
			fflush(stdout); fflush(stderr);
			memset(output_42sh, 0, MAX_CMD_LEN);
			memset(error_42sh, 0, MAX_CMD_LEN);
			memset(output_bash, 0, MAX_CMD_LEN);
			memset(error_bash, 0, MAX_CMD_LEN);

			cmd[strcspn(cmd, "\n")] = 0;
			if (isspaces(cmd)) continue;
			if (*cmd == '#') {
				memmove(cmd, &cmd[1], strlen(&cmd[1]) + 1);
				fprintf(output, "----------------------\n");
				fprintf(output, "%s\n", cmd);
				if (!noprint)fprintf(output, "----------------------\n");
				continue;
			} test_number++;

			if (execute(cmd, output_42sh, error_42sh, "./42sh") == 42 || execute(cmd, output_bash, error_bash, "bash") == 42) {
				fprintf(output, "%02d - Failed to execute %s\n", test_number, cmd);
				fprintf(output, "\n----------------------\n\n");
				printf(RD"%02d"NC" ", test_number);
				continue;
			}

			if (noprint)fprintf(output, "----------------------\n");
			else noprint = 1;

			if (compare_outputs(output_42sh, error_42sh, output_bash, error_bash) == 0) {
				fprintf(output, "%02d - PASS - '%s'\n", test_number, cmd);
				fprintf(output, "**********************\n");
				printf(G"%02d"NC" ", test_number);
			} else {
				fprintf(output, "%02d - FAIL - '%s'\n", test_number, cmd);
				fprintf(output, "**********************\n");
				printf(RD"%02d"NC" ", test_number);
			}

			int printline = 0;
			if (!isspaces(output_42sh) || !isspaces(output_bash)) {
				fprintf(output, "42sh:\n%s", output_42sh);
				fprintf(output, "- - - - - - - - - - - \n");
				fprintf(output, "bash:\n%s", output_bash);
				printline = 1;
			}
			if (!isspaces(error_42sh) || !isspaces(error_bash)) {
				if (printline) { fprintf(output, "----------------------\n"); }
				fprintf(output, "42sh err:\n%s", error_42sh);
				fprintf(output, "- - - - - - - - - - - \n");
				fprintf(output, "bash err:\n%s", error_bash);
			}

		}

		fprintf(output, "----------------------\n");
		fprintf(output, "\n======================\n");
		printf("\n\n");

		fclose(file);
	}

#pragma endregion

#pragma region "Main"

	int main(int argc, char **argv, char **envp) {
		if (argc < 2) {
			fprintf(stderr, "Usage: %s [test files]\n", *argv);
			return (1);
		}

		if (access("./42sh", X_OK)) {
			dprintf(2, "Cannot find ./42sh\n");
			return (1);
		}

		reset_output();
		output = fopen(OUTPUT_FILE, "a");
		fprintf(output, "======================\n");
		printf("\n""\033[?25l");

		while (*(++argv)) run_tests(*argv);
		fclose(output);
		printf("\033[?25h");

		return (0);
	}

#pragma endregion

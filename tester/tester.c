/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tester.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 21:06:44 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/29 17:22:09 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

static FILE *file;

int execute_command(const char *cmd, char *output, char *error_output, const char *shell) {
	int stdout_pipe[2], stderr_pipe[2];

    if (pipe(stdout_pipe) == -1 || pipe(stderr_pipe) == -1) return (42);

    pid_t pid = fork();
    if (pid == 0) {
        close(stdout_pipe[0]);
        close(stderr_pipe[0]);

		if (dup2(stdout_pipe[1], STDOUT_FILENO) == -1 || dup2(stderr_pipe[1], STDERR_FILENO) == -1) exit(42);
        if (strcmp(shell, "bash") == 0)	execlp("bash", "bash", "-c", cmd, (char *)NULL);
        else							execlp("./42sh", "42sh", "-c", cmd, (char *)NULL);
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

int is_empty_or_whitespace(const char *line) {
    while (*line) {
        if (!isspace((unsigned char)*line)) return (0);
        line++;
    } return (1);
}

void normalize_error(char *error_output) {
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
	normalize_error((char *)error1);
	normalize_error((char *)error2);
	return (strcmp(output1, output2) || strcmp(error1, error2));
}

void reset_output_file() {
    FILE *file = fopen(OUTPUT_FILE, "w");
	fclose(file);
}

void run_tests(const char *test_file) {
    FILE *testfile = fopen(test_file, "r");
    if (!testfile) {
		fprintf(file, "Error opening file '%s'\n", test_file);
		dprintf(2, "Error opening file '%s'\n", test_file);
        return;
    }

    char cmd[MAX_CMD_LEN];
	char output_42sh[MAX_CMD_LEN], error_42sh[MAX_CMD_LEN];
	char output_bash[MAX_CMD_LEN], error_bash[MAX_CMD_LEN];
    int test_number = 1;

	fprintf(file, "FILE: %s\n", test_file);
	fprintf(file, "======================\n");
    printf(Y"FILE: "C"%s"NC"\n\n", test_file);

	int noprint = 0;
    while (fgets(cmd, MAX_CMD_LEN, testfile)) {

		memset(output_42sh, 0, MAX_CMD_LEN);
		memset(error_42sh, 0, MAX_CMD_LEN);
		memset(output_bash, 0, MAX_CMD_LEN);
		memset(error_bash, 0, MAX_CMD_LEN);

        cmd[strcspn(cmd, "\n")] = 0;
        if (is_empty_or_whitespace(cmd)) continue;

        if (execute_command(cmd, output_42sh, error_42sh, "42sh") == 42) {
            fprintf(file, "%02d - Failed to execute %s\n", test_number, cmd);
			fprintf(file, "\n----------------------\n\n");
			dprintf(2, Y"%02d"NC" - Failed to execute '"C"%s"NC"'\n", test_number, cmd);
            test_number++;
            return;
        }

        if (execute_command(cmd, output_bash, error_bash, "bash") == 42) {
            fprintf(file, "%02d - Failed to execute '%s'\n", test_number, cmd);
			fprintf(file, "\n----------------------\n\n");
			dprintf(2, Y"%02d"NC" - Failed to execute '"C"%s"NC"'\n", test_number, cmd);
            test_number++;
            return;
        }

		if (noprint)fprintf(file, "----------------------\n");
		else noprint = 1;

        if (compare_outputs(output_42sh, error_42sh, output_bash, error_bash) == 0) {
			fprintf(file, "%02d - PASS - '%s'\n", test_number, cmd);
			fprintf(file, "**********************\n");
            printf(Y"%02d"NC" - "G"PASS"NC" - "C"%s"NC"\n", test_number, cmd);
        } else {
			fprintf(file, "%02d - FAIL - '%s'\n", test_number, cmd);
			fprintf(file, "**********************\n");
            printf(Y"%02d"NC" - "RD"FAIL"NC" - "C"%s"NC"\n", test_number, cmd);
        }

		int printline = 0;
		if (!is_empty_or_whitespace(output_42sh) || !is_empty_or_whitespace(output_bash)) {
			fprintf(file, "42sh:\n%s", output_42sh);
			fprintf(file, "- - - - - - - - - - - \n");
			fprintf(file, "bash:\n%s", output_bash);
			printline = 1;
		}
		if (!is_empty_or_whitespace(error_42sh) || !is_empty_or_whitespace(error_bash)) {
			if (printline) { fprintf(file, "----------------------\n"); printline = 0; }
			fprintf(file, "42sh err:\n%s", error_42sh);
			fprintf(file, "- - - - - - - - - - - \n");
			fprintf(file, "bash err:\n%s", error_bash);
		}

        test_number++;
    }

	fprintf(file, "----------------------\n");
	fprintf(file, "\n======================\n");
	printf("\n");

    fclose(testfile);
}

int main(int argc, char **argv, char **envp) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s [test files]\n", *argv);
        return (1);
    }

	if (access("./42sh", X_OK)) {
		dprintf(2, "Cannot find ./42sh\n");
		return (1);
	}

	reset_output_file();
	file = fopen(OUTPUT_FILE, "a");
	fprintf(file, "======================\n");
	printf("\n");
	while (*(++argv)) run_tests(*argv);
	fclose(file);

    return (0);
}

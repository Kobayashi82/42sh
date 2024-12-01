/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 15:57:35 by vzurera-          #+#    #+#             */
/*   Updated: 2024/11/29 16:23:29 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

int	g_signal = 0;

// //	Handle SIGQUIT signal
// void	sigquit_handler(int sig) {
// 	(void) sig;
// 	write(1, "Quit\n", 5);
// }

// //	Handle SIGINT signal
// void	sigint_handler(int sig) {
// 	if (sig == SIGINT) {
// 		g_signal = 130;
// 		ioctl(STDIN_FILENO, TIOCSTI, "\n");
// 		rl_replace_line("", 0);
// 		rl_on_new_line();
// 	}
// }

// //	Handle SIGINT signal
// void	sigint_killer(int sig) {
// 	if (sig == SIGINT) g_signal = 130;
// }

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   banner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 14:08:19 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/01 21:06:28 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

//	Print the banner
void print_banner() {
	print(1, BR"\n\t————————", RESET);
	print(1, "——————————————————"Y, JOIN);
	print(1, "——————————————", JOIN);
	print(1, "——————————————————\n", JOIN);
	print(1, BR"\t| "Y" __ __   _   __  _   _   "BR, JOIN);
	print(1, " __   _  _   ___   _     _    "Y"|\n", JOIN);
	print(1, BR"\t| "Y"|  V  | | | |  \\| | | | "BR, JOIN);
	print(1, "/' _/ | || | | __| | |   | |   "Y"|\n", JOIN);
	print(1, BR"\t| "Y"| \\_/ | | | | | ' | | | "BR, JOIN);
	print(1, "`._`. | >< | | _|  | |_  | |_  "Y"|\n", JOIN);
	print(1, BR"\t| "Y"|_| |_| |_| |_|\\__| |_| "BR, JOIN);
	print(1, "|___/ |_||_| |___| |___| |___| "Y"|\n", JOIN);
	print(1, BR"\t|                                    ", JOIN);
	print(1, "                    "Y"|\n", JOIN);
	print(1, BR"\t|  "C"As beautiful as a shell   ", JOIN);
	print(1, Y"         "C"    "Y"   vzurera-    "Y"|\n"NC, JOIN);
	print(1, BR"\t———————————————", JOIN);
	print(1, "———————————"Y, JOIN);
	print(1, "———————————————————", JOIN);
	print(1, "—————————————", JOIN);
	print(1, "\n\n"NC, PRINT);
}

void print_welcome() {
	print(1, G"\n\t\t\tWELCOME TO "Y"MINI"BR"SHELL"G" 1.0\n\n", RESET);
	print(1, C"\t This "BR"project"C" has been made with ", JOIN);
    print(1, G"effort"C" and "G"care\n", JOIN);
	print(1, C"\t Feel "G"free"C" to try anything you want\n", JOIN);
	print(1, "\t To get information about the "BR"builtins", JOIN);
	print(1, C" you can type "Y"help\n\n", JOIN);
    print(1, C"\t Check the "G"documentation"C" for", JOIN);
	print(1, " a complete list of features\n\n", PRINT);
}

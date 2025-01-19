/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   banner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 14:08:19 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/19 20:07:48 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "terminal/terminal.h"

#pragma region Banner

	void banner() {                                            
		print(1, RED600"\n\t        ,--,                                   \n", RESET);
		print(1, RED600"\t      ,--.'|      ,----,  "Y"           ,---,     \n", JOIN);
		print(1, RED600"\t   ,--,  | :    .'   .' \\"Y"          ,--.' |     \n", JOIN);
		print(1, RED600"\t,---.'|  : '  ,----,'    |"Y"         |  |  :     \n", JOIN);
		print(1, RED600"\t;   : |  | ;  |    :  .  ;"Y".--.--.  :  :  :     \n", JOIN);
		print(1, RED600"\t|   | : _' |  ;    |.'  /"Y"/  /    ' :  |  |,--. \n", JOIN);
		print(1, RED600"\t:   : |.'  |  `----'/  ;"Y"|  :  /`./ |  :  '   | \n", JOIN);
		print(1, RED600"\t|   ' '  ; :    /  ;  / "Y"|  :  ;_   |  |   /' : \n", JOIN);
		print(1, RED600"\t\\   \\  .'. |   ;  /  /-,"Y" \\  \\    `.'  :  | | | \n", JOIN);
		print(1, RED600"\t `---`:  | '  /  /  /.`|"Y"  `----.   \\  |  ' | : \n", JOIN);
		print(1, RED600"\t      '  ; |./__;      :"Y" /  /`--'  /  :  :_:,' \n", JOIN);
		print(1, RED600"\t      |  : ;|   :    .' "Y"'--'.     /|  | ,'     \n", JOIN);
		print(1, RED600"\t      '  ,/ ;   | .'     "Y" `--'---' `--''       \n", JOIN);
		print(1, RED600"\t      '--'  `---'                              \n\n"NC, PRINT);
	}

#pragma endregion

#pragma region Welcome

	void welcome() {
		print(1, G"\n\t\t\tWELCOME TO "RED600"42"Y"sh"G" 1.0\n\n", RESET);
		print(1, C"\t This "BR"project"C" has been made with ", JOIN);
		print(1, G"effort"C" and "G"care\n", JOIN);
		print(1, C"\t Feel "G"free"C" to try anything you want\n", JOIN);
		print(1, "\t To get information about the "BR"builtins", JOIN);
		print(1, C" you can type "Y"help\n\n", JOIN);
		print(1, C"\t Check the "G"documentation"C" for", JOIN);
		print(1, " a complete list of features\n\n", PRINT);
	}

#pragma endregion

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tilde.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 11:49:19 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/19 22:11:58 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// tilde expansion

// ~			→ $HOME (o home del usuario actual)
// ~user		→ home del usuario "user" (getpwnam)
// ~/path		→ $HOME/path
// ~user/path	→ /home/user/path

// ~+			→ $PWD (tu cwd interno)
// ~-			→ $OLDPWD (getenv("OLDPWD"))

// ~N			→ DIRSTACK[N] (tu stack[N])
// ~+N			→ DIRSTACK[N] (igual que ~N)
// ~-N			→ DIRSTACK[longitud - N] (desde el final)

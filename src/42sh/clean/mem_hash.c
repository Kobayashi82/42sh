/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem_hash.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 17:39:40 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/10 14:12:57 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

#pragma region Variables
	
	t_mem	*mem_table[MEM_HASH_SIZE];

#pragma endregion

#pragma region Index

	static unsigned int hash_index_ptr(void *ptr) {
		unsigned long addr = (unsigned long)ptr;
		addr = ~addr + (addr << 15);
		addr = addr ^ (addr >> 12);
		addr = addr + (addr << 2);
		addr = addr ^ (addr >> 4);
		addr = addr * 2057;
		addr = addr ^ (addr >> 16);

		return (addr % MEM_HASH_SIZE);
	}

#pragma endregion

#pragma region Find

	static t_mem *mem_find(void *ptr) {
		if (!ptr) return (NULL);

		t_mem *mem = mem_table[hash_index_ptr(ptr)];

		while (mem) {
			if (mem->ptr == ptr) return (mem);
			mem = mem->next;
		}

		return (NULL);
	}

#pragma endregion

#pragma region Add

	static void mem_add(void *ptr) {
		if (!ptr) return;

		t_mem *new_mem = mem_find(ptr);
		if (new_mem) {
			free(new_mem->ptr);
			new_mem->ptr = ptr;
		} else {
			unsigned int index = hash_index_ptr(ptr) % MEM_HASH_SIZE;
			t_mem *new_node = malloc(sizeof(t_mem));
			if (!new_node) exit_error(NO_MEMORY, 1, NULL, true);

			new_node->ptr = ptr;
			new_node->next = mem_table[index];
			mem_table[index] = new_node;
		}
	}

#pragma endregion

#pragma region Delete

	static void mem_delete(void *ptr) {
		if (!ptr) return;

		unsigned int index = hash_index_ptr(ptr);
		t_mem *mem = mem_table[index];
		t_mem *prev = NULL;

		while (mem) {
			if (mem->ptr == ptr) {
				if (prev)	prev->next = mem->next;
				else		mem_table[index] = mem->next;
				free(mem->ptr); free(mem);
				return;
			}
			prev = mem;
			mem = mem->next;
		}
		free(ptr);
	}

#pragma endregion

#pragma region Clear

	static void mem_clear() {
		for (unsigned int index = 0; index < MEM_HASH_SIZE; index++) {
			if (mem_table[index]) {
				t_mem *mem = mem_table[index];
				while (mem) {
					t_mem *next = mem->next;
					free(mem->ptr);
					free(mem);
					mem = next;
				}
				mem_table[index] = NULL;
			}
		}
	}

#pragma endregion

	#pragma region Free

		//	A free function
		void safe_free(void *ptr) {
			mem_delete(ptr);
		}

		//	A free all function
		void safe_free_all() {
			mem_clear();
		}

	#pragma endregion

	#pragma region Malloc

		//	A calloc function with builtin exit_error
		void *safe_malloc(long bytes) {
			void *ptr;

			if (!(ptr = malloc(bytes)))
				exit_error(NO_MEMORY, 1, NULL ,true);

			mem_add(ptr);
			return (ptr);
		}

	#pragma endregion

	#pragma region Calloc

		//	A calloc function with builtin exit_error
		void *safe_calloc(int count, long bytes) {

			char	*ptr;
			int		n = count * bytes - 1;

			if (!(ptr = malloc(count * bytes)))
				exit_error(NO_MEMORY, 1, NULL, true);
			while (n >= 0) ptr[n--] = '\0';

			mem_add(ptr);
			return (ptr);
		}

	#pragma endregion

	#pragma region Realloc

		//	A realloc function with builtin exit_error
		void *safe_realloc(void *ptr, int old_size, int size) {
			void	*new_ptr;
			int		copy_size;

			if (size > 0) {
				new_ptr = malloc(size);
				if (!new_ptr) { safe_free(ptr); exit_error(NO_MEMORY, 1, NULL, true); }
				if (ptr) {
					if (old_size < size)	copy_size = old_size;
					else					copy_size = size;
					ft_memcpy(new_ptr, ptr, copy_size);
					safe_free(ptr);
				}
				mem_add(new_ptr);
				return (new_ptr);
			}

			safe_free(ptr);
			return (NULL);
		}

	#pragma endregion
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memsafe.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 11:07:00 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/21 14:02:00 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdint.h>

#define MEM_HASH_SIZE	1031

void	*__real_malloc(size_t size);
void	*__real_calloc(size_t nmemb, size_t size);
void	*__real_realloc(void *ptr, size_t size);
void	__real_free(void *ptr);

static void	*hash_index(void *ptr)
{
	static void		*hashtable[MEM_HASH_SIZE];
	unsigned long	addr;

	if (ptr == NULL)
		return (hashtable);
	addr = (unsigned long)ptr;
	addr = ~addr + (addr << 15);
	addr ^= (addr >> 12);
	addr += (addr << 2);
	addr ^= (addr >> 4);
	addr *= 2057;
	addr ^= (addr >> 16);
	return ((void *)(addr % MEM_HASH_SIZE));
}

static void	**mem_find(void *ptr)
{
	void	**hashtable;
	void	**mem;

	hashtable = (void **)hash_index(NULL);
	if (!hashtable || !ptr)
		return (NULL);
	mem = (void **)hashtable[(uintptr_t)hash_index(ptr) % MEM_HASH_SIZE];
	while (mem)
	{
		if (mem[0] == ptr)
			return (mem);
		mem = mem[1];
	}
	return (NULL);
}

static void	mem_delete(void *ptr, int just_node)
{
	void			**hashtable;
	void			**mem;
	void			**prev;
	unsigned int	i;

	hashtable = (void **)hash_index(NULL);
	if (!hashtable || !ptr)
		return;
	i = (uintptr_t)hash_index(ptr) % MEM_HASH_SIZE;
	mem = (void **)hashtable[i];
	prev = NULL;
	while (mem)
	{
		if (mem[0] == ptr)
		{
			if (prev)
				prev[1] = mem[1];
			else
				hashtable[i] = mem[1];
			if (!just_node) __real_free(mem[0]);
			__real_free(mem);
			return;
		}
		prev = mem;
		mem = mem[1];
	}
	__real_free(ptr);
}

void	*__wrap_malloc(size_t size)
{
	void			*ptr;
	void			**hashtable;
	void			**mem;
	void			**new_mem;
	unsigned int	i;

	ptr = __real_malloc(size);
	if (!ptr)
		return (NULL);
	hashtable = (void **)hash_index(NULL);
	if (!hashtable)
		return (NULL);
	mem = mem_find(ptr);
	if (mem)
		return (__real_free(*mem), *mem = ptr, ptr);
	i = (uintptr_t)hash_index(ptr) % MEM_HASH_SIZE;
	new_mem = __real_malloc(sizeof(void *) * 2);
	if (!new_mem)
		return (__real_free(ptr), NULL);
	new_mem[0] = ptr;
	new_mem[1] = hashtable[i];
	hashtable[i] = new_mem;
	return (ptr);
}

void	*__wrap_calloc(size_t nmemb, size_t size)
{
	void			*ptr;
	void			**hashtable;
	void			**mem;
	void			**new_mem;
	unsigned int	i;

	ptr = __real_calloc(nmemb, size);
	if (!ptr)
		return (NULL);
	hashtable = (void **)hash_index(NULL);
	if (!hashtable)
		return (NULL);
	mem = mem_find(ptr);
	if (mem)
		return (__real_free(*mem), *mem = ptr, ptr);
	i = (uintptr_t)hash_index(ptr) % MEM_HASH_SIZE;
	new_mem = __real_malloc(sizeof(void *) * 2);
	if (!new_mem)
		return (__real_free(ptr), NULL);
	new_mem[0] = ptr;
	new_mem[1] = hashtable[i];
	hashtable[i] = new_mem;
	return (ptr);
}

void	*__wrap_realloc(void *ptr, size_t size)
{
	void			*oldptr = ptr;
	void			**hashtable;
	void			**mem;
	void			**new_mem;
	unsigned int	i;

	ptr = __real_realloc(ptr, size);
	if (!ptr)			return (NULL);
	if (oldptr == ptr)	return (ptr);
	if (oldptr != ptr)	mem_delete(oldptr, 1);
	
	hashtable = (void **)hash_index(NULL);
	if (!hashtable)		return (NULL);

	mem = mem_find(ptr);
	if (mem)			return (__real_free(*mem), *mem = ptr, ptr);
	i = (uintptr_t)hash_index(ptr) % MEM_HASH_SIZE;
	new_mem = __real_malloc(sizeof(void *) * 2);
	if (!new_mem)		return (__real_free(ptr), NULL);
	new_mem[0] = ptr;
	new_mem[1] = hashtable[i];
	hashtable[i] = new_mem;

	return (ptr);
}

void	__wrap_free(void *ptr)
{
	void			**hashtable;
	void			**mem;
	void			*next;
	unsigned int	i;

	if (ptr == (void *)-42)
	{
		i = -1;
		hashtable = (void **)hash_index(NULL);
		while (hashtable && ++i < MEM_HASH_SIZE)
		{
			mem = (void **)hashtable[i];
			while (mem)
			{
				next = mem[1];
				__real_free(mem[0]);
				__real_free(mem);
				mem = next;
			}
			hashtable[i] = NULL;
		}
		return;
	}
	mem_delete(ptr, 0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   maxlen_handling.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpierce <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/19 11:44:59 by bpierce           #+#    #+#             */
/*   Updated: 2018/01/04 19:57:00 by bpierce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

/*
** For t_ls_maxlen struct
** -----------------------------------------------------------------------------
** set_new_maxlens will set l to the initial VALUES.
** adjust_new_maxlens will set the largest value to it's LENGTH.
*/

void	set_new_maxlens(t_ls_maxlen *l, t_ls_arg *node)
{
	unsigned int	len;

	if (!node || !node->stat)
		return ;
	l->total_blocks += node->stat->path_stat.st_blocks;
	l->nlink_len = l->nlink_len < node->stat->path_stat.st_nlink ?
		node->stat->path_stat.st_nlink : l->nlink_len;
	len = ft_unsignedintlen(node->stat->path_stat.st_size);
	l->size_len = l->size_len < len ? len : l->size_len;
	l->major_len = l->major_len < node->stat->ma_len
		? node->stat->ma_len : l->major_len;
	l->minor_len = l->minor_len < node->stat->mi_len
		? node->stat->mi_len : l->minor_len;
	if (l->major_len || l->minor_len)
	{
		l->size_len =
			l->size_len < (node->stat->ma_len + node->stat->mi_len + 2) ?
			(node->stat->ma_len + node->stat->mi_len + 2) : l->size_len;
	}
	len = ft_strlen(node->stat->pwd.pw_name);
	l->pwname_len = l->pwname_len < len ? len : l->pwname_len;
	len = ft_strlen(node->stat->grp.gr_name);
	l->grname_len = l->grname_len < len ? len : l->grname_len;
}

void	adjust_new_maxlens(t_ls_maxlen *l)
{
	l->nlink_len = ft_unsignedintlen(l->nlink_len);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpierce <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/19 11:44:59 by bpierce           #+#    #+#             */
/*   Updated: 2018/01/04 18:39:20 by bpierce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	do_ls_on_good_files(t_ls_flags *f, t_ls_arg *r, t_ls_maxlen *l, int i)
{
	if (!r)
		return ;
	do_ls_on_good_files(f, f->r ? r->right : r->left, l, i);
	if (!(S_ISDIR(r->stat->path_stat.st_mode)))
	{
		f->file_exists = 1;
		if (i == 1)
			set_new_maxlens(l, r);
		else
		{
			if (f->l)
				pretty_ls_print(r, l);
			else
				ft_putendl(r->s);
		}
	}
	else
		f->dir_exists = 1;
	do_ls_on_good_files(f, f->r ? r->left : r->right, l, i);
}

void	do_ls_on_good_dirnames(t_ls_flags *f, t_ls_arg *root)
{
	if (!root)
		return ;
	do_ls_on_good_dirnames(f, f->r ? root->right : root->left);
	if (S_ISDIR(root->stat->path_stat.st_mode))
		do_ls_stuff(f, root->s);
	do_ls_on_good_dirnames(f, f->r ? root->left : root->right);
}

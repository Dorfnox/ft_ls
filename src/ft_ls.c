/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpierce <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/19 11:44:59 by bpierce           #+#    #+#             */
/*   Updated: 2018/01/30 00:50:41 by bpierce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

/*
** ft_ls, in action babyyyyyy
** -----------------------------------------------------------------------------
*/

void	do_recursive_ls_stuff(t_ls_flags *f, t_ls_arg *root, char *cur_dir)
{
	if (!root)
		return ;
	do_recursive_ls_stuff(f, (f->r) ? root->right : root->left, cur_dir);
	if (!root->stat || (root->s[0] == '.' && root->s[1] == '.') ||
			(root->s[0] == '.' && !root->s[1]))
		;
	else if (S_ISDIR(root->stat->path_stat.st_mode) && (f->print_dir = 1))
		do_ls_stuff(f, root->full_path);
	do_recursive_ls_stuff(f, (f->r) ? root->left : root->right, cur_dir);
}

void	still_do_ls_stuff(t_ls_flags *f, t_ls_arg *r, char *cdn, t_ls_maxlen *l)
{
	f->l ? adjust_new_maxlens(l) : 0;
	f->print_newline = f->print_newline ? write(1, "\n", 1) : 1;
	f->print_dir ? ft_pflite("%s:\n", cdn) : 0;
	f->l && r ? ft_pflite("total %u\n", l->total_blocks) : 0;
	f->r ? print_args_right(f, r, l) : print_args_left(f, r, l);
	f->rr ? do_recursive_ls_stuff(f, r, cdn) : 0;
	free_binary_tree_ls(r);
}

void	do_ls_stuff(t_ls_flags *f, char *current_direc_name)
{
	DIR				*temp;
	struct dirent	*dir;
	t_ls_arg		*new;
	t_ls_arg		*root;

	if (!(temp = opendir(current_direc_name)))
	{
		f->print_newline = f->print_newline ? write(1, "\n", 1) : 1;
		f->print_dir ? ft_pflite("%s:\n", current_direc_name) : 1;
		ft_pflite("ft_ls: %s: %s\n", current_direc_name, strerror(errno));
		return ;
	}
	root = NULL;
	ft_bzero(&f->len, sizeof(t_ls_maxlen));
	while ((dir = readdir(temp)))
	{
		if (!f->a && dir->d_name[0] == '.')
			continue ;
		new = new_arg(current_direc_name, dir->d_name);
		root = insert_arg(f, root, new);
		f->l ? set_new_maxlens(&f->len, new) : 0;
	}
	closedir(temp);
	still_do_ls_stuff(f, root, current_direc_name, &f->len);
}

void	ft_ls(t_ls_flags *f)
{
	t_ls_maxlen		l;

	ft_bzero(&l, sizeof(t_ls_maxlen));
	print_bad_dirnames(f->bad_dirnames);
	f->print_dir = f->bad_dirnames || (f->good_dirnames &&
			(f->good_dirnames->left || f->good_dirnames->right)) ? 1 : 0;
	if (f->good_dirnames || f->bad_dirnames)
	{
		do_ls_on_good_files(f, f->good_dirnames, &l, 1);
		do_ls_on_good_files(f, f->good_dirnames, &l, 2);
		if (f->file_exists && f->dir_exists)
			write(1, "\n", 1);
		do_ls_on_good_dirnames(f, f->good_dirnames);
	}
	else
		do_ls_stuff(f, ".");
	free_ls(f);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpierce <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/19 11:44:59 by bpierce           #+#    #+#             */
/*   Updated: 2018/01/06 23:31:21 by bpierce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

/*
** Various printing of agruments fo sh
** Used to use ft_memcpy(&temp[0][8], &temp[0][16], 4), but decided to hard code
*/

void	set_strings_before_pretty_ls_print(t_ls_arg *node, char *temp[2])
{
	char	buff[100];
	time_t	t;

	temp[1] = NULL;
	if (S_ISLNK(node->stat->path_stat.st_mode))
	{
		buff[readlink(node->full_path, buff, 100)] = '\0';
		temp[1] = ft_strjoin(node->s, " -> ");
		temp[1] = ft_strfjoin(&temp[1], buff);
	}
	temp[0] = &ctime(&node->stat->path_stat.st_mtimespec.tv_sec)[4];
	temp[0][12] = ' ';
	temp[0][13] = '\0';
	time(&t);
	if (t - node->stat->path_stat.st_mtimespec.tv_sec < 15724800)
		return ;
	temp[0][7] = ' ';
	temp[0][8] = temp[0][16];
	temp[0][9] = temp[0][17];
	temp[0][10] = temp[0][18];
	temp[0][11] = temp[0][19];
}

void	pretty_ls_print(t_ls_arg *node, t_ls_maxlen *l)
{
	char	*temp[2];

	set_strings_before_pretty_ls_print(node, temp);
	write(1, node->stat->perms, 12);
	ft_pflite("%^%u %s%^  %s%^  ",
			l->nlink_len - ft_unsignedintlen(node->stat->path_stat.st_nlink),
			node->stat->path_stat.st_nlink,
			node->stat->pwd.pw_name,
			l->pwname_len - ft_strlen(node->stat->pwd.pw_name),
			node->stat->grp.gr_name,
			l->grname_len - ft_strlen(node->stat->grp.gr_name));
	if (node->stat->ma_len || node->stat->mi_len)
		ft_pflite("%^%u, %^%u ",
				l->major_len - node->stat->ma_len,
				node->stat->major,
				l->minor_len - node->stat->mi_len,
				node->stat->minor);
	else
		ft_pflite("%^%u ",
				l->size_len - ft_unsignedintlen(node->stat->path_stat.st_size),
				node->stat->path_stat.st_size);
	write(1, temp[0], 13);
	ft_putendl(temp[1] ? temp[1] : node->s);
	free(temp[1]);
}

void	print_args_left(t_ls_flags *f, t_ls_arg *node, t_ls_maxlen *l)
{
	struct stat		temp;

	if (!node)
		return ;
	if (node->left)
		print_args_left(f, node->left, l);
	if (node->stat)
	{
		if (f->l)
			pretty_ls_print(node, l);
		else
			ft_putendl(node->s);
	}
	else
	{
		stat(node->full_path, &temp);
		ft_pflite("ft_ls: %s: %s\n", node->s, strerror(errno));
	}
	if (node->right)
		print_args_left(f, node->right, l);
}

void	print_args_right(t_ls_flags *f, t_ls_arg *node, t_ls_maxlen *l)
{
	struct stat		temp;

	if (!node)
		return ;
	if (node->right)
		print_args_right(f, node->right, l);
	if (node->stat)
	{
		if (f->l)
			pretty_ls_print(node, l);
		else
			ft_putendl(node->s);
	}
	else
	{
		stat(node->full_path, &temp);
		ft_pflite("ft_ls: %s: %s\n", node->s, strerror(errno));
	}
	if (node->left)
		print_args_right(f, node->left, l);
}

/*
** Special cases: Printing bad directory names at the beginning,
** and printing of just filenames given as arguments.
** -----------------------------------------------------------------------------
*/

void	print_bad_dirnames(t_ls_arg *node)
{
	if (!node)
		return ;
	if (node->left)
		print_bad_dirnames(node->left);
	ft_pflite("ft_ls: %s: No such file or directory\n", node->s);
	if (node->right)
		print_bad_dirnames(node->right);
}

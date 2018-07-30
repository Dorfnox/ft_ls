/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binary_tree_storage.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpierce <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/19 11:44:59 by bpierce           #+#    #+#             */
/*   Updated: 2018/01/06 23:18:39 by bpierce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

/*
** Binary Trees for storing directory information (arguments)
** -----------------------------------------------------------------------------
** new->stat->path_stat.st_mtimespec.tv_sec is used for getting time_t
*/

void			set_permissions_string(t_ls_stat *new, char *full_path)
{
	new->perms[0] = S_ISDIR(new->path_stat.st_mode) ? 'd' : '-';
	new->perms[0] = S_ISLNK(new->path_stat.st_mode) ? 'l' : new->perms[0];
	new->perms[0] = S_ISFIFO(new->path_stat.st_mode) ? 'p' : new->perms[0];
	new->perms[0] = S_ISBLK(new->path_stat.st_mode) ? 'b' : new->perms[0];
	new->perms[0] = S_ISCHR(new->path_stat.st_mode) ? 'c' : new->perms[0];
	new->perms[0] = S_ISSOCK(new->path_stat.st_mode) ? 's' : new->perms[0];
	new->perms[1] = new->path_stat.st_mode & S_IRUSR ? 'r' : '-';
	new->perms[2] = new->path_stat.st_mode & S_IWUSR ? 'w' : '-';
	new->perms[3] = new->path_stat.st_mode & S_IXUSR ? 'x' : '-';
	new->perms[4] = new->path_stat.st_mode & S_IRGRP ? 'r' : '-';
	new->perms[5] = new->path_stat.st_mode & S_IWGRP ? 'w' : '-';
	new->perms[6] = new->path_stat.st_mode & S_IXGRP ? 'x' : '-';
	new->perms[7] = new->path_stat.st_mode & S_IROTH ? 'r' : '-';
	new->perms[8] = new->path_stat.st_mode & S_IWOTH ? 'w' : '-';
	new->perms[9] = new->path_stat.st_mode & S_IXOTH ? 'x' : '-';
	new->perms[9] = new->path_stat.st_mode & S_ISVTX ? 't' : new->perms[9];
	new->perms[10] = listxattr(full_path, 0, 0, 0) > 0 ? '@' : ' ';
	new->perms[11] = ' ';
	new->perms[12] = '\0';
	new->major = 0;
	new->minor = 0;
	new->ma_len = 0;
	new->mi_len = 0;
}

t_ls_stat		*new_stat(char *full_path)
{
	t_ls_stat	*new_stat;

	if (!full_path)
		return (NULL);
	if (!(new_stat = (t_ls_stat *)malloc(sizeof(t_ls_stat))))
		return (ft_putnull("Failed to malloc a t_ls_stat *\n"));
	if (lstat(full_path, &new_stat->path_stat) == -1)
	{
		free(new_stat);
		return (NULL);
	}
	set_permissions_string(new_stat, full_path);
	if (new_stat->perms[0] == 'b' || new_stat->perms[0] == 'c')
	{
		new_stat->major = major(new_stat->path_stat.st_rdev);
		new_stat->minor = minor(new_stat->path_stat.st_rdev);
		new_stat->ma_len = ft_unsignedintlen(new_stat->major);
		new_stat->mi_len = ft_unsignedintlen(new_stat->minor);
	}
	return (new_stat);
}

t_ls_arg		*new_arg(char *cur_dir, char *name)
{
	t_ls_arg		*new;

	if (!(new = (t_ls_arg *)ft_memalloc(sizeof(t_ls_arg))))
		return (ft_putnull("Failed to malloc for new t_ls_arg *\n"));
	if (!(new->s = ft_strdup(name)))
	{
		free(new);
		return (ft_putnull("Failed to malloc for new->s\n"));
	}
	new->full_path = cur_dir ? ft_strjoin(cur_dir, "/") : NULL;
	new->full_path = cur_dir ?
		ft_strfjoin(&new->full_path, name) : ft_strdup(name);
	if ((new->stat = new_stat(new->full_path)))
	{
		new->stat->pwd = *getpwuid(new->stat->path_stat.st_uid);
		new->stat->pwd.pw_name = ft_strdup(new->stat->pwd.pw_name);
		new->stat->grp = *getgrgid(new->stat->path_stat.st_gid);
		new->stat->grp.gr_name = ft_strdup(new->stat->grp.gr_name);
	}
	return (new);
}

t_ls_arg		*insert_arg(t_ls_flags *f, t_ls_arg *node, t_ls_arg *new)
{
	if (!(node))
		return (new);
	if (f->ss && new->stat)
		if (new->stat->path_stat.st_size > node->stat->path_stat.st_size)
			if ((node->left = insert_arg(f, node->left, new)))
				return (node);
	if (f->ss && new->stat)
		if (new->stat->path_stat.st_size < node->stat->path_stat.st_size)
			if ((node->right = insert_arg(f, node->right, new)))
				return (node);
	if (f->t && new->stat)
		if (LS_SEC_SORT_GREATER)
			if ((node->left = insert_arg(f, node->left, new)))
				return (node);
	if (f->t && new->stat)
		if (LS_SEC_SORT_LESS)
			if ((node->right = insert_arg(f, node->right, new)))
				return (node);
	if (ft_strcmp(new->s, node->s) < 1)
		node->left = insert_arg(f, node->left, new);
	else
		node->right = insert_arg(f, node->right, new);
	return (node);
}

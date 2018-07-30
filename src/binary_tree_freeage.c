/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binary_tree_freeage.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpierce <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/19 11:44:59 by bpierce           #+#    #+#             */
/*   Updated: 2018/01/04 13:28:03 by bpierce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

/*
** Managing the Freeing of binary trees
** -----------------------------------------------------------------------------
** free_binary_tree_node_ls will destroy a single node on the tree.
** free_binary_tree_ls will destroy an entire tree.
*/

void	free_binary_tree_node_ls(t_ls_arg *node)
{
	if (node->s)
		free(node->s);
	if (node->full_path)
		free(node->full_path);
	if (node->stat)
	{
		free(node->stat->pwd.pw_name);
		free(node->stat->grp.gr_name);
		free(node->stat);
	}
	node->left = NULL;
	node->right = NULL;
	free(node);
}

void	free_binary_tree_ls(t_ls_arg *root)
{
	if (!root)
		return ;
	if (root->left)
		free_binary_tree_ls(root->left);
	if (root->right)
		free_binary_tree_ls(root->right);
	free_binary_tree_node_ls(root);
}

int		free_ls(t_ls_flags *f)
{
	if (!f)
		return (0);
	if (f->good_dirnames)
	{
		free_binary_tree_ls(f->good_dirnames);
		f->good_dirnames = NULL;
	}
	if (f->bad_dirnames)
	{
		free_binary_tree_ls(f->bad_dirnames);
		f->bad_dirnames = NULL;
	}
	return (1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flags.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpierce <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/19 11:44:59 by bpierce           #+#    #+#             */
/*   Updated: 2018/01/06 18:38:37 by bpierce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

/*
** Checking flags and any given directory names
** -----------------------------------------------------------------------------
** dir_flag is a flag that is turned on when given directory names are done
*/

void		set_ls_flag(t_ls_flags *f, char flag)
{
	if (flag == 'l')
		f->l = 1;
	else if (flag == 'r')
		f->r = 1;
	else if (flag == 'R')
		f->rr = 1;
	else if (flag == 't')
		f->t = 1;
	else if (flag == 'a')
		f->a = 1;
	else if (flag == 'S')
		f->ss = 1;
	f->t = f->ss ? 0 : f->t;
}

int			add_ls_flags(t_ls_flags *f, char *s)
{
	int		i;
	int		j;
	int		str_len;
	int		legal_len;
	char	*legal_ls_flags;

	i = 0;
	str_len = ft_strlen(s);
	legal_ls_flags = "lrRtaS";
	legal_len = ft_strlen(legal_ls_flags);
	while (++i < str_len && (j = -1))
	{
		while (++j < legal_len)
		{
			if (s[i] == legal_ls_flags[j])
			{
				set_ls_flag(f, s[i]);
				break ;
			}
		}
		if (j == legal_len)
			return ((int)s[i]);
	}
	return (0);
}

t_ls_flags	*set_ls_flags(char **v, int c, t_ls_flags *f)
{
	int			i;
	t_ls_arg	*new;

	i = 0;
	new = NULL;
	while (++i < c && !f->bad_flag)
	{
		if (f->dir_flag || ft_strnequ("-", v[i], 1) == 0 ||
				ft_strlen(v[i]) == 1)
		{
			f->dir_flag = 1;
			new = new_arg(NULL, v[i]);
			if (!(new->stat))
				f->bad_dirnames = insert_arg(f, f->bad_dirnames, new);
			else
				f->good_dirnames = insert_arg(f, f->good_dirnames, new);
		}
		else if (!f->dir_flag)
			if ((f->bad_flag = add_ls_flags(f, v[i])) != 0)
				ft_pflite("ls: illegal option -- %s\n", &f->bad_flag);
	}
	return (!f->bad_flag ? f : NULL);
}

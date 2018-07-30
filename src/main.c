/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpierce <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/19 11:44:59 by bpierce           #+#    #+#             */
/*   Updated: 2018/01/04 19:34:00 by bpierce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

int		main(int argc, char **argv)
{
	t_ls_flags	f;

	ft_bzero(&f, sizeof(t_ls_flags));
	if (!(set_ls_flags(argv, argc, &f)))
	{
		write(2, LS_USAGE, ft_strlen(LS_USAGE));
		return (free_ls(&f));
	}
	ft_ls(&f);
	return (1);
}

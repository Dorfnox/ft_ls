/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpierce <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/16 17:12:25 by bpierce           #+#    #+#             */
/*   Updated: 2018/01/06 23:18:48 by bpierce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
# define FT_LS_H

# include "libft.h"
# include <dirent.h>
# include <pwd.h>
# include <grp.h>
# include <sys/xattr.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <time.h>
# include <errno.h>

# define _BSD_SOURCE
# define NSFOD(x) ft_printf("ls: %s: No such file or director", (x));

/*
** DB(x) ft_putstr_fd((x), 2); ft_putstr_fd("^[[33m <--\n^[[0m", 2);
** DBI(x) ft_putstr_fd(ft_itoa((x)), 2); ft_putstr_fd("^[[32m <--\n^[[0m", 2);
*/

# define LS_USAGE "usage: ls [-alRrtS] [file ...]\n"

# define IF_P_ONE1 (s[i + j + 1] != '^')
# define IF_P_ONE2 (s[i + j + 1] != 'u')
# define IF_P_ONE3 (s[i + j + 1] != 's')
# define IF_P_ONE4 (IF_P_ONE1 && IF_P_ONE2 && IF_P_ONE3)
# define IF_INVALID_PERCENT_ADD_ONE ((IF_P_ONE4) ? 1 : 0)

# define LS_NEW_SEC (new->stat->path_stat.st_mtimespec.tv_sec)
# define LS_NODE_SEC (node->stat->path_stat.st_mtimespec.tv_sec)
# define LS_NEW_NSEC (new->stat->path_stat.st_mtimespec.tv_nsec)
# define LS_NODE_NSEC (node->stat->path_stat.st_mtimespec.tv_nsec)

# define LS_SEC_GR (LS_NEW_SEC > LS_NODE_SEC)
# define LS_SEC_LE (LS_NEW_SEC < LS_NODE_SEC)
# define LS_NSEC_GR (LS_NEW_NSEC > LS_NODE_NSEC)
# define LS_NSEC_LE (LS_NEW_NSEC < LS_NODE_NSEC)
# define LS_SEC_EQ (LS_NEW_SEC == LS_NODE_SEC)

# define LS_SEC_SORT_GREATER ((LS_SEC_GR) || (LS_SEC_EQ && LS_NSEC_GR))
# define LS_SEC_SORT_LESS ((LS_SEC_LE) || (LS_SEC_EQ && LS_NSEC_LE))

/*
** Keeps track of longest arguments for printf purposes
*/

typedef struct			s_ls_maxlen
{
	unsigned int		total_blocks;
	unsigned int		nlink_len;
	unsigned int		pwname_len;
	unsigned int		grname_len;
	unsigned int		size_len;
	unsigned int		major_len;
	unsigned int		minor_len;
}						t_ls_maxlen;

/*
** Contains struct stat information
*/

typedef struct			s_ls_stat
{
	char				perms[13];
	struct stat			path_stat;
	struct passwd		pwd;
	struct group		grp;
	unsigned int		major;
	unsigned int		minor;
	unsigned int		ma_len;
	unsigned int		mi_len;
}						t_ls_stat;

/*
** t_ls_arg keeps track of directory name, stat, and holds tree paths
*/

typedef struct			s_ls_arg
{
	char				*s;
	char				*full_path;
	t_ls_stat			*stat;
	struct s_ls_arg		*left;
	struct s_ls_arg		*right;
}						t_ls_arg;

/*
** l - add 'Total:' and list in long format
** R - recursively list subdirectories encountered
** a - include directories whose name begins with '.'
** r - reverse sort (z first, oldest first)
** t - Sort by recently modified first before sorting in lexicograph-ical order
*/

typedef struct			s_ls_flags
{
	unsigned int		l:1;
	unsigned int		rr:1;
	unsigned int		a:1;
	unsigned int		r:1;
	unsigned int		t:1;
	unsigned int		ss:1;
	unsigned int		dir_flag:1;
	unsigned int		print_dir:1;
	unsigned int		print_newline:1;
	unsigned int		dir_exists:1;
	unsigned int		file_exists:1;
	unsigned int		num_of_directories;
	int					bad_flag;
	t_ls_maxlen			len;
	t_ls_arg			*good_dirnames;
	t_ls_arg			*bad_dirnames;
}						t_ls_flags;

int						test(void);
void					display_path_stats(struct dirent *f);

void					set_new_maxlens(t_ls_maxlen *l, t_ls_arg *node);
void					adjust_new_maxlens(t_ls_maxlen *l);

/*
** binary_tree_storage.c
*/
void					set_permissions_string(t_ls_stat *new, char *full_path);
t_ls_stat				*new_stat(char *name);
t_ls_arg				*new_arg(char *cur_dir, char *name);
t_ls_arg				*insert_arg(t_ls_flags *f, t_ls_arg *node,
						t_ls_arg *new);

/*
** binary_tree_freeage.c
*/
void					free_binary_tree_node_ls(t_ls_arg *node);
void					free_binary_tree_ls(t_ls_arg *root);
int						free_ls(t_ls_flags *f);

/*
** printing.c
*/
void					set_strings_before_pretty_ls_print(t_ls_arg *node,
						char *temp[2]);
void					pretty_ls_print(t_ls_arg *node, t_ls_maxlen *l);
void					print_args_left(t_ls_flags *f,
						t_ls_arg *node, t_ls_maxlen *l);
void					print_args_right(t_ls_flags *f,
						t_ls_arg *node, t_ls_maxlen *l);
void					print_bad_dirnames(t_ls_arg *node);

/*
** flags.c
*/
void					set_ls_flag(t_ls_flags *f, char flag);
int						add_ls_flags(t_ls_flags *f, char *s);
t_ls_flags				*set_ls_flags(char **v, int c, t_ls_flags *f);
void					ft_pflite_print_chars(va_list *ap, char *temp);
void					ft_pflite(char *s, ...);

/*
** ft_ls.c
*/
void					do_recursive_ls_stuff(t_ls_flags *f,
						t_ls_arg *root, char *cur_dir);
void					still_do_ls_stuff(t_ls_flags *f, t_ls_arg *r,
						char *cdn, t_ls_maxlen *l);
void					do_ls_stuff(t_ls_flags *f, char *current_direc_name);
void					ft_ls(t_ls_flags *f);

/*
** ft_ls_helper.c
*/
void					do_ls_on_good_files(t_ls_flags *f, t_ls_arg *r,
						t_ls_maxlen *l, int i);
void					do_ls_on_good_dirnames(t_ls_flags *f, t_ls_arg *root);

#endif

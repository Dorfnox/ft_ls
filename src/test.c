/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpierce <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/16 18:36:53 by bpierce           #+#    #+#             */
/*   Updated: 2018/01/02 12:48:10 by bpierce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

int		test(void)
{
	DIR				*test;
	struct dirent	*f;

	ft_putstr("[95m----------------------------------------------------------------------\n[0m");
	test = opendir(".");
	while ((f = readdir(test)))
	{
		display_path_stats(f);
		ft_putstr("[95m----------------------------------------------------------------------\n[0m");
	}
	closedir(test);
	return (1);
}

void	display_path_stats(struct dirent *f)
{
	// testing the return of readdir on test
	ft_printf("[31mfilename: '%s', %lld\n[0m- - - - - - -\n",
			f->d_name,
			f->d_namlen);

	// testing opening file and reading contents
	int		i;
	char	buff[100];

	i = open(f->d_name, O_RDONLY);
	ft_bzero(buff, 100);
	read(i, buff, 100);
	ft_printf("[93;4mfile contents:[0m[93m\n%s\n[0m- - - - - - -\n");
	close(i);

	// testing getting file stats
	struct stat		buf;

	i = stat(f->d_name, &buf);
	ft_printf("[92mreturn value i: %lld,\n"
			"device inode resides on: %lld,\n"
			"inode's number: %lld,\n"
			"inode protection mode: %b,\n"
			"number of hard links to the file: %lld,\n"
			"user-id of owner: %lld,\n"
			"group-id of owner: %lld,\n"
			"device type, for special file inode: %lld,\n"
			"time of last access: %lld,\n"
			"time of last data modification: %lld,\n"
			"time of last file status change: %lld,\n"
			"file size, in bytes: %lld,\n"
			"blocks allocated for file: %lld,\n"
			"optimal file sys I/O ops blocksize: %lld,\n"
			"user defined flags for file: %lld,\n"
			"file generation number: %lld,\n[0m"
			"- - - - - - -\n",
			i,
			buf.st_dev,
			buf.st_ino,
			buf.st_mode,
			buf.st_nlink,
			buf.st_uid,
			buf.st_gid,
			buf.st_rdev,
			buf.st_atimespec,
			buf.st_mtimespec,
			buf.st_ctimespec,
			buf.st_size,
			buf.st_blocks,
			buf.st_blksize,
			buf.st_flags,
			buf.st_gen);

	// testing passwd function getpwuid
	struct passwd	*pwd;

	pwd = getpwuid(buf.st_uid); // use the uid returned in the 'stat' section?
	ft_printf("[96musername: %s,\n"
			"encrpyted password: %s,\n"
			"user uid: %lld,\n"
			"user gid: %lld,\n"
			"password change time: %lld,\n"
			"user access class: %s,\n"
			"Honeywell login info: %s,\n"
			"home directory: %s,\n"
			"default shell: %s,\n"
			"account expiration: %lld,\n[0m"
			"- - - - - - -\n",
			pwd->pw_name,
			pwd->pw_passwd,
			pwd->pw_uid,
			pwd->pw_gid,
			pwd->pw_change,
			pwd->pw_class,
			pwd->pw_gecos,
			pwd->pw_dir,
			pwd->pw_shell,
			pwd->pw_expire);

	// testing group function getgrgid
	struct group	*group;
	
	group = getgrgid(buf.st_gid); // use the gid returned in the 'stat' section?
	ft_printf("[95mgr_name: %s,\n"
			"gr_passwd: %s,\n"
			"gr_gid: %lld,\n"
			"gr_mem[0]: %s\n[0m"
			"- - - - - - -\n",
			group->gr_name,
			group->gr_passwd,
			group->gr_gid,
			group->gr_mem[0]);

	// testing listxattr and getxattr
	ssize_t			ret;
	char			list[1000];

	ft_bzero(list, 1000);
	ret = listxattr(f->d_name, list, 1000, 0);
	ft_printf("[94mlistxattr return value: %lld,\n"
			"List of Attributes: %s\n",
			ret,
			list);
	ret = getxattr(f->d_name, list, list, 1000, 0, 0); // Should NOT be 'list, list'
	ft_printf("[94mgetxattr return value: %lld,\n"
			"List of Something: %s\n[0m"
			"- - - - - - -\n",
			ret,
			list);

	// testing time and ctime
	time_t			t;
	time_t			*t2;
	char			*timey;

	t2 = NULL;
	t = time(t2);
	timey = ctime(&t);
	ft_printf("[97mtime: %lld,\n"
			"time2: %lld,\n"
			"ctime: %s\n"
			"- - - - - - -\n",
			t,
			t2,
			timey);
}

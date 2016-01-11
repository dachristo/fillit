/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_gettab.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldubos <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/14 18:36:14 by ldubos            #+#    #+#             */
/*   Updated: 2016/01/11 18:50:14 by dchristo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fillit.h"

static	int					ft_test(t_tetrimino tetrimino, int i)
{
	int						x_d;
	int						y_d;

	while (i < 3)
	{
		x_d = tetrimino.c_pos[i].x - tetrimino.c_pos[i + 1].x;
		y_d = tetrimino.c_pos[i].y - tetrimino.c_pos[i + 1].y;
		if ((x_d >= -1 && x_d <= 1 && y_d == 0) ||
			(y_d >= -1 && y_d <= 1 && x_d == 0))
			++i;
		else if (x_d >= -1 && x_d <= 1 && y_d >= -1 && y_d <= 1)
		{
			x_d = tetrimino.c_pos[i].x - tetrimino.c_pos[i + 2].x;
			y_d = tetrimino.c_pos[i].y - tetrimino.c_pos[i + 2].y;
			if ((x_d >= -1 && x_d <= 1 && y_d == 0) ||
				(y_d >= -1 && y_d <= 1 && x_d == 0) ||
				ft_test_dlm(tetrimino, i))
				++i;
			else
				return (0);
		}
		else if (ft_test_dlm(tetrimino, i))
			++i;
		else
			return (0);
	}
	return (1);
}

static	void				ft_init(int *x, int *y, int *i)
{
	*x = 0;
	*y = 0;
	*i = 0;
}

static	int					ft_statement(char *str, t_tetrimino *tetrimino, int b)
{
	int						x;
	int						y;
	int						i;

	ft_init(&x, &y, &i);
	while (*++str && i < 5 && (*str == '#' || *str == '.' || *str == '\n'))
	{
		if (*str == '\n')
		{
			if (!ft_is_back_line(&y, &x))
				return (0);
		}
		else if (*str == '#' && *str != '.')
		{
			tetrimino->c_pos[i].x = x;
			tetrimino->c_pos[i++].y = y;
		}
		(*str == '.' || *str == '#') ? x++ : x;
	}
	if (i > 4 || ((b == 21 && *--str != '\n') || (b == 20 && *str != '\0')))
		return (0);
	return (1);
}

static int					ft_read(int fd, int i, t_tetrimino **ret, int b)
{
	t_tetrimino				*t;
	char					*buf;
	int						b_save;

	buf = ft_strnew(BUF_S);
	t = *ret;
	while ((b = read(fd, buf, BUF_S)) != 0)
	{
		if (i != 0)
		{
			if (!(t->next = (t_tetrimino *)malloc(sizeof(t_tetrimino))))
				return (0);
			t = t->next;
		}
		t->w_pos.x = 0;
		t->w_pos.y = 0;
		if (!(ft_statement(--buf, t, b)))
			return (0);
		t->c = 'A' + i++;
		if (!(ft_test(*t, 0)))
			return (0);
		ft_bzero(buf, BUF_S + 1);
		b_save = b;
	}
	if (b_save != 20)
		return (0);
	t->next = NULL;
	return (1);
}

int							ft_gettab(const char *path, t_tetrimino **t)
{
	int						fd;
	
	if ((fd = open(path, O_RDONLY)) == -1)
		return (0);
	return (ft_read(fd, 0, t, 0));
}

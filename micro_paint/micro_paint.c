/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   micro_paint.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldubar <aldubar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 12:51:17 by aldubar           #+#    #+#             */
/*   Updated: 2021/09/15 01:41:19 by aldubar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

enum	e_error
{
	ARGUMENT,
	FILE_CORRUPTED,
	ALLOC
};

typedef struct	s_data
{
	int		width;
	int		height;
	char	bg;
}				t_data;

typedef struct	s_rect
{
	char	type;
	float	x;
	float	y;
	float	width;
	float	height;
	char	c;
}				t_rect;

int		error(enum e_error err, FILE *file, char *zone)
{
	int			i;
	const char	*s;

	if (err == ARGUMENT)
		s = "Error: argument\n";
	else if (err == ALLOC)
		s = "Error: malloc failed\n";
	else
		s = "Error: Operation file corrupted\n";
	i = 0;
	while (s[i])
		i++;
	write(STDOUT_FILENO, s, i);
	if (file)
		fclose(file);
	if (zone)
		free(zone);
	return (1);
}

int		parse(FILE *file, t_data *data)
{
	int		ret;

	if ((ret = fscanf(file, "%d %d %c\n", &data->width, &data->height, &data->bg)) != 3)
		return (0);
	if (data->width <= 0 || data->width > 300 || data->height <= 0 || data->height > 300)
		return (0);
	if (ret == -1)
		return (0);
	return (1);
}

char	*fill_bg(t_data *data)
{
	char	*zone;

	zone = (char *)malloc(sizeof(char) * data->width * data->height);
	if (!zone)
		return (NULL);
	memset(zone, data->bg, data->width * data->height);
	return (zone);
}

int		in_rect(t_rect *rect, int x, int y)
{
	return (rect->x <= x && rect->x + rect->width >= x
		&& rect->y <= y && rect->y + rect->height >= y);
}

int		in_border(t_rect *rect, int x, int y)
{
	return (x < rect->x + 1 || x > rect->x + rect->width - 1
		|| y < rect->y + 1 || y > rect->y + rect->height - 1);
}

void	draw_rect(char *zone, t_data *data, t_rect *rect)
{
	int		x;
	int		y;

	y = 0;
	while (y < data->height)
	{
		x = 0;
		while (x < data->width)
		{
			if ((rect->type == 'R' && in_rect(rect, x, y)) || (rect->type == 'r' && in_rect(rect, x, y) && in_border(rect, x, y)))
				zone[y * data->width + x] = rect->c;
			x++;
		}
		y++;
	}
}

int		draw_rectangles(FILE *file, t_data *data, char *zone)
{
	t_rect	rect;
	int		ret;

	while ((ret = fscanf(file, "%c %f %f %f %f %c\n", &rect.type, &rect.x, &rect.y, &rect.width, &rect.height, &rect.c)) == 6)
	{
		if (rect.width <= 0 || rect.height <= 0 || (rect.type != 'r' && rect.type != 'R'))
			return (0);
		draw_rect(zone, data, &rect);
	}
	if (ret != -1)
		return (0);
	return (1);
}

void	execute(t_data *data, char *zone)
{
	int		i;
	
	i = 0;
	while (i < data->height)
	{
		write(STDOUT_FILENO, zone + (i * data->width), data->width);
		write(STDOUT_FILENO, "\n", 1);
		i++;
	}
}

int		main(int ac, char **av)
{
	t_data	data;
	char	*zone;
	FILE	*file;

	data.width = 0;
	data.height = 0;
	data.bg = 0;
	zone = NULL;
	if (ac != 2)
		return (error(ARGUMENT, NULL, NULL));
	if (!(file = fopen(av[1], "r")))
		return (error(FILE_CORRUPTED, NULL, NULL));
	if (!parse(file, &data))
		return (error(FILE_CORRUPTED, file, NULL));
	zone = fill_bg(&data);
	if (!zone)
		return (error(ALLOC, file, NULL));
	if (!draw_rectangles(file, &data, zone))
		return (error(FILE_CORRUPTED, file, zone));
	fclose(file);
	execute(&data, zone);
	free(zone);
	return (0);
}

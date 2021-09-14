/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_paint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldubar <aldubar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/10 09:31:08 by aldubar           #+#    #+#             */
/*   Updated: 2021/09/15 01:41:10 by aldubar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

typedef struct	s_data
{
	int		w;
	int		h;
	char	bg;
}				t_data;

typedef struct	s_circle
{
	char	type;
	float	radius;
	float	x;
	float	y;
	char	c;
}				t_circle;

enum	e_error
{
	ARGUMENT,
	FILE_CORRUPTED,
	ALLOC
};

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

int		parse(t_data *data, FILE *file)
{
	int		ret;

	if ((ret = fscanf(file, "%d %d %c\n", &data->w, &data->h, &data->bg)) != 3)
		return (0);
	if (data->w <= 0 || data->h <= 0 || data->w > 300 || data->h > 300)
		return (0);
	if (ret == -1)
		return (0);
	return (1);
}

char	*fill_bg(t_data *data)
{
	char	*zone;

	zone = (char *)malloc(sizeof(char) * data->w * data->h);
	if (!zone)
		return (NULL);
	memset(zone, data->bg, data->w * data->h);
	return (zone);
}

int		in_circle(t_circle *circ, float x, float y)
{
	return (sqrtf(powf(x - circ->x, 2) + powf(circ->y - y, 2)) <= circ->radius);
}

int		in_border(t_circle *circ, float x, float y)
{
	return (sqrtf(powf(x - circ->x, 2) + powf(circ->y - y, 2)) > circ->radius - 1);
}

void	draw_circle(t_data *data, char *zone, t_circle *circ)
{
	int		x;
	int		y;

	y = 0;
	while (y < data->h)
	{
		x = 0;
		while (x < data->w)
		{
			if ((circ->type == 'C' && in_circle(circ, x, y)) || (circ->type == 'c' && in_circle(circ, x, y) && in_border(circ, x, y)))
				zone[y * data->w + x] = circ->c;
			x++;
		}
		y++;
	}
}

int		draw_circles(t_data *data, char *zone, FILE *file)
{
	int			ret;
	t_circle	circ;

	while ((ret = fscanf(file, "%c %f %f %f %c\n", &circ.type, &circ.x, &circ.y, &circ.radius, &circ.c)) == 5)
	{
		if (circ.radius <= 0 || (circ.type != 'c' && circ.type != 'C'))
			return (0);
		draw_circle(data, zone, &circ);
	}
	if (ret != -1)
		return (0);
	return (1);
}

void	execute(t_data *data, char *zone)
{
	int		i;

	i = 0;
	while (i < data->h)
	{
		write(STDOUT_FILENO, zone + (data->w * i), data->w);
		write(STDOUT_FILENO, "\n", 1);
		i++;
	}
}

int		main(int ac, char **av)
{
	t_data	data;
	char	*zone;
	FILE	*file;

	data.w = 0;
	data.h = 0;
	data.bg = 0;
	zone = NULL;
	if (ac != 2)
		return (error(ARGUMENT, NULL, NULL));
	if (!(file = fopen(av[1], "r")))
		return (error(FILE_CORRUPTED, NULL, NULL));
	if (!parse(&data, file))
		return (error(FILE_CORRUPTED, file, NULL));
	zone = fill_bg(&data);
	if (!zone)
		return (error(ALLOC, file, NULL));
	if (!draw_circles(&data, zone, file))
		return (error(FILE_CORRUPTED, file, zone));
	execute(&data, zone);
	fclose(file);
	free(zone);
	return (0);
}

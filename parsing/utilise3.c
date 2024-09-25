	/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilise3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-bouk <hel-bouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 11:29:06 by hel-bouk          #+#    #+#             */
/*   Updated: 2024/09/19 09:46:19 by hel-bouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

bool	check_map_err(t_map *map)
{
	int		i;
	bool	check;
	t_map	*ptr;

	i = 0;
	check = true;
	if (!map)
		return (false);
	ptr = get_position(map, 7);
	while (ptr)
	{
		if (!ptr->row[0])
			check = false;
		else if (ptr->row && !check)
			return (ft_putendl_fd(MAP_V, STDERR_FILENO), false);
		ptr = ptr->next;
	}
	return (true);
}

void back_to_default(char **map, char c)
{
	int	i;
	int	j;

	i = 0;
	while (map[i])
	{
		j = 0;
		while (map[i][j])
		{
			if (map[i][j] == 'F' || map[i][j] == 'X' || map[i][j] == c)
				map[i][j] = '0';
			j++;
		}
		i++;
	}
}

int	count_charcter(char **str, char c)
{
	int	i;
	int	j;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		j = 0;
		while (str[i][j])
		{
			if (str[i][j] == c)
				count++;
			j++;
		}
		i++;
	}
	return (count);
}

t_point	*get_position_door(char **map)
{
	int		i;
	int		j;
	int		k;
	t_point	*point;

	k = 0;
	i = count_charcter(map, 'D');
	point = (t_point *)malloc(sizeof(t_point) * (i + 1));
	if (!point)
		return (NULL);
	i = -1;
	while (map[++i])
	{
		j = -1;
		while (map[i][++j])
		{
			if (map[i][j] == 'D')
			{
				point[k].x = j;
				point[k++].y = i;
			}
		}
	}
	point[k].x = -1;
	point[k].y = -1;
	return (point);
}

t_int	load_colors(t_player *player, mlx_texture_t *texture, t_fpoint x)
{
	t_int x_texture;
	t_int y_texture;
	double	tmp;
	t_int	pos;
	uint8_t	*pixel_data;
	t_int	color;
	int texture_height;
	int texture_width;

	color = 0;
	// return(0);

	texture_height = texture->height;
	texture_width = texture->width;
	//calcule x_texture position
	if (!player->is_vertical)
		x_texture = ((x.x / PEX) - floor(x.x / PEX)) * texture->width;
	else
		x_texture = ((x.y / PEX) - floor(x.y / PEX)) * texture->width;
	//calcule y_texture position
	tmp = (int)(player->pos_y - player->wall_t) * texture->height;
	y_texture = tmp / player->wall_height + player->texture_offset;
	pos = ((int)y_texture * texture->width * texture->bytes_per_pixel) + (int)x_texture * texture->bytes_per_pixel;
	if (pos < 0 || y_texture < 0 || y_texture >= texture->height
		|| x_texture < 0 || x_texture >= texture->width || pos >= texture->width * texture->height * texture->bytes_per_pixel)
        return 0;
	pixel_data = &(texture->pixels[pos]);
	if (!pixel_data)
		return (0);
	color = ((pixel_data[0] << 24) | (pixel_data[1] << 16) | (pixel_data[2] << 8) | (pixel_data[3]));
	return (color);
}

bool	valid_door(char	**map, t_point *point)
{
	int	i;

	i = 0;
	while (point[i].x != -1 && point[i].y != -1)
	{
		if ((map[point[i].y][point[i].x + 1] != '1' || map[point[i].y][point[i].x - 1] != '1')
			&& (map[point[i].y + 1][point[i].x] != '1' || map[point[i].y - 1][point[i].x] != '1'))
			return (ft_putendl_fd(POS_DOOR, STDERR_FILENO), false);
		i++;
	}
	return (true);
}


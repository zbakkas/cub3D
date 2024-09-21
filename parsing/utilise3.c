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

void back_to_default(char **map)
{
	int	i;
	int	j;

	i = 0;
	while (map[i])
	{
		j = 0;
		while (map[i][j])
		{
			if (map[i][j] == 'F' || map[i][j] == 'X')
				map[i][j] = '0';
			j++;
		}
		i++;
	}
}

t_int	load_colors(t_player *player, mlx_texture_t *texture, t_fpoint x)
{
	t_int x_texture;
	t_int y_texture;
	double	tmp;
	t_int	pos;
	uint8_t	*pixel_data;
	t_int	color;

	color = 0;
	int texture_height;
	int texture_width;

	texture_height = texture->height;
	texture_width = texture->width;
	//calcule x_texture position
	if (!player->is_vertical)
		x_texture = ((x.x / PEX) - floor(x.x / PEX)) * texture->width;
	else
		x_texture = ((x.y / PEX) - floor(x.y / PEX)) * texture->width;
	//calcule y_texture position
	tmp = (int)(player->pos_y - player->wall_t) * texture->height;
	y_texture = tmp / player->wall_height;
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


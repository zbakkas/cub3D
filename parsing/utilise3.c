/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilise3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-bouk <hel-bouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 11:29:06 by hel-bouk          #+#    #+#             */
/*   Updated: 2024/09/17 14:00:40 by hel-bouk         ###   ########.fr       */
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
	double x_texture;
	double y_texture;
	double	size_wall;
	double	tmp;
	t_int	pos;
	uint8_t	*pixel_data;
	t_int	color;

	color = 0;
	size_wall =  player->wall_b - player->wall_t;
	x_texture = ((x.x / PEX) - floor(x.x / PEX)) * texture->width;
	tmp = (x.y - player->wall_t) * texture->height;
	y_texture = tmp / size_wall;
	pos = (y_texture * texture->width * texture->bytes_per_pixel) + x_texture * texture->bytes_per_pixel;
	if (y_texture < 0 || y_texture >= texture->height || x_texture < 0 || x_texture >= texture->width)
        return 0xff00ff40;
	pixel_data = &(texture->pixels[pos]);
	color = ((pixel_data[0] << 24) | (pixel_data[1] << 16) | (pixel_data[2] << 8) | (pixel_data[3]));
	printf("0x%X\n", color);
	return (color);
}

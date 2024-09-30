/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-bouk <hel-bouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 13:23:05 by hel-bouk          #+#    #+#             */
/*   Updated: 2024/09/30 10:39:33 by hel-bouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void    initialize_inf(t_inf *inf)
{
	inf->e_path = NULL;
	inf->w_path = NULL;
	inf->n_path = NULL;
	inf->s_path = NULL;
	inf->inf_map = NULL;
	inf->f_color = NULL;
	inf->c_color = NULL;
	inf->color_c = 0;
	inf->color_f = 0;
	inf->position = '\0';
}

int	load_normal_color(t_player *player, mlx_texture_t *texture, int x, int y)
{
	t_int	pos;
	uint8_t	*pixel_data;
	t_int	color;
	int texture_height;
	int texture_width;

	color = 0;
	texture_height = texture->height;
	texture_width = texture->width;
	pos = (y * texture->width * texture->bytes_per_pixel) + x * texture->bytes_per_pixel;
	if (pos < 0 || y < 0 || y >= texture->height
		|| x < 0 || x >= texture->width || pos >= texture->width * texture->height * texture->bytes_per_pixel)
        return 0;
	pixel_data = &(texture->pixels[pos]);
	if (!pixel_data || !pixel_data[0] || !pixel_data[1] || !pixel_data[2] || !pixel_data[3])
		return (0);
	color = 0xff89ff;
	return (color);
}
void draw_gnu(t_player *player, mlx_texture_t *texture)
{
    int 	x_c;
    int 	y_b;
	int		i;
	int 	j;
	uint32_t color;

	x_c = 0;
	y_b = HEIGHT - texture->height;
	i = 0;
    while (i < texture->height)
	{
		j = 0;
        while (j < texture->width)
		{
            // color = load_colors(player, texture, (t_fpoint){j, i});
			color = load_normal_color(player, texture, j, i);
			if (color != 0)
            	mlx_put_pixel(player->img, x_c + j, y_b + i, color);
			j++;
        }
		i++;
    }
}


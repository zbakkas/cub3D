/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-bouk <hel-bouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 13:23:05 by hel-bouk          #+#    #+#             */
/*   Updated: 2024/09/30 11:12:06 by hel-bouk         ###   ########.fr       */
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
	int	pos;
	uint8_t	*pixel_data;
	t_int	color;
	int texture_height;
	int texture_width;

	(void)player;
	color = 0;
	
	texture_height = texture->height;
	texture_width = texture->width;
	pos = (y * texture_width * texture->bytes_per_pixel) + x * texture->bytes_per_pixel;
	if (pos < 0 || y < 0 || y >= texture_height
		|| x < 0 || x >= texture_width || pos >= texture_width * texture_height * texture->bytes_per_pixel)
        return 0;
	pixel_data = &(texture->pixels[pos]);
	// if (!pixel_data || !pixel_data[0] || !pixel_data[1] || !pixel_data[2] || !pixel_data[3])
	// 	return (0);
	color = ((pixel_data[0] << 24) | (pixel_data[1] << 16) | (pixel_data[2] << 8) | (pixel_data[3])); 
	return (color);
}
void draw_gnu(t_player *player, mlx_texture_t *texture)
{
    int 	x_c;
    int 	y_b;
	int		i;
	int 	j;
	uint32_t color;

	x_c = (WIDTH / 2) - (texture->width);
	y_b = HEIGHT - texture->height * 2;
	i = 0;
    while (i < (int)texture->height * 2)
	{
		j = 0;
        while (j < (int)texture->width * 2)
		{
			color = load_normal_color(player, texture, j / 2, i / 2);
			if (color != 0)
            	mlx_put_pixel(player->img, x_c + j, y_b + i, color);
			j++;
        }
		i++;
    }
}


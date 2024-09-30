/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-bouk <hel-bouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 13:23:05 by hel-bouk          #+#    #+#             */
/*   Updated: 2024/09/30 13:52:14 by hel-bouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	initialize_inf(t_inf *inf)
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

bool	assign_texter(t_player *player, t_inf inf)
{
	player->n_texter = mlx_load_png(inf.n_path);
	player->s_texter = mlx_load_png(inf.s_path);
	player->e_texter = mlx_load_png(inf.e_path);
	player->w_texter = mlx_load_png(inf.w_path);
	player->door_tex = mlx_load_png("./textures/wall/door01.png");
	player->door_open_tex = mlx_load_png("./textures/wall/wall_open02.png");
	player->gun_texture = malloc(sizeof(mlx_texture_t *) * 8);
	if (!player->gun_texture)
		return (false);
	player->gun_texture[0] = mlx_load_png("./textures/GUN/shoo0.png");
	player->gun_texture[1] = mlx_load_png("./textures/GUN/shoo1.png");
	player->gun_texture[2] = mlx_load_png("./textures/GUN/shoo2.png");
	player->gun_texture[3] = mlx_load_png("./textures/GUN/shoo3.png");
	player->gun_texture[4] = mlx_load_png("./textures/GUN/shoo4.png");
	player->gun_texture[5] = mlx_load_png("./textures/GUN/shoo5.png");
	player->gun_texture[6] = mlx_load_png("./textures/GUN/shoo6.png");
	player->gun_texture[7] = NULL;
	if (!player->n_texter || !player->s_texter || !player->door_tex
		|| !player->gun_texture[0] || !player->gun_texture[2]
		|| !player->e_texter || !player->w_texter || !player->door_open_tex
		|| !player->gun_texture[1] || !player->gun_texture[3]
		|| !player->gun_texture[4] || !player->gun_texture[5]
		|| !player->gun_texture[6])
		return (ft_putendl_fd("texture not valide", STDERR_FILENO), false);
	return (true);
}

bool	assign_player(t_player *player, char **map, t_inf inf)
{
	t_point	p;

	p = find_empty_space(map, inf.position);
	player->x = (p.x * PEX) + 16;
	player->y = (p.y * PEX) + 16;
	player->color_sky = inf.color_c;
	player->color_floor = inf.color_f;
	player->map = map;
	player->map_height = count_arrays(map);
	player->map_weidth = ft_strlen(map[0]);
	player->yy = 0;
	if (!assign_texter(player, inf))
		return (false);
	if (inf.position == 'S')
		player->angle = (M_PI / 2.0);
	else if (inf.position == 'N')
		player->angle = (3 * M_PI / 2.0);
	else if (inf.position == 'E')
		player->angle = (0);
	else if (inf.position == 'W')
		player->angle = (M_PI);
	return (true);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-bouk <hel-bouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 10:04:47 by hel-bouk          #+#    #+#             */
/*   Updated: 2024/09/28 13:12:56 by hel-bouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	print_map(char **map)
{
	printf("---------\n");
	for(int i = 0; map[i]; i++)
		printf("%s\n", map[i]);
}

void	print_information(t_inf inf)
{
	printf("SO:%s\n", inf.s_path);
	printf("NO:%s\n", inf.n_path);
	printf("WE:%s\n", inf.w_path);
	printf("EA:%s\n", inf.e_path);
	printf("flow color:%d\n", inf.color_f);
	printf("celeng color:%d\n", inf.color_c);
}

bool	assign_texter(t_player *player, t_inf inf)
{
	player->n_texter = mlx_load_png(inf.n_path);
	player->s_texter = mlx_load_png(inf.s_path);
	player->e_texter = mlx_load_png(inf.e_path);
	player->w_texter = mlx_load_png(inf.w_path);
	player->door_tex = mlx_load_png("./textures/wall/door01.png");
	player->door_open_tex = mlx_load_png("./textures/wall/door_open01.png");
	player->gun_texture = malloc(sizeof(mlx_texture_t *) * 8);
	player->gun_texture[0] = mlx_load_png("./textures/GUN/shoo0.png");
	player->gun_texture[1] = mlx_load_png("./textures/GUN/shoo1.png");
	player->gun_texture[2] = mlx_load_png("./textures/GUN/shoo2.png");
	player->gun_texture[3] = mlx_load_png("./textures/GUN/shoo3.png");
	player->gun_texture[4] = mlx_load_png("./textures/GUN/shoo4.png");
	player->gun_texture[5] = mlx_load_png("./textures/GUN/shoo5.png");
	player->gun_texture[6] = mlx_load_png("./textures/GUN/shoo6.png");
	player->gun_texture[7] = NULL;
	if (!player->n_texter || !player->s_texter || !player->door_tex
		|| !player->e_texter || !player->w_texter || !player->door_open_tex)
		return (ft_putendl_fd("texture not valide", STDERR_FILENO), false);
	return (true);
}

bool	assign_player(t_player *player, char **map, t_inf inf, t_point* point)
{
	t_point	p;
	(void)point;
	p = find_empty_space(map, inf.position);
	player->x = p.x * PEX;
	player->y = p.y * PEX;
	player->color_sky = inf.color_c;
	player->color_floor = inf.color_f;
	player->map = map;
	// player->door_p = point;
	player->map_height = count_arrays(map);
	player->map_weidth = ft_strlen(map[0]); 
	player->yy =0;

	if (!assign_texter(player, inf))
		return (false);
	if (inf.position == 'N')
		player->angle = (M_PI / 2.0);
	else if (inf.position == 'S')
		player->angle = (3 * M_PI / 2.0);
	else if (inf.position == 'E')
		player->angle = (M_PI);
	else if (inf.position == 'W')
		player->angle = (0);
	return (true);
}

void	print_posintion_door(t_point *point)
{
	int	i;

	i = 0;
	printf("------------------------\n");
	while (point[i].x != -1 && point[i].y != -1)
	{
		printf("door[%d]=(%d, %d)\n", i, point[i].x, point[i].y);
		i++;
	}
	printf("------------------------\n");
}
void init_all_data(char **av, t_player *player)
{
	int		fd;
	t_inf	inf;
	t_map	*map;
	t_point	*point;
	char	**map_2d;

	map = NULL;
	initialize_inf(&inf);
	valide_arg(av[1], &fd);  
	take_map(&map, fd);
	take_information(map, &inf);
	parsing_information(&inf);
	if (!check_errors(&inf) || !valid_ch(map, &inf) || !check_map_err(map))
		return (free_inf(&inf), free_map(&map), exit(EXIT_FAILURE));
	print_information(inf);
	map_2d = built2darray(map);
	point = get_position_door(map_2d);
	print_posintion_door(point);
	if (!map_isclosed(map_2d, inf.position, point) || !valid_door(map_2d, point) || !assign_player(player, map_2d, inf, point))
		return (free_inf(&inf), free(point), free_map(&map), free_arrays(map_2d), exit(EXIT_FAILURE));
	back_to_default(map_2d, inf.position);
	print_map(map_2d);
	free_inf(&inf);
	free_map(&map);
	// free_arrays(map_2d);
}

// int main(int ac, char **av)
// {
// 	t_player	player;

// 	if (ac == 2)
// 	{
// 		init_all_data(av, &player);
// 		printf("------everything is okey------\n");
// 	}
// 	else
// 		ft_putendl_fd("Use ./cub3D file.cub", STDERR_FILENO);
// }

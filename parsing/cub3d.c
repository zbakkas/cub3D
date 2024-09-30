/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-bouk <hel-bouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 10:04:47 by hel-bouk          #+#    #+#             */
/*   Updated: 2024/09/30 20:52:27 by hel-bouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	init_all_data(char **av, t_player *player)
{
	int		fd;
	t_inf	inf;
	t_map	*map;
	t_point	*point;
	char	**map_2d;

	map = NULL;
	init_texutres(player);
	initialize_inf(&inf);
	valide_arg(av[1], &fd);
	take_map(&map, fd);
	take_information(map, &inf);
	parsing_information(&inf);
	if (!check_errors(&inf) || !valid_ch(get_position(map, 7), &inf)
		|| !check_map_err(map))
		return (free_inf(&inf), free_map(&map), exit(EXIT_FAILURE));
	map_2d = built2darray(map);
	point = get_position_door(map_2d);
	if ((!valid_door(map_2d, point)) || !map_isclosed(map_2d, inf.position, &point)
		|| !assign_player(player, map_2d, inf))
		return (free_inf(&inf), free(point), free_map(&map),
			free_arrays(map_2d), free_textures(player)
			, exit(EXIT_FAILURE));
	back_to_default(map_2d, inf.position);
	return (free_inf(&inf), free_map(&map));
}

void ll()
{
	system("leaks -q cub3D");
}

// int	main(int ac, char **av)
// {
// 	t_player	player;

// 	// atexit(ll);
// 	if (ac == 2)
// 	{
// 		init_texutres(&player);
// 		init_all_data(av, &player);
// 		free_arrays(player.map);
		// free_textures(&player);
// 		printf("------everything is okey------\n");
// 	}
// 	else
// 		ft_putendl_fd("Use ./cub3D file.cub", STDERR_FILENO);
// }

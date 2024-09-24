/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-bouk <hel-bouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 13:23:05 by hel-bouk          #+#    #+#             */
/*   Updated: 2024/09/24 19:22:47 by hel-bouk         ###   ########.fr       */
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

// int	get_coords_door(t_player *player, t_door position)
// {
// 	int i;

// 	i = 0;
// 	while (player->door_p[i].x != -1 && player->door_p[i].y != -1)
// 	{
// 		if (player->door_p[i].x == position.x
// 			&& player->door_p[i].y == position.y)
// 		return (i);
// 	}
// 	return (i);
// }
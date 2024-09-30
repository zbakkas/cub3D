/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilise5.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-bouk <hel-bouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 17:36:02 by hel-bouk          #+#    #+#             */
/*   Updated: 2024/09/30 18:14:36 by hel-bouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void    free_content_tex(t_player *player)
{
	if (player->n_texter)
		free(player->n_texter->pixels);
	if (player->s_texter)
		free(player->s_texter->pixels);
	if (player->e_texter)
		free(player->e_texter->pixels);
	if (player->w_texter)
		free(player->w_texter->pixels);
	if (player->door_tex)
		free(player->door_tex->pixels);
	if (player->door_open_tex)
		free(player->door_open_tex->pixels);
	free(player->n_texter);
	free(player->s_texter);
	free(player->e_texter);
	free(player->w_texter);
	free(player->door_tex);
	free(player->door_open_tex);
}

void	init_texutres(t_player *player)
{
	player->gun_texture = NULL;
	player->n_texter = NULL;
	player->s_texter = NULL;
	player->e_texter = NULL;
	player->w_texter = NULL;
	player->door_tex = NULL;
	player->door_open_tex = NULL;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbakkas <zouhirbakkas@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 13:23:05 by hel-bouk          #+#    #+#             */
/*   Updated: 2024/09/14 19:27:23 by zbakkas          ###   ########.fr       */
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
	inf->position = '\0';
}

// void	initialize_cords(char **map)
// {
	
// }

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbakkas <zouhirbakkas@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 13:27:45 by zbakkas           #+#    #+#             */
/*   Updated: 2024/10/01 13:35:00 by zbakkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void render_door(t_player  *player,int i,double ray_angle,int flag,t_intersection intersection)
{
    double wall_height;
    double wall_b;
    double wall_t;
    double sq;
    bool    check;
    t_int   color;
    double  tmp;

    check = false;
    player->texture_offset = 0;
    intersection.distance *= cos(ray_angle - player->angle);
    wall_height =(PEX/intersection.distance)*(WIDTH/2)/tan(FOV/2);
    wall_b= ((HEIGHT/2)+(wall_height/2)) +player->yy;
    wall_t= ((HEIGHT/2)-(wall_height/2))+player->yy;
    if(wall_b> HEIGHT) 
        wall_b =HEIGHT;
    if(wall_t<0)
    {   
        check = true;
        tmp = (-wall_t) / wall_height;
        wall_t=0;
    }
    sq = wall_t;
    player->wall_height = wall_height;
    player->wall_t = wall_t;
    player->is_vertical = flag;
    if(player->map[(int)(intersection.y/PEX)][(int)(intersection.x/PEX)]=='d')
    {
        while (wall_t< wall_b)
        {
            player->pos_y = wall_t;
            if (check)
                player->texture_offset = tmp * player->door_open_tex->height;
            color = load_colors(player, player->door_open_tex, (t_fpoint){intersection.x, intersection.y});
            if(color)
                mlx_put_pixel(player->img,i,wall_t, color);
            wall_t++;
        }
    }
}

int check_door(t_player *player, int distance)
{
    int i;
    double x;
    double y;
    
    i = 1;
    while (i<= distance)
    {
        x =player->x + cos(normal_ang(player->angle))*i;
        y = player->y + sin(normal_ang(player->angle))*i;
        if(player->map[(int)(y/PEX)][(int)(x/PEX)]=='D' && player->map[(int)(player->y/PEX)][(int)(player->x/PEX)]!='D')
        {
            player->map[(int)(y/PEX)][(int)(x/PEX)]='d';
            return 1;
        }
        else if(player->map[(int)(y/PEX)][(int)(x/PEX)]=='d'&& player->map[(int)(player->y/PEX)][(int)(player->x/PEX)]!='d')
        {
            player->map[(int)(y/PEX)][(int)(x/PEX)]='D';
            return 2;
        }
        i++;
    }

    return 0;
}

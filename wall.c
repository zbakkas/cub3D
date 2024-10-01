/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbakkas <zouhirbakkas@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 15:29:29 by zbakkas           #+#    #+#             */
/*   Updated: 2024/10/01 15:39:07 by zbakkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"


typedef struct s_args_render_wall
{
    double wall_height;
    double wall_b;
    double wall_t;
    double sq;
    bool    check;
    t_int   color;
    double  tmp;
}t_arg_w;

void render_wall(t_player  *player,int i,double ray_angle,int flag,t_intersection intersection)
{
    t_arg_w args;

    args.check = false;
    player->texture_offset = 0;
    intersection.distance *= cos(ray_angle - player->angle);
    args.wall_height =(PEX/intersection.distance)*(WIDTH/2)/tan(FOV/2);
    args.wall_b= ((HEIGHT/2)+(args.wall_height/2)) +player->yy;
    args.wall_t= ((HEIGHT/2)-(args.wall_height/2))+player->yy;
    if(args.wall_b> HEIGHT) 
        args.wall_b =HEIGHT;
    if(args.wall_t<0)
    {   
        args.check = true;
        args.tmp = (-args.wall_t) / args.wall_height;
        args.wall_t=0;
    }
    args.sq = args.wall_t;
    player->wall_height = args.wall_height;
    player->wall_t = args.wall_t;
    player->is_vertical = flag;
    while (args.wall_t< args.wall_b)
    {
        
        player->pos_y = args.wall_t;

         if(player->map[(int)(intersection.y/PEX)][(int)(intersection.x/PEX)]=='D')
        {
            if (args.check)
                player->texture_offset = args.tmp * player->door_tex->height;
            args.color = load_colors(player, player->door_tex, (t_fpoint){intersection.x, intersection.y});
            mlx_put_pixel(player->img,i,args.wall_t, args.color);
        }
        else if(flag) //vertical
        {
            
            if (ray_angle > M_PI / 2.0 && ray_angle < 3 * M_PI / 1) // left
            {
                if (args.check)
                    player->texture_offset = args.tmp * player->w_texter->height;
                args.color = load_colors(player, player->w_texter, (t_fpoint){intersection.x, intersection.y});
            }
            if ((ray_angle > 3 * M_PI / 2.0 && ray_angle < 2 * M_PI)
                || (ray_angle > 0 && ray_angle < M_PI / 2.0)) // right
            {
                if (args.check)
                    player->texture_offset = args.tmp * player->e_texter->height;
                args.color = load_colors(player, player->e_texter, (t_fpoint){intersection.x, intersection.y});
            }
        }
        else    
        {
            if (ray_angle > M_PI && ray_angle < 2 * M_PI)// down
            {
                if (args.check)
                    player->texture_offset = args.tmp * player->n_texter->height;
                args.color = load_colors(player, player->n_texter, (t_fpoint){intersection.x, intersection.y});
            }
            else // up
            {
                if (args.check)
                    player->texture_offset = args.tmp * player->s_texter->height;
                args.color = load_colors(player, player->s_texter, (t_fpoint){intersection.x, intersection.y});
            }
        }
    
        mlx_put_pixel(player->img,i,args.wall_t, args.color);
    
        args.wall_t++;
    }
    
    if(args.wall_b <=0)
        args.wall_b =1;
    while (args.wall_b >0 && args.wall_b < HEIGHT)
        mlx_put_pixel(player->img,i,args.wall_b++,player->color_floor);
    if(args.sq >=HEIGHT)
        args.sq= HEIGHT-1;
    while ( (args.sq>0 && args.sq< HEIGHT))
        mlx_put_pixel(player->img,i,args.sq--, player->color_sky);
}

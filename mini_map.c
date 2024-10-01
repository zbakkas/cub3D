/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbakkas <zouhirbakkas@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 12:19:05 by zbakkas           #+#    #+#             */
/*   Updated: 2024/10/01 13:06:08 by zbakkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"


typedef struct s_args_map
{
    double x ;
    double y;
    int h ;
    int v ;
    double start_y;
    double end_y;
    double start_x;
    double end_x ;
}t_args_map;

static void draw_player(t_player *player,int v,int h)
{
    int ii;

    ii =0;
    mlx_put_pixel(player->img,h,v,0xFF0000FF);
    mlx_put_pixel(player->img,h+1,v,0xFF0000FF);
    mlx_put_pixel(player->img,h-1,v,0xFF0000FF);
    mlx_put_pixel(player->img,h,v+1,0xFF0000FF);
    mlx_put_pixel(player->img,h,v-1,0xFF0000FF);
    mlx_put_pixel(player->img,h+1,v+1,0xFF0000FF);
    mlx_put_pixel(player->img,h-1,v-1,0xFF0000FF);
    mlx_put_pixel(player->img,h+1,v-1,0xFF0000FF);
    mlx_put_pixel(player->img,h-1,v+1,0xFF0000FF);
    while (ii < 10)
    {
        double an_x = h+(cos(normal_ang(player->angle)) *ii);
        double an_y= v+(sin(normal_ang(player->angle))*ii);
        mlx_put_pixel(player->img,an_x,an_y,0xFF0000FF);
        ii++;
    }
}

static void cader_map(t_player *player, int h, int v)
{
    int max_c_m_x =(h*2);
    int max_c_m_y =(v*2);
    int c_m_x=0;
    int c_m_y =0;
    while (c_m_x < max_c_m_x)
    {
        mlx_put_pixel(player->img,c_m_x,0,0xFF0000FF);
        mlx_put_pixel(player->img,c_m_x,1,0xFF0000FF);
        mlx_put_pixel(player->img,c_m_x++,2,0xFF0000FF);

    }
    c_m_x =0;
    while (c_m_x < max_c_m_x)
    {
        mlx_put_pixel(player->img,c_m_x,max_c_m_y,0xFF0000FF);
        mlx_put_pixel(player->img,c_m_x,max_c_m_y- 1,0xFF0000FF);
        mlx_put_pixel(player->img,c_m_x++,max_c_m_y- 2,0xFF0000FF);

    }
    while (c_m_y< max_c_m_y)
    {
        mlx_put_pixel(player->img,0,c_m_y,0xFF0000FF);
        mlx_put_pixel(player->img,1,c_m_y,0xFF0000FF);
        mlx_put_pixel(player->img,2,c_m_y++,0xFF0000FF);
    }
    c_m_y =0 ;
    while (c_m_y< max_c_m_y)
    {
        mlx_put_pixel(player->img,max_c_m_x,c_m_y,0xFF0000FF);
        mlx_put_pixel(player->img,max_c_m_x-1,c_m_y,0xFF0000FF);
        mlx_put_pixel(player->img,max_c_m_x-2,c_m_y++,0xFF0000FF);
    }
}

void mini_map(t_player *player)
{
    t_args_map args;

    args.x =player->x/2;
    args.y=player->y/2;
    args.h = 8 * (PEX/2);
    args.v = 6 * (PEX/2);
    args.start_y= args.y-args.v;
    args.end_y = args.y+args.v;
    while (args.start_y < args.end_y)
    {
        args.start_x = args.x - args.h;
        args.end_x = args.x + args.h;
        while (args.start_x < args.end_x)
        {
            if((args.start_x/(PEX/2)) >=0 && (args.start_y/(PEX/2))>=0 && (args.start_y/(PEX/2))< player->map_height && (args.start_x/(PEX/2)) < (int)ft_strlen(player->map[(int)(args.start_y/(PEX/2))]) &&player->map[(int)(args.start_y/(PEX/2))][(int)(args.start_x/(PEX/2))]=='1')
                mlx_put_pixel(player->img,args.start_x-(args.x-args.h),args.start_y-(args.y-args.v),0xFFFFFFFF);
            else if((args.start_x/(PEX/2)) >=0 && (args.start_y/(PEX/2))>=0 && (args.start_y/(PEX/2))< player->map_height && (args.start_x/(PEX/2)) < (int)ft_strlen(player->map[(int)(args.start_y/(PEX/2))]) &&player->map[(int)(args.start_y/(PEX/2))][(int)(args.start_x/(PEX/2))]=='D')
                mlx_put_pixel(player->img,args.start_x-(args.x-args.h),args.start_y-(args.y-args.v),0xFF0000FF);
             else if((args.start_x/(PEX/2)) >=0 && (args.start_y/(PEX/2))>=0 && (args.start_y/(PEX/2))< player->map_height && (args.start_x/(PEX/2)) < (int)ft_strlen(player->map[(int)(args.start_y/(PEX/2))]) &&player->map[(int)(args.start_y/(PEX/2))][(int)(args.start_x/(PEX/2))]=='d')
                mlx_put_pixel(player->img,args.start_x-(args.x-args.h),args.start_y-(args.y-args.v),0xFFFFFFF);
            else
                mlx_put_pixel(player->img,args.start_x-(args.x-args.h),args.start_y-(args.y-args.v),0x000000FF);
            args.start_x++;
        }
        args.start_y++;
    }
    cader_map(player,args.h,args.v);
    draw_player(player,args.v,args.h);
}

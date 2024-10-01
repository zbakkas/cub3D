/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersection.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbakkas <zouhirbakkas@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 13:10:04 by zbakkas           #+#    #+#             */
/*   Updated: 2024/10/01 13:26:24 by zbakkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"


void int__(t_player *player ,t_intersection *int_ , bool is_door, double xa, double ya)
{
    while ((int)(int_->x/PEX) >=0 && (int)(int_->x/PEX) <= player->map_weidth && (int)(int_->y/PEX) >=0 && (int)(int_->y/PEX)<=player->map_height)
    {
        if((int)(int_->y/PEX) < player->map_height &&((int)(int_->x/PEX)) < (int)ft_strlen(player->map[(int)(int_->y/PEX)]) && (player->map[(int)(int_->y/PEX)][(int)(int_->x/PEX)]=='1'|| (player->map[(int)(int_->y/PEX)][(int)(int_->x/PEX)]=='D' || (player->map[(int)(int_->y/PEX)][(int)(int_->x/PEX)]=='d'))))
        {
            if(player->map[(int)(int_->y/PEX)][(int)(int_->x/PEX)] == 'd' && is_door)
            {
                int_->x+=xa;
                int_->y+=ya;
            }
            else
            {
                int_->distance = (sqrt(pow(int_->x - player->x, 2) + pow(int_->y - player->y, 2)));
                break;
            }
        }
        else
        {
            int_->x+=xa;
            int_->y+=ya;
        }
    }
}

t_intersection get_h(t_player *player, double ray_angle, bool is_door)
{
    t_intersection int_;
    double first_y;
    double first_x;
    double ya;
    double xa;
    
    int_.distance =99999999999999;
    if(ray_angle > 0 && ray_angle < M_PI)//down
        first_y =(floor(player->y/PEX)*PEX)+PEX;
    else    
        first_y =(floor(player->y/PEX) *PEX)-0.00001;
    first_x = player->x+((first_y-player->y)/tan(ray_angle));
    int_.x = first_x;
    int_.y = first_y;
    if(ray_angle > 0 && ray_angle < M_PI)
        ya=PEX;
    else    
        ya=-PEX;
    xa = ya/tan(ray_angle);
   int__(player,&int_,is_door,xa,ya);
    return int_;
}

t_intersection get_v(t_player *player ,double ray_angle , bool is_door)
{
    t_intersection int_;
    double first_x;
    double first_y;
    double xa;
    double ya;
    
    int_.distance =99999999999999;
      // if(ray_angle < (0.5*M_PI) || ray_angle >(1.5*M_PI))//rigth
    if(!(ray_angle > M_PI / 2 && ray_angle < 3 * M_PI / 2))
        first_x=(floor(player->x/PEX)*PEX)+PEX;
    else    
        first_x = (floor(player->x/PEX)*PEX)-0.001;
    first_y = player->y+( first_x -player->x)*tan(ray_angle);
    int_.x =first_x;
    int_.y =first_y;
     if(!(ray_angle > M_PI / 2 && ray_angle < 3 * M_PI / 2))
        xa =PEX;
    else
        xa=-PEX;
    ya= xa*tan(ray_angle);
    int__(player,&int_,is_door,xa,ya);
    return int_;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbakkas <zouhirbakkas@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 18:54:47 by zbakkas           #+#    #+#             */
/*   Updated: 2024/09/25 13:15:56 by zbakkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <string.h>


// typedef struct s_player
// {
//     float x;
//     float y;
//     double angle;
//     float  ro;  // In radians

//     float is_up;
//     float is_down;
//     float is_right;
//     float is_left;

//     mlx_image_t* img;
//     mlx_image_t *black;
//     mlx_image_t *ray;
//     mlx_image_t *wall;
//     mlx_t *mlx;
//     char **map;
// } t_player;



float normal_ang(double angle)
{
     angle = fmod(angle,M_PI*2);
    if(angle < 0)
        angle = M_PI*2+angle;
    return angle;
}

void clear_screen(t_player *p)
{
    int y = 0;
    int x;
    while (y < (int)p->black->height)
    {
        x = 0;
        while (x < (int)p->black->width)
        {
        
            mlx_put_pixel(p->black, x, y, 0x000000FF);
            x++;
        }
        y++;
    }
}

void render_wall(t_player  *player, double ray_length,int i,double ray_angle,int flag,t_intersection intersection)
{
    double wall_height;
    double wall_b;
    double wall_t;
    double sq ;

    player->texture_offset = 0;
    ray_length *= cos(ray_angle - player->angle);
    wall_height =(PEX/ray_length)*(WIDTH/2)/tan(FOV/2);
    wall_b= ((HEIGHT/2)+(wall_height/2)) +player->yy;
    wall_t= ((HEIGHT/2)-(wall_height/2))+player->yy;
    if(wall_b> HEIGHT) 
        wall_b =HEIGHT;
    if(wall_t<0)
    {   
        int texture_height = player->n_texter->height;
        player->texture_offset = (-wall_t) / wall_height * texture_height;
        wall_t=0;
    }
    sq = wall_t;
    player->wall_height = wall_height;
    player->wall_t = wall_t;
    player->is_vertical = flag;
    while (wall_t< wall_b)
    {
        player->pos_y = wall_t;
        
        if(player->map[(int)(intersection.y/PEX)][(int)(intersection.x/PEX)]=='D')
        {
            t_int color = load_colors(player, player->door_tex, (t_fpoint){intersection.x, intersection.y});
            mlx_put_pixel(player->img,i,wall_t, color);
        }
        else if(flag) //vertical
        {
            if (ray_angle > M_PI / 2.0 && ray_angle < 3 * M_PI / 1) // left
            {
                t_int color = load_colors(player, player->w_texter, (t_fpoint){intersection.x, intersection.y});
				mlx_put_pixel(player->img,i,wall_t, color);
            }
            if ((ray_angle > 3 * M_PI / 2.0 && ray_angle < 2 * M_PI)
                || (ray_angle > 0 && ray_angle < M_PI / 2.0)) // right
            {
                t_int color = load_colors(player, player->e_texter, (t_fpoint){intersection.x, intersection.y});
				mlx_put_pixel(player->img,i,wall_t, color);
            }
        }
        else    
        {
            if (ray_angle > M_PI && ray_angle < 2 * M_PI)// down
            {
                t_int color = load_colors(player, player->s_texter, (t_fpoint){intersection.x, intersection.y});
                mlx_put_pixel(player->img,i,wall_t, color); 
            }
            else// up
            {
                  t_int color = load_colors(player, player->n_texter, (t_fpoint){intersection.x, intersection.y});
                mlx_put_pixel(player->img,i,wall_t, color); // up
            }
            }
        wall_t++;
    }
    
    if(wall_b <=0)
        wall_b =1;
    while (wall_b >0 && wall_b < HEIGHT)
        mlx_put_pixel(player->img,i,wall_b++,player->color_floor);
    if(sq >=HEIGHT)
        sq= HEIGHT-1;
    while ( (sq>0 && sq< HEIGHT))
    {
        // printf("%f\n",sq);
        mlx_put_pixel(player->img,i,sq--, player->color_sky);
    }
}
t_intersection get_h(t_player *player, double ray_angle)
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
    while ((int)(int_.x/PEX) >=0 && (int)(int_.x/PEX) <= player->map_weidth && (int)(int_.y/PEX) >=0 && (int)(int_.y/PEX)<=player->map_height)
    {
        ////////////////////////door///////////bool
        if((int)(int_.y/PEX) < player->map_height &&((int)(int_.x/PEX)) < (int)strlen(player->map[(int)(int_.y/PEX)]) && (player->map[(int)(int_.y/PEX)][(int)(int_.x/PEX)]=='1' || (player->map[(int)(int_.y/PEX)][(int)(int_.x/PEX)]=='D' && !player->open_door)))
        {
            int_.distance = (sqrt(pow(int_.x - player->x, 2) + pow(int_.y - player->y, 2)));
            break;   
        }
        else
        {
            int_.x+=xa;
            int_.y+=ya;
        }
    }
    return int_;
}

t_intersection get_v(t_player *player ,double ray_angle)
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
    while ((int)(int_.x/PEX) >=0 && (int)(int_.x/PEX) <= player->map_weidth && (int)(int_.y/PEX) >=0 && (int)(int_.y/PEX)<=player->map_height)
    {
              ////////////////////////door///////////bool
        if((int)(int_.y/PEX) < player->map_height &&((int)(int_.x/PEX)) < (int)strlen(player->map[(int)(int_.y/PEX)]) && (player->map[(int)(int_.y/PEX)][(int)(int_.x/PEX)]=='1'|| (player->map[(int)(int_.y/PEX)][(int)(int_.x/PEX)]=='D' && !player->open_door)))
        {
            int_.distance = (sqrt(pow(int_.x - player->x, 2) + pow(int_.y - player->y, 2)));
            break;
        }
        else
        {

            int_.x+=xa;
            int_.y+=ya;
        }
    }
    return int_;
}


void draw_rays2(t_player *player)
{
    // printf("yy_in_rays==%d\n",player->yy);
  
    int i=0;
    t_intersection distance_h;
    t_intersection distance_v;
    float ray_angle = (normal_ang(player->angle) -FOV/2 );
    while (i< NUM_RAYS)
    {
        ray_angle = normal_ang(ray_angle);

        /////H//////////////
        distance_h = get_h(player,ray_angle);
   
        /////////////-V-//////////////
        distance_v =get_v(player,ray_angle);

        //////////
        if(distance_v.distance <  distance_h.distance)
        {
            render_wall(player,distance_v.distance,i,ray_angle,1,distance_v);
        }
        else
        {
            render_wall(player,distance_h.distance,i,ray_angle,0,distance_h);
        }
        //////////////wall 3D
        // if((next_h_x>=0 && next_h_x<=WIDTH && next_h_y >=0 && next_h_y<=HEIGHT)||(next_v_x>=0 && next_v_x<=WIDTH && next_v_y >=0 && next_v_y<=HEIGHT))
        //////////
        i++;
        ray_angle+=(FOV/NUM_RAYS);
        
    }
}


void mini_map(t_player *player)
{
    
    double x =player->x/2;
    double y=player->y/2;
    int h = 8 * (PEX/2);
    int v = 6 * (PEX/2);
    double start_y= y-v;
    double end_y = y+v;

    
    while (start_y < end_y)
    {
        double start_x =x-h;
        double end_x = x+h;
        while (start_x < end_x)
        {
            if((start_x/(PEX/2)) >=0 && (start_y/(PEX/2))>=0 && (start_y/(PEX/2))< player->map_height && (start_x/(PEX/2)) < (int)strlen(player->map[(int)(start_y/(PEX/2))]) &&player->map[(int)(start_y/(PEX/2))][(int)(start_x/(PEX/2))]=='1')
            {
                mlx_put_pixel(player->img,start_x-(x-h),start_y-(y-v),0xFFFFFFFF);
            }
            else if((start_x/(PEX/2)) >=0 && (start_y/(PEX/2))>=0 && (start_y/(PEX/2))< player->map_height && (start_x/(PEX/2)) < (int)strlen(player->map[(int)(start_y/(PEX/2))]) &&player->map[(int)(start_y/(PEX/2))][(int)(start_x/(PEX/2))]=='D')
            {
                mlx_put_pixel(player->img,start_x-(x-h),start_y-(y-v),0xFFFFFFF);
            }
            else
                mlx_put_pixel(player->img,start_x-(x-h),start_y-(y-v),0x000000FF);

            start_x++;
        }
        
        start_y++;
    }
    
 ///cadre_map/////
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
    ///////
    mlx_put_pixel(player->img,h,v,0xFF0000FF);
    mlx_put_pixel(player->img,h+1,v,0xFF0000FF);
    mlx_put_pixel(player->img,h-1,v,0xFF0000FF);
    mlx_put_pixel(player->img,h,v+1,0xFF0000FF);
    mlx_put_pixel(player->img,h,v-1,0xFF0000FF);
    mlx_put_pixel(player->img,h+1,v+1,0xFF0000FF);
    mlx_put_pixel(player->img,h-1,v-1,0xFF0000FF);
    mlx_put_pixel(player->img,h+1,v-1,0xFF0000FF);
    mlx_put_pixel(player->img,h-1,v+1,0xFF0000FF);
    int ii =0;
    while (ii < 10)
    {
        double an_x = h+(cos(normal_ang(player->angle)) *ii);
        double an_y= v+(sin(normal_ang(player->angle))*ii);
        mlx_put_pixel(player->img,an_x,an_y,0xFF0000FF);

    ii++;
    }
    

}

//////////////doooor////////bool
void key_mov(t_player * player, float x, float y)
{
    float ray_x = player->x + x;
    float ray_y = player->y + y;
    if(player->map[(int)(player->y/PEX)][(int)(ray_x/PEX)] !='1' && player->map[(int)(ray_y/PEX)][(int)(player->x/PEX)] !='1'
        && player->map[(int)(ray_y/PEX)][(int)(ray_x/PEX)] !='1')
    {
        if ((player->map[(int)(player->y/PEX)][(int)(ray_x/PEX)] != 'D' && player->map[(int)(ray_y/PEX)][(int)(player->x/PEX)] !='D'
        && player->map[(int)(ray_y/PEX)][(int)(ray_x/PEX)] !='D') ||player->open_door)
        {
            player->x = ray_x;
            player->y = ray_y;
        }
    }
}

void my_keyhook(mlx_key_data_t keydata, void* param)
{
    t_player *player = (t_player *)param;
    float x ;
    float y ;

    if (keydata.key == CLOSE)
        player->open_door = false;
    else if (keydata.key == OPEN)
        player->open_door = true;
    if (keydata.key == MLX_KEY_W  && keydata.action)  // Move forward
    {
        x =(cos(player->angle) * MOVE_SPEED);
        y =(sin(player->angle) * MOVE_SPEED);
        key_mov(player,x,y);

    }
    if (keydata.key == MLX_KEY_S&& keydata.action)  // Move backward
    {
        x =(cos(player->angle) * -MOVE_SPEED);
        y =(sin(player->angle) * -MOVE_SPEED);
         key_mov(player,x,y);
    }
    if(keydata.key==MLX_KEY_A&& keydata.action)
    {
        x =(cos(player->angle -M_PI/2) * MOVE_SPEED);
        y =(sin(player->angle -M_PI/2) * MOVE_SPEED);
        key_mov(player,x,y);

    }
    if(keydata.key==MLX_KEY_D && keydata.action)
    {
        x =(cos(player->angle +M_PI/2) * MOVE_SPEED);
        y =(sin(player->angle +M_PI/2) * MOVE_SPEED);
        key_mov(player,x,y);

    }
    
    if (keydata.key == MLX_KEY_LEFT && keydata.action)  
    {
        player->ro = -1;
        player->angle += player->ro * ROTATE_SPEED;
    
    }
    if (keydata.key == MLX_KEY_RIGHT && keydata.action)  
    {
        player->ro = 1;
        player->angle += player->ro * ROTATE_SPEED;
    }
////
    if (keydata.key == MLX_KEY_UP && keydata.action && player->yy <= HEIGHT)   
        player->yy += 15;
    if (keydata.key == MLX_KEY_DOWN && keydata.action && player->yy >= -HEIGHT)  
        player->yy -= 15;
    if(keydata.key== MLX_KEY_SPACE && keydata.action)
       player->space =true;

    if (keydata.key == MLX_KEY_ESCAPE)
        exit(0);
    if(keydata.key ==MLX_KEY_LEFT_SHIFT)
    {
        player->is_fire =true;
        // printf("fire\n");
    }

}


void f_mouse( void *param)
{
    t_player *player = (t_player *)param;
    
   int xx=0;
    int yy=0;
    
// mlx_set_mouse_pos(player->mlx,WIDTH/2,HEIGHT/2);
    mlx_get_mouse_pos(player->mlx,&xx,&yy);
    if(xx+15< WIDTH/2)
    {
        player->ro= -MOUSE_SENSITIVE;
        player->angle += player->ro * ROTATE_SPEED;
        // printf("left\n");
    }
    else if( xx-15> WIDTH/2)
    {
        player->ro =MOUSE_SENSITIVE;
        player->angle += player->ro * ROTATE_SPEED;
        // printf("rigth\n");
    }
    
     if(yy+15<HEIGHT/2 && player->yy <= HEIGHT)
    {
        player->yy+=20;
    }
    else if(yy-15>HEIGHT/2 && player->yy >= -HEIGHT)
    {
        player->yy-=20;
    }
    
    mlx_set_mouse_pos(player->mlx,WIDTH/2,HEIGHT/2);
    
    // printf("Mouse clicked: Button %d at (%d, %d)\n", button, x, y);
}

void jump(t_player *player)
{
    if(player->space && player->time_space>=0  && !player->player_up)
    {
       
        player->yy+=15;
        player->time_space--;
    }
    if(player->space && player->time_space>=0  && player->player_up)
    {
       
        player->yy-=15;
        player->time_space--;
    }
    
    if(player->time_space < 0 && !player->player_up)
    {
        player->player_up =true;
        player->time_space =12;
    }
    if(player->time_space < 0&& player->player_up && player->space)
    {
        player->player_up =false;
        player->space =false;
        player->time_space =12;
    }
}


void game_loop(void *param)
{
    t_player *player= (t_player*)param;
    // printf("0000\n");
///

    // mlx_image_to_window(player->mlx, player->gun_image, (WIDTH - player->gun_texture->width) /2, (HEIGHT - player->gun_texture->height) );
    mlx_delete_image(player->mlx,player->img);

    player->img = mlx_new_image(player->mlx, 1500, 1200);
    mlx_image_to_window(player->mlx, player->img, 0, 0);
   /// 
    // printf("11111\n");
    //////////////////////
    mlx_delete_image(player->mlx,player->gun_image);
    if(player->is_fire)
    {
        // player->gun_image = mlx_new_image(player->mlx, 1500, 1200);
        player->gun_image= mlx_texture_to_image(player->mlx,player->gun_texture[player->i_fire]);
        mlx_image_to_window(player->mlx, player->gun_image, (WIDTH - player->gun_texture[player->i_fire]->width) /2 , (HEIGHT - player->gun_texture[player->i_fire]->height) );
        if(player->i_time >1 )
        {
         player->i_fire++;
            player->i_time=0;
        }
         
        if(player->i_fire ==11)
        {
            player->i_fire =0;
            player->is_fire =false;
        }
        player->i_time++;
    }
    else
    {
        player->gun_image= mlx_texture_to_image(player->mlx,player->gun_texture[0]);
        mlx_image_to_window(player->mlx, player->gun_image, (WIDTH - player->gun_texture[0]->width) /2 , (HEIGHT - player->gun_texture[0]->height) );
    }
        

   
    ////////
    mlx_key_hook(player->mlx, my_keyhook, player);

   jump(player);
//    printf("%f\n",normal_ang(player->angle));
    draw_rays2(player);
    clear_screen(player);
   mini_map(player);
    if(player->start_mouse--<0)
    {
        f_mouse(player);
        player->start_mouse=-1;
    }
    else    
        mlx_set_mouse_pos(player->mlx,WIDTH/2,HEIGHT/2);
    player->ro =0 ;




   
    
}

int main(int arc, char **arv)
{
    
    t_player	player;
    	player.yy =0;
        player.i_fire =0;
        player.i_time =0;
	player.space =false;
	player.time_space = 12;
	player.player_down =true;
	player.player_up =false;
    player.start_mouse =15;

	if (arc == 2)
		init_all_data(arv, &player);
	else
    {
		ft_putendl_fd("Use ./cub3D file.cub", STDERR_FILENO);
        exit(0);
    }
    player.mlx = mlx_init(WIDTH, HEIGHT, "cub3D", false);

    // player.img = mlx_new_image(player.mlx, WIDTH, HEIGHT);
    player.black = mlx_new_image(player.mlx, WIDTH, HEIGHT);

    // player.ray = mlx_new_image(player.mlx, WIDTH, HEIGHT);

    
////////////////////////////////////////
   

    player.gun_texture =malloc(10*sizeof(mlx_texture_t));
    
    player.gun_texture[0] = mlx_load_png("textures/GUN/shoo0.png");
    player.gun_texture[1] = mlx_load_png("textures/GUN/shoo5.png");
    player.gun_texture[2] = mlx_load_png("textures/GUN/shoo6.png");
    player.gun_texture[3] = mlx_load_png("textures/GUN/shoo4.png");
    player.gun_texture[4] = mlx_load_png("textures/GUN/shoo3.png");
    player.gun_texture[5] = mlx_load_png("textures/GUN/shoo2.png");
    player.gun_texture[6] = mlx_load_png("textures/GUN/shoo1.png");
    player.gun_texture[7] = mlx_load_png("textures/GUN/shoo1.png");
    player.gun_texture[8] = mlx_load_png("textures/GUN/shoo2.png");
    player.gun_texture[9] = mlx_load_png("textures/GUN/shoo3.png");
    player.gun_texture[10] = mlx_load_png("textures/GUN/shoo4.png");
	
    /////
    player.img = mlx_new_image(player.mlx, WIDTH, HEIGHT);


    
    mlx_image_to_window(player.mlx, player.black, 0, 0);
    mlx_image_to_window(player.mlx, player.img, 0, 0);

    
    
    // mlx_image_to_window(player.mlx, player.img, 0, 0);
    // mlx_image_to_window(player.mlx, player.ray, 0, 0);
    mlx_set_cursor_mode(player.mlx,MLX_MOUSE_HIDDEN);
   
    mlx_loop_hook(player.mlx,game_loop,&player);
    
    
    // mlx_scroll_hook(player.mlx,&f_mouse,&player);
    
    mlx_loop(player.mlx);

    return 0;
}


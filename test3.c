/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-bouk <hel-bouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 18:54:47 by zbakkas           #+#    #+#             */
/*   Updated: 2024/09/17 12:44:01 by hel-bouk         ###   ########.fr       */
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

void render_wall(t_player  *player, double ray_length,int i,double ray_angle,int flag)
{
    double wall_light;
    double wall_b;
    double wall_t;
    double sq ;
    
    ray_length *= cos(ray_angle - player->angle);
    wall_light =(PEX/ray_length)*(WIDTH/2)/tan(FOV/2);
    wall_b= ((HEIGHT/2)+(wall_light/2)) +player->yy;
    wall_t= ((HEIGHT/2)-(wall_light/2))+player->yy;
    if(wall_b> HEIGHT)  
        wall_b =HEIGHT;
    if(wall_t<0)
        wall_t=0;
    sq = wall_t;

    player->wall_t = wall_t;
    player->wall_b = wall_b;
    while (wall_t< wall_b)
    {
        if(flag) //vertical
        {
            if (ray_angle > M_PI / 2.0 && ray_angle < 3 * M_PI / 1) // left
            {
                // load_colors(player, player->w_texter, t_fpoint(i, wall_t));
                mlx_put_pixel(player->img,i,wall_t,0x80FF40);
            }
            if ((ray_angle > 3 * M_PI / 2.0 && ray_angle < 2 * M_PI)
                || (ray_angle > 0 && ray_angle < M_PI / 2.0)) // right
                mlx_put_pixel(player->img,i,wall_t,0xFFFF20);
        }
        else    
        {
            if (ray_angle > M_PI && ray_angle < 2 * M_PI)
            {
                t_int color = load_colors(player, player->w_texter, (t_fpoint){i, wall_t});
                mlx_put_pixel(player->img,i,wall_t, color); // down
            }
            else
            {
                t_int color = load_colors(player, player->w_texter, (t_fpoint){i, wall_t});
                mlx_put_pixel(player->img,i,wall_t, color); // up
            }
        }
        wall_t++;
    }
    while (wall_b >0 && wall_b < WIDTH)
        mlx_put_pixel(player->img,i,wall_b++,player->color_floor);
    while ( sq>=0)
        mlx_put_pixel(player->img,i,sq--, player->color_sky);
}
double get_h(t_player *player, double ray_angle)
{
    double first_y;
    double first_x;
    double ya;
    double xa;
    double next_h_x;
    double next_h_y;
    
    if(ray_angle > 0 && ray_angle < M_PI)//down
        first_y =(floor(player->y/PEX)*PEX)+PEX;
    else    
        first_y =(floor(player->y/PEX) *PEX)-0.00001;

    first_x = player->x+((first_y-player->y)/tan(ray_angle));

    next_h_x = first_x;
    next_h_y = first_y;

    if(ray_angle > 0 && ray_angle < M_PI)
        ya=PEX;
    else    
        ya=-PEX;
    xa = ya/tan(ray_angle);
    while (next_h_x>=0 && next_h_x<=WIDTH && next_h_y >=0 && next_h_y<=HEIGHT)
    {
        if((int)(next_h_y/PEX) < player->map_height &&((int)(next_h_x/PEX)) < (int)strlen(player->map[(int)(next_h_y/PEX)]) && player->map[(int)(next_h_y/PEX)][(int)(next_h_x/PEX)]=='1')
            return (sqrt(pow(next_h_x - player->x, 2) + pow(next_h_y - player->y, 2)));
        else
        {
            next_h_x+=xa;
            next_h_y+=ya;
        }
    }
    return 99999999999999;
}

double get_v(t_player *player ,double ray_angle)
{
    double first_x;
    double first_y;
    double next_v_x;
    double  next_v_y;
    double xa;
    double ya;
    
      // if(ray_angle < (0.5*M_PI) || ray_angle >(1.5*M_PI))//rigth
    if(!(ray_angle > M_PI / 2 && ray_angle < 3 * M_PI / 2))
        first_x=(floor(player->x/PEX)*PEX)+PEX;
    else    
        first_x = (floor(player->x/PEX)*PEX)-0.001;

    first_y = player->y+( first_x -player->x)*tan(ray_angle);

    next_v_x =first_x;
    next_v_y =first_y;
     if(!(ray_angle > M_PI / 2 && ray_angle < 3 * M_PI / 2))
        xa =PEX;
    else
        xa=-PEX;
    ya= xa*tan(ray_angle);
    while (next_v_x>=0 && next_v_x<=WIDTH && next_v_y >=0 && next_v_y<=HEIGHT)
    {
        if((int)(next_v_y/PEX) < player->map_height &&((int)(next_v_x/PEX)) < (int)strlen(player->map[(int)(next_v_y/PEX)]) && player->map[(int)(next_v_y/PEX)][(int)(next_v_x/PEX)]=='1')
            return (sqrt(pow(next_v_x - player->x, 2) + pow(next_v_y - player->y, 2)));
        else
        {

            next_v_x+=xa;
            next_v_y+=ya;
        }
    }
    return 99999999999999;
}


void draw_rays2(t_player *player)
{
    // printf("yy_in_rays==%d\n",player->yy);
  
    int i=0;
    double distance_h;
    double distance_v ;
    double ray_length;
    distance_h=0;
    distance_v=0;
    ray_length =0;
    
    float ray_angle = (normal_ang(player->angle) -FOV/2 );
    while (i< NUM_RAYS)
    {
        ray_angle = normal_ang(ray_angle);
        int flag =0;

        /////H//////////////
       
        distance_h = get_h(player,ray_angle);
   
        /////////////-V-//////////////
        distance_v =get_v(player,ray_angle);

        //////////
        if(distance_v< distance_h)
        {
            ray_length = distance_v;
            flag =1;
        }
        else
            ray_length = distance_h;
        //////////////wall 3D
        // if((next_h_x>=0 && next_h_x<=WIDTH && next_h_y >=0 && next_h_y<=HEIGHT)||(next_v_x>=0 && next_v_x<=WIDTH && next_v_y >=0 && next_v_y<=HEIGHT))
            render_wall(player,ray_length,i,ray_angle,flag);
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


void key_mov(t_player * player, float x, float y)
{
    float ray_x = player->x + x;
    float ray_y = player->y + y;
    if(player->map[(int)(player->y/PEX)][(int)(ray_x/PEX)] !='1' && player->map[(int)(ray_y/PEX)][(int)(player->x/PEX)] !='1' && player->map[(int)(ray_y/PEX)][(int)(ray_x/PEX)] !='1')
    {
        player->x = ray_x;
        player->y = ray_y;
    }
}

void my_keyhook(mlx_key_data_t keydata, void* param)
{
    t_player *player = (t_player *)param;
    float x ;
    float y ;

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
        printf("left\n");
    }
    else if( xx-15> WIDTH/2)
    {
        player->ro =MOUSE_SENSITIVE;
        player->angle += player->ro * ROTATE_SPEED;
        printf("rigth\n");
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

    mlx_delete_image(player->mlx,player->img);
    player->img = mlx_new_image(player->mlx, 1500, 1200);

    mlx_delete_image(player->mlx,player->ray);
    player->ray = mlx_new_image(player->mlx, 1500, 1200);
     mlx_image_to_window(player->mlx, player->img, 0, 0);
    mlx_image_to_window(player->mlx, player->ray, 0, 0);
   /// 
    // printf("11111\n");

   
    ////////
    mlx_key_hook(player->mlx, my_keyhook, player);

   jump(player);
//    printf("%f\n",normal_ang(player->angle));
    draw_rays2(player);
    clear_screen(player);
   mini_map(player);
    f_mouse(player);
    player->ro =0 ;
}

int main(int arc, char **arv)
{
    
    t_player	player;
    	player.yy =0;
	player.space =false;
	player.time_space = 12;
	player.player_down =true;
	player.player_up =false;

	if (arc == 2)
		init_all_data(arv, &player);
	else
    {
		ft_putendl_fd("Use ./cub3D file.cub", STDERR_FILENO);
        exit(0);
    }
    player.mlx = mlx_init(WIDTH, HEIGHT, "cub3D", false);

    // player.img = mlx_new_image(player.mlx, WIDTH, HEIGHT);
    player.wall = mlx_new_image(player.mlx, WIDTH, HEIGHT);
    player.black = mlx_new_image(player.mlx, WIDTH, HEIGHT);
    // player.ray = mlx_new_image(player.mlx, WIDTH, HEIGHT);

 

    player.img = mlx_new_image(player.mlx, 1500, 1200);
    mlx_put_pixel(player.img, (player.x), (player.y), 0xFF0000FF);


    mlx_image_to_window(player.mlx, player.wall, 0, 0);
    mlx_image_to_window(player.mlx, player.black, 0, 0);
    mlx_image_to_window(player.mlx, player.img, 0, 0);
    // mlx_image_to_window(player.mlx, player.img, 0, 0);
    // mlx_image_to_window(player.mlx, player.ray, 0, 0);
    mlx_set_mouse_pos(player.mlx,WIDTH/2,HEIGHT/2);
    mlx_set_cursor_mode(player.mlx,MLX_MOUSE_HIDDEN);
   
    mlx_loop_hook(player.mlx,game_loop,&player);
    
    // mlx_scroll_hook(player.mlx,&f_mouse,&player);
    
    mlx_loop(player.mlx);

    return 0;
}

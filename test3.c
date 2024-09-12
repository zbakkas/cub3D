#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "MLX/include/MLX42/MLX42.h"

#define MOVE_SPEED 2.5
#define ROTATE_SPEED 2* (M_PI/180)//0.01745329252 =>> 0.1745329252
#define FOV 60*(M_PI/180)  // 60 degrees field of view (FOV) in radians
#define PEX 32 
#define HEIGHT 1200
#define WIDTH 1500
#define NUM_RAYS (WIDTH)   // Number of rays to cast
typedef struct s_player
{
    float x;
    float y;
    double angle;
    float  ro;  // In radians

    float is_up;
    float is_down;
    float is_right;
    float is_left;

    mlx_image_t* img;
    mlx_image_t *black;
    mlx_image_t *ray;
    mlx_image_t *wall;
    mlx_t *mlx;
    char **map;
} t_player;



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


void draw_rays(t_player *player)
{   
    float ray_angle = (normal_ang(player->angle)-FOV/2);
    for (int i = 0; i < NUM_RAYS; i++)
    {
        // Calculate the angle for each ray
        

    

        // Bresenham's line algorithm for drawing the ray from the player's position to the end point
        int ray_length=1;
            // float ray_angle = player->angle - (FOV / 2) + (i * FOV / NUM_RAYS);
           
            // float ray_angle = player->angle ;
        //   printf("=========================\n");
        float ray_x =0;
        float ray_y =0;
        while (ray_length++)
        {
           
            // Calculate the end point of the ray

            int af_x = (int)ray_x;
            int af_y = (int)ray_y;
            ray_x = player->x + (cos(ray_angle) * ray_length);
            ray_y = player->y + (sin(ray_angle) * ray_length);
            int x = (int)ray_x;
            int y = (int)ray_y;
            // printf("x=%d,y=%d\n",x,y);
            

            // Check if the ray hits a wall and stop if it does
          
            // printf("x=%d,y=%d , x/PEX=%d, y/PEX=%d,angle=%f\n",x,y,x / PEX,y / PEX,player->angle);
            if (af_x && af_y && (player->map[y / PEX][af_x / PEX] == '1' || player->map[af_y / PEX][x / PEX] == '1'))
            {
                // int y_sta= (WIDTH/2) -(ray_length/2);
                // while ()
                // {
                //     mlx_put_pixel(player->ray, ray_x, ray_y, 0xFFFFFF);

                // }
                // printf("x=%d,y=%d\n",x,y);
                break;
            }
            mlx_put_pixel(player->ray, ray_x, ray_y, 0xFFFFFF); // Draw pixel along the ray
        }
         ray_angle+=FOV/NUM_RAYS;
    }
}


void draw_rays2(t_player *player)
{
    double first_y=0;
    double first_x=0;
    double xa=0;
    double ya =0;
    double next_h_x;
    next_h_x =0;
    double next_h_y;
    next_h_y=0;
    double next_v_x;
    next_v_x=0;
    double next_v_y;
    next_v_y =0;
    int i=0;
        double distance_h;
        double distance_v ;
        distance_h=0;
        distance_v=0;
        double ray_length;
        ray_length =0;
        float ray_angle = (normal_ang(player->angle)  -FOV/2);
    while (i< NUM_RAYS)
    {

        distance_h=99999999999999;
        distance_v =99999999999999;
        int flag =0;
    
        // float ray_angle = player->angle - (FOV / 2) + (i * FOV / NUM_RAYS);
        // float ray_angle = player->angle ;

        /////H//////////////
        printf("ray_angle=%f\n",ray_angle);
        if(ray_angle > 0 && ray_angle < M_PI)//down
            first_y =(floor(player->y/PEX)*PEX)+PEX;
        else    
            first_y =(floor(player->y/PEX) *PEX)-0.1;

        first_x = player->x+((first_y-player->y)/tan(ray_angle));

        next_h_x =first_x;
        next_h_y=first_y;

        if(ray_angle > 0 && ray_angle < M_PI)
            ya=PEX;
        else    
            ya=-PEX;
        xa = ya/tan(ray_angle);
        while (next_h_x>=0 && next_h_x<=WIDTH && next_h_y >=0 && next_h_y<=HEIGHT)
        {
            printf("x=%dy=%d\n",(int)(next_h_x/PEX),(int)(next_h_y/PEX));
            if(player->map[(int)(next_h_y/PEX)][(int)(next_h_x/PEX)]=='1')
            {
                // printf("is wall\n");
                distance_h = (sqrt(pow(next_h_x - player->x, 2) + pow(next_h_y - player->y, 2)));
                break;
            }
            else
            {

                next_h_x+=xa;
                next_h_y+=ya;
            }
        }
        /////////////-V-//////////////
        // if(!(ray_angle > M_PI / 2 && ray_angle < 3 * M_PI / 2))
        if(ray_angle < (0.5*M_PI) || ray_angle >(1.5*M_PI))//rigth
            first_x=(floor(player->x/PEX)*PEX)+PEX;
        else    
            first_x = (floor(player->x/PEX)*PEX)-0.1;

        first_y = player->y+( first_x -player->x)*tan(ray_angle);

        next_v_x =first_x;
        next_v_y =first_y;
        if(ray_angle < (0.5*M_PI) || ray_angle >(1.5*M_PI))
            xa =PEX;
        else
            xa=-PEX;
        ya= xa*tan(ray_angle);
        while (next_v_x>=0 && next_v_x<=WIDTH && next_v_y >=0 && next_v_y<=HEIGHT)
        {
            // printf("x=%dy=%d\n",(int)(next_v_x/PEX),(int)(next_v_y/PEX));
           
            if((int)(next_v_y/PEX) <= 22 &&((int)(next_v_x/PEX)) < (int)strlen(player->map[(int)(next_v_y/PEX)]) && player->map[(int)(next_v_y/PEX)][(int)(next_v_x/PEX)]=='1')
            {
                // printf("is wall\n");
                distance_v =(sqrt(pow(next_v_x - player->x, 2) + pow(next_v_y - player->y, 2)));
            
                break;
            }
            else
            {

                next_v_x+=xa;
                next_v_y+=ya;
            }
            //  printf("2\n");
        }
        printf("draw\n");
        if(distance_v<= distance_h)
        {
                ray_length = distance_v;
                
            printf("v\n");
            
            if(next_v_x>=0 && next_v_x<=WIDTH && next_v_y >=0 && next_v_y<=HEIGHT)
            {
            mlx_put_pixel(player->img,next_v_x,next_v_y,0xFF0000FF);
            mlx_put_pixel(player->img,next_v_x+1,next_v_y,0xFF0000FF);
            mlx_put_pixel(player->img,next_v_x-1,next_v_y,0xFF0000FF);
            mlx_put_pixel(player->img,next_v_x,next_v_y+1,0xFF0000FF);
            mlx_put_pixel(player->img,next_v_x,next_v_y-1,0xFF0000FF);
            mlx_put_pixel(player->img,next_v_x+1,next_v_y+1,0xFF0000FF);
            mlx_put_pixel(player->img,next_v_x-1,next_v_y-1,0xFF0000FF);
            mlx_put_pixel(player->img,next_v_x+1,next_v_y-1,0xFF0000FF);
            mlx_put_pixel(player->img,next_v_x-1,next_v_y+1,0xFF0000FF);
            }
        }
        else
        {
            ray_length = distance_h;
            printf("H\n");
            flag =1;
            if( distance_h !=99999999999999 &&next_h_x>=0 && next_h_x<=WIDTH && next_h_y >=0 && next_h_y<=HEIGHT)
            {
            mlx_put_pixel(player->img,next_h_x,next_h_y,0xFF0000FF);
            mlx_put_pixel(player->img,next_h_x+1,next_h_y,0xFF0000FF);
            mlx_put_pixel(player->img,next_h_x-1,next_h_y,0xFF0000FF);
            mlx_put_pixel(player->img,next_h_x,next_h_y+1,0xFF0000FF);
            mlx_put_pixel(player->img,next_h_x,next_h_y-1,0xFF0000FF);
            mlx_put_pixel(player->img,next_h_x+1,next_h_y+1,0xFF0000FF);
            mlx_put_pixel(player->img,next_h_x-1,next_h_y-1,0xFF0000FF);
            mlx_put_pixel(player->img,next_h_x+1,next_h_y-1,0xFF0000FF);
            mlx_put_pixel(player->img,next_h_x-1,next_h_y+1,0xFF0000FF);
            }
        }
            printf("distance_v====%f\n",distance_v);
            printf("distance_h====%f\n",distance_h);
        
        printf("end-draw\n");

        //////////////wall 3D
        // ray_length = (sqrt(pow(next_h_x - player->x, 2) + pow(next_h_y - player->y, 2)));
        if((next_h_x>=0 && next_h_x<=WIDTH && next_h_y >=0 && next_h_y<=HEIGHT)||(next_v_x>=0 && next_v_x<=WIDTH && next_v_y >=0 && next_v_y<=HEIGHT))
        {
            ray_length *= cos(ray_angle - player->angle);
            double wall_light =(PEX/ray_length)*(WIDTH/2)/tan(FOV/2);
            double wall_b= (HEIGHT/2)+(wall_light/2);
            double wall_t= (HEIGHT/2)-(wall_light/2);
            if(wall_b> HEIGHT)  
                wall_b =HEIGHT;
            if(wall_t<0)
                wall_t=0;

            while (wall_t< wall_b)
                if(flag)
                    mlx_put_pixel(player->img,i,wall_t++,0xFF0000FF);
                else    
                    mlx_put_pixel(player->img,i,wall_t++,0xFFFFFFFF);
        }
        //////////
        i++;
        ray_angle+=(FOV/NUM_RAYS);
        ray_angle = normal_ang(ray_angle);
    }
}


void draw_wall(t_player *pl)
{
    int y =0;
    int x =0;
    while (pl->map[y])
    {
        x =0;
        while (pl->map[y][x])
        {
            if(pl->map[y][x]=='1')
            {
                int p_y=0;
                int p_x;
                while (p_y < PEX)
                {
                   p_x =0;
                   while (p_x <PEX)
                   {
                        if(p_x !=PEX-1 && p_y !=PEX-1)
                            mlx_put_pixel(pl->wall,p_x+(PEX*x),p_y+(PEX*y),0xFFFFFFFF);
                        p_x++;
                   }
                   p_y++;
                }
                
            }
            x++;
        }
        y++;
    }
    
}

void my_keyhook(mlx_key_data_t keydata, void* param)
{
    t_player *player = (t_player *)param;

    /////////////////////
    printf("right=%1.f\n",player->is_right);
   
    ////////
    mlx_delete_image(player->mlx,player->img);
    player->img = mlx_new_image(player->mlx, 1500, 1200);

    mlx_delete_image(player->mlx,player->ray);
    player->ray = mlx_new_image(player->mlx, 1500, 1200);
    mlx_image_to_window(player->mlx, player->ray, 0, 0);
     mlx_image_to_window(player->mlx, player->img, 0, 0);
    if (keydata.key == MLX_KEY_W  )  // Move forward
    {
        float x =(cos(player->angle) * MOVE_SPEED);
        float y =(sin(player->angle) * MOVE_SPEED);
 
        float ray_x = player->x + x;
        float ray_y = player->y + y;
   
            player->x = ray_x;
            player->y = ray_y;
  
    }
    if (keydata.key == MLX_KEY_S)  // Move backward
    {
        float x =(cos(player->angle) * -MOVE_SPEED);
        float y =(sin(player->angle) * -MOVE_SPEED);
         float ray_x = player->x + x;
        float ray_y = player->y + y;
        player->x = ray_x;
        player->y = ray_y;
    }
    
    if (keydata.key == MLX_KEY_LEFT && keydata.action)  // Rotate left
    {
        player->ro = -1;
    
    }
    if (keydata.key == MLX_KEY_RIGHT && keydata.action)  // Rotate right
    {
        player->ro = 1;
    }
    if (keydata.key == MLX_KEY_ESCAPE)
        exit(0);
    player->angle += player->ro * ROTATE_SPEED;

 
    clear_screen(player);
    draw_wall(player);
    draw_rays(player);
    draw_rays2(player);
   
    mlx_put_pixel(player->img, (player->x), (player->y), 0xFF0000FF);
    player->ro=0;
    //////
    player->is_down=0;
  player->is_up=0;
  player->is_left=0;
  player->is_right=0;
  
}


int main()
{
    t_player player = {190, 3*32, 0,0,0,0,0,0,NULL,NULL,NULL,NULL,NULL,NULL};
    // x=343,y=372 
    player.map =malloc(23*sizeof(char *));

    player.map[0] = strdup("111111111111111111111111111111111111");
    player.map[1] = strdup("101001000000000000000000000000000001");
    player.map[2] = strdup("110010100000000000000000000000000001");
    player.map[3] = strdup("10000100000000000100000000000111111111111");
   player.map[4] = strdup( "100000000000000010100000000000000000000001");
    player.map[5] = strdup("11110111111111110000000000000000111111111");
    player.map[6] = strdup("11110111111111111000000000000000000010001");
   player.map[7] = strdup( "10000000000000000111111111111110000100011");
   player.map[8] = strdup( "10000000000000000000000000000000000100011");
   player.map[9] = strdup( "10000000000000000000000000000000000100011");
   player.map[10] = strdup( "10000000000000000000000000000000000010001");
   player.map[11] = strdup( "100000000000000000000000000000000011000111");
    player.map[12] = strdup("100000000000000000000000000000000000001");
    player.map[13] = strdup("100000000000000000000000000000000000001");
    player.map[14] = strdup("100000000000000000000000000000000000001");
    player.map[15] = strdup("100000000000000000000000000000000000001");
    player.map[16] = strdup("100000000000000000000000000000000000001");
    player.map[17] = strdup("100000000000000000000000000000000000001");
    player.map[18] = strdup("100000000000000000000000000000000000001");
    player.map[19] = strdup("100000000000000000000000000000000000001");
    player.map[20] = strdup("100000000000000000000000000000000000001");
    player.map[21] = strdup("100000000000000000000000000000000000001");
    player.map[22] = strdup("11111111111111111111111111111111111111");
    player.map[23] =NULL;
    player.mlx = mlx_init(WIDTH, HEIGHT, "cub3D", true);

    // player.img = mlx_new_image(player.mlx, WIDTH, HEIGHT);
    player.black = mlx_new_image(player.mlx, WIDTH, HEIGHT);
    player.wall = mlx_new_image(player.mlx, WIDTH, HEIGHT);
    // player.ray = mlx_new_image(player.mlx, WIDTH, HEIGHT);


    player.img = mlx_new_image(player.mlx, 1500, 1200);
    mlx_image_to_window(player.mlx, player.img, 0, 0);
    mlx_put_pixel(player.img, (player.x), (player.y), 0xFF0000FF);


    mlx_image_to_window(player.mlx, player.black, 0, 0);
    mlx_image_to_window(player.mlx, player.wall, 0, 0);
    // mlx_image_to_window(player.mlx, player.img, 0, 0);
    // mlx_image_to_window(player.mlx, player.ray, 0, 0);

    mlx_key_hook(player.mlx, my_keyhook, &player);
    mlx_loop(player.mlx);

    return 0;
}

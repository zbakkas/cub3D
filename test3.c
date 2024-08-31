#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "MLX/include/MLX42/MLX42.h"

#define PI 3.14159265358979323846
#define MOVE_SPEED 2
#define FOV (PI / 3)  // 60 degrees field of view (FOV) in radians
#define NUM_RAYS 60   // Number of rays to cast
#define PEX 16 
typedef struct s_player
{
    int x;
    int y;
    float angle;  // In radians

    mlx_image_t* img;
    mlx_image_t *black;
    mlx_image_t *ray;
    mlx_image_t *wall;
    mlx_t *mlx;
    char **map;
} t_player;



void clear_screen(t_player *p)
{
    int y = 0;
    int x;
    while (y < p->black->height)
    {
        x = 0;
        while (x < p->black->width)
        {
            mlx_put_pixel(p->black, x, y, 0x000000FF);
            x++;
        }
        y++;
    }
}



void draw_rays(t_player *player, int ray_length)
{   

    for (int i = 0; i < NUM_RAYS; i++)
    {
        // Calculate the angle for each ray
        float ray_angle = player->angle - (FOV / 2) + (i * FOV / NUM_RAYS);

        // Calculate the end point of the ray
        int ray_x = player->x + (int)(cos(ray_angle) * ray_length);
        int ray_y = player->y + (int)(sin(ray_angle) * ray_length);

        // Draw the ray
        mlx_put_pixel(player->ray, ray_x, ray_y, 0xFFFFFF);
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
                        mlx_put_pixel(pl->wall,p_x+(16*x),p_y+(16*y),0xFFFFFFFF);
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

    mlx_delete_image(player->mlx,player->img);
    player->img = mlx_new_image(player->mlx, 1500, 1200);

    mlx_delete_image(player->mlx,player->ray);
    player->ray = mlx_new_image(player->mlx, 1500, 1200);
    mlx_image_to_window(player->mlx, player->ray, 0, 0);
     mlx_image_to_window(player->mlx, player->img, 0, 0);
    if (keydata.key == MLX_KEY_W )  // Move forward
    {
        int x =round(cos(player->angle) * MOVE_SPEED);
        int y =round(sin(player->angle) * MOVE_SPEED);
        // printf("%c\n");
        if(player->map[((player->y+y)/PEX)][((player->x+x)/PEX)] !='1')
        {
            player->x += x;
            player->y += y;
        }
        // printf("x=%d\n",x);
        // printf("y=%d\n",y);
        // printf("Moving forward\n");
    }
    if (keydata.key == MLX_KEY_S)  // Move backward
    {
        int x =round(cos(player->angle) * MOVE_SPEED);
        int y =round(sin(player->angle) * MOVE_SPEED);
        // printf("%c\n");
        if(player->map[((player->y-y)/PEX)][((player->x-x)/PEX)] !='1')
        {
            player->x -= x;
            player->y -= y;
        }
    }
    if (keydata.key == MLX_KEY_A)  // Strafe left
    {
        int x =round(cos(player->angle - PI / 2) * MOVE_SPEED);
        int y =round(sin(player->angle - PI / 2) * MOVE_SPEED);
        if(player->map[((player->y+y)/PEX)][((player->x+x)/PEX)] !='1')
        {
            player->x += x;
            player->y += y;
        }
    }
    if (keydata.key == MLX_KEY_D)  // Strafe right
    {

        int x =round(cos(player->angle + PI / 2) * MOVE_SPEED);
        int y =round(sin(player->angle + PI / 2) * MOVE_SPEED);
        if(player->map[((player->y+y)/PEX)][((player->x+x)/PEX)] !='1')
        {
            player->x += x;
            player->y += y;
        }
    }
    if (keydata.key == MLX_KEY_LEFT)  // Rotate left
    {
        player->angle -= 0.1;
        if (player->angle < 0)
            player->angle += 2 * PI;
    }
    if (keydata.key == MLX_KEY_RIGHT)  // Rotate right
    {
        player->angle += 0.1;
        if (player->angle > 2 * PI)
            player->angle -= 2 * PI;
    }
    if (keydata.key == MLX_KEY_ESCAPE)
        exit(0);

    // Clear screen and previous rays
    clear_screen(player);
draw_wall(player);
    // Draw player at new position
    

    // Draw rays across 60 degrees FOV
    int i =1;
    while (i<100)
    {
        draw_rays(player,i++);
    }
    mlx_put_pixel(player->img, player->x, player->y, 0xFF0000FF);
}

int main()
{
    t_player player = {17, 16*9, 0};
    
    player.map =malloc(15*sizeof(char *));

    player.map[0] = strdup("1111111111111111111111111");
    player.map[1] = strdup("1000000000110000000000001");
    player.map[2] = strdup("1011000001110000000000001");
    player.map[3] = strdup("100100000000000000000000111111111");
   player.map[4] = strdup( "111111111011000001110000000000001");
    player.map[5] = strdup("100000000011000001110111111111111");
    player.map[6] = strdup("11110111111111011100000010001");
   player.map[7] = strdup( "11110111111111011101010010001");
   player.map[8] = strdup( "11000000110101011100000010001");
   player.map[9] = strdup( "10000000000000001100000010001");
   player.map[10] = strdup( "10000000000000001101010010001");
   player.map[11] = strdup( "1100000111010101111101111000111");
    player.map[12] = strdup("111101111110101101111010001");
    player.map[13] = strdup("111111111111111111111111111");
    player.map[14] =NULL;

    player.mlx = mlx_init(1500, 1200, "cub3D", true);

    // player.img = mlx_new_image(player.mlx, 1500, 1200);
    player.black = mlx_new_image(player.mlx, 1500, 1200);
    player.wall = mlx_new_image(player.mlx, 1500, 1200);
    // player.ray = mlx_new_image(player.mlx, 1500, 1200);

    mlx_image_to_window(player.mlx, player.black, 0, 0);
    mlx_image_to_window(player.mlx, player.wall, 0, 0);
    // mlx_image_to_window(player.mlx, player.img, 0, 0);
    // mlx_image_to_window(player.mlx, player.ray, 0, 0);

    mlx_key_hook(player.mlx, my_keyhook, &player);
    mlx_loop(player.mlx);

    return 0;
}

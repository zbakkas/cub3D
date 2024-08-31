// #include "cub3d.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include "MLX/include/MLX42/MLX42.h"


typedef struct s_player
{
    int x;
    int y;
    float angle;

    mlx_image_t* img;
    mlx_t *mlx;
    mlx_image_t* background;
    mlx_image_t* background2;
    mlx_texture_t *wall;
    mlx_texture_t *floor;
    mlx_image_t *wall_i;
} t_player;

char *map[] = {
"1111111111111111111111111\0",
"1000000000110000000000001\0",
"1011000001110000000000001\0",
"100100000000000000000000111111111\0",
"111111111011000001110000000000001\0",
"100000000011000001110111111111111\0",
"11110111111111011100000010001\0",
"11110111111111011101010010001\0",
"11000000110101011100000010001\0",
"10000000000000001100000010001\0",
"10000000000000001101010010001\0",
"1100000111010101111101111000111\0",
"111101111110101101111010001\0",
"111111111111111111111111111\0",
};







void c_floor(t_player *player)
 {
    int y=0;
    int x;
    int i =16;
    while (map[y] && y< 14) 
    {
        x =0;
        while (map[y][x]) 
        {
 
            if(map[y][x]=='0')
                mlx_image_to_window(player->mlx, player->background, x*i, y*i);
            else if(map[y][x]=='1')
                mlx_image_to_window(player->mlx, player->wall_i, x*i, y*i);
            x++;
        }
        y++;
    }
}

void clear_window(t_player *player)
{
    int width = player->background2->width;
    int height = player->background2->height;

    // Iterate through each pixel and set it to black
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            mlx_put_pixel(player->background2, x, y, 0x000000FF); // Black color
        }
    }
}



void my_keyhook(mlx_key_data_t keydata, void* param)
{
    t_player *player = (t_player *)param;
    
    if (keydata.key == MLX_KEY_W )
    {
        player->y-=1;
        printf("w\n");
    }
    if (keydata.key == MLX_KEY_S )
    {
        player->y+=1;
        printf("S\n");
    }
    if (keydata.key == MLX_KEY_A )
    {
        player->x-=1;
        printf("A\n");
    }
    if (keydata.key == MLX_KEY_D )
    {
        player->x+=1;
        printf("D\n");
    }
    if (keydata.key == MLX_KEY_LEFT) // Turn left
        player->angle -= 0.1; // Adjust the value as needed
    if (keydata.key == MLX_KEY_RIGHT) // Turn right
        player->angle += 0.1; // Adjust the value as needed
    if(keydata.key==MLX_KEY_ESCAPE)
        exit(0);
    // c_floor(player);
clear_window(player);
mlx_image_to_window(player->mlx, player->background2, 0, 0);
    mlx_image_to_window(player->mlx, player->img, player->x, player->y);
    mlx_image_to_window(player->mlx, player->img, player->x+1, player->y);
    mlx_image_to_window(player->mlx, player->img, player->x+2, player->y);
    mlx_image_to_window(player->mlx, player->img, player->x+3, player->y);
    mlx_image_to_window(player->mlx, player->img, player->x-1, player->y);
    mlx_image_to_window(player->mlx, player->img, player->x, player->y+1);
    mlx_image_to_window(player->mlx, player->img, player->x, player->y-1);

}

int main()
{
    t_player player={16,16,0};

   player.mlx = mlx_init(1500,1200,"cub3D",true);

    player.wall = mlx_load_png("./wall16.png");
    player.floor = mlx_load_png("./floor16.png");
    
	player.img = mlx_new_image(player.mlx, 1500, 1200);
    player.background2 = mlx_new_image(player.mlx, 1500, 1200);
    mlx_image_to_window(player.mlx, player.background2, 0, 0);
    player.background = mlx_texture_to_image(player.mlx,player.floor);
    player.wall_i = mlx_texture_to_image(player.mlx,player.wall);
	mlx_put_pixel(player.img, player.x, player.y, 0xFF0000FF);
    // c_floor(&player);
    mlx_key_hook(player.mlx,my_keyhook,&player);
    mlx_loop(player.mlx);
}
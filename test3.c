/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-bouk <hel-bouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 18:54:47 by zbakkas           #+#    #+#             */
/*   Updated: 2024/09/20 20:56:23 by hel-bouk         ###   ########.fr       */
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
typedef struct s_intersection {
	double distance;
	double x;
	double y;
} t_intersection;


float normal_ang(double angle)
{
	 angle = fmod(angle,M_PI*2);
	if(angle < 0)
		angle = M_PI*2+angle;
	return angle;
}

uint32_t get_texture_pixel(mlx_texture_t *texture, int x, int y)
{
	if (x < 0 || x >= texture->width || y < 0 || y >= texture->height)
		return 0; // Out of bounds, return a default color (e.g., black)
	
	// Calculate the pixel location in the texture data
	uint32_t *pixels = (uint32_t *)texture->pixels;
	return pixels[y * texture->width + x];
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

void render_wall(t_player *player, double ray_length, int i, double ray_angle, int flag, double intersection_x, double intersection_y)
{
	double wall_light;
	double wall_b;
	double wall_t;
	double sq;
	
	// Correct fisheye effect
	ray_length *= cos(ray_angle - player->angle);

	// Calculate projected wall height
	wall_light = (PEX / ray_length) * (WIDTH / 2) / tan(FOV / 2); // ?
	wall_b = ((HEIGHT / 2) + (wall_light / 2)) + player->yy;
	wall_t = ((HEIGHT / 2) - (wall_light / 2)) + player->yy;

	if (wall_b > HEIGHT) 
		wall_b = HEIGHT;
	if (wall_t < 0)
		wall_t = 0;

	player->wall_t = wall_t;
	// player->wall_b = wall_b;
	player->wall_light = wall_light;
	player->is_vertical = flag;

	// Loop through each pixel of the wall slice and map texture
	while (wall_t < wall_b)
	{
		player->pos_y = wall_t;
		// texture_y = (int)((wall_t - player->wall_t) * texture_height / wall_light);
		if (flag)
		{
			if (ray_angle > M_PI / 2.0 && ray_angle < 3 * M_PI / 1) // left
			{
				t_int color = load_colors(player, player->w_texter, (t_fpoint){intersection_x, intersection_y});
				mlx_put_pixel(player->img,i,wall_t, color);
			}
			if ((ray_angle > 3 * M_PI / 2.0 && ray_angle < 2 * M_PI)
				 || (ray_angle > 0 && ray_angle < M_PI / 2.0)) // right
			{
				t_int color = load_colors(player, player->e_texter, (t_fpoint){intersection_x, intersection_y});
				mlx_put_pixel(player->img,i,wall_t, color);
			}
		}
		else
		{
			if (ray_angle > M_PI && ray_angle < 2 * M_PI)
            {
                t_int color = load_colors(player, player->s_texter, (t_fpoint){intersection_x, intersection_y});
                mlx_put_pixel(player->img,i,wall_t, color); // down
            }
            else
            {
                t_int color = load_colors(player, player->n_texter, (t_fpoint){intersection_x, intersection_y});
                mlx_put_pixel(player->img,i,wall_t, color); // up
            }
		}
		wall_t++;
	}

	// Draw the floor and ceiling (sky and ground)
	while (wall_b > 0 && wall_b < HEIGHT)
		mlx_put_pixel(player->img, i, wall_b++, player->color_floor);
	
	while (player->wall_t > 0 && player->wall_t < HEIGHT)
		mlx_put_pixel(player->img, i, player->wall_t--, player->color_sky);
}


t_intersection get_h(t_player *player, double ray_angle)
{
	t_intersection result;
	double first_y;
	double first_x;
	double ya;
	double xa;

	if(ray_angle > 0 && ray_angle < M_PI) // Down
		first_y = (floor(player->y / PEX) * PEX) + PEX;
	else
		first_y = (floor(player->y / PEX) * PEX) - 0.00001;

	first_x = player->x + ((first_y - player->y) / tan(ray_angle));

	if (ray_angle > 0 && ray_angle < M_PI)
		ya = PEX;
	else
		ya = -PEX;
	xa = ya / tan(ray_angle);

	while (first_x >= 0 && first_x <= WIDTH && first_y >= 0 && first_y <= HEIGHT) {
		if ((int)(first_y / PEX) < player->map_height &&
			(int)(first_x / PEX) < (int)strlen(player->map[(int)(first_y / PEX)]) &&
			player->map[(int)(first_y / PEX)][(int)(first_x / PEX)] == '1') {
			result.distance = sqrt(pow(first_x - player->x, 2) + pow(first_y - player->y, 2));
			result.x = first_x;
			result.y = first_y;
			return result;
		}
		first_x += xa;
		first_y += ya;
	}
	result.distance = 10e9;
	return result;
}

t_intersection get_v(t_player *player, double ray_angle)
{
	t_intersection result;
	double first_x;
	double first_y;
	double xa;
	double ya;

	if (!(ray_angle > M_PI / 2 && ray_angle < 3 * M_PI / 2)) // Right
		first_x = (floor(player->x / PEX) * PEX) + PEX;
	else
		first_x = (floor(player->x / PEX) * PEX) - 0.001;

	first_y = player->y + (first_x - player->x) * tan(ray_angle);

	if (!(ray_angle > M_PI / 2 && ray_angle < 3 * M_PI / 2))
		xa = PEX;
	else
		xa = -PEX;
	ya = xa * tan(ray_angle);

	while (first_x >= 0 && first_x <= WIDTH && first_y >= 0 && first_y <= HEIGHT) {
		if ((int)(first_y / PEX) < player->map_height &&
			(int)(first_x / PEX) < (int)strlen(player->map[(int)(first_y / PEX)]) &&
			player->map[(int)(first_y / PEX)][(int)(first_x / PEX)] == '1') {
			result.distance = sqrt(pow(first_x - player->x, 2) + pow(first_y - player->y, 2));
			result.x = first_x;
			result.y = first_y;
			return result;
		}
		first_x += xa;
		first_y += ya;
	}
	result.distance = 99999999999999;
	return result;
}


void draw_rays2(t_player *player)
{
	int i = 0;
	t_intersection h_intersection;
	t_intersection v_intersection;
	double ray_length;
	
	float ray_angle = normal_ang(player->angle - FOV / 2);
	while (i < NUM_RAYS)
	{
		ray_angle = normal_ang(ray_angle);
		int flag = 0;

		// Get horizontal and vertical intersections
		h_intersection = get_h(player, ray_angle);
		v_intersection = get_v(player, ray_angle);

		// Use the closer of the two intersections
		if (v_intersection.distance < h_intersection.distance) {
			ray_length = v_intersection.distance;
			flag = 1; // Vertical wall
		} else {
			ray_length = h_intersection.distance;
		}
		// Render wall with texture mapping
		if (flag)
			render_wall(player, ray_length, i, ray_angle, flag, v_intersection.x, v_intersection.y);
		else
			render_wall(player, ray_length, i, ray_angle, flag, h_intersection.x, h_intersection.y);		
		i++;
		ray_angle += (FOV / NUM_RAYS);
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
	}
	else if( xx-15> WIDTH/2)
	{
		player->ro =MOUSE_SENSITIVE;
		player->angle += player->ro * ROTATE_SPEED;
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
	mlx_delete_image(player->mlx,player->img);
	player->img = mlx_new_image(player->mlx, 1500, 1200);
	 mlx_image_to_window(player->mlx, player->img, 0, 0);
	mlx_key_hook(player->mlx, my_keyhook, player);

   jump(player);
//    printf("%f\n",normal_ang(player->angle));
	draw_rays2(player);
	clear_screen(player);
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

	

	player.img = mlx_new_image(player.mlx, 1500, 1200);
	mlx_put_pixel(player.img, (player.x), (player.y), 0xFF0000FF);


	player.wall_texture = player.e_texter;

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


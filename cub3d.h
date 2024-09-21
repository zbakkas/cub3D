#ifndef CUB3D_H
# define CUB3D_H

# include <math.h>
# include <fcntl.h>
# include <stdio.h>
#include <stdlib.h>
# include "libft/libft.h"
# include "get_next_line/get_next_line.h"
# include "MLX/include/MLX42/MLX42.h"

#define PI 3.14
#define V_CH " 10NSEW"
#define P_PLAYER "NSEW"
#define ERR_CH "Invalid character or you have more than one player"
#define NOT_EXIST "Map not exist"
#define MAP_ERR "map is not sourounded or closed"
#define MAP_V "MAP is not valid"
#define PY_ERR "ERR dont have Player"

typedef unsigned int t_int;

typedef struct s_point
{
	int	x;
	int	y;
}		t_point;

typedef struct s_fpoint
{
	double	x;
	double	y;
}		t_fpoint;

typedef struct s_map
{
	char			*row;
	int				s_x;
	struct s_map	*prev;
	struct s_map	*next;
}   				t_map;

typedef struct s_color
{
	int r;
	int g;
	int b;
}		t_color;

typedef struct s_inf
{
	char	**inf_map;
	char	*n_path;
	char	*s_path;
	char	*e_path;
	char	*w_path;
	char	*f_color;
	char	*c_color;
	char	position;
	int		color_c;
	int		color_f;
}			t_inf;

typedef struct s_player
{
	char	**map;
	float	x;
	float	y;
	int 	map_height;
	int		map_weidth;
	unsigned int	color_sky;
	unsigned int	color_floor;		
	double	angle;
	float 	ro;  // In radians

	mlx_image_t* img;
	mlx_image_t *black;
	mlx_t *mlx;
	int yy;
	bool space;
	bool is_vertical;
	int time_space;
	bool player_up;
	bool player_down;
	double	wall_t;
	double	wall_b;
	double	pos_y;
	double wall_height;
	int start_mouse;
	mlx_texture_t *n_texter;
	mlx_texture_t *s_texter;
	mlx_texture_t *e_texter;
	mlx_texture_t *w_texter;
	
	
	  mlx_texture_t **gun_texture;
    mlx_image_t *gun_image;
	bool is_fire;
	int i_fire ;
	int i_time;

} t_player;

typedef struct s_intersection {
	double distance;
	double x;
	double y;
} t_intersection;


//all function for paring
void	print_map(char **map);
int		get_max(t_map *map);
void	free_inf(t_inf	*inf);
void    free_map(t_map **map);
bool	check_errors(t_inf *inf);
bool	valide_path(char **path);
bool	check_map_err(t_map *map);
char	**built2darray(t_map *map);
void    initialize_inf(t_inf *inf);
void	back_to_default(char **map);
void	print_information(t_inf inf);
int		count_exist_line(t_map *map);
void    take_map(t_map **map, int fd);
void	valide_arg(char *file, int *fd);
void	parsing_information(t_inf *inf);
bool	map_isclosed(char **map, char c);
bool	valid_ch(t_map *map, t_inf *inf);
t_map	*get_position(t_map *map, int idx);
void	find_start(t_map *map, char	*line);
void	creat_list(t_map **map, char *line);
t_point	find_empty_space(char **map, char c);
bool	flood_fill(char **tab, t_point begin);
bool	catch_color(char *p_color, int *n_color);
void    take_information(t_map *map, t_inf *inf);
char	*fill_spaces(char	*str, int len, int size);
bool	check_wall(char **map, t_point begin, t_point size);
t_int	load_colors(t_player *player, mlx_texture_t *texture, t_fpoint x);
// all function for recasting

#define MOVE_SPEED 10
#define ROTATE_SPEED 10 * (M_PI/180) //0.01745329252 =>> 0.1745329252
#define FOV 60 * (M_PI/180)  // 60 degrees field of view (FOV) in radians
#define PEX 32 
#define HEIGHT 1200
#define WIDTH 1500
#define NUM_RAYS (WIDTH)
#define MOUSE_SENSITIVE 0.5

void draw_rays2(t_player *player);
void init_all_data(char **av, t_player *player);
#endif

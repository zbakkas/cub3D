#include "cub3d.h"

void render_wall(t_player *player, double ray_length, int i, double ray_angle, int flag, t_intersection intersection)
{
    double wall_height;
    double wall_b;
    double wall_t;
    double sq;
    double texture_offset;

    ray_length *= cos(ray_angle - player->angle); // Correct for fisheye effect
    wall_height = (PEX / ray_length) * (WIDTH / 2) / tan(FOV / 2); // Calculate wall height
    wall_b = ((HEIGHT / 2) + (wall_height / 2)) + player->yy;
    wall_t = ((HEIGHT / 2) - (wall_height / 2)) + player->yy;

    // Calculate texture offset if the wall is clipped at the top
    texture_offset = 0;
    if (wall_t < 0)
    {
        texture_offset = (-wall_t / wall_height) * player->n_texter->height; // Adjust y_texture starting point for clipping
        wall_t = 0;
    }
    if (wall_b > HEIGHT)
        wall_b = HEIGHT; // Clip bottom of the wall to screen height

    sq = wall_t;

    player->wall_height = wall_height;
    player->wall_t = wall_t;
    player->wall_b = wall_b;
    player->is_vertical = flag;

    // Render the wall with texture
    while (wall_t < wall_b)
    {
        player->pos_y = wall_t;
        t_int color;

        // Calculate texture coordinates based on the wall's position and whether it's a vertical or horizontal wall
        if (flag) // Vertical wall
        {
            if (ray_angle > M_PI / 2.0 && ray_angle < 3 * M_PI / 2.0) // Left wall
                color = load_colors(player, player->w_texter, (t_fpoint){intersection.x, intersection.y}, wall_t, texture_offset);
            else // Right wall
                color = load_colors(player, player->e_texter, (t_fpoint){intersection.x, intersection.y}, wall_t, texture_offset);
        }
        else // Horizontal wall
        {
            if (ray_angle > M_PI && ray_angle < 2 * M_PI) // Down
                color = load_colors(player, player->s_texter, (t_fpoint){intersection.x, intersection.y}, wall_t, texture_offset);
            else // Up
                color = load_colors(player, player->n_texter, (t_fpoint){intersection.x, intersection.y}, wall_t, texture_offset);
        }

        // Draw the pixel if within bounds
        if (!(wall_t < 0 || wall_t >= HEIGHT))
            mlx_put_pixel(player->img, i, wall_t, color);

        wall_t++;
    }

    // Fill the space below the wall with the floor color
    while (wall_b < HEIGHT)
        mlx_put_pixel(player->img, i, wall_b++, player->color_floor);

    // Fill the space above the wall with the sky color
    while (sq > 0)
        mlx_put_pixel(player->img, i, sq--, player->color_sky);
}

t_int load_colors(t_player *player, mlx_texture_t *texture, t_fpoint intersection, double wall_t, double texture_offset)
{
    t_int x_texture;
    t_int y_texture;
    double tmp;
    t_int pos;
    uint8_t *pixel_data;
    t_int color;

    int texture_height = texture->height;
    int texture_width = texture->width;

    // Calculate the x position in the texture based on whether it's a vertical or horizontal wall
    if (!player->is_vertical)
        x_texture = ((intersection.x / PEX) - floor(intersection.x / PEX)) * texture_width;
    else
        x_texture = ((intersection.y / PEX) - floor(intersection.y / PEX)) * texture_width;

    // Calculate the y position in the texture and apply the texture offset for clipping
    tmp = (wall_t - player->wall_t) * texture_height;
    y_texture = (tmp / player->wall_height) + texture_offset;

    // Ensure y_texture is within texture bounds
    if (y_texture < 0 || y_texture >= texture_height)
        return 0;

    // Calculate the position in the texture pixel array
    pos = (y_texture * texture_width + x_texture) * texture->bytes_per_pixel;

    // Ensure x_texture is within bounds and extract the pixel color
    if (x_texture >= 0 && x_texture < texture_width && pos >= 0 && pos < texture_height * texture_width * texture->bytes_per_pixel)
    {
        pixel_data = &(texture->pixels[pos]);
        color = (pixel_data[0] << 24) | (pixel_data[1] << 16) | (pixel_data[2] << 8) | pixel_data[3];
    }
    else
        color = 0;  // Return black if out of bounds

    return color;
}


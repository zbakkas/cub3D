https://github.com/user-attachments/assets/1a3d539b-78bd-4537-99fb-021bc2ffb1a5
# cub3D

cub3D is a graphical 3D game engine project written primarily in C, inspired by classic raycasting games such as Wolfenstein 3D. The project is designed for educational purposes and demonstrates fundamental concepts in raycasting, player movement, and map parsing using the MLX42 graphics library.

## Features

- **Raycasting engine**: Simulates a 3D environment using a 2D map and renders walls, doors, and textures.
- **Player movement**: Includes walking, rotation, and interaction with doors.
- **Map parsing**: Supports custom `.cub` files for defining maps, textures, and colors.
- **Minimap**: Renders a minimap overlay for navigation.
- **Bonus mode**: Additional features like gun rendering, firing animation, and advanced door mechanics (see `cub3D_bonus`).
- **Custom textures**: Supports north, south, east, and west wall textures as well as door textures.
- **Basic error handling**: Detects invalid characters, map errors, and missing player positions.

## Getting Started

### Dependencies

- **MLX42** (MiniLibX graphics library): You must have MLX42 installed. See [MLX42 documentation](https://github.com/codam-coding-college/MLX42) for installation.
- **libft**: Custom C utility library (included as a submodule or in `libft` directory).
- **C compiler**: GCC or Clang recommended.

### Build

Clone the repository and build the project using `make`:

```bash
git clone https://github.com/zbakkas/cub3D.git
cd cub3D
make           # builds the mandatory part
make bonus     # builds the bonus part with extra features
```

### Usage

Run the game with a map file:

```bash
./cub3D path/to/map.cub          # for standard mode
./cub3D_bonus path/to/map.cub    # for bonus mode
```

Map files must follow the `.cub` format specifying textures, colors, and the map layout.

### Example Map File

```
NO ./textures/north.xpm
SO ./textures/south.xpm
WE ./textures/west.xpm
EA ./textures/east.xpm
F 220,100,0
C 225,30,0

1111111111111111111111111
1000000000110000000000001
1011000001110000000000001
1001000000000000000000001
1111111110110000011100001
1000000000110000011100001
1000000000000000000000001
1111111111111111111111111
```

### Controls

- **W/A/S/D**: Move player
- **Arrow keys / Mouse**: Rotate view
- **Space**: Interact (open doors, fire gun in bonus mode)
- **ESC**: Quit

## Directory Structure

- `mandatory/`: Core engine source files
- `bonus/`: Bonus features and extended functionality
- `MLX/`: MLX42 graphics library (required)
- `libft/`: Utility functions
- `textures/`: Example textures

## Notable Constants

- **MOVE_SPEED**: Player movement speed
- **ROTATE_SPEED**: Rotation speed in radians
- **FOV**: Field of view (60 degrees by default)
- **WIDTH/HEIGHT**: Window dimensions

## Error Messages

- `Invalid character or you have more than one player`
- `Map not exist`
- `map is not sourounded or closed`
- `MAP is not valid`
- `ERR dont have Player`
- `DOOR :: BAD Position`

## License

This project does not include a license by default. Please add one if you intend to share or modify beyond educational use.

## Credits

Developed by [zbakkas](https://github.com/zbakkas) and contributors. MLX42 by Codam Coding College.

---

For more information, see the source code and comments in `mandatory/cub3d.h` and `bonus/cub3d_bonus.h`.

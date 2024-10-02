CC = cc
RM = rm -f
NAME = cub3D
HAEDER = cub3d.h
CFLAGS = -Wall -Wextra -Werror -Ofast -funroll-loops -O3 -march=native -g #-fsanitize=address
SRC = ./bonus/parsing/cub3d.c ./bonus/parsing/utilise2.c ./bonus/parsing/initialize.c ./bonus/parsing/utilice.c ./bonus/parsing/check_map.c \
	./bonus/parsing/handle_err.c ./bonus/parsing/utilise4.c ./bonus/parsing/utilise5.c\
	./bonus/get_next_line/get_next_line.c ./bonus/get_next_line/get_next_line_utils.c ./bonus/parsing/utilice1.c ./bonus/parsing/utilise3.c\
	./bonus/raycasting/mainn.c ./bonus/raycasting/mini_map.c ./bonus/raycasting/intersection.c ./bonus/raycasting/door.c ./bonus/raycasting/keys.c ./bonus/raycasting/wall.c\

OBJS = $(SRC:.c=.o)
LIBS = -L./libft -lft

LIB = MLX/build/libglfw3.a MLX/build/libmlx42.a  -framework Cocoa -framework OpenGL -framework IOKit

all: libft $(NAME)

libft:
	make all -C ./libft
	make bonus -C ./libft

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(LIB) $(LIBS) $(OBJS) -o $(NAME)
%.o: %.c $(HAEDER)
	$(CC) $(CFLAGS) $< -c -o $@

clean:
	@$(RM) $(OBJS)
	@make clean -C ./libft

fclean: clean
	@$(RM) $(NAME)
	@make fclean -C ./libft

re: fclean all

.PHONY: libft

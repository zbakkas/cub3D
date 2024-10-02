CC = cc
RM = rm -f
NAME = cub3D
HAEDER = cub3d.h
CFLAGS = -Wall -Wextra -Werror -Ofast -funroll-loops -O3 -march=native -g -fsanitize=address
SRC = ./parsing/cub3d.c ./parsing/utilise2.c ./parsing/initialize.c ./parsing/utilice.c ./parsing/check_map.c \
	./parsing/handle_err.c ./parsing/utilise4.c ./parsing/utilise5.c\
	./get_next_line/get_next_line.c ./get_next_line/get_next_line_utils.c ./parsing/utilice1.c ./parsing/utilise3.c\
	./raycasting/mainn.c ./raycasting/mini_map.c ./raycasting/intersection.c ./raycasting/door.c ./raycasting/keys.c ./raycasting/wall.c\

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

CC = cc
RM = rm -f
NAME = cub3D
HAEDER = cub3d.h
CFLAGS = -g -fsanitize=address
SRC = ./parsing/cub3d.c ./parsing/utilise2.c ./parsing/initialize.c ./parsing/utilice.c ./parsing/check_map.c \
	./parsing/handle_err.c \
	./get_next_line/get_next_line.c ./get_next_line/get_next_line_utils.c ./parsing/utilice1.c ./parsing/utilise3.c\
	test3.c\

OBJS = $(SRC:.c=.o)
LIBS = -L./libft -lft

LIB = MLX/build/libglfw3.a MLX/build/libmlx42.a  -framework Cocoa -framework OpenGL -framework IOKit

all: libft $(NAME)

libft:
	make all -C ./libft
	make bonus -C ./libft

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(LIB) $(LIBS) $(OBJS) -o $(NAME)
%.o: %.c
	$(CC) $(CFLAGS) $< -c -o $@

clean:
	@$(RM) $(OBJS)
	@make clean -C ./libft

fclean: clean
	@$(RM) $(NAME)
	@make fclean -C ./libft

re: fclean all

.PHONY: libft

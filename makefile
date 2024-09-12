CC =cc
NAME = cub3d
CFLAGS = -Wall -Wextra -Werror
LIB = MLX/build/libglfw3.a MLX/build/libmlx42.a  -framework Cocoa -framework OpenGL -framework IOKit

FILES = test3.c

OBJS= $(FILES:.c=.o)

all:$(NAME)

$(NAME):$(OBJS)
	$(CC) $(CFLAGS) $(LIB) -o $(NAME) $(OBJS)

clean :
	rm -f *.o
fclean : clean
	rm -f $(NAME)

re: fclean all

NAME=gnl
CFLAGS=-Wall -Werror -Wextra -g3 -fsanitize=address -Weverything -Wno-padded
INC=-I./classique
BUFFER_SIZE=32
CC=clang
SRCDIR=./classique
SRCS=get_next_line.c \
	get_next_line_utils.c \
	main.c


BUILDDIR=./build/
OBJS=$(SRCS:%.c=$(BUILDDIR)%.o)

$(NAME): $(OBJS)
	$(CC) -o $(NAME) $(CFLAGS) $(OBJS)

$(BUILDDIR):
	mkdir -pv $(BUILDDIR)

$(BUILDDIR)%.o: $(SRCDIR)/%.c $(BUILDDIR)
	$(CC) $(INC) -c $< $(CFLAGS) -D BUFFER_SIZE=$(BUFFER_SIZE) -o $@

clean:
	$(RM) -r $(BUILDDIR)

fclean: clean
	$(RM) $(NAME)

re: fclean $(NAME)
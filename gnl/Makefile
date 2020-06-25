NAME=gnl
NAME_BONUS=gnl_bonus
CFLAGS=-Wall -Werror -Wextra -g3 -fsanitize=address -Weverything -Wno-padded
INC=-I./classique
BUFFER_SIZE=32
CC=clang
SRCDIR=./classique
SRCS=get_next_line.c \
	get_next_line_utils.c \
	main_mwa.c
SRCDIR_BONUS=./bonus
SRCS_BONUS=get_next_line_bonus.c \
	get_next_line_utils_bonus.c \
	main_mwa.c


BUILDDIR=./build/
OBJS=$(SRCS:%.c=$(BUILDDIR)%.o)
OBJS_BONUS=$(SRCS_BONUS:%.c=$(BUILDDIR)%.o)

$(NAME_BONUS): $(OBJS_BONUS)
	$(CC) -o $(NAME_BONUS) $(CFLAGS) $(OBJS_BONUS)

$(NAME): $(OBJS)
	$(CC) -o $(NAME) $(CFLAGS) $(OBJS)

$(BUILDDIR):
	mkdir -pv $(BUILDDIR)

$(BUILDDIR)%.o: $(SRCDIR)/%.c $(BUILDDIR)
	$(CC) $(INC) -c $< $(CFLAGS) -D BUFFER_SIZE=$(BUFFER_SIZE) -o $@

$(BUILDDIR)%.o: $(SRCDIR_BONUS)/%.c $(BUILDDIR)
	$(CC) $(INC) -c $< $(CFLAGS) -D BUFFER_SIZE=$(BUFFER_SIZE) -o $@

clean:
	$(RM) -r $(BUILDDIR)

fclean: clean
	$(RM) $(NAME)

re: fclean $(NAME)
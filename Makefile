###############################################################################

# Basics #######################################################################
NAME	:=	nm
OPT		:=	
CC		:=	gcc
CFLAGS	:=	-Wall -Wextra -Werror -Wpedantic
MMD		:=	-MMD

DEBUG_FLAG	:=	-g
FSA_FLAGS	:=	$(DEBUG_FLAG) -fsanitize=address
VAL_FLAGS	:=	--track-origins=yes --show-leak-kinds=all --track-fds=yes 		\
				--show-reachable=no --leak-check=full

# Libraries ####################################################################
LIBFT_DIR		:=	libft

LIB_INCL		:=	-L $(LIBFT_DIR) -lft
LIBFT_A			:=	$(addprefix $(LIBFT_DIR)/,libft.a)

LIBS			:= $(LIBFT_A)

# Includes #####################################################################
INCL_DIR	:=	include libft/include
INCL_CMD	:=	$(addprefix -I,$(INCL_DIR))

INCL_FILES	:=	nm.h

INCLS		:=	$(addprefix include/,$(INCL_FILES))

# Directories ##################################################################
SRC_DIR	:=	src

#VPATH specifies a list of directories that 'make' should search
VPATH	:=	$(SRC_DIR)

# Srcs file names ##############################################################
SRC_FILES	:=	nm.c result_list.c

#list of all .c files
C_FILES	:=	$(SRC_FILES)

# Complete path of each .c files ###############################################
SRC_PATH			:=	$(addprefix $(SRC_DIR)/,$(SRC_FILES))

#list of all "path/.c"
SRCS	:=	$(SRC_PATH)

#Object ########################################################################
OBJ_DIR		:=	objs
OBJ_FILES	:=	$(C_FILES:.c=.o)
DEPS		:=	$(OBJ_FILES:.o=.d)
OBJS		:=	$(addprefix $(OBJ_DIR)/,$(OBJ_FILES))


DEPENDENCIES	:= $(addprefix $(OBJ_DIR)/,$(DEPS))


# Rules ########################################################################

.PHONY: all
all: $(LIBFT_A) $(OBJ_DIR) $(NAME)
	@echo
	@./nm nm

$(LIBFT_A): FORCE
	@make -C $(LIBFT_DIR)

.PHONY: fsa
fsa: $(SRCS) $(LIBS) $(INCLS)
	@$(CC) $(CFLAGS) $(FSA_FLAGS) $(INCL_CMD) $(LIB_INCL) $(SRCS) $(LIBS) -o $(NAME)

.PHONY: val
val: $(SRCS) $(LIBS) $(INCLS)
	$(CC) $(DEBUG_FLAG) $(INCL_CMD) $(LIB_INCL) $(SRCS) -o $(NAME)
	valgrind $(VAL_FLAGS) $(OPT) ./$(NAME)

$(NAME): $(OBJS) libft/libft.a
	$(CC) $(CFLAGS) $(INCL_CMD) $^ -o $@ $(LIB_INCL)

-include $(DEPENDENCIES)
$(OBJ_DIR)/%.o: %.c Makefile | $(OBJ_DIR)
	@echo Compiling $@
	@$(CC) $(CFLAGS) $(MMD) $(INCL_CMD) -o $@ -c $<

$(OBJ_DIR):
	@mkdir $(OBJ_DIR)

.PHONY: test
test:
	@make fsa
	@python3 tests/run.py

#print-%  : ; @echo $* = $($*)

.PHONY: clean
clean: 
	$(MAKE) clean -C libft
	$(RM) -rf $(OBJ_DIR)

.PHONY: fclean
fclean: clean
	$(MAKE) fclean -C libft
	$(RM) $(NAME)
	$(RM) -r $(NAME).dSYM

FORCE:

.PHONY: re
re: fclean all

.PHONY: d
d: all
	@$(OPT) ./$(NAME)

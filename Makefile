# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lboiteux <lboiteux@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/15 13:00:22 by lboiteux          #+#    #+#              #
#    Updated: 2024/03/26 15:22:21 by lboiteux         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# **************************************************************************** #
# 								NAME PROJECT								   #
# **************************************************************************** #

NAME				=	pipex

# **************************************************************************** #
# 								  COMMANDS									   #
# **************************************************************************** #

CC					=	@clang
CFLAGS				=	-Wall -Wextra -Werror -g3
MK					=	@mkdir -p
CPT_BONUS			=	$(shell ls ./bonus_file -lR | grep -F .c | wc -l)
FILE_MANDATORY		=	$(shell ls ./mandatory_file -lR | grep -F .c | wc -l)
FILE_BONUS			=	$(shell echo "$(CPT_BONUS) + 1" | bc)
PROJ_CMP_BONUS		=	1
PROJ_CMP_MANDATORY	=	1
IFLAGS				=	 -I ./includes -I ./libft/includes
RM					= 	@rm -rf
BONUS				=	pipex_bonus

LIBFT_NAME			= 	./libft/libft.a
LIBFT_PATH			=	./libft --no-print-directory

# **************************************************************************** #
#                                   COLORS                                     #
# **************************************************************************** #
COLOR_1	= \033[1;38;5;28m
COLOR_2	= \033[1;38;5;120m
COLOR_3	= \033[1;38;5;240m
COLOR_4	= \033[1;38;5;255m
COLOR_5	= \033[1;38;5;248m
RESET	= \033[0m

# **************************************************************************** #
# 								   SOURCES									   #
# **************************************************************************** #

#SOURCES
SRCS_1	=	mandatory_file/main.c \
			
SRCS_2	=	mandatory_file/init.c \


#BONUSES SOURCES
SRCS_BONUSES = 	bonus_file/main_bonus.c \
					bonus_file/pipex_bonus.c\
					bonus_file/utils_bonus.c\

# OBJECTS

SRCS_OBJS_1  =  $(SRCS_2) $(SRCS_1)
SRCS_OBJS_2  = $(SRCS_BONUSES) $(SRCS_2)
OBJS_DIR_1	:=	.objs_mandatory
OBJS_DIR_2	:=	.objs_bonus
OBJS_1		:=	$(addprefix $(OBJS_DIR_1)/, $(SRCS_OBJS_1:%.c=%.o))
OBJS_2		:=	$(addprefix $(OBJS_DIR_2)/, $(SRCS_OBJS_2:%.c=%.o))

# **************************************************************************** #
# 								     RULES									   #
# **************************************************************************** #

$(OBJS_DIR_1)/%.o: %.c
	$(MK) $(@D)
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@
	@if [ "$(PROJ_CMP_MANDATORY)" -ne "$(FILE_MANDATORY)" ]; then \
		printf " $(COLOR_1) [$(RESET)$(PROJ_CMP_MANDATORY)$(COLOR_1)/$(RESET)$(FILE_MANDATORY)$(COLOR_1)]\t$(RESET)$<                     \r"; \
	else \
		printf " $(COLOR_1) [$(RESET)$(PROJ_CMP_MANDATORY)$(COLOR_1)/$(RESET)$(FILE_MANDATORY)$(COLOR_1)]\t$(RESET)$<                 \n\n"; \
	fi
	@$(eval PROJ_CMP_MANDATORY=$(shell echo $$(($(PROJ_CMP_MANDATORY)+1))))

$(OBJS_DIR_2)/%.o: %.c
	$(MK) $(@D)
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@
	@if [ "$(PROJ_CMP_BONUS)" -ne "$(FILE_BONUS)" ]; then \
		printf " $(COLOR_1) [$(RESET)$(PROJ_CMP_BONUS)$(COLOR_1)/$(RESET)$(FILE_BONUS)$(COLOR_1)]\t$(RESET)$<                     \r"; \
	else \
		printf " $(COLOR_1) [$(RESET)$(PROJ_CMP_BONUS)$(COLOR_1)/$(RESET)$(FILE_BONUS)$(COLOR_1)]\t$(RESET)$<                 \n\n"; \
	fi
	@$(eval PROJ_CMP_BONUS=$(shell echo $$(($(PROJ_CMP_BONUS)+1))))

all: $(LIBFT_NAME) $(NAME)

$(LIBFT_NAME):
		@make -C $(LIBFT_PATH) -j

$(NAME): $(OBJS_1) $(LIBFT_NAME)
	$(CC) $(CFLAGS) $(IFLAGS) $(OBJS_1) $(LIBFT_NAME) -o $(NAME)
	@printf "  ✅ $(COLOR_2)$(NAME) successfully compiled$(RESET)\n"
	@printf "  🔄 $(COLOR_3)$(NAME) is ready to run$(RESET) \n\n"
	
$(BONUS): $(OBJS_2) $(LIBFT_NAME)
	$(CC) $(CFLAGS) $(IFLAGS) $(OBJS_2) $(LIBFT_NAME) -o $(BONUS)
	@printf "  ✅ $(COLOR_2)$(BONUS) successfully compiled$(RESET)\n"
	@printf "  🔄 $(COLOR_3)$(BONUS) is ready to run$(RESET) \n\n"

bonus: $(LIBFT_NAME) $(BONUS)


clean:
		@make clean -C $(LIBFT_PATH)
		@printf "  👾 \033[1;4;38;5;240m$(NAME)$(RESET)   \n  $(COLOR_3)└──> 🗑️    \033[1;38;5;255m.o \033[1;38;5;248mhave been deleted$(RESET)\n"	
		$(RM) $(OBJS_DIR_1) $(OBJS_DIR_2)
	
fclean:
		@make fclean -C $(LIBFT_PATH)
		$(RM) $(OBJS_DIR_1) $(OBJS_DIR_2)
		@printf "  👾 \033[1;4;38;5;240m$(NAME)$(RESET)   \n  $(COLOR_3)└──> 🗑️    \033[1;38;5;255m.o \033[1;38;5;248mhave been deleted$(RESET)\n"	
		$(RM) $(NAME) $(BONUS)
		@printf "  $(COLOR_3)└──> 🗑️    \033[1;38;5;255m$(NAME) binary \033[1;38;5;248mhas been deleted$(RESET)\n\n"

re:	fclean all

.PHONY:	re fclean all clean bonus
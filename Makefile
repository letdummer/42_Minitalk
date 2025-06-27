# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ldummer- <ldummer-@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/28 19:49:32 by ldummer-          #+#    #+#              #
#    Updated: 2025/06/27 18:24:30 by ldummer-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SHELL := bash

USER			= letdummer
NAME_SERVER		= server
NAME_CLIENT		= client
UNAME 			= $(shell uname)

#------------------------------------------------------------------------------#
#					FILES  	     			       #
#------------------------------------------------------------------------------#

SRC_DIR		= src
BUILD_DIR	= .build

SRC_SERVER	= $(addprefix $(SRC_DIR)/, server.c )
SRC_CLIENT	= $(addprefix $(SRC_DIR)/, client.c )

# adicionar atalho para compilar utils.c

OBJS_SERVER	= $(SRC_SERVER:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
OBJS_CLIENT	= $(SRC_CLIENT:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
DEPS		= $(OBJS:.o=.d)

LIBFT_DIR	= libft
LIBFT_LIB	= $(LIBFT_DIR)/libft.a


#------------------------------------------------------------------------------#
#				COMPILATION 	   			       #
#------------------------------------------------------------------------------#

CC			= cc
CFLAGS		= -Wall -Wextra -Werror
DFLAGS		= -g
INC			= -I
RM			= rm -rf
AR			= ar rcs
MKDIR_P		= mkdir -p
MAKE		= make -C

# INPUT = 
#------------------------------------------------------------------------------#
#				BASE					       #
#------------------------------------------------------------------------------#

all: $(BUILD_DIR) deps $(NAME_SERVER) $(NAME_CLIENT)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

$(NAME_SERVER): $(LIBFT_LIB) $(OBJS_SERVER)
	@printf "[$(YELLOW)Compiling minitalk Server$(RESET)]"
	@cc $(CFLAGS) $(OBJS_SERVER) $(LIBFT_LIB) -o $(NAME_SERVER)
	@printf "[Compiling $(PURPLE)server:$(RESET)] $(_SUCCESS)"
	@printf "\n"

$(NAME_CLIENT): $(LIBFT_LIB) $(OBJS_CLIENT)
	@printf "[$(YELLOW)Compiling minitalk Client$(RESET)]"
	@cc $(CFLAGS) $(OBJS_CLIENT) $(LIBFT_LIB) -o $(NAME_CLIENT)
	@printf "[Compiling $(PURPLE)client:$(RESET)] $(_SUCCESS)"
	@printf "\n"

$(LIBFT_LIB):
	@$(MAKE) $(LIBFT_DIR) extra --silent

deps: get_libft
	@if [ -d "$(LIBFT_DIR)" ]; then \
		$(MAKE) $(LIBFT_DIR) all bonus extra --silent > /dev/null; \
		echo "All deps installed!"; \
	else \
		echo "Error: Libft directory not found after attempting to download"; \
		exit 1; \
	fi

get_libft:
	@if [ ! -d "$(LIBFT_DIR)" ]; then \
		echo "Getting Libft"; \
		git clone --depth 1 https://github.com/letdummer/42_libft.git $(LIBFT_DIR); \
		echo "Done downloading Libft"; \
	else \
		echo "Libft directory already exists."; \
	fi


#------------------------------------------------------------------------------#
#				CLEAN-UP RULES 		  		       #
#------------------------------------------------------------------------------#

clean:
	@echo "Cleaning Libft objects"
	@$(MAKE) $(LIBFT_DIR) clean --quiet
	@echo "Removing $(BUILD_DIR) folder & files"
	@$(RM) $(BUILD_DIR)
	@echo "Removing Server pid file"
	@$(RM) server.pid
	@$(MAKE) $(LIBFT_DIR) clean --quiet
#	@make -C $(FT_PRINTF_DIR) clean

# clean the .o objects, the objs folder and the project file
fclean: clean
	@$(RM) $(NAME_SERVER) $(NAME_CLIENT)
	@$(MAKE) $(LIBFT_DIR) fclean --quiet
	@echo "----------------------------------"
	@printf "$(GREEN_BOLD)FULL CLEANING DONE! ✅$(RESET)"
	@printf "\n"

re: fclean all
	@printf "$(GREEN_BOLD)All files have been recompiled ✅$(RESET)"


#------------------------------------------------------------------------------#
#			COLORS AND WARNINGS		 	 	       #
#------------------------------------------------------------------------------#
# font name for titles: ANSI REGULAR
# https://patorjk.com/software/taag/#p=display&f=ANSI%20Regular&t=push_swap

# or https://www.asciiart.eu/text-to-ascii-art
# Alligator width 80

#________	DEFINING ANSI COLORS___________________________________________#

RED_BOLD	  := $(shell echo "\033[1;31m")
GREEN_BOLD	:= $(shell echo "\033[1;32m")
PURPLE  := $(shell echo "\033[0;35m")
BLUEE	 := $(shell echo "\033[0;34m")
CYAN	:= $(shell echo "\e[0;36m")
CYAN_BOLD	:= $(shell echo "\e[1;36m")
YELLOW	:= $(shell echo "\033[0;33m")
RESET	:= $(shell echo "\033[0m")

#________	FUNCTIONS TO PRINT COLORS______________________________________#

text = @printf "$(PURPLE)$(1)$(RESET)"
warn = @printf "$(BLUE)$(1)$(RESET)"
error = @printf "$(RED_BOLD)$(1)$(RESET)"
success = @printf "$(GREEN_BOLD)$(1)$(RESET)"
highligth = @printf "$(CYAN)$(1)$(RESET)"
highligth_bold = @printf "$(CYAN_BOLD)$(1)$(RESET)"


### Message Vars
_SUCCESS 		= [$(GREEN_BOLD)SUCCESS$(RESET)]


#______________________________________________________________________________#
.PHONY: all clean fclean re help manual norm valgrind gdb deps get_libft
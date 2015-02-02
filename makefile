BASE_DIR= ${HOME}/develop/server

DEBUG_LIB_DIR = $(BASE_DIR)/lib/Debug
RELEASE_LIB_DIR = $(BASE_DIR)/lib/Release

INC = -I$(NETEVENT_INCLUDE_DIR)

CC := gcc

ifeq ($(mode),d)
	LIB_DIR = $(DEBUG_LIB_DIR)
	CPPFLAGS= -g -fPIC -Wall -DDEBUG $(INC) -Wno-invalid-offsetof
	LDFLAGS = -g -fPIC -L$(LIB_DIR)
	DEBUG_TARGET = $(LIB_DIR)/libnetevent_cwrapper.a
	TARGET	= $(DEBUG_TARGET)
else
	LIB_DIR = $(RELEASE_LIB_DIR)
	CPPFLAGS= -fPIC -Wall  $(INC) -Wno-invalid-offsetof
	LDFLAGS = -fPIC -L$(LIB_DIR)
	RELEASE_TARGET = $(LIB_DIR)/libnetevent_cwrapper.a
	TARGET	= $(RELEASE_TARGET)
endif

OBJ_DIR	= ./.objs

SRC = $(wildcard *.c)
OBJS = $(addprefix $(OBJ_DIR)/,$(subst .c,.o,$(SRC)))

all : $(TARGET)

$(TARGET) : $(OBJS)
	$(AR) $(ARFLAGS) $@ $?

$(OBJ_DIR)/%.o : %.c
	$(CC) $(CPPFLAGS) -c $< -o $@

clean: 
	rm -f $(OBJS) $(TARGET)

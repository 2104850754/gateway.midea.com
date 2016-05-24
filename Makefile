#CFLAGS += -DUSE_WECHAT

# Globle  
CUR_DIR = $(shell pwd)
SRC_DIR = $(CUR_DIR)/src
OBJDIR = $(CUR_DIR)/obj
TARGET = $(OBJDIR)/gateway_ota

SOURCE_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS += $(addprefix $(OBJDIR)/,$(notdir $(SOURCE_FILES:%.c=%.o)))
$(OBJDIR)/%.o:$(SRC_DIR)/%.c
	$(CC)  -Wall -g -c $< -o $@


all: build

build: $(OBJECTS)
	$(CC)  $(OBJECTS) -o $(TARGET) -lc -lm -ldl $(LDFLAGS) $(INCLUDES)


clean:
	-rm -f $(OBJECTS)
	-rm -f $(TARGET)


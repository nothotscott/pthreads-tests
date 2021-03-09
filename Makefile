include Makefile.include
### Flags ###
CFLAGS		:= -Wall
LINK_FLAGS	:= -pthread

### Main ###
.PHONY:	all
all:	setup $(PROJECT_NAME)

.PHONY:	setup
setup:	
		@mkdir -p $(BIN_DIR)

.PHONY:	clean
clean:
ifneq ($(BIN_DIR),)
		$(if $(wildcard $(BIN_DIR)),cd $(BIN_DIR) && rm -rf *.o *.so *.elf,)
endif

.PHONY:	run
run:	
		./$(PROJECT_BIN)



### Targets ###
SOURCES	:= $(wildcard $(SRC_DIR)/*.c)
TARGETS	:= $(addprefix $(BIN_DIR)/, $(addsuffix .o, $(subst .,_, $(notdir $(SOURCES)))))

.PHONY:	$(PROJECT_NAME)
$(PROJECT_NAME):	$(PROJECT_BIN)
$(PROJECT_BIN):		$(TARGETS)
					$(LINKER) $(LINK_FLAGS) $(TARGETS) -o $@

### Rules ###
$(BIN_DIR)/%_c.o:	$(SRC_DIR)/%.c
					$(CC) $(CFLAGS) -c $< -o $@

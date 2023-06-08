CC = g++
CFLAGS = -std=c++17 -Wall -pedantic

SRC_DIR = src
LIB_DIR = $(SRC_DIR)/libraries
BIN_DIR = stefam17
OBJ_DIR = $(BIN_DIR)
DOC_DIR = doc
EXAMPLE_DIR = examples/normal

SRCS = $(wildcard $(SRC_DIR)/*.cpp $(SRC_DIR)/*/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

EXECUTABLE = $(BIN_DIR)/Semestralka_PA2

INC_DIRS = -I$(SRC_DIR) -I$(LIB_DIR)

.PHONY: all compile run clean doc copy_files

all: clean compile copy_files

compile: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(INC_DIRS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)  # Create the directory for each object file
	$(CC) $(CFLAGS) $(INC_DIRS) -c $< -o $@

run: compile
	@cd $(BIN_DIR) && ./Semestralka_PA2

clean:
	@rm -rf $(BIN_DIR)
	@rm -rf $(DOC_DIR)
	@find . -name "*.o" -type f -delete

doc:
	@mkdir -p $(DOC_DIR)
	@doxygen Doxyfile

copy_files:
	@cp -r $(EXAMPLE_DIR)/* $(BIN_DIR)

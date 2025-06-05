SRC_DIR = src
BUILD_DIR = build

CFLAGS = -Wall -Wextra -std=c99

OBJECTS = $(BUILD_DIR)/main.o \
          $(BUILD_DIR)/menu.o \
          $(BUILD_DIR)/quiz.o

EXECUTABLE = $(BUILD_DIR)/stani_bogat

all: $(BUILD_DIR) $(EXECUTABLE)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $(OBJECTS)

$(BUILD_DIR)/main.o: $(SRC_DIR)/main.c
	$(CC) $(CFLAGS) -I$(SRC_DIR) -c $< -o $@

$(BUILD_DIR)/menu.o: $(SRC_DIR)/menu/menu.c
	$(CC) $(CFLAGS) -I$(SRC_DIR) -c $< -o $@

$(BUILD_DIR)/quiz.o: $(SRC_DIR)/quiz/quiz.c
	$(CC) $(CFLAGS) -I$(SRC_DIR) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
CXX       := -g++
CXXFLAGS  := -pedantic-errors -Wall -Wextra
LDFLAGS   := -lSDL2
BUILD     := ./build
OBJ_DIR   := $(BUILD)/objects
APP_DIR   := $(BUILD)/apps
TARGET    := accel

OBJECTS   :=            \
	$(wildcard src/*.cpp)

all: build $(APP_DIR)/$(TARGET)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $@ -c $<

$(APP_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(LDFLAGS) -o $(APP_DIR)/$(TARGET) $(OBJECTS)

.PHONY: all build clean debug release

build:
	@mkdir -p $(APP_DIR)
	@mkdir -p $(OBJ_DIR)

debug: CXXFLAGS += -DDEBUG -g
debug: all

release: CXX := -aarch64-unknown-linux-musl-g++
release: CXXFLAGS += -O2
release: all
release:
	-@patchelf --set-interpreter /lib/ld-musl-aarch64.so.1 $(APP_DIR)/$(TARGET)

clean:
	-@rm -rvf $(OBJ_DIR)/*
	-@rm -rvf $(APP_DIR)/*

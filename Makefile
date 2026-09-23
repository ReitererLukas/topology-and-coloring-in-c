CXX = g++
CADICAL_DIR = ../cadical
CADICAL_BUILD_DIR = $(CADICAL_DIR)/build
CADICAL_LIB = $(CADICAL_BUILD_DIR)/libcadical.a
CPPFLAGS = -Isrc -I$(CADICAL_DIR)/src
LDLIBS = -L$(CADICAL_BUILD_DIR) -lcadical
CXXFLAGS = -Wall -Wextra -Wpedantic -Wshadow -Wconversion \
           -Woverloaded-virtual -Wnon-virtual-dtor -Wdelete-non-virtual-dtor \
           -Wformat=2 -Winit-self -Wunused-variable -Wunused-function \
           -std=c++17 -O2
SRCDIR = src
OBJDIR = build
BINDIR = .
TESTDIR = tests
TESTBIN = $(BINDIR)/testrunner

SOURCES = $(shell find $(SRCDIR) -type f -name '*.cpp' | sort)
OBJECTS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SOURCES))
TARGET = $(BINDIR)/topology

# Test sources (exclude main.cpp from linking)
TEST_SOURCES = $(filter-out $(SRCDIR)/main.cpp, $(SOURCES))
TEST_OBJECTS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(TEST_SOURCES))
TEST_OBJECTS += $(OBJDIR)/test_runner_main.o

.PHONY: all clean run test

%:
	@:

all: $(TARGET)

$(CADICAL_LIB):
	@if [ ! -f "$(CADICAL_DIR)/configure" ]; then \
		echo "CaDiCaL checkout not found at $(CADICAL_DIR)"; \
		exit 1; \
	fi
	@if [ ! -f "$(CADICAL_BUILD_DIR)/makefile" ]; then \
		cd "$(CADICAL_DIR)" && ./configure; \
	fi
	$(MAKE) -C "$(CADICAL_DIR)" cadical

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS) $(CADICAL_LIB)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(OBJECTS) $(LDLIBS) -o $(TARGET)

run: $(TARGET)
	./$(TARGET) $(filter-out $@,$(MAKECMDGOALS))

test: $(TESTBIN)
	./$(TESTBIN)

$(TESTBIN): $(TEST_OBJECTS) $(CADICAL_LIB)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(TEST_OBJECTS) $(LDLIBS) -o $(TESTBIN)

$(OBJDIR)/test_runner_main.o: $(TESTDIR)/test_runner.cpp | $(OBJDIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(TARGET) $(TESTBIN)

.PHONY: help
help:
	@echo "Available targets:"
	@echo "  all   - Build the program (default)"
	@echo "  run   - Build and run the program"
	@echo "  test  - Build and run tests"
	@echo "  clean - Remove built files"
	@echo "  help  - Show this help message"

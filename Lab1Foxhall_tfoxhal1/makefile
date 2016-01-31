CC := gcc
# CC := clang --analyze # and comment out the linker last line for sanity
TARGET := lab1

SRCEXT := c
SOURCES := $(shell find . -type f -name "*.$(SRCEXT)")
OBJECTS := $(patsubst %.c,%.o,$(SOURCES))
CFLAGS := -g -Wall -std=c99 -Wextra -pedantic

$(TARGET): $(OBJECTS)
	@echo " Linking..."
	@echo " $(CC) $^ -o $(TARGET)"; $(CC) $^ -o $(TARGET)

%.o: %.$(SRCEXT)
	@echo " $(CC) $(CFLAGS) -c -o $@ $<"; $(CC) $(CFLAGS) -c -o $@ $<

clean:
	@echo " Cleaning...";
	@echo " $(RM) *.o $(TARGET)"; $(RM) *.o $(TARGET)

.PHONY: clean

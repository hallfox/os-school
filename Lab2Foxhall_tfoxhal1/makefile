CC := gcc
# CC := clang --analyze # and comment out the linker last line for sanity
TARGET := lab2
DISTNAME := Lab2Foxhall_tfoxhal1

SRCEXT := c
HEADEREXT := h
SOURCES := $(shell find . -type f -name "*.$(SRCEXT)")
OBJECTS := $(patsubst %.c,%.o,$(SOURCES))
HEADERS := $(shell find . -type f -name "*.$(HEADEREXT)")
CFLAGS := -g -Wall -std=c99 -Wextra -pedantic

$(TARGET): $(OBJECTS)
	@echo " Linking..."
	@echo " $(CC) $^ -o $(TARGET)"; $(CC) $^ -o $(TARGET)

%.o: %.$(SRCEXT)
	@echo " $(CC) $(CFLAGS) -c -o $@ $<"; $(CC) $(CFLAGS) -c -o $@ $<

clean:
	@echo " Cleaning...";
	@echo " $(RM) *.o $(TARGET)"; $(RM) *.o $(TARGET)

dist:
	@echo " Taring source files...";
	@echo " tar czf $(DISTNAME).tgz $(SOURCES) $(HEADERS) README makefile"; tar czf $(DISTNAME).tgz $(SOURCES) $(HEADERS) README makefile

.PHONY: clean

CC := gcc
# CC := clang --analyze # and comment out the linker last line for sanity
SRCDIR := .
BUILDDIR := .
TARGET := lab1

SRCEXT := c
SOURCES := $(shell find $(SRCDIR) -type f -name "*.$(SRCEXT)")
OBJECTS := $(patsubst %.c,%.o,$(SOURCES))
CFLAGS := -g -Wall -Wextra -pedantic
LIB :=  # plus any other libs IF I HAD THEM
INC :=

$(TARGET): $(OBJECTS)
	@echo " Linking..."
	@echo " $(CC) $^ -o $(TARGET) $(LIB)"; $(CC) $^ -o $(TARGET) $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo " Cleaning..."; 
	@echo " $(RM) *.o $(TARGET)"; $(RM) *.o $(TARGET)

.PHONY: clean

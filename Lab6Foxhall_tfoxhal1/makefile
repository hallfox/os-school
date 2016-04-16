CC := gcc
TARGET := lab6

SRCEXT := c
HEADEREXT := h
SOURCES := $(shell find . -type f -name "*.$(SRCEXT)")
OBJECTS := $(patsubst %.c,%.o,$(SOURCES))
HEADERS := $(shell find . -type f -name "*.$(HEADEREXT)")
CFLAGS := -g -Wall -std=c99 -Wextra -pedantic -pthread #-DNDEBUG
LIBS :=

$(TARGET): $(OBJECTS)
	@echo " Linking..."
	@echo " $(CC) $^ -o $(TARGET) $(LIBS)"; $(CC) $^ -o $(TARGET) $(LIBS)

%.o: %.$(SRCEXT)
	@echo " $(CC) $(CFLAGS) -c -o $@ $<"; $(CC) $(CFLAGS) -c -o $@ $<

clean:
	@echo " Cleaning...";
	@echo " $(RM) *.o $(TARGET)"; $(RM) *.o $(TARGET)

.PHONY: clean

CC := gcc
TARGET := lab3

SRCEXT := c
HEADEREXT := h
SOURCES := $(shell find . -type f -name "*.$(SRCEXT)")
OBJECTS := $(patsubst %.c,%.o,$(SOURCES))
HEADERS := $(shell find . -type f -name "*.$(HEADEREXT)")
CFLAGS := -g -Wall -std=c99 -Wextra -pedantic
LIBS := -lrt

$(TARGET): $(OBJECTS)
	@echo " Linking..."
	@echo " $(CC) $^ -o $(TARGET)"; $(CC) $^ -o $(TARGET)

%.o: %.$(SRCEXT)
	@echo " $(CC) $(CFLAGS) $(LIBS) -c -o $@ $<"; $(CC) $(CFLAGS) $(LIBS) -c -o $@ $<

clean:
	@echo " Cleaning...";
	@echo " $(RM) *.o $(TARGET)"; $(RM) *.o $(TARGET)

.PHONY: clean

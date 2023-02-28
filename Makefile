FLAGS = -std=c++20

SDIR = src
HDIR = include
ODIR = obj
BDIR = bin

BUILD = $(BDIR)/exec
CC = g++
SRCS = $(wildcard $(SDIR)/*.cpp)
OBJS = $(patsubst $(SDIR)/%.cpp,$(ODIR)/%.o,$(SRCS))


$(shell mkdir -p $(ODIR) $(BDIR))


$(BUILD): $(OBJS)
	$(CC) $^ -o $@
	@echo "[COMPILED]"

-include $(ODIR)/*.d

$(ODIR)/%.o: $(SDIR)/%.cpp $(HDIR)/%.hpp
	$(CC) $(FLAGS) -c -I$(HDIR) -MMD -o $@ $<


clean:
	rm -rf $(ODIR) $(BDIR)


.PHONY: clean

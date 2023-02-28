FLAGS = -std=c++20

SDIR = src
HDIR = include
ODIR = obj
BDIR = bin

BUILD = $(BDIR)/exec
CC = @g++
SRCS = $(filter-out $(SDIR)/main.cpp,$(wildcard $(SDIR)/*.cpp))
OBJS = $(patsubst $(SDIR)/%.cpp,$(ODIR)/%.o,$(SRCS))


$(shell mkdir -p $(ODIR) $(BDIR))


$(BUILD): $(SDIR)/main.cpp $(OBJS)
	@echo -n "Compiling $@..."
	$(CC) $^ -o $@ && echo " OK" || echo " FAIL"

-include $(ODIR)/*.d

$(ODIR)/%.o: $(SDIR)/%.cpp $(HDIR)/%.hpp
	@echo -n "Compiling $@..."
	$(CC) $(FLAGS) -c -I$(HDIR) -MMD -o $@ $< && echo " OK" || echo " FAIL"


clean:
	rm -rf $(ODIR) $(BDIR)


.PHONY: clean

FLAGS = -std=c++20

SDIR = src
HDIR = include
ODIR = obj
BDIR = bin
SUBDIRS = net client server queries

BUILD = $(BDIR)/exec
CC = @g++
SRCS = $(filter-out $(SDIR)/main.cpp,$(wildcard $(SDIR)/*.cpp) $(wildcard $(foreach SUBDIR,$(SUBDIRS),$(SDIR)/$(SUBDIR)/*.cpp)))
OBJS = $(patsubst $(SDIR)/%.cpp,$(ODIR)/%.o,$(SRCS))


$(BUILD): $(SDIR)/main.cpp $(OBJS)
	@mkdir -p $(dir $@)
	@echo -n "Compiling $@... "
	$(CC) $(FLAGS) -I$(HDIR) $^ -o $@ && echo "OK" || echo "FAIL"

-include $(ODIR)/*.d

$(ODIR)/%.o: $(SDIR)/%.cpp $(HDIR)/%.hpp
	@mkdir -p $(dir $@)
	@echo -n "Compiling $@... "
	$(CC) $(FLAGS) -c -I$(HDIR) -MMD -o $@ $< && echo "OK" || echo "FAIL"


clean:
	rm -rf $(ODIR) $(BDIR)


.PHONY: clean

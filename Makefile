FLAGS = -std=c++2a 
LIBFLAGS = -lcryptopp
SDIR = src     
HDIR = include
LIBS = -I/usr/include/cryptopp -I $(HDIR)
ODIR = obj
BDIR = bin
SUBDIRS = net queries

CC = @g++
CSRCS = $(filter-out $(wildcard $(SDIR)/client/main.cpp),$(wildcard $(SDIR)/*.cpp) $(wildcard $(foreach SUBDIR,$(SUBDIRS) client,$(SDIR)/$(SUBDIR)/*.cpp)))
COBJS = $(patsubst $(SDIR)/%.cpp,$(ODIR)/%.o,$(CSRCS))
SSRCS = $(filter-out $(wildcard $(SDIR)/server/main.cpp),$(wildcard $(SDIR)/*.cpp) $(wildcard $(foreach SUBDIR,$(SUBDIRS) server,$(SDIR)/$(SUBDIR)/*.cpp)))
SOBJS = $(patsubst $(SDIR)/%.cpp,$(ODIR)/%.o,$(SSRCS))


all: $(BDIR)/client $(BDIR)/server

$(BDIR)/client: $(SDIR)/client/main.cpp $(COBJS)
	@mkdir -p $(dir $@)
	@echo -n "Linking $@... "
	$(CC) $(FLAGS) $(LIBS) $^ -o $@ $(LIBFLAGS) && echo "OK" || echo "FAIL"

$(BDIR)/server: $(SDIR)/server/main.cpp $(SOBJS)
	@mkdir -p $(dir $@)
	@echo -n "Linking $@... "
	$(CC) $(FLAGS) $(LIBS) $^ -o $@ $(LIBFLAGS) && echo "OK" || echo "FAIL"


-include $(ODIR)/*.d

$(ODIR)/%.o: $(SDIR)/%.cpp $(HDIR)/%.hpp
	@mkdir -p $(dir $@)
	@echo -n "Compiling $@... "
	$(CC) $(FLAGS) -c $(LIBS) -MMD -o $@ $< $(LIBFLAGS) && echo "OK" || echo "FAIL"


clean:
	rm -rf $(ODIR) $(BDIR)


.PHONY: all clean

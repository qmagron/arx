FLAGS = -std=c++2a
LIBFLAGS = -lcryptopp
SDIR = src
HDIR = include
LIBS = -I/usr/include/cryptopp -I $(HDIR)
ODIR = obj
BDIR = bin
SUBDIRS = net queries

CC = @g++
CPSRCS = $(filter-out $(wildcard $(SDIR)/client-proxy/main.cpp),$(wildcard $(SDIR)/*.cpp) $(wildcard $(foreach SUBDIR,$(SUBDIRS) client-proxy,$(SDIR)/$(SUBDIR)/*.cpp)))
CPOBJS = $(patsubst $(SDIR)/%.cpp,$(ODIR)/%.o,$(CPSRCS))
SPSRCS = $(filter-out $(wildcard $(SDIR)/server-proxy/main.cpp),$(wildcard $(SDIR)/*.cpp) $(wildcard $(foreach SUBDIR,$(SUBDIRS) server-proxy,$(SDIR)/$(SUBDIR)/*.cpp)))
SPOBJS = $(patsubst $(SDIR)/%.cpp,$(ODIR)/%.o,$(SPSRCS))


all: $(BDIR)/client-proxy $(BDIR)/server-proxy

$(BDIR)/client-proxy: $(SDIR)/client-proxy/main.cpp $(CPOBJS)
	@mkdir -p $(dir $@)
	@echo -n "Linking $@... "
	$(CC) $(FLAGS) $(LIBS) $^ -o $@ $(LIBFLAGS) && echo "OK" || echo "FAIL"

$(BDIR)/server-proxy: $(SDIR)/server-proxy/main.cpp $(SPOBJS)
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

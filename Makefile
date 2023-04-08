include .env


FLAGS = -std=c++2a -g -O2 -lcryptopp -lpthread -lboost_regex -DK=64

SDIR = src
HDIR = include
LIBS = -I/usr/include/cryptopp -I$(HDIR)
ODIR = obj
BDIR = bin
SUBDIRS = net queries crypto


CC = @g++
CSRCS = $(filter-out $(wildcard $(SDIR)/client/main.cpp),$(wildcard $(SDIR)/*.cpp) $(wildcard $(foreach SUBDIR,$(SUBDIRS) client,$(SDIR)/$(SUBDIR)/*.cpp)))
COBJS = $(patsubst $(SDIR)/%.cpp,$(ODIR)/%.o,$(CSRCS))
CPSRCS = $(filter-out $(wildcard $(SDIR)/client-proxy/main.cpp),$(wildcard $(SDIR)/*.cpp) $(wildcard $(foreach SUBDIR,$(SUBDIRS) client-proxy,$(SDIR)/$(SUBDIR)/*.cpp)))
CPOBJS = $(patsubst $(SDIR)/%.cpp,$(ODIR)/%.o,$(CPSRCS))
SPSRCS = $(filter-out $(wildcard $(SDIR)/server-proxy/main.cpp),$(wildcard $(SDIR)/*.cpp) $(wildcard $(foreach SUBDIR,$(SUBDIRS) server-proxy server-proxy/indexes,$(SDIR)/$(SUBDIR)/*.cpp)))
SPOBJS = $(patsubst $(SDIR)/%.cpp,$(ODIR)/%.o,$(SPSRCS))


all: setup $(BDIR)/client $(BDIR)/client-proxy $(BDIR)/server-proxy

setup:
ifdef __CRYPTOPP_BYTE__
FLAGS+= -D__CRYPTOPP_BYTE__
endif

$(BDIR)/client: $(SDIR)/client/main.cpp $(COBJS)
	@mkdir -p $(dir $@)
	@echo -n "Linking $@... "
	$(CC) $(LIBS) $^ -o $@ $(FLAGS) && echo "OK" || echo "FAIL"

$(BDIR)/client-proxy: $(SDIR)/client-proxy/main.cpp $(CPOBJS)
	@mkdir -p $(dir $@)
	@echo -n "Linking $@... "
	$(CC) $(LIBS) $^ -o $@ $(FLAGS) && echo "OK" || echo "FAIL"

$(BDIR)/server-proxy: $(SDIR)/server-proxy/main.cpp $(SPOBJS)
	@mkdir -p $(dir $@)
	@echo -n "Linking $@... "
	$(CC) $(LIBS) $^ -o $@ $(FLAGS) && echo "OK" || echo "FAIL"


-include $(ODIR)/*.d

$(ODIR)/%.o: $(SDIR)/%.cpp $(HDIR)/%.hpp
	@mkdir -p $(dir $@)
	@echo -n "Compiling $@... "
	$(CC) $(FLAGS) -c $(LIBS) -MMD -o $@ $< $(LIBFLAGS)  && echo "OK" || echo "FAIL"


clean:
	rm -rf $(ODIR) $(BDIR)


.PHONY: all setup clean

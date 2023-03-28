include .env


FLAGS = -std=c++2a -g -O2 -lcryptopp -lpthread -lboost_regex

SDIR = src
HDIR = include
LIBS = -I/usr/include/cryptopp -I$(HDIR) -I$(LDIR)/tinygarble/include
ODIR = obj
BDIR = bin
SUBDIRS = net queries
LDIR = lib


CC = @g++
CSRCS = $(filter-out $(wildcard $(SDIR)/client/main.cpp),$(wildcard $(SDIR)/*.cpp) $(wildcard $(foreach SUBDIR,$(SUBDIRS) client,$(SDIR)/$(SUBDIR)/*.cpp)))
COBJS = $(patsubst $(SDIR)/%.cpp,$(ODIR)/%.o,$(CSRCS))
CPSRCS = $(filter-out $(wildcard $(SDIR)/client-proxy/main.cpp),$(wildcard $(SDIR)/*.cpp) $(wildcard $(foreach SUBDIR,$(SUBDIRS) client-proxy,$(SDIR)/$(SUBDIR)/*.cpp)))
CPOBJS = $(patsubst $(SDIR)/%.cpp,$(ODIR)/%.o,$(CPSRCS))
SPSRCS = $(filter-out $(wildcard $(SDIR)/server-proxy/main.cpp),$(wildcard $(SDIR)/*.cpp) $(wildcard $(foreach SUBDIR,$(SUBDIRS) server-proxy,$(SDIR)/$(SUBDIR)/*.cpp)))
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

$(BDIR)/server-proxy: $(SDIR)/server-proxy/main.cpp $(SPOBJS) $(LDIR)/tinygarble/lib/libemp-tool.so
	@mkdir -p $(dir $@)
	@echo -n "Linking $@... "
	$(CC) $(LIBS) $^ -o $@ $(FLAGS) -lemp-tool -lssl -lcrypto -lboost_program_options -L$(LDIR)/tinygarble/lib && echo "OK" || echo "FAIL"

$(LDIR)/tinygarble/lib/libemp-tool.so:
	@./deps.sh install


-include $(ODIR)/*.d

$(ODIR)/%.o: $(SDIR)/%.cpp $(HDIR)/%.hpp
	@mkdir -p $(dir $@)
	@echo -n "Compiling $@... "
	$(CC) $(FLAGS) -c $(LIBS) -MMD -o $@ $< $(LIBFLAGS)  && echo "OK" || echo "FAIL"


clean:
	@./deps.sh uninstall
	rm -rf $(ODIR) $(BDIR)


.PHONY: all setup clean

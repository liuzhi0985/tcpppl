# Created by Samvel Khalatyan on Nov 17, 2013
# Copyright (c) 2013 Samvel Khalatyan. All rights reserved
#
# Use of this source code is governed by a MIT-style license that can be
# found in the LICENSE file.
#
# Set CPPFLAGS environment variable for debugging:
#
# 	export CPPFLAGS=-gdwarf-2

CXX = clang++

# Subsystems that have compilable libraries
#
subsys = 
#lib = lib/lib_tcpl.so.0.0
lib =

# Get list of all heads, sources and objects. Each source (%.cc) whould have
# an object file except programs listed in PROGS
#
heads  = $(wildcard ./interface/*.h)
templates = $(wildcard ./interface/*.tcc)
srcs = $(wildcard ./src/*.cc)
tests = $(foreach test,$(wildcard ./test/*.cpp),$(addprefix ./bin/test_,$(patsubst ./test/%.cpp,%,${test})))
progs = $(foreach prog,$(wildcard ./src/*.cpp),$(addprefix ./bin/tcpl_,$(patsubst ./src/%.cpp,%,${prog})))

objs = $(foreach obj,${srcs},$(addprefix ./obj/,$(patsubst %.cc,%.o,$(notdir ${obj}))))


CPPFLAGS += ${debug} -fPIC -pipe -std=c++11 -Wc++11-extensions -Wall -I.
#LDFLAGS += 

# Rules to be always executed: empty ones
#
.PHONY: prog

lib: ${lib}

all: test

obj: ${objs}

test: ${tests}

prog: ${progs}



# Regular compilcation
#
${objs}: obj/%.o: src/%.cc interface/%.h
	@echo "[+] Compiling objects $@ ..."
	${CXX} ${CPPFLAGS} -c $(addprefix ./src/,$(patsubst %.o,%.cc,$(notdir $@))) -o $@
	@echo



# Libraries
#
${lib}: ${objs}
	@echo "[+] Generating Library $@ ..."
	$(eval lib_name=$(notdir $@))
	${CXX} -shared -W1,-soname,${lib_name} ${LDFLAGS} -o $@ ${objs}
	@cd ./lib; ln -fs ${lib_name} $(basename ${lib_name}); ln -fs $(basename ${lib_name}) $(basename $(basename ${lib_name}))
	@echo



# Tests
#
${tests}: bin/test_%: test/%.cpp ${lib} ${templates}
	@echo "[+] Compiling test $@ ..."
	$(eval test_name=$(patsubst bin/test_%,%,$@))
	${CXX} ${CPPFLAGS} -c test/${test_name}.cpp -o ./obj/test_${test_name}.o
	${CXX} ${LDFLAGS} ${lib} ./obj/test_${test_name}.o -o $@
	@echo



# Executables
#
${progs}: bin/tcpl_%: src/%.cpp ${lib}
	@echo "[+] Compiling programs $@ ..."
	$(eval prog_name=$(patsubst bin/tcpl_%,%,$@))
	${CXX} ${CPPFLAGS} -c src/${prog_name}.cpp -o ./obj/tcpl_${prog_name}.o
	${CXX} ${LDFLAGS} ${lib} ./obj/tcpl_${prog_name}.o -o $@
	@echo



# Cleaning
#
cleanbin:
ifneq ($(strip ${tests}),)
	rm -f ./bin/test_*
endif

clean: cleanbin
	rm -f ./obj/*.o
	rm -f ./lib/*

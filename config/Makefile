TESTS_OK = $(wildcard tst/sapo*.ok.txt)
RES_OK   = $(patsubst tst/%.txt, tst/%.tst, $(TESTS_OK))

TESTS_KO = $(wildcard tst/sapo*.err.txt)
RES_KO = $(patsubst tst/%.txt, tst/%.tst, $(TESTS_KO))

TMP_FILE = /tmp/sapo.tmp

all:
	gcc -Wall -Wextra -std=c99 src/sapotache.c -o bin/sapotache

test: all $(RES_OK) $(RES_KO)
	@echo "" # For beauty

%.ok.tst: %.ok.txt
	@./bin/sapotache $< > $(TMP_FILE) && \
	if [ -z "$$(diff $(patsubst tst/%.txt,tst/%.exp, $<) $(TMP_FILE) )" ]; \
	then echo -n "."; else echo -n "[$<]"; fi

%.err.tst: %.err.txt
	@if ! ( ./bin/sapotache $< || false ) >/dev/null 2>&1; then echo -n "."; else echo -n "[$<]"; fi

clean:
	@rm -f *~ bin/sapotache

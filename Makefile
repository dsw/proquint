# This file is part of proquint.  See License.txt for copyright and
# terms of use.

all:
.SUFFIXES:

.PHONY: clean
clean: clean-docs clean-code

# **** documentation

DOCS :=
DOCS += proquint-proposal.txt

DOCS_HTML := $(DOCS:.txt=.gen.html)

.PHONY: all
all: $(DOCS_HTML)

.PHONY: clean-docs
clean-docs:
	rm -f $(DOCS_HTML)

.PRECIOUS: $(DOCS_HTML)
$(DOCS_HTML): %.gen.html: %.txt
	asciidoc - < $^ > $@

# **** code

all: proquint Proquint.class ProquintMain.class

.PHONY: clean-code
clean-code: clean-out
	rm -f *.o proquint *.class

.PHONY: clean-out
clean-out:
	rm -f check_out.txt

proquint.o proquint_main.o: %.o: %.c
	gcc -ansi -Wall -c -o $@ $<

proquint: proquint.o proquint_main.o
	gcc -ansi -Wall -o $@ $^

%.class: %.java
	javac $<

.PHONY: check-dquad-hex
check-dquad-hex:
	./dquad-hex 127.0.0.1 63.84.220.193 >> check_out.txt
	./dquad-hex x7f000001 x3f54dcc1 >> check_out.txt
	./dquad-hex 127.0.0.1 63.84.220.193 | xargs ./dquad-hex >> check_out.txt

.PHONY: check-proquint
check-proquint:
	./dquad-hex 127.0.0.1 63.84.220.193 255.255.255.255 | xargs ./proquint >> check_out.txt
	./proquint lusab-babad gutih-tugad zuzuz-zuzuz | xargs ./dquad-hex >> check_out.txt

.PHONY: check-proquint-java
check-proquint-java:
	./dquad-hex 127.0.0.1 63.84.220.193 255.255.255.255 | xargs java ProquintMain >> check_out.txt
	java ProquintMain lusab-babad gutih-tugad zuzuz-zuzuz | xargs ./dquad-hex >> check_out.txt

.PHONY: check
check: clean-out check-dquad-hex check-proquint check-proquint-java
	diff -u check_cor.txt check_out.txt

COMPUTER_ORG_IP := 63.84.220.193
COMPUTER_ORG_PQ := gutih-tugad
.PHONY: demo
demo:
	@echo
	@echo "The dotted-quad IP of computer.org is $(COMPUTER_ORG_IP)."
	@echo "Convert it to a proquint as follows."
	./dquad-hex $(COMPUTER_ORG_IP) | xargs ./proquint
	@echo
	@echo "Convert it back to a dotted-quad as follows."
	./proquint $(COMPUTER_ORG_PQ) | xargs ./dquad-hex

.PHONY: examples
examples:
	./quad-examples

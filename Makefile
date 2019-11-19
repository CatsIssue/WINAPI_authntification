include ../Makefile.mingw

CFLAGS=-lwldap32 -g
PROG=set_option

$(PROG).exe: $(PROG).c
	$(CC) $< -o $@ $(CFLAGS)

clean: 
	rm -f $(PROG).exe

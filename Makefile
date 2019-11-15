include ../Makefile.mingw

CFLAGS=-lwldap32
PROG=set_option

$(PROG).exe: $(PROG).c
	$(CC) $< -o $@ $(CFLAGS)

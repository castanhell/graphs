CFLAGS  = -std=c99 \
	  -pipe \
	  -ggdb3 -Wstrict-overflow=5 -fstack-protector-all \
          -W -Wall -Wextra \
	  -Wbad-function-cast \
	  -Wcast-align \
	  -Wcast-qual \
	  -Wconversion \
	  -Wfloat-equal \
	  -Wformat-y2k \
	  -Winit-self \
	  -Winline \
	  -Winvalid-pch \
	  -Wmissing-declarations \
	  -Wmissing-field-initializers \
	  -Wmissing-format-attribute \
	  -Wmissing-include-dirs \
	  -Wmissing-noreturn \
	  -Wmissing-prototypes \
	  -Wnested-externs \
	  -Wnormalized=nfc \
	  -Wold-style-definition \
	  -Woverlength-strings \
	  -Wpacked \
	  -Wpadded \
	  -Wpointer-arith \
	  -Wredundant-decls \
	  -Wshadow \
	  -Wsign-compare \
	  -Wstack-protector \
	  -Wstrict-aliasing=2 \
	  -Wstrict-prototypes \
	  -Wundef \
	  -Wunsafe-loop-optimizations \
	  -Wvolatile-register-var \
	  -Wwrite-strings

#------------------------------------------------------------------------------
.PHONY : all clean

#------------------------------------------------------------------------------
all : exemplo teste

exemplo : exemplo.c
	$(CC) $(CFLAGS) \
	      -Wno-cast-qual \
              -Wno-padded \
              -Wno-undef \
              -o $@ \
              $^ \
	      -l cgraph

teste : teste.o grafo.o
	$(CC) $(CFLAGS) -o $@ $^ -l cgraph

run : teste
        sh teste < sample

#------------------------------------------------------------------------------
clean :
	$(RM) exemplo teste *.o

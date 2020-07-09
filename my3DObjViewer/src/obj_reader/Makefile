
VPATH = ./ ../include ../include/share

YFLAGS = --debug
CPPFLAGS = -g -I./ -I../include -I../include/share
SOURCES = main.c obj_reader_func.c obj_reader_flex.c obj_reader_bison.tab.c libgen_share.c stdio_share.c string_share.c

%.ln: %.y
%.c: %.y
.y.c:


obj_reader: $(subst .c,.o,$(SOURCES))
	$(CC) -D DEBUG -g -o $@ $^

obj_reader_bison.tab.h: obj_reader_bison.y
	bison $(YFLAGS) -d $< 

main.o: main.c obj_reader.h
obj_reader_func.o: obj_reader_func.c obj_reader.h
obj_reader_flex.o: obj_reader_flex.c
obj_reader_bison.tab.o: obj_reader_bison.tab.c

obj_reader_flex.c: obj_reader_flex.l
obj_reader_bison.tab.c: obj_reader_bison.y

stdio_share.o: stdio_share.c stdio_share.h
string_share.o: string_share.c string_share.h
libgen_share.o: libgen_share.c libgen_share.h


include $(subst .c,.d,$(SOURCES))

%.d: %.c
	$(CC) -M $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$


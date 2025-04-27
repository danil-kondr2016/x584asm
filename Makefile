SUFFIXES+=.dep
NODEPS:=clean
SOURCES:=$(shell find src/ -name "*.c")
OBJECTS:=$(patsubst src/%.c,obj/%.o,$(SOURCES))
DEPFILES:=$(patsubst %.c,%.dep,$(SOURCES))

CFLAGS=-Wall
CPPFLAGS=-Iinclude/ -DUTF8PROC_STATIC

x584asm: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJECTS)

src/%.dep: src/%.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -MM -MT '$(patsubst src/%.c,obj/%.o,$<)' $< -MF $@

obj/%.o: src/%.c
	mkdir -p $(dir $@)
	echo $(SOURCES)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ -c $<

ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NODEPS))))
#Chances are, these files don't exist.  GMake will create them and
#clean up automatically afterwards
    -include $(DEPFILES)
endif



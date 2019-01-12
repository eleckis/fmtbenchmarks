
SOURCES = jsonpars.c
TARGET  = libjsonpars.a
OBJECTS = $(SOURCES:%.c=%.o)
CFLAGS	= -I../ubftab -I ../include -g
LIBS = -latmiclt -latmi -lubf -lnstd -lpthread -lrt -ldl -lm -ljansson

default: target

target: $(TARGET)

#$(TARGET): $(OBJECTS)
#	$(CC) -o $@ $(LDFLAGS) $^ $(LIBS)
#    ar -rcs libmylib.a mylib.o

%.o: %.c
	$(CC) $(CFLAGS) -c $<  -o $*.o
	ar -rcs $(TARGET) $*.o
	rm $*.o

$(TARGET): $(OBJECTS)

clean:
	-rm *.o
	-rm $(TARGET)


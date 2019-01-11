
SOURCES = userregsv_ubf.c
TARGET  = userregsv_ubf
OBJECTS = $(SOURCES:%.c=%.o)
CFLAGS	= -I../ubftab -I/home/eleckis/endurox/dist/include
LIBS = -latmisrvinteg -latmi -lubf -lnstd -lpthread -lrt -ldl -lm

default: target

target: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $(LDFLAGS) $^ $(LIBS)

%.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	-rm *.o
	-rm $(TARGET)



SOURCES = userregclt_ubf.c
TARGET  = userregclt_ubf
OBJECTS = $(SOURCES:%.c=%.o)
CFLAGS	= -I../ubftab -I/home/eleckis/endurox/dist/include -L../xmlpars
LIBS = -lxmlpars -latmiclt -latmi -lubf -lnstd -lpthread -lrt -ldl -lm 

default: target

target: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(LDFLAGS) $^ $(LIBS)

%.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	-rm *.o
	-rm $(TARGET)


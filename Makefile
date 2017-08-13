INCDIR = -I.
DBG    = -g
OPT    = -O3
CPP    = g++
CFLAGS = $(DBG) $(OPT) $(INCDIR)
LINK   = -lm 

.cpp.o:
	$(CPP) $(CFLAGS) -c $< -o $@

all: segment

segment: jpeg-compressor/jpgd.h jpeg-compressor/jpge.h lodepng.h segment.cpp segment-image.h segment-graph.h disjoint-set.h
	$(CPP) $(CFLAGS) -o segment segment.cpp jpeg-compressor/jpgd.cpp jpeg-compressor/jpge.cpp lodepng.cpp $(LINK)

clean:
	/bin/rm -f segment *.o

clean-all: clean
	/bin/rm -f *~ 




libcommon=libcommon.a
libFCFS=libFCFS.a
# hstruct=struct.h
# cread=read_proc.c
# hread=read_proc.h
libSTCF=libSTCF.a
cmain=main.cpp
hcommon=common.h
cround=roundRobin.cpp
cSTCF=shortestremainingtimefirst.cpp
hSTCF=STCF.h
hFCFS=FCFS.h
cFCFS=FCFS.cpp
CC=g++
CFLAG= -c -o


all: cround  cSTCF cFCFS


cround:hcommon
	$(CC) $(cround) $(CFLAG) $(libcommon)

hcommon:
	$(CC) $(hcommon)

main: all
	$(CC) main.cpp -L. -lcommon -lSTCF -lFCFS -o main

hSTCF:cround
	$(CC) $(hSTCF) 
cSTCF:hSTCF
	$(CC) $(cSTCF) $(CFLAG) $(libSTCF)
hFCFS:cround
	$(CC) $(hFCFS) 
cFCFS:hFCFS
	$(CC) $(cFCFS) $(CFLAG) $(libFCFS)

clean:
	rm *.a;rm *.gch;rm *.o; rm ab;rm *~

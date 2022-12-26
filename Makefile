libcommon=libcommon.a
libFCFS=libFCFS.a
libMLFQ=libMLFQ.a
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
cMLFQ=MLFQ.cpp
CC=g++
CFLAG= -c -o


all: cround  cSTCF cFCFS cMLFQ


cround:hcommon
	$(CC) $(cround) $(CFLAG) $(libcommon)
cMLFQ:cround
	$(CC) $(cMLFQ) $(CFLAG) $(libMLFQ)
hcommon:
	$(CC) $(hcommon)

main: all
	$(CC) main.cpp -L. -lcommon -lSTCF -lFCFS -lMLFQ -o main

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

CC = gcc
EXE = TelnetServer.exe
$(EXE): TelnetServer.o server.o fileutil.o
	$(CC) -o $(EXE) TelnetServer.o server.o fileutil.o -lwsock32
TelnetServer.o: TelnetServer.c server.h
	$(CC) -c TelnetServer.c
server.o: server.c server.h
	$(CC) -c server.c
fileutil.o: fileutil.c fileutil.h
	$(CC) -c fileutil.c
clean:
	del *.o

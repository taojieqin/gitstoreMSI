selectsrc:
	gcc -c sckutil.c
	ar -r sckutil.a sckutil.o
	gcc selectsrc.c sckutil.a -o main

	gcc -c commsocket.c
	ar -r commsocket.a commsocket.o
	gcc selectclient2.c commsocket.a -o client
clean:
	rm -f *.o

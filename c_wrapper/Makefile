OUT = libserver.so libserver_c_connector.so c_server

all: $(OUT)

libserver.so:
	g++ -fpic -shared server.cpp -o libserver.so
libserver_c_connector.so:
	g++ -fpic -shared server_c_connector.cpp -L. -lserver -o libserver_c_connector.so
c_server:
	gcc main.c -L. -lserver_c_connector -o c_server
clean:
	rm -f $(OUT)


CC =g++
AR= ar
RANLIB= ranlib
#LIB = -L. -lxml
LIB = -L. -lisci

all:isci server client init

#xml: testxml.o xml.o tinyxml.o tinyxmlparser.o tinystr.o tinyxmlerror.o
#	$(CC) -o testxml  testxml.o xml.o tinyxml.o tinyxmlparser.o tinystr.o tinyxmlerror.o
#xml: xml.o tinyxml.o  tinyxmlparser.o tinyxmlerror.o tinystr.o 
#	$(AR) rc libxml.a  xml.o tinyxml.o tinyxmlparser.o tinyxmlerror.o tinystr.o
#	$(RANLIB) libxml.a
#fileopr:isci testfileopr.o
#	$(CC) -o test  testfileopr.o $(LIB)
	
isci: xml.o tinyxml.o  tinyxmlparser.o tinyxmlerror.o tinystr.o  fileopr.o
	$(AR) rc libisci.a  xml.o tinyxml.o tinyxmlparser.o tinyxmlerror.o tinystr.o fileopr.o
	$(RANLIB) libisci.a
server:isci server.o
	$(CC) -o server server.o $(LIB)
client:client.o
	$(CC) -o client client.o $(LIB)
init:init.o
	$(CC) -o init init.o $(LIB)

#test:isci testfileopr.o
#	$(CC) -o test testfileopr.o $(LIB)
clean:
	rm -rf *.o
	rm -rf server client init
	rm -rf libisci.a

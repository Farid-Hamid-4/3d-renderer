##variables for both targest 
CC=gcc
##variables for target a1
##PYTHON_PATH = -L/usr/lib/python3.9/config-3.9m-x86_64-linux-gnu
PYTHON_PATH = /usr/local/Cellar/python3.10/3.10.2/bin/python3.10-config
##LIB_PATH = -L/usr/lib 
LIB_PATH = /usr/local/Cellar
LIBRARY_LINKS = -lpython3.10 -lcrypt -lpthread -ldl -lutil -lm

##variables for target a1.o
##PYTHON_INCLUDE = -I/usr/include/python3.5m -I/usr/include/python3.9
PYTHON_INCLUDE = /usr/local/Cellar/python@3.10/3.10.2/Frameworks/Python.framework/Versions/3.10/include/python3.10
PYTHON_DEBUG = -fdebug-prefix-map=/build/python3.9-3.9.12=. -specs=/usr/share/dpkg/no-pie-compile.specs


a1: a1.o
	${CC} a1.o -o a1 ${PYTHON_PATH} ${LIB_PATH} ${LIBRARY_LINKS} -framework -Xlinker -export-dynamic -Wl,-O1 -Wl,-Bsymbolic-functions


a1.o: a1.c 
	${CC}  a1.c -o a1.o -c ${PYTHON_INCLUDE} -Wno-unused-result -Wsign-compare -g ${PYTHON_DEBUG} -fstack-protector -Wformat -Werror=format-security -DNDEBUG -g -fwrapv -O3 -Wall -fPIC

clean:
	@if [ -f a1 ]; then\
		echo "Removing a1";\
		rm a1;\
	fi

	@if [ -f a1.o ]; then\
		echo "Removing a1.o";\
		rm a1.o;\
	fi



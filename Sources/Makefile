#######################################################################
#  Ferruccio Vitale (unixo@devzero.it)
#  20/07/2010
#
#  Applied Computer Science - Urbino University "Carlo Bo"
#  Software Engineering
#  Professor Edoardo Bonta'
#  Academic Year 2009 - 2010
#######################################################################

# Use this definition to add debug information
# CFLAGS = -I/usr/local/include/mysql++ -I/usr/local/mysql/include -O0 -ggdb

# Use this under MacOS X
# CFLAGS = -I/usr/local/include/mysql++ -I/usr/local/mysql/include \
#          -O -Wall -Werror


CPP    = g++
CFLAGS = -I/usr/include/mysql++ -I/usr/include/mysql -O -Wall -Werror \
         -Wno-unused-result         
LIBS   = -L/usr/local/lib -lmysqlpp
OBJS   = Basket.o Category.o Database.o ManagedObject.o Order.o \
         Product.o User.o UserMenu.o DataModel.o Observable.o \
         CommandLine.o

.PHONY: all
all: ec++ white-box

ec++: $(OBJS) main.o
	${CPP} ${LIBS} ${OBJS} main.o -o ec++
    
white-box: $(OBJS) white-box.o
	${CPP} ${LIBS} ${OBJS} white-box.o -o white-box
    
.cpp.o:
	${CPP} ${CFLAGS} ${INCLUDES} -c $<

.PHONY: clean
clean:
	rm -f *.o core *~ ec++ white-box
    

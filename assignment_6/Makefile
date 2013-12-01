CXX = u++
CXXFLAGS = -g -Wall -Wno-unused-label -MMD ${OPT}

MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}

OBJECTS = Bank.o BottlingPlant.o ConfigParams.o MPRNG.o NameServer.o Parent.o Student.o Truck.o VendingMachine.o WATCard.o WATCardOffice.o Printer.o soda.o
DEPENDS = ${OBJECTS:.o=.d}
EXEC = soda

${EXEC} : ${OBJECTS}
	${CXX} ${CXXFLAGS} $^ -o $@

${OBJECTS} : ${MAKEFILE_NAME}

-include ${DEPENDS}

clean :
	rm -f *.d *.o ${EXEC}

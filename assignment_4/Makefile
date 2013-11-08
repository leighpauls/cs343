
CXX = u++
CXXFLAGS = -g -multi -Wall -Wno-unused-label -MMD ${OPT}

MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}

OBJECTS = q2Elf.o q2Main.o q2Printer.o q2Reindeer.o q2Santa.o q2Workshop.o q2Yielder.o
DEPENDS = ${OBJECTS:.o=.d}
EXEC = northpole

${EXEC} : ${OBJECTS}
	${CXX} ${CXXFLAGS} $^ -o $@

${OBJECTS} : ${MAKEFILE_NAME}

-include ${DEPENDS}

clean :
	rm -f *.d *.o ${EXEC}

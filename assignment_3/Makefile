
CXX = u++
CXXFLAGS = -g -multi -Wall -Wno-unused-label -MMD ${OPT} -DTYPE="${TYPE}"

MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}


OBJECTS = q1kvqueue.o q1mapper.o q1reducer.o q1sortmapper.o q1main.o
DEPENDS = ${OBJECTS:.o=.d}
EXEC = wordcount

${EXEC} : ${OBJECTS}
	${CXX} ${CXXFLAGS} $^ -o $@

${OBJECTS} : ${MAKEFILE_NAME}

-include ${DEPENDS}

clean :
	rm -f *.d *.o ${EXEC}
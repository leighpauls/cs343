TYPE:=INT

CXX = u++					# compiler
CXXFLAGS = -g -Wall -Wno-unused-label -MMD -DTABLETYPE_${TYPE}
MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}	# makefile name

OBJECTS = q2Main.o q2TableCommon.o q2TableInternal.o q2TableInternalBarge.o q2TableAuto.o q2TableTask.o q2Printer.o q2Philosopher.o
DEPENDS = ${OBJECTS:.o=.d}			# substitute ".o" with ".d"
EXEC = phil

.PHONY : clean

all : ${EXEC}					# build all executables

-include ImplType

ifeq (${IMPLTYPE},${TYPE})			# same implementation type as last time ?
${EXEC} : ${OBJECTS}
	${CXX} ${CXXFLAGS} $^ -o $@
else
ifeq (${TYPE},)					# no implementation type specified ?
# set type to previous type
TYPE=${IMPLTYPE}
${EXEC} : ${OBJECTS}
	${CXX} ${CXXFLAGS} $^ -o $@
else						# implementation type has changed
.PHONY : ${EXEC}
${EXEC} :
	rm -f ImplType
	${MAKE} clean
	touch q2table.h
	${MAKE} ${EXEC} TYPE="${TYPE}"
endif
endif

ImplType :
	echo "IMPLTYPE=${TYPE}" > ImplType
	sleep 2

${OBJECTS} : ${MAKEFILE_NAME}			# OPTIONAL : changes to this file => recompile

-include ${DEPENDS}

clean :						# remove files that can be regenerated
	rm -f *.d *.o ${EXEC} *.class ImplType

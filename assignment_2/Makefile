TYPE:=int
OPT:=

CXX = u++						# compiler
CXXFLAGS = -g -multi -Wall -Wno-unused-label -MMD ${OPT} -DTYPE="${TYPE}" # compiler flags
MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}	# makefile name

OBJECTS1 = q1Main.o q1Player.o q1Umpire.o q1Potato.o
EXEC1 = hotpotato

OBJECTS3 = q3Mergesort.o q3Main.o q3ValueGenerator.o
EXEC3 = mergesort

OBJECTS = ${OBJECTS1} ${OBJECTS3}			# all object files
DEPENDS = ${OBJECTS:.o=.d}				# substitute ".o" with ".d"
EXECS = ${EXEC1} ${EXEC3}				# all executables

#############################################################

.PHONY : all clean

all : ${EXECS}						# build all executables

${EXEC1} : ${OBJECTS1}					# link step 1st executable
	${CXX} ${CXXFLAGS} $^ -o $@

-include ImplType

ifeq (${IMPLTYPE},${TYPE})				# same implementation type as last time ?
${EXEC3} : ${OBJECTS3}
	${CXX} ${CXXFLAGS} $^ -o $@
else
ifeq (${TYPE},)						# no implementation type specified ?
# set type to previous type
TYPE=${IMPLTYPE}
${EXEC3} : ${OBJECTS3}
	${CXX} ${CXXFLAGS} $^ -o $@
else							# implementation type has changed
.PHONY : ${EXEC3}
${EXEC3} :
	rm -f ImplType
	touch q3${EXEC3}.h
	sleep 2
	${MAKE} TYPE="${TYPE}"
endif
endif

ImplType :
	echo "IMPLTYPE=${TYPE}" > ImplType

#############################################################

${OBJECTS} : ${MAKEFILE_NAME}				# OPTIONAL : changes to this file => recompile

-include ${DEPENDS}					# include *.d files containing program dependences

clean :							# remove files that can be regenerated
	rm -f *.d *.o ${EXECS} ImplType

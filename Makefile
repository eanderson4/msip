# A Makefile for Eric's Concert code to buidl multistage problems


#This is for Jeff (on opt machines) Linux 64bit
CPLEX_STUDIO=/home/eric/sub/lib/CPLEX_Studio
CPLEX_ARCH=x86-64_sles10_4.1

CPLEX_INC_DIR=${CPLEX_STUDIO}/cplex/include
CONCERT_INC_DIR=${CPLEX_STUDIO}/concert/include

CPLEX_LIB_DIR=${CPLEX_STUDIO}/cplex/lib/${CPLEX_ARCH}/static_pic
CONCERT_LIB_DIR=${CPLEX_STUDIO}/concert/lib/${CPLEX_ARCH}/static_pic


CPLEX_INCS = -I${CPLEX_INC_DIR} -I${CONCERT_INC_DIR} 
CPLEX_LIBS = -L$(CPLEX_LIB_DIR) -lilocplex -lcplex -L$(CONCERT_LIB_DIR) -lconcert -lm -pthread

CXX	= cc
CXXFLAGS := -g -m64 -Wall -Wno-deprecated -Wunused-variable -fPIC\
        -fexceptions -DIL_STD 

PROJ_DIR = /home/eric/sub/dfo/
PROJ_INC = -I${PROJ_DIR}src/

INCS = ${CPLEX_INCS} ${PROJ_INC}
LIBS = ${CPLEX_LIBS}

SRCS = 	branch.cpp\
        bus.cpp\
	grid.cpp\
	main.cpp\
	node.cpp\
	nodedata.cpp\
	problemdata.cpp\
	rootproblem.cpp\
	rootresult.cpp\
	scenarioproblem.cpp\
	scenarioresult.cpp\
	subproblem.cpp\
	tree.cpp	

OBJS = ${SRC2:.cpp=.o}
SRC2 = $(addprefix src/,${SRCS})

.cpp.o:
	g++ ${CXXFLAGS} ${INCS} -c $< -o $@

msip: ${OBJS}
	g++ ${CXXFLAGS} ${INCS} ${OBJS} ${LIBS} -o $@ 

depend:
	$(CXX) -MM ${CXXFLAGS} ${INCS} ${SRC2} > msip.d

clean:
	\rm -f ${OBJS}

include msip.d




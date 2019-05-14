
CXX=g++ -g
CXXFLAGS= -std=c++11

OBJS=computation.o ir.o irmutator.o irvisitor.o iter.o node.o schedule.o simple_codegen.o type.o lowered_func.o type.o tensor.o constantfold_pass.o applysplitresult_pass.o applyfuseresult_pass.o

all:main

main: test.o ${OBJS}
	$(CXX) ${CXXFLAGS} $^ -o $@

clean :
	rm -f *.o main


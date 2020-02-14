CC = g++
CFLAGS = -Wall -O3
OUT_FILE=BC_P76071218

DIR_SRC = ./src
DIR_OBJ = ./obj

SRC = $(wildcard ${DIR_SRC}/*.cpp)  
OBJ = $(patsubst %.cpp,${DIR_OBJ}/%.o,$(notdir ${SRC})) 


main:${OBJ}
	# $(CC) $(OBJ) -o ${OUT_FILE}
	$(CC) $(OBJ) -o BC_P76071218
${DIR_OBJ}/%.o:${DIR_SRC}/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@
.PHONY:clean
clean:
	find ${DIR_OBJ} -name *.o -exec rm -rf {} \;
	rm -f ${OUT_FILE};

test1:
	@./${OUT_FILE} benchmark/adaptec1/adaptec1.nodes benchmark/adaptec1/adaptec1.nets adaptec1.clusters adaptec1.cnets
test2:
	@./${OUT_FILE} benchmark/adaptec2/adaptec2.nodes benchmark/adaptec2/adaptec2.nets adaptec2.clusters adaptec2.cnets
test3:
	@./${OUT_FILE} benchmark/adaptec3/adaptec3.nodes benchmark/adaptec3/adaptec3.nets adaptec3.clusters adaptec3.cnets
test4:
	@./${OUT_FILE} benchmark/adaptec4/adaptec4.nodes benchmark/adaptec4/adaptec4.nets adaptec4.clusters adaptec4.cnets

test_all:
	@./${OUT_FILE} benchmark/adaptec1/adaptec1.nodes benchmark/adaptec1/adaptec1.nets adaptec1.clusters adaptec1.cnets
	@./${OUT_FILE} benchmark/adaptec2/adaptec2.nodes benchmark/adaptec2/adaptec2.nets adaptec2.clusters adaptec2.cnets
	@./${OUT_FILE} benchmark/adaptec3/adaptec3.nodes benchmark/adaptec3/adaptec3.nets adaptec3.clusters adaptec3.cnets
	@./${OUT_FILE} benchmark/adaptec4/adaptec4.nodes benchmark/adaptec4/adaptec4.nets adaptec4.clusters adaptec4.cnets


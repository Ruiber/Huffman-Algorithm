#Compiler
CC=gcc

#Flags
CC_FLAGS=-c -Wall

#.h files
H_SOURCE=$(wildcard ./include/*.h)

#Objects for encoder
ENC_OBJ=encoder.o heap.o encMain.o

#Objects for decoder
DEC_OBJ=decoder.o heap.o decMain.o

#remove
RM=rm -rf

encode: $(ENC_OBJ)
	$(CC) -o $@ $^

decode: $(DEC_OBJ)
	$(CC) -o $@ $^

%.o: ./src/%.c $(H_SOURCE)
	$(CC) -o $@ $(CC_FLAGS) $<

clean:
	$(RM) *.o encode decode I-O/arvhuf.txt I-O/saida.txt I-O/texto.txt

Compiler: Compiler.o SecondTransition.o FirstTransition.o OperationEncoder.o DataEncoder.o EntryEncoder.o ExternEncoder.o SymbolTable.o Utilities.o
	gcc -g -ansi -Wall -pedantic Compiler.o SecondTransition.o FirstTransition.o OperationEncoder.o DataEncoder.o EntryEncoder.o ExternEncoder.o SymbolTable.o Utilities.o -o Compiler

Compiler.o: Compiler.c FirstTransition.h SecondTransition.h Utilities.h Consts.h DataEncoder.h SymbolTable.h OperationEncoder.h
	gcc -c -ansi -Wall -pedantic Compiler.c -o Compiler.o

SecondTransition.o: SecondTransition.c SecondTransition.h Consts.h DataEncoder.h EntryEncoder.h ExternEncoder.h Types.h OperationEncoder.h Utilities.h SymbolTable.h
	gcc -c -ansi -Wall -pedantic SecondTransition.c -o SecondTransition.o

FirstTransition.o: FirstTransition.c FirstTransition.h Consts.h DataEncoder.h EntryEncoder.h Enums.h ExternEncoder.h Types.h OperationEncoder.h Utilities.h SymbolTable.h
	gcc -c -ansi -Wall -pedantic FirstTransition.c -o FirstTransition.o

OperationEncoder.o: OperationEncoder.c OperationEncoder.h Types.h Consts.h Utilities.h SymbolTable.h ExternEncoder.h
	gcc -c -ansi -Wall -pedantic OperationEncoder.c -o OperationEncoder.o

DataEncoder.o: DataEncoder.c DataEncoder.h Types.h Utilities.h Consts.h SymbolTable.h
	gcc -c -ansi -Wall -pedantic DataEncoder.c -o DataEncoder.o

EntryEncoder.o: EntryEncoder.c EntryEncoder.h Types.h Consts.h SymbolTable.h Utilities.h
	gcc -c -ansi -Wall -pedantic EntryEncoder.c -o EntryEncoder.o

ExternEncoder.o: ExternEncoder.c ExternEncoder.h Types.h Consts.h SymbolTable.h Utilities.h
	gcc -c -ansi -Wall -pedantic ExternEncoder.c -o ExternEncoder.o

SymbolTable.o: SymbolTable.c SymbolTable.h Types.h Consts.h Utilities.h 
	gcc -c -ansi -Wall -pedantic SymbolTable.c -o SymbolTable.o

Utilities.o: Utilities.c Utilities.h Types.h Enums.h Consts.h
	gcc -c -ansi -Wall -pedantic Utilities.c -o Utilities.o

clean:
	rm *.o

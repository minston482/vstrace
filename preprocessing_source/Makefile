main : main.o common.o preprocessing.o init_values.o memory_free.o output_result.o print_units.o set_values.o
	gcc -o ../preprocessing main.o common.o data_structure.h preprocessing.o init_values.o memory_free.o output_result.o print_units.o set_values.o

main.o : common.o preprocessing.o init_values.o memory_free.o output_result.o print_units.o set_values.o
	gcc -c main.c common.h data_structure.h preprocessing.h init_values.h memory_free.h output_result.h print_units.h set_values.h

common.o : 
	gcc -c common.c common.h

#data_structure.o : 
#	gcc -h data_structure.h

preprocessing.o : common.o data_structure.h
	gcc -c preprocessing.c preprocessing.h

init_values.o : common.o data_structure.h
	gcc -c init_values.c init_values.h

memory_free.o : common.o data_structure.h
	gcc -c memory_free.c memory_free.h

output_result.o : common.o data_structure.h
	gcc -c output_result.c output_result.h

print_units.o : common.o data_structure.h
	gcc -c print_units.c print_units.h

set_values.o : common.o data_structure.h
	gcc -c set_values.c set_values.h



clean:
	rm *.o
	rm *.h.gch

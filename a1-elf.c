//-----------------------------------------
//// NAME: Kajal Tomar 
//// STUDENT NUMBER: 7793306
//// COURSE: COMP 3430, SECTION: A01
//// INSTRUCTOR: Franklin Bristow
//// ASSIGNMENT: assignment 1, QUESTION: question 1
//// 
//// REMARKS: What is the purpose of this program?
////
////-----------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <assert.h>
#include <unistd.h>

typedef struct ELF_HEADER{
	uint8_t format; // 1 = 32-bit and 2 = 64-bit
	uint8_t endianness; // 1 = little endian, 2 = big endian // needed to read multibyte fields
	uint8_t target_OS; 
	uint16_t obj_file_type;
	uint16_t ISA;
	
	uint64_t entrypoint_address;
	uint64_t p_header_address;
	uint16_t p_header_size;
	uint16_t p_header_amount;
	
	uint16_t s_header_size;
	uint16_t s_header_amount;

	uint16_t indexof_s_header;

} elf_header;

typedef struct PROGRAM_HEADER{
	uint32_t segment_type;
	uint64_t segment_address;
	uint64_t segment_size;	
} program_header;


void read_header(elf_header * header, int handle);
void print_header(elf_header * header); 
void read_program_header(program_header * p_header, elf_header * elf_header, int handle); 
void print_program_header(program_header * header); 

int main(void){
	
	char *filename = "hello.out32";
	elf_header header; 
	program_header p_header;
	
	int elf_fd = open(filename, O_RDONLY);
	
	read_header(&header, elf_fd);
	print_header(&header);

	
	read_program_header(&p_header, &header, elf_fd);

	printf("End of process.\n");
	
	return EXIT_SUCCESS;

}

void read_header(elf_header * header, int handle){
	
	assert(header != NULL);
	assert(handle >= 0);

	lseek(handle, 4, SEEK_SET); 

	read(handle, &header->format, 1); 
	read(handle, &header->endianness, 1); 
	
	lseek(handle, 1, SEEK_CUR); 

	read(handle, &header->target_OS, 1);
	
	lseek(handle, 8, SEEK_CUR);
	
	read(handle, &header->obj_file_type, 2);
	read(handle, &header->ISA, 2);

	lseek(handle, 4, SEEK_CUR);

	if(header->format == 1){ // 32-bit
		read(handle, &header->entrypoint_address, 4);
		read(handle, &header->p_header_address, 4);
		lseek(handle, 4, SEEK_CUR);
	}
	else if(header->format == 2){ // 64-bit
		read(handle, &header->entrypoint_address, 8);
		read(handle, &header->p_header_address, 8);
		lseek(handle, 8, SEEK_CUR);
	}

	lseek(handle, 6, SEEK_CUR);
	
	read(handle, &header->p_header_size, 2);
	read(handle, &header->p_header_amount, 2);
 	read(handle, &header->s_header_size, 2);
  	read(handle, &header->s_header_amount, 2);
	read(handle, &header->indexof_s_header, 2);

}

void print_header(elf_header * header){
	assert(header != NULL);
	
	printf("ELF header:\n");
	
	if(header->format == 1){
		printf("* 32-bit\n");
	}
	else if (header->format == 2){
		printf("* 64-bit\n");
	}

	if(header->endianness == 1){
		printf("* little\n");
	}
	else if(header->endianness == 2){
		printf("* big\n");
	}
	
	
	printf("* compiled for 0x%02x (operating system).\n"
			"* has type %#04x. \n"
			"* compiled for %#04x (isa). \n",
			header->target_OS, header->obj_file_type, header->ISA);
	
	if(header->format == 1){ // 32-bit
		printf("* entry point address 0x%08lx. \n"
			"* program header table starts at 0x%08lx. \n",
			header->entrypoint_address, header->p_header_address);
	}
	else if(header->format == 2){ // 64-bit
		printf("* entry point address  0x%016lx. \n"
				"* program header table starts at 0x%016lx. \n",
				header->entrypoint_address, header->p_header_address);
	}

	printf("There are %i program headers, each is %i bytes. \n"
			"There are %i section headers, each is %i bytes. \n"
			"The section header string table is %i.\n",
			header->p_header_amount, header->p_header_size, header->s_header_amount, header->s_header_size, header->indexof_s_header);
}

void read_program_header(program_header * p_header, elf_header * elf_header, int handle){
		
	assert(elf_header != NULL);
	assert(p_header != NULL);
	assert(handle >= 0);	
		
	
	for(int i = 0; i < elf_header->p_header_amount; i++){
	
		lseek(handle, elf_header->p_header_address, SEEK_SET);
		lseek(handle, i*elf_header->p_header_size, SEEK_CUR); 

		read(handle, &p_header->segment_type, 4); 

		if(elf_header->format == 2){ // 64-bit 
			lseek(handle, 12, SEEK_CUR);
			read(handle, &p_header->segment_address, 8);
			lseek(handle, 8, SEEK_CUR);
			read(handle, &p_header->segment_size, 8);
		}
	
		if(elf_header->format == 1){ // 32-bit
			lseek(handle,4, SEEK_CUR);
			read(handle, &p_header->segment_address, 4);
			lseek(handle, 4, SEEK_CUR);
			read(handle, &p_header->segment_size, 4);
		}
			printf("\nProgram header #%i\n", i+1);
			print_program_header(p_header);
		}

	}

void print_program_header(program_header * header){
	assert(header != NULL);
	
	printf("*segment type 0x%08ix.\n"
			"*virtual address of segment 0x%08lx. \n"
			"*size in file %lu bytes.\n",
			header->segment_type, header->segment_address, header->segment_size);
}
















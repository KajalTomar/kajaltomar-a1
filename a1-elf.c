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
	uint64_t program_header_address;
	uint64_t section_header_address;

} elf_header;

void read_header(elf_header * header, int handle);
void print_header(elf_header * header); 

int main(void){
	
	char *filename = "hello.out32";
	elf_header header; 

	
	int elf_fd = open(filename, O_RDONLY);
	
	read_header(&header, elf_fd);
	print_header(&header);

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
		read(handle, &header->program_header_address, 4);
		read(handle, &header->section_header_address, 4);
	}
	else if(header->format == 2){ // 64-bit
		read(handle, &header->program_header_address, 8);
		read(handle, &header->section_header_address, 8);
	}
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
	
	
	printf("* compiled for 0x%02x (operating system)\n"
			"* has type %#04x \n"
			"* compiled for %#04x (isa) \n",
			header->target_OS, header->obj_file_type, header->ISA);
	
	if(header->format == 1){ // 32-bit
		printf("* entry point address 0x%08lx \n"
			"* program header table starts at 0x%08lx \n",
			header->program_header_address, header->section_header_address);
	}
	else if(header->format == 2){ // 64-bit
		printf("* entry point address  0x%016lx \n"
				"* program header table starts at 0x%016lx \n",
				header->program_header_address, header->section_header_address);
	}
}


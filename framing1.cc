#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

const int MAX_LEN = 17;
const char escape = 't';
const char flag = ' ';

struct header
{
   int id;
   int length;//contain a sequence of bytes
   char *start;//can hold start of frame byte sequence(DLE STX)
};

struct footer
{
   int id;
   int length;//contain a sequence of bytes
   char *end;//can hold end of frame byte sequence (DLE ETX)
};
struct msg
{
   header head;//access header struct
   footer foot;//access footer struct
   char data[MAX_LEN];//array of frame length
};

void writer(struct msg *msgs,int size, FILE *fout)
{
	/*
	packet = msgs
	sizeof(struct msg) = size
	fout = fout
	*/

	int i;
	char *dbg = (char *)malloc(sizeof(char)*msgs->head.length);//allocate enough memory to fit msgs->head.length amount
    strncpy(dbg, msgs->data, msgs->head.length);//copy the first msgs->head.length characters from msgs->data to dbg
	for (i = 0; i< msgs->head.length; ++i)//loop through header.length times
	{
		if (dbg[i] == flag) //if any character in dbg is a flag(space)
		{
		   fprintf(stdout,"%s","");//delete the space
		   fwrite((void *)&escape,sizeof(char), 1, fout);//write it to the output file
		}

		fprintf(stdout,"%c",dbg[i]);//if it is not a flag, prints out the character from the standard output stream
		fwrite((void *)&dbg[i],sizeof(char), 1, fout);//write it to the output file
		/*
		fwrite(1,2,3,4)//takes four parameters
		1: Pointer to the array of elements to be written
		2: Size in bytes of each element to be written.
		3: Number of elements, each one with a size of size bytes.
		4: Pointer to a FILE object that specifies an output stream
		Return value: The total number of elements successfully written is returned.
		*/

       }
	
        free(dbg);////Deallocate memory block, making it available for further reallocations
}

void read_file_and_frame(const char *infilename, const char *outfilename )//takes an input and output file as parameters
{
	FILE* fin;//declare a FILE pointer pointing to the input file
	FILE *fout;//declare a FILE pointer pointing to the output file

	int read = 0;
	struct msg *packet;//creates a pointer to msg, called packet which can access everything in the struct
	char str[MAX_LEN];//create a character array of the size of the frame

	fin = fopen(infilename, "r");//opens file for reading purposes
	fout = fopen(outfilename, "w+");//opens file and rewrites anything in it
	while(!feof(fin) && !ferror(fin))//while no errors and end of file hasnt been reached
	{
		packet = (msg *)malloc(sizeof(struct msg));//allocate enough memory to fit msg length amount
		/*
		fread(1,2,3,4)//takes four parameters
		1: Pointer to a block of memory,
		2: Size(in bytes) of each element to be read
		3: Number of elements, each one with a size of size bytes
		4: Pointer to a FILE object that specifies an input stream
		Return value: The total number of elements successfully read is returned
		*/
	//reads from fin, MAX_LEN characters, 1 byte at a time and stores it in the array str, and assaigns read the return value of fread
		read = fread(str,1, MAX_LEN, fin);//read contains the amount of characters read from input file
		packet->head.length = read;//makes header length equal to amount of characers read
               	packet->head.id = 8;//assaigns it an id
                strncpy(packet->data, str, read);//copy the amount of read length characters from str to packet->data
                packet->data[read] =  flag;//asaigns the last element a flag(space)
		writer(packet, sizeof(struct msg), fout);
		free(packet);//Deallocate memory block, making it available for further reallocations
	}
	fclose(fin);//close input so content in buffer can be succesfully written
	fclose(fout);//close output so content in buffer can be succesfully written
}


int main(int argc, char **argv)
{
   read_file_and_frame(argv[1], argv[2]);//call main program, which takes an input and output file
   return 0;//succesfully exit program with a return value of 0
}




#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
const int MAX_LEN = 17;
const char escape = 't';
const char flag = ' ';

struct header{
   int id;
   int length;
};

struct msg{
   header head;
   char data[MAX_LEN];//+2
};

/*Write message to the output */

void writer(struct msg *msgs,int size, FILE *fout)
{
	int i;
	char *dbg = (char *)malloc(sizeof(char)*msgs->head.length);//+2
        strncpy(dbg, msgs->data, msgs->head.length);//+2
	for (i = 0; i< msgs->head.length/*+2*/; ++i)
	{
		if (dbg[i] == escape || dbg[i] == flag)
		{
		   fprintf(stdout,"%c",escape);
		   fwrite((void *)&escape,sizeof(char), 1, fout);
		}

		fprintf(stdout,"%c",dbg[i]);
		fwrite((void *)&dbg[i],sizeof(char), 1, fout);
       }
	
        free(dbg);
}

void read_file_and_frame(const char *infilename, const char *outfilename )
{
	FILE* fin;
	FILE *fout;

	int read = 0;
	struct msg *packet;	
	char str[MAX_LEN];

	fin = fopen(infilename, "r");
	fout = fopen(outfilename, "w+");
	while(!feof(fin) && !ferror(fin))
	{
		packet = (msg *)malloc(sizeof(struct msg));
		read = fread(str,1, MAX_LEN, fin);
		packet->head.length = read;
               	packet->head.id = 8;
                strncpy(packet->data, str, read);
                packet->data[read] =  flag;
		writer(packet, sizeof(struct msg), fout);
		free(packet);
	}
	fclose(fin);
	fclose(fout);
}


int main(int argc, char **argv)
{
   read_file_and_frame(argv[1], argv[2]);
   return 0;
}




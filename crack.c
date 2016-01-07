
// Code Written by Venkata Ravi Chandra Cherukuri and Manasa Gadde as part of CSC 574 Project-1
// Reference for this code is from lecture slides and papers provided by Dr. Harfoush 



#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int sim_resol(unsigned char *, unsigned char *, int);
unsigned char out;

void main(int argc,char* argv[]){
	FILE *fd;
	unsigned char counts[256];
	unsigned char key[5];
	unsigned char tmp_line[256];
	unsigned char plain = 'C';
	int byte_index;
	size_t read;
	int i;
	int tmp;
	memset(key, 0, 5);
	
	
	for (byte_index=0; byte_index<5; byte_index++) {
		memset(counts, 0, 256);
		fd = fopen (argv[1],"r");
		read = fread (tmp_line, sizeof(unsigned char), 143, fd);
		while (read !=0 ){
			//Process read packet and update counts array 
			if ((tmp_line[24] == byte_index+3) && (tmp_line[25] == 255)){
				//printf ("candidate\n");
				out = plain^tmp_line[27];
				tmp = sim_resol(&tmp_line[24], key, byte_index);
				if ((tmp >= 0) && (tmp <=255))
				counts[tmp]++;
			}
			read = fread (tmp_line, sizeof(unsigned char), 143, fd);
		}
		//Process counts array and find key[byte_index]
		
		int max=0;
		int max_index=0;
		for (i=0; i<256; i++){
			if (counts[i] > max){
				max_index=i;
				max = counts[i];
			}
		}
		key[byte_index]=(unsigned char)max_index;
	}
	for(i=0;i<5;i++)
		printf("%c",key[i]);
	printf("\n");
	fclose (fd);
	
}




int sim_resol(unsigned char *iv, unsigned char *key, int byte_index){
	unsigned char s[256];
	unsigned char a, s_1, k,j;
	unsigned char tmp_key[8];
	int i;
	memcpy(tmp_key, iv, 3);
	memcpy(&tmp_key[3], key, 5);
	for (i=0; i<256; i++)
		s[i] = i;
	j=k=0;
	for (i=0; i<byte_index+3; i++){
		j = (unsigned char)(j+s[i]+tmp_key[k]);
		a = s[i];
		s[i] = s[j];
		s[j] = a;
		if (++k >= 8)
			k = 0;
	}
	for (i=0; i<256; i++)
		if (s[i] == out)
			s_1 = i;
	if ((s[0] != byte_index + 3) || (s[1] != 0))
		return -1;
	else
		return (s_1-j-s[byte_index+3]);
}

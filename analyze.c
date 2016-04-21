#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<math.h>

int main(){
	FILE *fd=fopen("image","r");
	struct stat buf; stat("image",&buf);
	unsigned char *image=malloc(buf.st_size);
	if(image == NULL ){
		fprintf(stderr,"Out of memory!\n");
		return 1;
	}
	printf("%c\n",image[512]);
	if(fread(image,1,buf.st_size,fd)!=buf.st_size){
		fprintf(stderr,"Error reading\n");
		return 0;
	}
	printf("read %i bytes from %s\n",buf.st_size,"image");

	int count[257];
	for(int i=0;i<256;i++) count[i]=0;
	int a;
	for(int i=0;i<buf.st_size;i++){	
	count[image[i]]++;
	}
	free(image);
//	count[image[i]]++;
	for(int i=0;i<256;i++){
		if(count[i]) {
			printf("%x = %i",i,count[i]);
			if(isprint(i)) printf(" \"%c\"",(char)i);
			printf("\n");
		}

	}
	
	return 1;
}

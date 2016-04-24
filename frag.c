/*
So our filesystem is 30 megabyte big right? So what we do is that we create 30 1mb files A1,B1,A2,B2 etc. Lets say that the As are filled with A and B file is filled with B. Now the disk is filled with files. Now let's remove half of them. Lets say we remove the B files. (We want only one character in the files then we can easily spot the different files in the filesystem with a hex editor). Next step would be to create a file which is 16 mb. Now there won't be one nice long run where this file can be stored. So if we fill this file with Cs. We would have A1, C1, A2, C1, A3, C1, etc. Not necessarily in this order, but somewhere along the lines of this.
Then we can simply spin up the hex editor and watch what happens in the filesystem. The files ought to be split into 512 byte blocks so 1Mb is a bad example. Using 512byte files for A and B would be better, this doesn't affect the size of the C file however.
*/
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>

void createfile(const char *folder,int num,int size,char c){
	char filename[16];	
	char *data;
	FILE *fd;
	
	if(num!=-1){
		sprintf(filename,"%s/tst%c_%02d",folder,c,num);
	}else{
		sprintf(filename,"%s/tst%c",folder,c);
	}
	printf("creating %s size=%i kbytes\n",filename,size/1024); 
	fd=fopen(filename,"w");
	if(!fd) {
		printf("couldn't create %f\n",filename);
		exit(1);
	}
	data=malloc(size);
	if(!data) printf("malloc failed\n");
	for(int i=0;i<size;i++) data[i]=c;
	int w=fwrite(data,1,size,fd);
	if(w!=size){
		printf("wrote %s %i bytes\n",strerror(errno),w);
		char *str;
	}
	free(data);
}

void deletefile(const char *folder,int num,int c){
	char filename[16];
	sprintf(filename,"%s/tst%c_%002d",folder,c,num);
	printf("deleting %s\n",filename);
	if(-1==unlink(filename)) printf("couldn't delete %s\n",filename);
}

#define MAX 25 

int main(int argc,char *argv[]){
	char *name;
	int size,j;
	char content;
	for(int i=1;i<argc;i++){
		switch(i){
			case 1:
				name=argv[i];
				break;
			case 2:
				size=atoi(argv[i]);
				break;
			case 3:
				content=*argv[i];
				break;
			case 4:
				j=atoi(argv[i]);
				break;
		}
	}
	printf("size=%i\n",size);
	createfile("mount",j,size,content);
	return;
	FILE *fd;
	char file[16];

// Create A B A B A B 
	for(int i=0;i<MAX;i++){

		createfile("mount",i,1024*1024,i%2?'B':'A');
	}
	printf("**** done A B A B \n");
// Delete the B:s in the hex editor this will not be visible. Since only the references to the blocks are deleted.
	int cnt=0;
	for(int i=0;i<MAX;i++){
		if( i%2) deletefile("mount",i,'B'); 
	}
	printf("df -h mount\n");
	system("df -h mount");
	printf("sync\n");
	system("sync");
	printf("df -h mount\n");
	system("df -h mount");
	system("ls -sh mount");
	system("ls -sh mount | wc -l");
	system("sleep 10");
// Create one file with the C:s this will be 10 MB in total filling the spaces left from the B:s with C. This file will not be continous => the harddrive will be fragmented
	createfile("mount",-1,4*1024*1024,'C');
	createfile("mount",-1,4*1024*1024,'D');

	printf("MAX=%i\n",MAX);
	return 0;
}

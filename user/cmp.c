#include "kernel/types.h"
#include "user/user.h"
#include "kernel/memlayout.h"
#include "kernel/stat.h"

uint64
open_file(char *file) {
	int fd = open(file, 0);
	if (fd <0){
		fprintf(2, "cannot open %s\n", file);
		exit(2);
	}
	return fd;
}

uint64
compare_files(int f1, int f2, char *name1, char *name2){
	unsigned char buf1, buf2;
	int status1, status2;
	long byte = 1;
	long line = 1;

	for(;;){
		status1 = read(f1, &buf1, 1);
		status2 = read(f2, &buf2, 1);

		if (status1 == 0 && status2 ==0){
			return 0;
		}
		if (status1 == 0 || status2 ==0){
			fprintf(1, "%s %s differ: byte %ld, line %ld\n", name1, name2, byte, line);
			return 1;
		}
		if (buf1 != buf2){
			fprintf(1, "%s %s differ: byte %ld, line %ld\n", name1, name2, byte, line);
			return 1;
		}
		if (buf1 == '\n'){
			line++;
		}
		byte++;
	}
}



uint64
main(int argc, char *argv[])
{
   if (argc != 3){
   		fprintf(2, "Need two files");
   		exit(2);
   }

   int f1 = open_file(argv[1]);
   int f2 = open_file(argv[2]);

   int result = compare_files(f1, f2, argv[1], argv[2]);

   close(f1);
   close(f2);

   exit(result);
   
}

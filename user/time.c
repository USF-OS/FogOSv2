#include "kernel/types.h"
#include "user/user.h"

void printtime(char *category, int raw, int POSIX) {

	if (raw <=0) {
		if (POSIX) {
			printf("%s\t0.00s\n", category);
		} else {
			printf("%s\t0m0.000s\n", category);
		}
	} else {
		uint8 seconds = ((raw % 1000000000) / 1000) % 60;
		// printf("SECONDS: %ld\n", seconds);
		if (POSIX) {
			uint8 p_ms = (raw % 1000) / 10;
			if (p_ms < 10) {
				printf("%s\t%d.0%ds\n", category, seconds, p_ms);
			} else {
				printf("%s\t%d.%ds\n", category, seconds, p_ms);
			}
		} else {
			uint64 minutes = ((raw % 1000000000) / 1000) / 60;
			uint16 ms = (raw % 1000);
			if (ms < 100) {
				if (ms > 9) {
					printf("%s\t%lum%d.0%ds\n", category, minutes, seconds, ms);
				} else {
					printf("%s\t%lum%d.00%ds\n", category, minutes, seconds, ms);	
				}
			} else {
				printf("%s\t%lum%d.%ds\n", category, minutes, seconds, ms);
			}
		}
	}
	
}

int main(int argc, char *argv[]) {

	// A buffer
	ctime();
	ctime();
	ctime();

	short POSIX = 0;
	short arg_start_idx = 1;
	if (argc == 1) {
		printtime("\nreal", 0, 0);
		printtime("user", 0, 0);
		printtime("sys", 0, 0);
		return 1;
	} else if (argc >= 2){
		char *flag = "-p";
		if (strcmp(flag, argv[1]) == 0) {
			POSIX = 1;
			if (argc == 2) {
				printtime("\nreal", 0, 1);
				printtime("user", 0, 1);
				printtime("sys", 0, 1);
				return 1;
			} else {
				arg_start_idx++;
			}
			
		}
	}

	timtog();
	uint64 time_before = ctime();
	int pid = fork();
	int ret = 0;
	if (pid == 0) {
		timtog();
		ret = exec(argv[arg_start_idx], &argv[arg_start_idx]);
	} else {
		uint64 utime, kerntime;
		wait2(0, &utime, &kerntime);
		uint64 time_after = ctime();
		uint64 final_time = (time_after - time_before);
		printtime("\nreal", final_time, POSIX);
		printtime("user", utime, POSIX);
		printtime("sys", kerntime, POSIX);
		// printf("%ld\n", final_time);
		// printf("%ld\n", utime);
		// printf("%ld\n", kerntime);
		ret = 1;
	}
	return ret;
}

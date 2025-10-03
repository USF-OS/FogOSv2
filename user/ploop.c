#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {

	if (argc == 1) {
		printf("Please enter a number of loops\n");
		printf("Note: 1550000000 is pushing it to around 5+ seconds\n");
		return 0;
	}
	uint64 loop_max = atoi(argv[1]);
	if (!loop_max) {
		printf("Please enter a valid number\n");
		return 0;
	}
	
	printf("Looping...\n");
	uint64 a = 0;
	for (int i = 0; i < loop_max; i++) {
		// printf("ctime: %ld\n", ctime());
		a++;
	}
	printf("Done!\n");
	return 1;
	
}

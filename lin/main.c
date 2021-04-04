#include "so_stdio.h"
#include "stdio.h"

int main(void)
{
	SO_FILE *f = NULL;
	//f = so_fopen("./test_file", "xyz");
	//f = so_fopen("./test_file", "w");
	//f = so_fopen("./test_file123", "r");
	f = so_fopen("./test_file123", "r");
	int target_fd = so_fileno(f);
	int c = so_fgetc(f);
	c = so_fgetc(f);

	// if (f != NULL) {
	// 	printf("!!!! %c\n\n", so_fgetc(f)); //H
	// 	printf("!!!! %c\n\n", so_fgetc(f)); //E 
	// 	printf("!!!! %c\n\n", so_fgetc(f)); //L
	// 	printf("!!!! %c\n\n", so_fgetc(f)); //L
	// 	printf("!!!! %c\n\n", so_fgetc(f)); //O

	// }
	so_fclose(f);
	
	return 0;
}

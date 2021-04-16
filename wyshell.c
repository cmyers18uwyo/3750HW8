#include"wyscanner.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char* argv[]) {
	char msg[4092];
	int parseLineInt = 0;
	while(1) {
		printf("\n $> ");
		fgets(msg,4092,stdin);
		parseLineInt = parse_line(msg);
		printf("%i", parseLineInt);
	}
}
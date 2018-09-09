#include <stdio.h>
#include <string.h>
#include <termios.h>


char* getpassword(const char* prompt)
{   
	struct termios oflags, nflags;
	static char password[65];

	/* disabling echo */
	tcgetattr(fileno(stdin), &oflags);
	nflags = oflags;
	nflags.c_lflag &= ~ECHO;
	nflags.c_lflag |= ECHONL;

	if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0) {
		perror("tcsetattr");
		return NULL;
	}

	printf("%s", prompt);
	fgets(password, sizeof(password), stdin);
	if(password[strlen(password) - 1] == '\n')
		password[strlen(password) - 1] = '\0';

	/* restore terminal */
	if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0) {
		perror("tcsetattr");
		return NULL;
	}

	return password;
}

#include <stdio.h>
#include <Windows.h>

#define true 1
#define false 0

int main()
{
	int isExit = false;

	char src[255];
	char flush[1];

	while (!isExit)
	{
		printf_s("==============\n1 - Set string\n2 - Show string\n3 - Get PID\n4 - Exit\n");

		char action;
		scanf_s("%c", &action, 1);
		gets(flush);

		switch (action)
		{
		case '1':
			gets(src);
			break;
		case '2':
			printf_s("%s\n", src);
			break;
		case '3':
			printf_s("%d\n", GetCurrentProcessId());
			break;
		case '4':
			isExit = true;
			break;
		default:
			break;
		}
	}
}

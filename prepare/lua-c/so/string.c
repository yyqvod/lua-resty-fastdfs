#include <stdio.h>

char *get()
{
	return "ok...";
}

int main()
{
	char *str = "ddd";
	char *get_str = get();
	printf("%s--->helloword...--->%s\n", str, get_str);
}

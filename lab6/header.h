#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <sys\types.h>
#include <sys/stat.h>
#include <stdlib.h>




// ��� �������� ���� �� ��� ������ ���� ��������

//1)
#define OPEN_ERR									\
{													\
	printf("[ERROR]������ ��� �������� �����\n");   \
	return 1;										\
}				

//2)
#define ALLOC_MEMORY_ERR								\
{														\
	printf("[ERROR]������ ��� ��������� ������\n");     \
	return 1;										    \
}				

//3)
#define WRITING_DATA_ERR								  \
{														  \
	printf("[ERROR]������ ��� ������ ������\n");		  \
	return 1;											  \
}		

//4)
#define READING_DATA_ERR								\
{													    \
	printf("[ERROR]������ ��� ������ ������\n");        \
	return 1;								            \
}	

//5)
#define CLOSING_FILE_ERR					            \
{											            \
	printf("[ERROR]������ ��� �������� �����\n");       \
	return 1;								            \
}

//6)
#define FSEEK_ERR										   \
{														    \
	printf("[ERROR]������ ��� ������ ������� fseek\n");      \
	return 1;												  \
}

//7)
#define CREATE_FILE_ERR									   \
{														    \
	printf("[ERROR]������ ��� �������� �����\n");			 \
	return 1;												  \
}
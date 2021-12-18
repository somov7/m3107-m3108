#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <sys\types.h>
#include <sys/stat.h>
#include <stdlib.h>




// это наверное треш но мне удобно было дебажить

//1)
#define OPEN_ERR									\
{													\
	printf("[ERROR]Ошибка при открытии файла\n");   \
	return 1;										\
}				

//2)
#define ALLOC_MEMORY_ERR								\
{														\
	printf("[ERROR]Ошибка при выделении памяти\n");     \
	return 1;										    \
}				

//3)
#define WRITING_DATA_ERR								  \
{														  \
	printf("[ERROR]Ошибка при записи данных\n");		  \
	return 1;											  \
}		

//4)
#define READING_DATA_ERR								\
{													    \
	printf("[ERROR]Ошибка при чтении данных\n");        \
	return 1;								            \
}	

//5)
#define CLOSING_FILE_ERR					            \
{											            \
	printf("[ERROR]Ошибка при закрытии файла\n");       \
	return 1;								            \
}

//6)
#define FSEEK_ERR										   \
{														    \
	printf("[ERROR]ОШИБКА ПРИ РАБОТЕ ФУНКЦИИ fseek\n");      \
	return 1;												  \
}

//7)
#define CREATE_FILE_ERR									   \
{														    \
	printf("[ERROR]ОШИБКА ПРИ СОЗДАНИИ ФАЙЛА\n");			 \
	return 1;												  \
}
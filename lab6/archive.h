/* Orlov Aleksandr, 12-5, M3107, 15.12.2021 */

#ifndef LAB6_ARCHIVE_H
#define LAB6_ARCHIVE_H

#define MAX_STRING_SIZE 512

int create_archive(char *argv[], int files, char *filename);
int unpack_archive(char *filename);
void file_list(char *filename);

#endif //LAB6_ARCHIVE_H

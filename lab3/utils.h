#ifndef LOGS_ANALYSIS_UTILS_H
#define LOGS_ANALYSIS_UTILS_H

long long getErrors(FILE *filePointer, char **errors);

long long toSeconds(char *str);

void printMaxRequests(FILE *filePointer, long long seconds);

#endif

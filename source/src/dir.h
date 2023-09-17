#ifndef __DIR_H__
#define __DIR_H__

#include <string.h>
#include <stdio.h>
#include <stdbool.h>

// Check if file is .mp3 file
bool is_mp3(char* file_name) {
	// Get the length of the file's name
	size_t name_size = strlen(file_name);
	char ext[5];

	//
	//	somemusic.mp3
	//	'3' is at index [strlen(str) - 1]
	//	'p' => [strlen(str) - 2]
	//	'm' => [strlen(str) - 3]
	//	'.' => [strlen(str) - 4]
	//
	
	sprintf(ext, "%c%c%c%c", file_name[name_size - 1], file_name[name_size - 2], file_name[name_size - 3], file_name[name_size - 4]);

	if (strcmp(ext, "3pm.")) return false;
	else return true;
}

#endif

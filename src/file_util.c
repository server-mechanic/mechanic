/*
 * Copyright (c) 2016,2017 the Server Mechanic Team.
 * Server Mechanic (http://server-mechanic.org)
 *
 * This file is part of Server Mechanic.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 */


#include "file_util.h"
#include "string_util.h"
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <dirent.h>
#include <libgen.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

#define FOLDER_PERMS (S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH)
#define FILE_PERMS (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)

bool is_file(const char* path) {
  struct stat buffer;   
  return (stat (path, &buffer) == 0);
}

void rmrf(const char* dirname) {

  DIR *dp;
  struct dirent *ep;
  char abs_filename[FILENAME_MAX];

  if ( (dp = opendir(dirname)) != NULL)
    {
      while ( (ep = readdir (dp)) != NULL ) {
        struct stat stFileInfo;

        snprintf(abs_filename, (size_t)FILENAME_MAX, "%s/%s", dirname, ep->d_name);

        if (lstat(abs_filename, &stFileInfo) < 0) {
          perror ( abs_filename );
	}

        if(S_ISDIR(stFileInfo.st_mode)) {
          if(strcmp(ep->d_name, ".") && strcmp(ep->d_name, "..")) {
            rmrf(abs_filename);
          }
        } else {
                  remove(abs_filename);
        }
          }
      (void) closedir (dp);
        }
  else {
	fprintf(stderr, "rmrf: Couldn't open the directory: %s\n", dirname);
    perror ("rmrf: Couldn't open the directory");
  }

  remove(dirname);
}

int create_file(const char* path) {
	int fd = creat(path, FILE_PERMS);
	return fd == -1 ? 0 : 1;
}

void get_basename(char* buf, size_t buf_capacity, const char* path) {
	size_t len = strlen(path);
	char* last_slash;

	string_util_strcpy(buf, buf_capacity, path);
	if( buf[len-1] == '/' ) {
		buf[len-1] = 0;
		--len;
	}

	last_slash = strrchr(buf, '/');
	string_util_strcpy(buf, buf_capacity, &last_slash[1]);
}

void mkdirp(const char *dir) {
	char tmp[256];
	char *p = NULL;
	size_t len;

	snprintf(tmp, sizeof(tmp),"%s",dir);
	len = strlen(tmp);
	if(tmp[len - 1] == '/') {
		tmp[len - 1] = 0;
	}

	for(p = tmp + 1; *p; p++) {
		if(*p == '/') {
			*p = 0;
			mkdir(tmp, (__mode_t)FOLDER_PERMS);
			*p = '/';
		}
	}

	mkdir(tmp, (__mode_t)FOLDER_PERMS);
}

void mkdirp2(const char *dir) {
	char tmp[256];
	size_t len;

	snprintf(tmp, sizeof(tmp),"%s",dir);
	len = strlen(tmp);
	if(tmp[len - 1] == '/') {
		tmp[len - 1] = 0;
	}

	len = (size_t)(strrchr(tmp, '/')-tmp);
	tmp[len] = 0;

	mkdirp(tmp);
}

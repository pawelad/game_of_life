#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "misc.h"

void make_dir( char *dir )
{
	struct stat st = { 0 };
	if ( stat( dir, &st ) == -1 )
		mkdir( dir, 0700 );
}

char *create_path( char *filename, char *dir )
{

	char *path = malloc( (strlen(filename) + strlen(dir) + 2) * sizeof(char) );
	strcpy( path, dir );
	strcat( path, "/" );
	strcat( path, filename );

	return path;
}

char *filename_from_path( char *path )
{
	char *ssc;
	int l = 0;
	ssc = strstr( path, "/" );
	do{
		l = strlen(ssc) + 1;
		path = &path[ strlen(path) - l + 2 ];
	 	ssc = strstr( path, "/" );
	}while(ssc);

	return path;
}

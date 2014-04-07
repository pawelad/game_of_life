#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "misc.h"

void create_dir( char *dir )
{
	struct stat st = { 0 };
	if( stat( dir, &st ) == -1 )
	{
		mkdir( dir, 0700 );
	}
}


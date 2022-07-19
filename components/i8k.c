/* See LICENSE file for copyright and license details. */
#include <stdio.h>
#include <string.h>

#include "../util.h"

#if defined(__linux__)
	struct i8k_v1
	{
		char	FMT_VERSION[4];
	    char	BIOS_VERSION[4];
	    char	MACHINE_ID[16];
	    int 	CPU_TEMP;
	    int 	LEFT_FAN_STATUS;
	    int 	RIGHT_FAN_STATUS;
   	    int 	LEFT_FAN_SPEED;
	    int 	RIGHT_FAN_SPEED;
	    int 	AC_STATUS;
	    int 	BUTTONS_STATUS;
	};

	int _read_i8k_record(struct i8k_v1 *rec)
	{
		return pscanf("/proc/i8k",
					  	"%4s %4s %16s %d %d %d %d %d %d %d",
					  	&rec->FMT_VERSION,
					  	&rec->BIOS_VERSION,
						&rec->MACHINE_ID,
						&rec->CPU_TEMP,
						&rec->LEFT_FAN_STATUS,
						&rec->RIGHT_FAN_STATUS,
						&rec->LEFT_FAN_SPEED,
						&rec->RIGHT_FAN_SPEED,
						&rec->AC_STATUS,
						&rec->BUTTONS_STATUS);
	}

	const char*	i8k_param(const char *name)
	{
		struct i8k_v1 rec;

		if (10 != _read_i8k_record(&rec))
		{
			warn("error reading i8k record.");
			return NULL;
		}

		if(0 != strcmp("1.0", rec.FMT_VERSION))
		{
			warn("i8k version %s is not supported.", rec.FMT_VERSION);
			return NULL;
		}

		if(0 == strcmp(name, "CPU_TEMP"))
			return bprintf("%d", rec.CPU_TEMP);
		else if(0 == strcmp(name, "LEFT_FAN_STATUS"))
			return bprintf("%d", rec.LEFT_FAN_STATUS);
		else if(0 == strcmp(name, "RIGHT_FAN_STATUS"))
			return bprintf("%d", rec.RIGHT_FAN_STATUS);
		else if(0 == strcmp(name, "LEFT_FAN_SPEED"))
			return bprintf("%d", rec.LEFT_FAN_SPEED);
		else if(0 == strcmp(name, "RIGHT_FAN_SPEED"))
			return bprintf("%d", rec.RIGHT_FAN_SPEED);
		else
			warn("i8k parameter %s is not supported.", name);

		return NULL;
	}
#endif

#include <stdio.h>
#include <string.h>
#include <limits.h>

#include "../util.h"

enum block_stats
{	
	read_IOs = 0,		// requests      number of read I/Os processed
	read_merges,		// requests      number of read I/Os merged with in-queue I/O
	read_sectors,		// sectors       number of sectors read
	read_ticks,			// milliseconds  total wait time for read requests
	write_IOs,			// requests      number of write I/Os processed
	write_merges,		// requests      number of write I/Os merged with in-queue I/O
	write_sectors,		// sectors       number of sectors written
	write_ticks,		// milliseconds  total wait time for write requests
	//in_flight,		// requests      number of I/Os currently in flight
	//io_ticks,			// milliseconds  total time this block device has been active
	//time_in_queue,	// milliseconds  total wait time for all requests
	//discard_IOs,		// requests      number of discard I/Os processed
	//discard_merges,	// requests      number of discard I/Os merged with in-queue I/O
	//discard_sectors,	// sectors       number of sectors discarded
	//discard_ticks,	// milliseconds  total wait time for discard requests
};
#define STATS_COUNT write_ticks - read_IOs + 1
#define ull_t unsigned long long int

int get_stat_record(const char *dev, ull_t rec[STATS_COUNT]) {
	char path[PATH_MAX];
	if (0 > esnprintf(path, sizeof(path), "/sys/class/block/%s/stat", dev))
	{
		warn("error building device path.");
		return 0;
	}

	return pscanf(path,
				 "%llu %llu %llu %llu "
				 "%llu %llu %llu %llu ",
				 &rec[read_IOs],
				 &rec[read_merges],
				 &rec[read_sectors],
				 &rec[read_ticks],
				 &rec[write_IOs],
				 &rec[write_merges],
				 &rec[write_sectors],
				 &rec[write_ticks]);
}

int get_sector_size(const char *dev)
{
	char path[PATH_MAX];
	if (0 > esnprintf(path, sizeof(path), "/sys/block/%s/queue/hw_sector_size", dev))
	{
		warn("error building device path.");
		return 0;
	}

	int sector_size = 0;
	if(1 != pscanf(path, "%d", &sector_size))
	{
		warn("error reading sector_size for %s.", dev);
		return 0;
	}

	return sector_size;
}

const char*	diskreads(const char *dev)
{
	static
	ull_t curr_rec[STATS_COUNT];
	ull_t prev_rec[STATS_COUNT];
	memcpy(prev_rec, curr_rec, sizeof(prev_rec));

	if(STATS_COUNT != get_stat_record(dev, curr_rec))
	{
		warn("error reading block %s stat record.", dev);
		return NULL;
	}
	if(0 == prev_rec[read_IOs])
		return NULL;

	extern const unsigned int interval;
	int sector_size = get_sector_size(dev);

	return fmt_human((curr_rec[read_sectors] - prev_rec[read_sectors])
	 		* 1000 / interval * sector_size, 1000);
}

const char*	diskwrites(const char *dev)
{
	static
	ull_t curr_rec[STATS_COUNT];
	ull_t prev_rec[STATS_COUNT];
	memcpy(prev_rec, curr_rec, sizeof(prev_rec));

	if(STATS_COUNT != get_stat_record(dev, curr_rec))
	{
		warn("error reading block %s stat record.", dev);
		return NULL;
	}
	if(0 == prev_rec[read_IOs])
		return NULL;

	extern const unsigned int interval;
	int sector_size = get_sector_size(dev);

	return fmt_human((curr_rec[write_sectors] - prev_rec[write_sectors])
	 		* 1000 / interval * sector_size, 1000);
}
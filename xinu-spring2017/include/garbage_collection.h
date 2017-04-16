#ifndef gc_h

#define gc_h

#define RECORD_SIZE 1000
// define an array entry struct to save a block returned ffrom getmem()
struct allocated_block
{
	char *blkAddr;
	uint32 size;
};

struct allocated_mem
{
	struct allocated_block blocks[RECORD_SIZE];
	int trackedBlocks;
};

syscall initMemRecord( struct allocated_mem * d_mem_record);

syscall trackedBlock(struct allocated_mem * d_mem_record,  char* newBlkAddr, uint32 blkSize);

syscall untrackBlock(struct allocated_mem * d_mem_record,  char* newBlkAddr);

struct allocated_block popMemRecord(struct allocated_mem * d_mem_record);


#endif

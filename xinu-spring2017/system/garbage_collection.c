#include <xinu.h>

syscall initMemRecord( struct allocated_mem * d_mem_record)
{
	d_mem_record->trackedBlocks = 0;
	int i;
	for(i = 0; i < RECORD_SIZE; i++)
	{
		d_mem_record->blocks[i].blkAddr = (char*)EMPTY;
		d_mem_record->blocks[i].size = 0;
	}
	return OK;
}

syscall trackBlock(struct allocated_mem * d_mem_record,  char* newBlkAddr, uint32 size)
{
	if(d_mem_record->trackedBlocks == RECORD_SIZE)
		return SYSERR;

	d_mem_record->blocks[d_mem_record->trackedBlocks].blkAddr = newBlkAddr;
	d_mem_record->blocks[d_mem_record->trackedBlocks].size = size;
	d_mem_record->trackedBlocks++;
	return OK;
}

syscall untrackBlock(struct allocated_mem * d_mem_record,  char* newBlkAddr)
{
	if(d_mem_record->trackedBlocks == 1 && d_mem_record->blocks[0].blkAddr == newBlkAddr)
	{
		d_mem_record->blocks[0].blkAddr = (char*)EMPTY;
		d_mem_record->blocks[0].size = 0;
		d_mem_record->trackedBlocks--;
		return OK;
	}
	else if( d_mem_record->trackedBlocks == 1 )
		return SYSERR;
		
	int i, ei;
	ei = -1;
	for(i = 0; i < d_mem_record->trackedBlocks; i++)
	{
		if(d_mem_record->blocks[i].blkAddr == newBlkAddr )
			ei = i;
	}
	// block doesn't exist or no tracked blocks
	if(ei < 0 || ei > d_mem_record->trackedBlocks )
		return SYSERR;
	// swap the intended block with the last block
	int swap_i = d_mem_record->trackedBlocks -1;	
	if(ei != swap_i)
	{
		d_mem_record->blocks[ei].blkAddr = d_mem_record->blocks[swap_i].blkAddr;
		d_mem_record->blocks[ei].size = d_mem_record->blocks[swap_i].size;		
	}
	// empty last block
	d_mem_record->blocks[swap_i].blkAddr = (char*)EMPTY;
	d_mem_record->blocks[swap_i].size = 0;

	d_mem_record->trackedBlocks--;
	return OK;
}

struct allocated_block popMemRecord(struct allocated_mem * d_mem_record)
{
	struct allocated_block res;
	if(d_mem_record->trackedBlocks == 0)
	{
		// kprintf("[pop] returning 0 res\n");
		res.size = 0;
		res.blkAddr = (char*)EMPTY;
		return res;
	}
	res.size = d_mem_record->blocks[0].size;
	res.blkAddr = d_mem_record->blocks[0].blkAddr;
	untrackBlock(d_mem_record, res.blkAddr);
	// kprintf("[pop] returning block. addr: %d, @ trackedBlocks: %d\n", res.blkAddr, d_mem_record->trackedBlocks );
	return res;
}

void print_dmem(struct allocated_mem * d_mem_record)
{
	int n = d_mem_record->trackedBlocks;
	int i;
	kprintf("dmem : { ");
	for(i = 0; i < n;i++)
		kprintf("%u ", d_mem_record->blocks[i].blkAddr );
	kprintf("}\n");
	return;
}

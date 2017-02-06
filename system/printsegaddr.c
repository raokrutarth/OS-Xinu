 #include <xinu.h>


void printsegaddr()
{
	/* from memory.h */
	
	// extern	int	text;			/* Start of text segment	*/
	// extern	void*	_etext;			/* End of text segment		*/
	// extern	int	data;			/* Start of data segment	*/
	// extern	int edata;			/* End of data segment		*/
	// extern	int	bss;			/* Start of bss segment		*/
	// extern	int	ebss;			/* End of bss segment		*/
	// extern	int	end;			/* End of program		*/
	
	/* print 
		1. start of the text, data, bss
		2. end of the bss segment
		3. 4 bytes at start of segment boundaries
		3. 4 bytes following the end of the bss segment
		4. lengths of the text, data, and bss segments
	*/

	kprintf("\n***Printing memory segments***\n");
	
	kprintf("Start of text = %X \n", (uint32)&text);
	//kprintf("etext = %X \n", (uint32)&etext);
	uint32 text_size= (uint32)&etext - (uint32)&text;
	kprintf("Size of text : %d bytes\n", text_size);
	
	
	kprintf("Start of data = %X \n", (uint32)&data);	
	//kprintf("&_edata = %X \n", (uint32)&edata);	
	uint32 data_size = (uint32)&edata - (uint32)&data;
	kprintf("Size of data : %d bytes\n", data_size);
	
	kprintf("Start of bss = %X \n", (uint32)&bss);
	uint32 bss_size = (uint32)&ebss - (uint32)&bss;
	kprintf("Size of bss : %d bytes \n", bss_size);
	
	kprintf("End of bss = %X \n", (uint32)&ebss);
	
	kprintf("\n");	
}


/* code to reverse endianess of a long */
long net2hostlgcc(long tc)
{
	long x = tc;
	x = ( x >> 24 ) | (( x << 8) & 0x00ff0000 )| ((x >> 8) & 0x0000ff00) | ( x << 24)  ; 
	return x;
}
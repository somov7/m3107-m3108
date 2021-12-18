void WriteBitInit( FILE *F );
void WriteBit( int Bit );
void WriteBitClose( void );
int ReadBit( void );


static FILE *BitF;
static int BitAccum, BitPos;
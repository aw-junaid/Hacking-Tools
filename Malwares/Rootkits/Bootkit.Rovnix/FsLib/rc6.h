//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FsLib project
//	
// module: rc6.h
// $Revision: 39 $
// $Date: 2012-05-02 15:38:09 +0400 (Ср, 02 май 2012) $
// description: 
//	Lightweight RC6 algorithm implementation.



#define	RC6_KEY_SIZE	128				// bits
#define RC6_KEY_CHARS	RC6_KEY_SIZE/8	// bytes

#define	RC6_BLOCK_SIZE	16				// bytes

typedef unsigned long rc6_key[44];

typedef struct{
	rc6_key	skey;
} RC6CONTEXT, *HRC6;

void	RC6KeySetup (HRC6 hAlgorithm, unsigned char* key);
void	MainRC6Decrypt (HRC6 hAlgorithm, PULONG In, PULONG Out);
void	MainRC6Encrypt (HRC6 hAlgorithm, PULONG In, PULONG Out);
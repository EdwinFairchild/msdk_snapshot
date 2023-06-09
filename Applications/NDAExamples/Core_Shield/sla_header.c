#define SWAP(x) (((x) >> 24) | (((x) & 0x00FF0000) >> 8) | (((x) & 0x0000FF00) << 8) | (((x) & 0x000000FF) << 24))

typedef enum
{
	ROM_A1_VERSION = 0x01000000,
} enum_rom_version_t;

//#ifdef __SLA_FWK__
/***** Includes *****/
#include <stdio.h>
#include <stdint.h>

extern unsigned int _start_SWAP;
extern unsigned int _SLA_Size_SWAP;

typedef enum
{
	MagicH = 0xF6495357,
	MagicL = 0x45444744,
} enum_magic_t;

typedef struct
{
	enum_magic_t		MagicHigh;				//> SLA Header magic
	enum_magic_t		MagicLow;				//> SLA Header magic
} magic_t;

typedef struct
{
	magic_t			Magic;
	enum_rom_version_t	RomVersion;				//> ROM version
	unsigned int	LoadAddr;				//> Relocation address.
	unsigned int	SLA_CodeSize;  			//> SLA code size in bytes
	unsigned int*	JumpAddr; 				//> Rom code will jump at this address
	unsigned int 	ArgSize;  				//> Size of the Argument
	unsigned int 	AppVersionNumber;  		//> Version of this application
} flash_app_header_t;

__attribute__ ((section(".sb_sla_header"))) __attribute__ ((__used__))
const flash_app_header_t sb_header =
{
		.Magic 				= {
				.MagicHigh			= SWAP(MagicH),
				.MagicLow			= SWAP(MagicL),
		},

		.RomVersion			= SWAP(ROM_A1_VERSION),
		.LoadAddr     		= SWAP(0x10000000),
		.SLA_CodeSize 		= (unsigned int)&_SLA_Size_SWAP,  // Trick to get constant defined at link time
		.JumpAddr   		= &_start_SWAP,
		.ArgSize  			= 0,
		.AppVersionNumber   = SWAP(0x01000000), // 0xAABBCCCC for version AA.BB.CCCC
};

__attribute__ ((section(".sb_sla_trailer"))) __attribute__ ((__used__))
const unsigned int dummy_signature=0xCAFEFADE;

//#endif //__SLA_FWK__

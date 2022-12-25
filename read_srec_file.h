#ifndef __READ_SREC_FLIE_H__
#define __READ_SREC_FLIE_H__
/*******************************************************************************
 * Includes
 ******************************************************************************/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define TYPE_S0 ((uint8_t)0)

#define TYPE_S1 ((uint8_t)1)
#define TYPE_S2 ((uint8_t)2)
#define TYPE_S3 ((uint8_t)3)

#define TYPE_S4 ((uint8_t)4)

#define TYPE_S5 ((uint8_t)5)
#define TYPE_S6 ((uint8_t)6)

#define TYPE_S7 ((uint8_t)7)
#define TYPE_S8 ((uint8_t)8)
#define TYPE_S9 ((uint8_t)9)

#define TYPE_NONE ((uint8_t)10)
#define ERROR_SIZE_LINE ((uint8_t)10)
#define ERROR_FIRST_CHARACTER ((uint8_t)11)
#define ERROR_STRANGE_CHARACTERS ((uint8_t)12)

#define MAX ((uint8_t)79)

#define TYPE_HEX_DEGITS ((uint8_t)1)
#define COUNT_HEX_DEGITS ((uint8_t)2)
#define ADDRESS_HEX_DEGITS ((uint8_t)8)
#define DATA_HEX_DEGITS ((uint8_t)64)
#define CHECKSUM_HEX_DIGITS ((uint8_t)2)

#define ADDRESS_16_BIT ((uint8_t)4)
#define ADDRESS_24_BIT ((uint8_t)6)
#define ADDRESS_32_BIT ((uint8_t)8)

#define ONE_BYTE ((uint8_t)2) // Convert tung byte mot

typedef struct
{
    uint8_t recordType;
    char byteCount[COUNT_HEX_DEGITS];
    char address[ADDRESS_HEX_DEGITS];
    char data[DATA_HEX_DEGITS];
    char checkSum[CHECKSUM_HEX_DIGITS];
    uint8_t sizeOfLine ;

} Data_Srec_Line_Struct_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
#endif /*__READ_SREC_FLIE_H__*/

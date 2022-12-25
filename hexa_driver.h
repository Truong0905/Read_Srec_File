#ifndef __HEXA_DRIVER_H__
#define __HEXA_DRIVER_H__

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdint.h>
#include <stdbool.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*
 * Position of characters in the ascii
 */
#define ZERO_ASCII ((uint8_t)48) /*Position character 0 in the ascii*/
#define NINE_ASCII ((uint8_t)57) /*/*Position character 9 in the ascii*/

#define A_CAPITALIZE_LETTER ((uint8_t)65) /*The position of the capital letter 'A' in the ascii */
#define F_CAPITALIZE_LETTER ((uint8_t)70) /*The position of the capital letter 'F' in the ascii */

#define ENTER_ASCII ((uint8_t)10) /*/*Position character '\n' in the ascii*/


#define EOF_STRING '\0'
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

bool CheckHexa(char stringInput[], uint8_t sizeOfStringInput);
uint32_t ConvertStringToHexa(const char stringInput[],uint8_t sizeOfStringInput);



#endif /*__HEXA_DRIVER_H__*/


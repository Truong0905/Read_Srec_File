/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <string.h>
#include "hexa_driver.h"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static uint32_t myPowFunction(uint32_t radix, uint8_t exponent);

/*******************************************************************************
 * Code
 ******************************************************************************/

bool CheckHexa(char stringInput[], uint8_t sizeOfStringInput)
{
    uint8_t i; /*Index value*/
    bool acknowledge = true;

    if (ENTER_ASCII == stringInput[sizeOfStringInput - 1])
    {
        stringInput[sizeOfStringInput - 1] = EOF_STRING;
        sizeOfStringInput--;
    }
    else
    {
        /*Do nothing*/
    }

    for (i = 0; i < sizeOfStringInput; i++)
    {
        if ((F_CAPITALIZE_LETTER >= stringInput[i]) && (A_CAPITALIZE_LETTER <= stringInput[i]))
        {
            /*Do nothing*/
        }
        else if ((NINE_ASCII >= stringInput[i]) && (ZERO_ASCII <= stringInput[i]))
        {
            /*Do nothing*/
        }
        else
        {
            acknowledge = false;
        }
    }
    return acknowledge;
}

uint32_t ConvertStringToHexa(const char stringInput[],uint8_t sizeOfStringInput)
{
    uint8_t i; /*Index value*/
    uint32_t valueReturn = 0 ;

    for (i = 0; i < sizeOfStringInput; i++)
    {
        if ((F_CAPITALIZE_LETTER >= stringInput[i]) && (A_CAPITALIZE_LETTER <= stringInput[i]))
        {
            valueReturn += ((uint32_t)stringInput[i] - 55) * myPowFunction(16, sizeOfStringInput - i - 1);
        }
        else
        {
            valueReturn += ((uint32_t)stringInput[i] - 48) * myPowFunction(16, sizeOfStringInput - i - 1);
        }
    }
    return valueReturn ;
}

/************************************************************************************
 * Static function
 *************************************************************************************/

static uint32_t myPowFunction(uint32_t radix, uint8_t exponent)
{
    uint8_t i = 0;
    uint32_t valueReturn = 1;

    for (i = 0; i < exponent; i++)
    {
        valueReturn *= radix;
    }
    return valueReturn;
}

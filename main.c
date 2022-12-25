/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "main.h"
/*******************************************************************************
 * Code
 ******************************************************************************/

/**
 * @brief  The entry point.
 *  @return int
 */

int main(void)
{
    fflush(stdin);
    FILE *stream = NULL;
    stream = fopen("freg", "r");
    fflush(stream);

    if (stream)
    {
        ReadSrecFile(stream);
    }
    else
    {
        printf("Can not open file\n");
    }
    fclose(stream);

    return 0;
}
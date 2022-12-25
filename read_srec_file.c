/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hexa_driver.h"
#include "read_srec_file.h"
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void SplitData(Data_Srec_Line_Struct_t *dataLine, const char stringTransport[], uint8_t typeRecord);
static uint8_t CheckType(bool hexaCheck, const char stringTransportOne[]);
static bool CheckError(const Data_Srec_Line_Struct_t *dataLine, uint32_t orderOfLine);
static bool SaveData(const Data_Srec_Line_Struct_t *dataLine, uint32_t orderOfThisLine, FILE *streamData);
static bool CheckCount(const Data_Srec_Line_Struct_t *dataLine, uint32_t orderOfThisLine, uint32_t sumLineData, uint8_t bitCount);
static bool CheckCountHandler(const Data_Srec_Line_Struct_t *dataLine, uint32_t orderOfLine);
static bool CheckSumHandler(const Data_Srec_Line_Struct_t *dataLine, uint32_t orderOfLine);

/*******************************************************************************
 * Code
 ******************************************************************************/
void ReadSrecFile(FILE *streamInput)
{
    uint8_t typeRecord = TYPE_NONE;
    bool hexaCheck = false;
    bool acknowledge = true;
    bool CheckSizeOfLine = true;
    uint8_t sizeOfLine = 0;
    uint32_t orderOfThisLine = 1;
    uint32_t sumLineData = 0;

    Data_Srec_Line_Struct_t dataLine;
    char stringTransport[MAX];
    FILE *streamData = NULL;

    if (!streamInput)
    {
        printf("No files");
    }
    else
    {
        streamData = fopen("File_Data.txt", "w");
        fflush(streamData);
    }

    while (acknowledge && (fgets(stringTransport, MAX, streamInput) != NULL))
    {
        memset(&dataLine, 0, sizeof(Data_Srec_Line_Struct_t));
        /* Step 1 :Check Record start*/
        if (stringTransport[0] == 'S')
        {
            sizeOfLine = strlen(stringTransport);
            /*Step 2 : Check hexadecimal number*/
            hexaCheck = CheckHexa(&stringTransport[1], sizeOfLine - 1);

            /*Step 3 : Check size of line*/
            sizeOfLine = strlen(stringTransport);
            dataLine.sizeOfLine = sizeOfLine;

            if (MAX == sizeOfLine)
            {
                typeRecord = ERROR_SIZE_LINE;
            }
            else
            {

                /*Step 4 :  Check Record type*/
                typeRecord = CheckType(hexaCheck, &stringTransport[1]);

                /*Step 5 : Divide the received data into different fields*/
                SplitData(&dataLine, &stringTransport[2], typeRecord);
            }
        }
        else
        {
            typeRecord = ERROR_FIRST_CHARACTER;
            break;
        }

        /*Step 6: */
        switch (typeRecord)
        {
        case TYPE_S0:
            acknowledge = CheckError(&dataLine, orderOfThisLine);
            break;
        case TYPE_S1:
        case TYPE_S2:
        case TYPE_S3:
            acknowledge = SaveData(&dataLine, orderOfThisLine, streamData);
            sumLineData++;
            break;
        case TYPE_S4:
            break;
        case TYPE_S5:
            acknowledge = CheckCount(&dataLine, orderOfThisLine, sumLineData, ADDRESS_16_BIT);
            break;
        case TYPE_S6:
            acknowledge = CheckCount(&dataLine, orderOfThisLine, sumLineData, ADDRESS_24_BIT);
            break;
        case TYPE_S7:
        case TYPE_S8:
        case TYPE_S9:
            acknowledge = CheckError(&dataLine, orderOfThisLine);
            break;
        case ERROR_SIZE_LINE:
            printf("Line [%d] size exceeds the allowed limit\n", orderOfThisLine);
            acknowledge = false;
        case ERROR_FIRST_CHARACTER:
            printf("The first character of line [%d] is not the character S \n", orderOfThisLine);
            acknowledge = false;
            break;
        case ERROR_STRANGE_CHARACTERS:
            printf("There are strange characters in the line [%d]\n", orderOfThisLine);
            acknowledge = false;
            break;
        default:
            break;
        }
        orderOfThisLine++;
    }
    if (acknowledge)
    {
        fclose(streamData);
    }
    else
    {
        fclose(streamData);
        system("del File_Data.txt");
        printf("The data file is not created \n");
    }
}

/************************************************************************************
 * Static function
 *************************************************************************************/

static uint8_t CheckType(bool hexaCheck, const char stringTransportOne[])
{
    char temp[2];
    uint8_t valueRecieve;

    if (hexaCheck)
    {
        valueRecieve = ConvertStringToHexa(stringTransportOne, TYPE_HEX_DEGITS);
    }
    else
    {
        valueRecieve = ERROR_STRANGE_CHARACTERS;
    }

    return valueRecieve;
}

static void SplitData(Data_Srec_Line_Struct_t *dataLine, const char stringTransport[], uint8_t typeRecord)
{
    uint8_t sizeOfStringTranspost = 0;
    uint8_t sizeData = 0;

    sizeOfStringTranspost = strlen(stringTransport);
    switch (typeRecord)
    {
    case TYPE_S0:
    case TYPE_S1:
    case TYPE_S9:
    case TYPE_S5:
        /*16 bit address - 4 ky tu*/
        if (8 <= sizeOfStringTranspost)
        {
            dataLine->recordType = typeRecord;
            memcpy(dataLine->byteCount, stringTransport, 2);
            memcpy(dataLine->address, &stringTransport[2], ADDRESS_16_BIT);
            memcpy(dataLine->checkSum, &stringTransport[sizeOfStringTranspost - 2], 2);
            sizeData = sizeOfStringTranspost - 2 - ADDRESS_16_BIT - 2;
            memcpy(dataLine->data, &stringTransport[2 + ADDRESS_16_BIT], sizeData);
        }
        else
        {
            /*error*/
        }
        break;

    case TYPE_S2:
    case TYPE_S8:
    case TYPE_S6:
        /*24 bit - 6 ky tu*/
        if (10 <= sizeOfStringTranspost)
        {
            dataLine->recordType = typeRecord;
            memcpy(dataLine->byteCount, stringTransport, 2);
            memcpy(dataLine->address, &stringTransport[2], ADDRESS_24_BIT);
            memcpy(dataLine->checkSum, &stringTransport[sizeOfStringTranspost - 2], 2);
            sizeData = sizeOfStringTranspost - 2 - ADDRESS_24_BIT - 2;
            memcpy(dataLine->data, &stringTransport[2 + ADDRESS_24_BIT], sizeData);
        }
        else
        {
            /*error*/
        }
        break;
    case TYPE_S3:
    case TYPE_S7:
        /*32 bit - 8 ky tu*/
        if (12 <= sizeOfStringTranspost)
        {
            dataLine->recordType = typeRecord;
            memcpy(dataLine->byteCount, stringTransport, 2);
            memcpy(dataLine->address, &stringTransport[2], ADDRESS_32_BIT);
            memcpy(dataLine->checkSum, &stringTransport[sizeOfStringTranspost - 2], 2);
            sizeData = sizeOfStringTranspost - 2 - ADDRESS_32_BIT - 2;
            memcpy(dataLine->data, &stringTransport[2 + ADDRESS_32_BIT], sizeData);
        }
        else
        {
            /*error*/
        }
        break;
    default:
        break;
    }
}

static bool CheckError(const Data_Srec_Line_Struct_t *dataLine, uint32_t orderOfLine)
{
    bool noError = true;

    noError = CheckCountHandler(dataLine, orderOfLine);
    if (noError)
    {
        noError = CheckSumHandler(dataLine, orderOfLine);
    }
    else
    {
        /*Do nothing*/
    }

    return noError;
}

static bool CheckCountHandler(const Data_Srec_Line_Struct_t *dataLine, uint32_t orderOfLine)
{
    uint8_t compareByteCount = 0;
    uint8_t sizeCheckCount = 0;
    bool noError = true;

    compareByteCount = 2 * ConvertStringToHexa(dataLine->byteCount, COUNT_HEX_DEGITS); // 1 byte bang 2 ky tu
    sizeCheckCount = dataLine->sizeOfLine - 4;                                         // 1 ky tu S va 1 ky tu type va 12 ky tu count
    if (compareByteCount == sizeCheckCount)
    {
        /*Do nothing*/
    }
    else
    {
        noError = false;
        printf("Byte count error of line %d \n", orderOfLine);
    }
    return noError;
}

static bool CheckSumHandler(const Data_Srec_Line_Struct_t *dataLine, uint32_t orderOfLine)
{
    uint32_t sumByte = 0;
    uint8_t i; /*Index value*/
    bool noError = true;
    uint8_t sizeAddress = 0;
    uint32_t compareCheckSum = 0;

    compareCheckSum = ConvertStringToHexa(dataLine->checkSum, CHECKSUM_HEX_DIGITS);

    sumByte = ConvertStringToHexa(dataLine->byteCount, ONE_BYTE); // 1 byte count

    switch (dataLine->recordType)
    {
    case TYPE_S0:
    case TYPE_S1:
    case TYPE_S5:
    case TYPE_S9:
        sizeAddress = ADDRESS_16_BIT;
        break;
    case TYPE_S2:
    case TYPE_S6:
    case TYPE_S8:
        sizeAddress = ADDRESS_24_BIT;
        break;
    case TYPE_S3:
    case TYPE_S4:
    case TYPE_S7:
        sizeAddress = ADDRESS_32_BIT;
        break;
    default:
        noError = false;
        break;
    }

    for (i = 0; i < sizeAddress; i += 2)
    {
        sumByte += ConvertStringToHexa(&dataLine->address[i], ONE_BYTE);
    }

    for (i = 0; (i < DATA_HEX_DEGITS) && ('\0' != dataLine->data[i]); i += 2)
    {
        sumByte += ConvertStringToHexa(&dataLine->data[i], ONE_BYTE);
    }

    sumByte ^= 0xFFFFFFFF; // Dao bit
    sumByte &= 0xFF;       // Lay byte LSB

    if (noError && (sumByte != compareCheckSum))
    {
        noError = false;
        printf("Check sum error of line %d", orderOfLine);
    }
    else
    {
        /*Do nothing*/
    }
    return noError;
}

static bool SaveData(const Data_Srec_Line_Struct_t *dataLine, uint32_t orderOfThisLine, FILE *streamData)
{
    uint8_t step = 0;
    bool checkError = true;
    bool returnSaveData = false;
    while (checkError)
    {
        switch (step)
        {
        case 0:
            checkError = CheckCountHandler(dataLine, orderOfThisLine);
            if (checkError)
            {
                step = 1;
            }
            else
            {
                step = 5;
            }
            break;
        case 1:
            checkError = CheckSumHandler(dataLine, orderOfThisLine);
            if (checkError)
            {
                step = 2;
            }
            else
            {
                step = 5;
            }
            break;
        case 2:
            fputs(dataLine->data, streamData);
            fputc('\n', streamData);
            returnSaveData = true;
            checkError = false;
            break;
        default:
            checkError = false;
            break;
        }
    }
    return returnSaveData;
}

static bool CheckCount(const Data_Srec_Line_Struct_t *dataLine, uint32_t orderOfThisLine, uint32_t sumLineData, uint8_t bitCount)
{
    uint8_t step = 0;
    bool checkError = true;
    bool returnCheckCount = false;
    uint32_t compareCheckCount = 0;
    while (checkError)
    {
        switch (step)
        {
        case 0:
            checkError = CheckCountHandler(dataLine, orderOfThisLine);
            if (checkError)
            {
                step = 1;
            }
            else
            {
                step = 5;
            }
            break;
        case 1:
            checkError = CheckSumHandler(dataLine, orderOfThisLine);
            if (checkError)
            {
                step = 2;
            }
            else
            {
                step = 5;
            }
            break;
        case 2:
            compareCheckCount = ConvertStringToHexa(dataLine->address, bitCount);
            if (compareCheckCount == sumLineData && (strlen(dataLine->data)==0))
            {
                returnCheckCount = true;
            }
            else
            {
                printf("Check sum error of line %d \n", orderOfThisLine);
                returnCheckCount = false;
            }
            checkError = false;
            break;
        default:
            checkError = false;
            break;
        }
    }
    return returnCheckCount;
}

#include <project.h>
#include "defines.h"
#include "stdio.h"


/* Prüft, ob ein 'm', wie Messung empfangen wurde. Falls ja, wird der übergebene String gesendet. */
void checkAndSendString(char8 lineStr[LINE_STR_LENGTH])
{
    uint16 count;
    uint8 buffer[USBUART_BUFFER_SIZE];
    
    if (0u != USBUART_IsConfigurationChanged())
    {
        if (0u != USBUART_GetConfiguration())
        {
            USBUART_CDC_Init();
        }
    }

    if (0u != USBUART_GetConfiguration())
    {
        if (0u != USBUART_DataIsReady())
        {
            count = USBUART_GetAll(buffer);

            if ((0u != count) && ('m' == *buffer))
            {
                while (0u == USBUART_CDCIsReady())
                {
                }
                USBUART_PutString(lineStr);
                while (0u == USBUART_CDCIsReady())
                {
                }
                USBUART_PutChar('\n');
            }
        }
    }
}


/* Sendet ein Integer-Array mit Elementen durch Komma getrennt und abgeschlossen durch '\n'.
Wenn das Array größer als der Puffer ist, wird anstelle des Arrays eine Meldung gesendet. */
void sendIntArray(const int32_t dataArray[], uint16 dataArrayLength)
{
    char8 resultStr[LINE_STR_LENGTH];
    char8 tmpStr[16];
    
    if(USBUART_BUFFER_SIZE < dataArrayLength * sizeof(dataArray[0]))
    {
        sprintf(resultStr, "Array zu lang! Puffer: %u, Array: %u", USBUART_BUFFER_SIZE, dataArrayLength * sizeof(dataArray[0]));
    }
    else
    {
        for(uint16 i = 0; i < dataArrayLength; i++)
        {
            sprintf(tmpStr, "%d,", dataArray[i]);
            if(i==0)
            {
                strcpy(resultStr, tmpStr);
            }
            else if(i == dataArrayLength-1)
            {
                sprintf(tmpStr, "%d", dataArray[i]);
                strcat(resultStr, tmpStr);
            }
            else
            {
                strcat(resultStr, tmpStr);
            }
        }
    }
    checkAndSendString(resultStr);
}


/* Sendet ein Double-Array mit Elementen durch Komma getrennt und abgeschlossen durch '\n'.
Wenn das Array größer als der Puffer ist, wird anstelle des Arrays eine Meldung gesendet. */
void sendFloatArray(const double dataArray[], uint16 dataArrayLength)
{
    char8 resultStr[LINE_STR_LENGTH];
    char8 tmpStr[32];
    
    if(USBUART_BUFFER_SIZE < dataArrayLength * sizeof(dataArray[0]))
    {
        sprintf(resultStr, "Array zu lang! Puffer: %u, Array: %u", USBUART_BUFFER_SIZE, dataArrayLength * sizeof(dataArray[0]));
    }
    else
    {
        for(uint16 i = 0; i < dataArrayLength; i++)
        {
            sprintf(tmpStr, "%lf,", dataArray[i]);
            if(i==0)
            {
                strcpy(resultStr, tmpStr);
            }
            else if(i == dataArrayLength-1)
            {
                sprintf(tmpStr, "%lf", dataArray[i]);
                strcat(resultStr, tmpStr);
            }
            else
            {
                strcat(resultStr, tmpStr);
            }
        }
    }
    checkAndSendString(resultStr);
}

/* [] END OF FILE */
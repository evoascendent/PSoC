#include <project.h>
#include "defines.h"
#include "stdio.h"


char8 order = 'm';

uint8 check(char8 character)
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

            if ((0u != count) && (character == *buffer))
            {
                return 0;
            }
            else
            {
                return 1;
            }
        }
        else
        {
            return 1;
        }
    }
    else
    {
        return 1;
    }
}


void sendString(char8 lineStr[LINE_STR_LENGTH])
{
    if (0u != USBUART_IsConfigurationChanged())
    {
        if (0u != USBUART_GetConfiguration())
        {
            USBUART_CDC_Init();
        }
    }

    if (0u != USBUART_GetConfiguration())
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


/* Prüft, ob 'char8 order'(siehe oben) empfangen wurde. Falls ja, wird der übergebene String gesendet. */
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

            if ((0u != count) && (order == *buffer))
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
Wenn das Array größer als der Puffer ist, wird anstelle des Arrays eine Meldung gesendet.
Vor der Verarbeitung der Daten wird der Empfang eines Sendebefehls geprüft. Falls 'char8 order'(siehe oben)
empfangen wurde, wird verarbeitet und versendet. */
uint8 sendIntArray(const int32_t dataArray[], uint16 dataArrayLength)
{
    char8 resultStr[LINE_STR_LENGTH];
    char8 tmpStr[16];
    
    if(0 == check(order))
    {
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
        sendString(resultStr);
        return 0;
    }
    else
    {
        return 1;
    }
}


/* Sendet ein Double-Array mit Elementen durch Komma getrennt und abgeschlossen durch '\n'.
Wenn das Array größer als der Puffer ist, wird anstelle des Arrays eine Meldung gesendet.
Vor der Verarbeitung der Daten wird der Empfang eines Sendebefehls geprüft. Falls 'char8 order'(siehe oben)
empfangen wurde, wird verarbeitet und versendet. */
uint8 sendFloatArray(const double dataArray[], uint16 dataArrayLength)
{
    char8 resultStr[LINE_STR_LENGTH];
    char8 tmpStr[32];

    if(0 == check(order))
    {
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
        return 0;
    }
    else
    {
        return 1;
    }
}

/* [] END OF FILE */

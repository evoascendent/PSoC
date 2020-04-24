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


void sendBytes(const uint8* array, uint16 arrayLengthBytes)
{
    if (0u != USBUART_IsConfigurationChanged())
    {
        if (0u != USBUART_GetConfiguration())
        {
            USBUART_CDC_Init();
        }
    }

    /* Service USB CDC when device is configured. */
    if (0u != USBUART_GetConfiguration())
    {
        /* Wait until component is ready to send data to host. */
        while (0u == USBUART_CDCIsReady())
        {
        }

        /* Send data back to host. */
        USBUART_PutData(array, arrayLengthBytes);

        /* If the last sent packet is exactly the maximum packet 
        *  size, it is followed by a zero-length packet to assure
        *  that the end of the segment is properly identified by 
        *  the terminal.
        */
        if (MAX_PACKET_SIZE == arrayLengthBytes)
        {
            /* Wait until component is ready to send data to PC. */
            while (0u == USBUART_CDCIsReady())
            {
            }

            /* Send zero-length packet to PC. */
            USBUART_PutData(NULL, 0u);
        }
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


uint8 sendBinArray(const uint8* dataArray, uint16 dataArrayLengthBytes)
{
    char8 resultStr[LINE_STR_LENGTH];
    
    if(0 == check(order))
    {
        if(MAX_PACKET_SIZE < dataArrayLengthBytes)
        {
            sprintf(resultStr, "Array zu lang! Puffer: %u, Array: %u", MAX_PACKET_SIZE, dataArrayLengthBytes);
            sendString(resultStr);
            return 1;
        }
        else
        {   
            sendBytes(dataArray, dataArrayLengthBytes);
            return 0;
        }
    }
    else
    {
        return 1;
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

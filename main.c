#include <project.h>
#include "stdio.h"
#include "defines.h"
#include "rxtx.h"

#if defined (__GNUC__)
    /* Add an explicit reference to the floating point printf library */
    /* to allow usage of the floating point conversion specifiers. */
    /* This is not linked in by default with the newlib-nano library. */
    asm (".global _printf_float");
#endif

    
int main()
{
    int32_t dataArray[] = {1000, 577, -58989, 5647665, -290902, 45, 1, -290902, 45, 15647665, -256757788, 321, 42567577, 58989, 5647665, 290902, 45, -5647665};
    //double dataArray[] = {100.0, 57.7, -589.89, 5647.665, -290.902, 4.5, 1.0, -290902, 4.5, 15647.665, -25675.7788, 66.5};
    
    CyGlobalIntEnable;

    /* Start USBFS operation with 5-V operation. */
    USBUART_Start(USBFS_DEVICE, USBUART_5V_OPERATION);
    
    for(;;)
    {
        /* Anzahl der Elemente im Datenarray. */
        uint16 dataArrayLength = sizeof(dataArray) / sizeof(*dataArray);
        
        /* Das Datenarray und die Länge müssen übergeben werden. Nachdem geprüft wurde, ob ein 'm' empfangen wurde, wird das Array gesendet.*/
        sendIntArray(dataArray, dataArrayLength);
        //sendFloatArray(dataArray, dataArrayLength);
    }
}


/* [] END OF FILE */

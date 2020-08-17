/*
 ************************************************************************
 * FileName: dnvme_show.c
 * Description: show data.
 * Author: Xing Zou
 * Date: Aug-17-2020
 ************************************************************************
*/
#include "dnvme_show.h"

void show_raw_data(uint8_t *data, uint32_t bytes, char *description)
{
    uint32_t i;
    printf("%s:\n", description);
    for (i=0; i<bytes; i++)
    {
        if ((i&0xF)==0x0)
            printf("%02x: ", i);
        printf("%02x ", data[i]);
        if ((i&0xF)==0xF)
            printf("\b\n");
    }
    printf("\n");
}



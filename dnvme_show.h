/*
 ************************************************************************
 * FileName: dnvme_show.h
 * Description: show data APIs.
 * Author: Xing Zou
 * Date: Aug-17-2020
 ************************************************************************
*/

#ifndef __DNVME_SHOW_H__
#define __DNVME_SHOW_H__
#include <stdio.h>
#include <stdint.h>

void show_raw_data(uint8_t *data, uint32_t bytes, char *description);

#endif


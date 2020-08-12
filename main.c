/*
 ************************************************************************
 * FileName: main.c
 * Description: testdnvme commands.
 * Author: Xing Zou
 * Date: Aug-1-2020
 ************************************************************************
*/

#include <stdio.h>
#include "dnvme_commands.h"

int main(int argc, char *argv[])
{
    int fd = open_dev("/dev/nvme0");
    uint16_t qsize = NVME_QUEUE_ELEMENTS;
    uint16_t cq_id = 1;
    uint16_t sq_id = 1;
    uint16_t irq_no = 1;
    uint8_t contig = 1;
    void *iocq_buffer = NULL;
    void *iosq_buffer = NULL;
    int ret = malloc_4k_aligned_buffer(iocq_buffer, NVME_IOCQ_ELEMENT_SIZE, qsize);
    malloc_4k_aligned_buffer(iosq_buffer, NVME_IOSQ_ELEMENT_SIZE, qsize);
    dnvme_create_admin_cq(fd);
    dnvme_create_admin_sq(fd);
    dnvme_create_iocq(fd, cq_id, irq_no, qsize, contig, iocq_buffer);
    dnvme_create_iosq(fd, sq_id, cq_id, qsize, contig, iosq_buffer);
    ioctl_drive_metrics(fd);
    ioctl_device_metrics(fd);
    ioctl_ring_doorbell(fd, 0);
    return 0;
}


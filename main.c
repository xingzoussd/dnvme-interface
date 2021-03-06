/*
 ************************************************************************
 * FileName: main.c
 * Description: testdnvme commands.
 * Author: Xing Zou
 * Date: Aug-1-2020
 ************************************************************************
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <ctype.h>
#include <malloc.h>
#include <stdint.h>
#include <sys/mman.h>

#include "dnvme_commands.h"

#define DEVICE_FILE_NAME "/dev/nvme0"

void show_pcie_capability(int fd)
{
    uint32_t i;
    uint32_t bytes = 0x100;
    uint8_t *data = (uint8_t*)malloc(sizeof(char)*bytes);
    int ret = dnvme_pcie_capability_read_block(fd, 0, bytes, data);
    for (i=0; i<bytes; i++)
    {
        if ((i&0xF)==0x0)
            printf("%02x: ", i);
        printf("%02x ", data[i]);
        if ((i&0xF)==0xF)
            printf("\b\n");
    }
    free(data);
}

void show_cc(int fd)
{
    uint32_t bytes = 0x4;
    uint8_t *data = (uint8_t*)malloc(sizeof(char)*bytes);
    int ret = dnvme_controller_reg_read_dword(fd, 0x14, data);
    printf("CC = 0x%08x\n", *(uint32_t*)data);
    free(data);
}

void show_csts(int fd)
{
    uint32_t bytes = 0x4;
    uint8_t *data = (uint8_t*)malloc(sizeof(char)*bytes);
    int ret = dnvme_controller_reg_read_dword(fd, 0x1C, data);
    printf("CSTS = 0x%08x\n", *(uint32_t*)data);
}

int main(int argc, char *argv[])
{
    int fd = 0;
    uint16_t qsize = NVME_QUEUE_ELEMENTS;
    uint16_t cq_id = 1;
    uint16_t sq_id = 1;
    uint16_t irq_no = 1;
    uint8_t contig = 1;
    void *iocq_buffer = NULL;
    void *iosq_buffer = NULL;
    void *identify_ctrl_buffer = NULL;
    int ret = 0;
    struct nvme_id_ctrl ctrl_info;
    fd = open_dev(DEVICE_FILE_NAME);
    if (fd < 0) {
        printf("Can't open device file: %s\n", DEVICE_FILE_NAME);
        exit(-1);
    }
    printf("Device File Successfully Opened = %d\n", fd);
    ret = malloc_4k_aligned_buffer(&iocq_buffer, NVME_IOCQ_ELEMENT_SIZE, qsize);
    ret = malloc_4k_aligned_buffer(&iosq_buffer, NVME_IOSQ_ELEMENT_SIZE, qsize);
    ret = malloc_4k_aligned_buffer(&identify_ctrl_buffer, sizeof(struct nvme_id_ctrl), 1);
    ret = dnvme_controller_disable(fd);
    ret = dnvme_create_admin_cq(fd);
    ret = dnvme_create_admin_sq(fd);
    ret = dnvme_controller_enable(fd);
    ioctl_drive_metrics(fd);
    ioctl_device_metrics(fd);
    show_pcie_capability(fd);
    show_cc(fd);
    show_csts(fd);
    ret = dnvme_create_iocq(fd, cq_id, irq_no, qsize, contig, iocq_buffer);
    ret = dnvme_create_iosq(fd, sq_id, cq_id, qsize, contig, iosq_buffer);
    ret = dnvme_identify_ctrl(fd, 0, identify_ctrl_buffer);
    ret = ioctl_ring_doorbell(fd, 0);
    memcpy(&ctrl_info, identify_ctrl_buffer, sizeof(struct nvme_id_ctrl));
    return 0;
}


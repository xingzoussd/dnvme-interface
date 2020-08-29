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

#include "dnvme.h"
#include "dnvme_commands.h"
#include "dnvme_show.h"

#define DEVICE_FILE_NAME "/dev/nvme0"

void show_pcie_capability(int fd)
{
    uint32_t i;
    uint32_t bytes = 0x100;
    uint8_t *data = (uint8_t*)malloc(sizeof(char)*bytes);
    int ret = dnvme_pcie_capability_read_block(fd, 0, bytes, data);
    show_raw_data(data, bytes, "PCIE capability");
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
    void *identify_ns_buffer = NULL;
    void *cq_buffer = NULL;
    uint16_t cq_remaining = 0;
    uint16_t cq_buffer_size = 0;
    int ret = 0;
    struct nvme_id_ctrl ctrl_info;
    struct nvme_id_ns ns_info;
    fd = open_dev(DEVICE_FILE_NAME);
    if (fd < 0) {
        printf("Can't open device file: %s\n", DEVICE_FILE_NAME);
        exit(-1);
    }
    printf("Device File Successfully Opened = %d\n", fd);
    ret = malloc_4k_aligned_buffer(&iocq_buffer, NVME_IOCQ_ELEMENT_SIZE, qsize);
    ret = malloc_4k_aligned_buffer(&iosq_buffer, NVME_IOSQ_ELEMENT_SIZE, qsize);
    ret = malloc_4k_aligned_buffer(&identify_ctrl_buffer, sizeof(struct nvme_id_ctrl), 1);
    ret = malloc_4k_aligned_buffer(&identify_ns_buffer, sizeof(struct nvme_id_ns), 1);
    ret = dnvme_controller_disable(fd);
    ret = dnvme_create_admin_cq(fd);
    ret = dnvme_create_admin_sq(fd);
    ret = dnvme_controller_enable(fd);
    ioctl_drive_metrics(fd);
    ioctl_device_metrics(fd);
    show_pcie_capability(fd);
    show_cc(fd);
    show_csts(fd);
    ret = dnvme_admin_create_iocq(fd, cq_id, irq_no, qsize, contig, iocq_buffer);
    ret = dnvme_admin_create_iosq(fd, sq_id, cq_id, qsize, contig, iosq_buffer);
    ret = dnvme_admin_identify_ctrl(fd, 0, identify_ctrl_buffer);
    ret = dnvme_admin_identify_ns(fd, 0, 1, identify_ns_buffer);
    ret = dnvme_admin_abort(fd, 0, NVME_ADMIN_IDENTIFY);
    ret = ioctl_ring_doorbell(fd, 0);
    sleep(2);
    memcpy(&ctrl_info, identify_ctrl_buffer, sizeof(struct nvme_id_ctrl));
    memcpy(&ns_info, identify_ns_buffer, sizeof(struct nvme_id_ns));
    show_raw_data((uint8_t *)identify_ctrl_buffer, sizeof(struct nvme_id_ctrl), "Identify controller");
    show_raw_data((uint8_t *)identify_ns_buffer, sizeof(struct nvme_id_ns), "Identify namespace");
    cq_remaining = dnvme_cq_remain(fd, 0);
    cq_buffer_size = cq_remaining*16;
    ret = malloc_4k_aligned_buffer(&cq_buffer, cq_buffer_size, 1);
    ret = dnvme_cq_reap(fd, 0, cq_remaining, cq_buffer, cq_buffer_size);
    show_raw_data(cq_buffer, cq_buffer_size, "CQ data");
    return 0;
}


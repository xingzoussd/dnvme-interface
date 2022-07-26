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
#include "dnvme_ioctrl.h"
#include "dnvme_commands.h"
#include "dnvme_show.h"

#define DEVICE_FILE_NAME "/dev/nvme0"

int show_pcie_capability(int fd)
{
    uint32_t bytes = 0x100;
    uint8_t *data = (uint8_t*)malloc(sizeof(char)*bytes);
    int ret = dnvme_pcie_capability_read_block(fd, 0, bytes, data);
    if (ret)
        return ret;
    show_raw_data(data, bytes, "PCIE capability");
    free(data);
    return 0;
}

int show_cc(int fd)
{
    uint32_t data = 0;
    int ret = dnvme_controller_reg_read_dword(fd, 0x14, &data);
    if (ret)
        return ret;
    printf("CC = 0x%08x\n", data);
    return 0;
}

int show_csts(int fd)
{
    uint32_t data = 0;
    int ret = dnvme_controller_reg_read_dword(fd, 0x1C, &data);
    if (ret)
        return ret;
    printf("CSTS = 0x%08x\n", data);
    return 0;
}

int main(int argc, char *argv[])
{
    int fd = 0;
    uint16_t qsize = NVME_QUEUE_ELEMENTS;
    uint16_t cq_id = 1;
    uint16_t sq_id = 1;
    uint16_t irq_no = 1;
    uint8_t contig = 1;
//    uint16_t msix_cap = 0;
//    uint16_t msix_entry_count = 0;
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
//    msix_cap = dnvme_pcie_msix_capability(fd);
//    msix_entry_count = dnvme_pcie_msix_get_entry_count(fd, msix_cap);
//    //ioctl_device_metrics(fd);
//    //dnvme_controller_disable(fd);
//    //ret = dnvme_set_irq(fd, msix_entry_count, INT_MSIX);
//    //if (ret<0)
//    //    printf("Set IRQ MSIX failed.\n");
//    //else
//    //    printf("Set IRQ MSIX successfully.\n");
//    ////dnvme_pcie_msix_enable(fd, msix_cap);
//    //dnvme_controller_enable(fd);
//    //dnvme_pcie_msix_enable(fd, msix_cap);
//    //ioctl_device_metrics(fd);
//    if (ret)
//        return ret;
    ret = malloc_4k_aligned_buffer(&iocq_buffer, NVME_IOCQ_ELEMENT_SIZE, qsize);
    if (ret)
        return ret;
    ret = malloc_4k_aligned_buffer(&iosq_buffer, NVME_IOSQ_ELEMENT_SIZE, qsize);
    if (ret)
        return ret;
    ret = malloc_4k_aligned_buffer(&identify_ctrl_buffer, sizeof(struct nvme_id_ctrl), 1);
    if (ret)
        return ret;
    ret = malloc_4k_aligned_buffer(&identify_ns_buffer, sizeof(struct nvme_id_ns), 1);
    if (ret)
        return ret;
//    ret = dnvme_controller_disable(fd);
//    ret = dnvme_set_irq(fd, msix_entry_count, INT_MSIX);
//    if (ret)
//        return ret;
//    ret = dnvme_create_admin_cq(fd);
//    if (ret)
//        return ret;
//    ret = dnvme_create_admin_sq(fd);
//    if (ret)
//        return ret;
//    ret = dnvme_controller_enable(fd);
//    if (ret)
//        return ret;
    ret = init_drive(fd);
    ret = show_pcie_capability(fd);
    if (ret)
        return ret;
    ret = show_cc(fd);
    if (ret)
        return ret;
    ret = show_csts(fd);
    if (ret)
        return ret;
//    dnvme_pcie_msix_enable(fd, msix_cap);
//    ioctl_device_metrics(fd);
    ret = dnvme_admin_create_iocq(fd, 0, cq_id, irq_no, qsize, contig, iocq_buffer);
    if (ret)
        return ret;
    ret = dnvme_admin_create_iosq(fd, 0, sq_id, cq_id, qsize, contig, iosq_buffer, 1, 0);
    if (ret)
        return ret;
    ret = dnvme_admin_identify_ctrl(fd, 0, 0, identify_ctrl_buffer);
    if (ret)
        return ret;
    ret = dnvme_admin_identify_ns(fd, 1, 0, identify_ns_buffer);
    if (ret)
        return ret;
    //ret = dnvme_admin_abort(fd, 0, NVME_ADMIN_IDENTIFY);
    ret = dnvme_set_power_state(fd, NVME_NSID_ALL, 0, 0, 0);
    if (ret)
        return ret;
    ret = dnvme_get_power_state(fd, NVME_NSID_ALL, 0);
    if (ret)
        return ret;
    ret = dnvme_set_power_state(fd, NVME_NSID_ALL, 0, 1, 0);
    if (ret)
        return ret;
    ret = dnvme_get_power_state(fd, NVME_NSID_ALL, 0);
    if (ret)
        return ret;
    ret = dnvme_set_power_state(fd, NVME_NSID_ALL, 0, 2, 0);
    if (ret)
        return ret;
    ret = dnvme_get_power_state(fd, NVME_NSID_ALL, 0);
    if (ret)
        return ret;
    ret = dnvme_set_power_state(fd, NVME_NSID_ALL, 0, 3, 0);
    if (ret)
        return ret;
    ret = dnvme_get_power_state(fd, NVME_NSID_ALL, 0);
    if (ret)
        return ret;
    ret = dnvme_set_power_state(fd, NVME_NSID_ALL, 0, 4, 0);
    if (ret)
        return ret;
    ret = dnvme_get_power_state(fd, NVME_NSID_ALL, 0);
    if (ret)
        return ret;
    ret = dnvme_ring_doorbell(fd, 0);
    if (ret)
        return ret;
    sleep(2);
    memcpy(&ctrl_info, identify_ctrl_buffer, sizeof(struct nvme_id_ctrl));
    memcpy(&ns_info, identify_ns_buffer, sizeof(struct nvme_id_ns));
    show_raw_data((uint8_t *)identify_ctrl_buffer, sizeof(struct nvme_id_ctrl), "Identify controller");
    show_raw_data((uint8_t *)identify_ns_buffer, sizeof(struct nvme_id_ns), "Identify namespace");
    cq_remaining = dnvme_cq_remain(fd, 0);
    cq_buffer_size = cq_remaining*16;
    ret = malloc_4k_aligned_buffer(&cq_buffer, cq_buffer_size, 1);
    if (ret)
        return ret;
    ret = dnvme_cq_reap(fd, 0, cq_remaining, cq_buffer, cq_buffer_size);
    if (ret)
        return ret;
    show_raw_data(cq_buffer, cq_buffer_size, "CQ data");
    return 0;
}


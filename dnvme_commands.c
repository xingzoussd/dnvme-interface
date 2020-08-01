/*
 ************************************************************************
 * FileName: dnvme_commands.c
 * Description: dnvme commands.
 * Author: Xing Zou
 * Date: Jul-30-2020
 ************************************************************************
*/
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <dnvme.h>
#include "dnvme_commands.h"
#include "dnvme_ioctrl.h"

int open_dev(char *dev)
{
    int err, fd;
    struct stat nvme_stat;
    char *devicename = basename(dev);
    err = open(dev, O_RDONLY);
    if (err < 0)
    {
        perror(dev);
        return err;
    }
    fd = err;
    err = fstat(fd, &nvme_stat);
    if (err < 0)
    {
        perror(dev);
        return err;
    }
    if (!S_ISCHR(nvme_stat.st_mode) && !S_ISBLK(nvme_stat.st_mode)) {
        fprintf(stderr, "%s is not a block or character device\n", dev);
        return -ENODEV;
    }
    return fd;
}

int dnvme_create_admin_cq(int fd, uint8_t *buffer, uint32_t buffer_len)
{
    struct nvme_get_q_metrics metrics = {
        .q_id = 0,
        .type = METRICS_CQ,
        .nBytes = buffer_len,
        .buffer = buffer,
    };
    return ioctl_create_admin_cq(fd, &metrics);
}

int dnvme_create_admin_sq(int fd, uint8_t *buffer, uint32_t buffer_len)
{
    struct nvme_get_q_metrics metrics = {
        .q_id = 0,
        .type = METRICS_SQ,
        .nBytes = buffer_len,
        .buffer = buffer,
    };
    return ioctl_create_admin_sq(fd, &metrics);
}

/************************************** Admin commands **************************************/

int dnvme_create_iocq(int fd, uint16_t cq_id, uint16_t irq_no, uint8_t *buffer, uint32_t buffer_len)
{
    int ret = 0;
    struct nvme_prep_cq cmd = {
        .elements = NVME_QUEUE_ELEMENTS,
        .cq_id = cq_id,
        .contig = 1,
    };
    struct nvme_get_q_metrics metrics = {
        .q_id = cq_id,
        .type = METRICS_CQ,
        .nBytes = buffer_len,
        .buffer = buffer,
    };
    ret = ioctl_create_iocq(fd, &cmd, &metrics);
    return ret;
}

int dnvme_create_iosq(int fd, uint16_t sq_id, uint16_t cq_id, uint8_t *buffer, uint32_t buffer_len)
{
    int ret = 0;
    struct nvme_prep_sq cmd = {
        .elements = NVME_QUEUE_ELEMENTS,
        .sq_id = sq_id,
        .cq_id = cq_id,
        .contig = 1,
    };
    struct nvme_get_q_metrics metrics = {
        .q_id = sq_id,
        .type = METRICS_SQ,
        .nBytes = buffer_len,
        .buffer = buffer,
    };
    ret = ioctl_create_iosq(fd, &cmd, &metrics);
    return ret;
}

int dnvme_delete_iosq(int fd, struct nvme_64b_send *cmd)
{
}

int dnvme_delete_iocq(int fd, struct nvme_64b_send *cmd)
{
} 

int dnvme_get_log_page(int fd, struct nvme_64b_send *cmd)
{
}

int dnvme_identify(int fd, struct nvme_64b_send *cmd)
{
}

int dnvme_abort(int fd, struct nvme_64b_send *cmd)
{
}

int dnvme_set_feature(int fd, struct nvme_64b_send *cmd)
{
}

int dnvme_get_feature(int fd, struct nvme_64b_send *cmd)
{
}

int dnvme_async_event_request(int fd, struct nvme_64b_send *cmd)
{
}

int dnvme_namespace_management(int fd, struct nvme_64b_send *cmd)
{
}

int dnvme_firmware_commit(int fd, struct nvme_64b_send *cmd)
{
}

int dnvme_firmware_image_download(int fd, struct nvme_64b_send *cmd)
{
}

int dnvme_device_self_test(int fd, struct nvme_64b_send *cmd)
{
}

int dnvme_namespace_attachment(int fd, struct nvme_64b_send *cmd)
{
}

int dnvme_format_nvm(int fd, struct nvme_64b_send *cmd)
{
}

int dnvme_security_send(int fd, struct nvme_64b_send *cmd)
{
}

int dnvme_security_receive(int fd, struct nvme_64b_send *cmd)
{
}

int dnvme_sanitize_nvm(int fd, struct nvme_64b_send *cmd)
{
}

/************************************** IO commands **************************************/

int dnvme_flush(int fd, struct nvme_64b_send *cmd)
{
}

int dnvme_write(int fd, struct nvme_64b_send *cmd)
{
}

int dnvme_read(int fd, struct nvme_64b_send *cmd)
{
}

int dnvme_write_uncorrectable(int fd, struct nvme_64b_send *cmd)
{
}

int dnvme_compare(int fd, struct nvme_64b_send *cmd)
{
}

int dnvme_write_zeroes(int fd, struct nvme_64b_send *cmd)
{
}

int dnvme_dataset_management(int fd, struct nvme_64b_send *cmd)
{
}









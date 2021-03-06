/*
 ************************************************************************
 * FileName: dnvme_ioctrl.c
 * Description: dnvme ioctl interface.
 * Author: Xing Zou
 * Date: Jul-30-2020
 ************************************************************************
*/

#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include "inc/dnvme_interface.h"
#include "dnvme_ioctrl.h"

int ioctl_send_command(int fd, struct nvme_64b_send *cmd)
{
    return ioctl(fd, NVME_IOCTL_SEND_64B_CMD, cmd);
}

int ioctl_create_admin_cq(int fd)
{
    struct nvme_create_admn_q cmd = {
        .type = ADMIN_CQ,
        .elements = NVME_QUEUE_ELEMENTS,
    };
    return ioctl(fd, NVME_IOCTL_CREATE_ADMN_Q, &cmd);
}

int ioctl_create_admin_sq(int fd)
{
    struct nvme_create_admn_q cmd = {
        .type = ADMIN_SQ,
        .elements = NVME_QUEUE_ELEMENTS,
    };
    return ioctl(fd, NVME_IOCTL_CREATE_ADMN_Q, &cmd);
}

int ioctl_create_iocq(int fd, struct nvme_create_cq *cmd, uint8_t contig)
{
    struct nvme_prep_cq prep_cmd = {
        .elements = cmd->qsize,
        .cq_id = cmd->cqid,
        .contig = contig,
    };
    struct nvme_64b_send user_cmd = {
        .q_id = 0,
        .bit_mask = MASK_PRP1_PAGE,
        .cmd_buf_ptr = (uint8_t *)cmd,
        .data_buf_size = cmd->qsize*NVME_IOCQ_ELEMENT_SIZE,
        .data_buf_ptr = (uint8_t *)cmd->prp1,
        .data_dir = DATA_DIR_TO_DEVICE,
    };
    int ret = ioctl(fd, NVME_IOCTL_PREPARE_CQ_CREATION, &prep_cmd);
    if (ret) return ret;
    ret = ioctl(fd, NVME_IOCTL_SEND_64B_CMD, &user_cmd);
    return ret;
}

int ioctl_create_iosq(int fd, struct nvme_create_sq *cmd, uint8_t contig)
{
    struct nvme_prep_sq prep_cmd = {
        .elements = cmd->qsize,
        .sq_id = cmd->sqid,
        .cq_id = cmd->cqid,
        .contig = contig,
    };
    struct nvme_64b_send user_cmd = {
        .q_id = 0,
        .bit_mask = MASK_PRP1_PAGE,
        .cmd_buf_ptr = (uint8_t *)cmd,
        .data_buf_size = cmd->qsize*NVME_IOSQ_ELEMENT_SIZE,
        .data_buf_ptr = (uint8_t *)cmd->prp1,
        .data_dir = DATA_DIR_TO_DEVICE,
    };
    int ret = ioctl(fd, NVME_IOCTL_PREPARE_SQ_CREATION, &prep_cmd);
    if (ret) return ret;
    ret = ioctl(fd, NVME_IOCTL_SEND_64B_CMD, &user_cmd);
    return ret;
}

int ioctl_delete_ioq(int fd, struct nvme_del_q *cmd)
{
    struct nvme_64b_send user_cmd = {
        .q_id = 0,
        .bit_mask = MASK_NON_PRP,
        .cmd_buf_ptr = (uint8_t *)cmd,
        .data_buf_size = 0,
        .data_buf_ptr = NULL,
        .data_dir = DATA_DIR_FROM_DEVICE,
    };
    return ioctl(fd, NVME_IOCTL_SEND_64B_CMD, &user_cmd);
}

int ioctl_get_log_page(int fd, struct nvme_get_log_page *cmd)
{
    struct nvme_64b_send user_cmd = {
        .q_id = 0,
        .bit_mask = MASK_PRP1_PAGE,
        .cmd_buf_ptr = (uint8_t *)cmd,
        .data_buf_size = 0,
        .data_buf_ptr = NULL,
        .data_dir = DATA_DIR_FROM_DEVICE,
    };
    return ioctl(fd, NVME_IOCTL_SEND_64B_CMD, &user_cmd);
}

int ioctl_identify(int fd, struct nvme_identify *cmd)
{
    struct nvme_64b_send user_cmd = {
        .q_id = 0,
        .bit_mask = MASK_PRP1_PAGE,
        .cmd_buf_ptr = (uint8_t *)cmd,
        .data_buf_size = sizeof(struct nvme_id_ctrl),
        .data_buf_ptr = (uint8_t *)cmd->prp1,
        .data_dir = DATA_DIR_FROM_DEVICE,
    };
    return ioctl(fd, NVME_IOCTL_SEND_64B_CMD, &user_cmd);
}


int ioctl_ring_doorbell(int fd, uint16_t sq_id)
{
    return ioctl(fd, NVME_IOCTL_RING_SQ_DOORBELL, sq_id);
}

void ioctl_drive_metrics(int fd)
{
    struct metrics_driver get_drv_metrics;
    int ret = -1;

    ret = ioctl(fd, NVME_IOCTL_GET_DRIVER_METRICS, &get_drv_metrics);
    if (ret < 0) {
        printf("\tDrive metrics failed!\n");
    }
    printf("Drive Version = 0x%X\n", get_drv_metrics.driver_version);
    printf("Api Version = 0x%X\n", get_drv_metrics.api_version);
}

void ioctl_device_metrics(int fd)
{
    struct public_metrics_dev get_dev_metrics;
    int ret = -1;

    ret = ioctl(fd, NVME_IOCTL_GET_DEVICE_METRICS, &get_dev_metrics);
    if (ret < 0) {
        printf("\tDevice metrics failed!\n");
    }
    printf("\nIRQ Type = %d (0=INTx/1=MSI/2=MSIX/3=NULL)", get_dev_metrics.irq_active.irq_type);
    printf("\nIRQ No's = %d\n", get_dev_metrics.irq_active.num_irqs);
}


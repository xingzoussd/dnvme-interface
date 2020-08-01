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

int ioctl_create_admin_cq(int fd, struct nvme_get_q_metrics *metrics)
{
    struct nvme_create_admn_q cmd = {
        .type = ADMIN_CQ,
        .elements = NVME_QUEUE_ELEMENTS,
    };
    int ret = ioctl(fd, NVME_IOCTL_GET_Q_METRICS, metrics);
    if (ret)
    {
        return ret;
    }
    return ioctl(fd, NVME_IOCTL_CREATE_ADMN_Q, &cmd);
}

int ioctl_create_admin_sq(int fd, struct nvme_get_q_metrics *metrics)
{
    struct nvme_create_admn_q cmd = {
        .type = ADMIN_SQ,
        .elements = NVME_QUEUE_ELEMENTS,
    };
    int ret = ioctl(fd, NVME_IOCTL_GET_Q_METRICS, metrics);
    if (ret)
    {
        return ret;
    }
    return ioctl(fd, NVME_IOCTL_CREATE_ADMN_Q, &cmd);
}

int ioctl_create_iocq(int fd, struct nvme_prep_cq *cmd, struct nvme_get_q_metrics *metrics)
{
    int ret = ioctl(fd, NVME_IOCTL_GET_Q_METRICS, metrics);
    if (ret)
    {
        return ret;
    }
    return ioctl(fd, NVME_IOCTL_PREPARE_CQ_CREATION, cmd);
}

int ioctl_create_iosq(int fd, struct nvme_prep_sq *cmd, struct nvme_get_q_metrics *metrics)
{
    int ret = ioctl(fd, NVME_IOCTL_GET_Q_METRICS, metrics);
    if (ret)
    {
        return ret;
    }
    return ioctl(fd, NVME_IOCTL_PREPARE_SQ_CREATION, cmd);
}

int ioctl_ring_doorbell(int fd, uint16_t sqid)
{
    return ioctl(fd, NVME_IOCTL_RING_SQ_DOORBELL, sqid);
}


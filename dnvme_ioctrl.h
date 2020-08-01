/*
 ************************************************************************
 * FileName: dnvme_ioctrl.h
 * Description: dnvme ioctl interface.
 * Author: Xing Zou
 * Date: Jul-30-2020
 ************************************************************************
*/

#ifndef __DNVME_IOCTRL_H__
#define __DNVME_IOCTRL_H__

#include "inc/dnvme_ioctls.h"
#include "inc/dnvme_interface.h"

int ioctl_send_command(int fd, struct nvme_64b_send *cmd);
int ioctl_create_admin_cq(int fd, struct nvme_get_q_metrics *metrics);
int ioctl_create_admin_sq(int fd, struct nvme_get_q_metrics *metrics);
int ioctl_create_iocq(int fd, struct nvme_prep_cq *cmd, struct nvme_get_q_metrics *metrics);
int ioctl_create_iosq(int fd, struct nvme_prep_sq *cmd, struct nvme_get_q_metrics *metrics);
int ioctl_ring_doorbell(int fd, uint16_t sqid);



#endif


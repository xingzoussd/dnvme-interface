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
int ioctl_create_admin_cq(int fd);
int ioctl_create_admin_sq(int fd);
int ioctl_create_iocq(int fd, struct nvme_create_cq *cmd, uint8_t contig);
int ioctl_create_iosq(int fd, struct nvme_create_sq *cmd, uint8_t contig);
int ioctl_delete_ioq(int fd, struct nvme_del_q *cmd);

int ioctl_identify(int fd, struct nvme_identify *cmd);






int ioctl_ring_doorbell(int fd, uint16_t sq_id);



#endif


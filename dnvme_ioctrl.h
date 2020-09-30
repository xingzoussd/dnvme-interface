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
int ioctl_create_iocq(int fd, struct nvme_admin_cmd *cmd);
int ioctl_create_iosq(int fd, struct nvme_admin_cmd *cmd);
int ioctl_delete_ioq(int fd, struct nvme_admin_cmd *cmd);

int ioctl_identify(int fd, struct nvme_admin_cmd *cmd);
int ioctl_get_log_page(int fd, struct nvme_admin_cmd *cmd);
int ioctl_abort(int fd, struct nvme_admin_cmd *cmd);
int ioctl_set_feature(int fd, struct nvme_set_feature *cmd, uint32_t buffer_size);
int ioctl_get_feature(int fd, struct nvme_get_feature *cmd, uint32_t buffer_size);



int ioctl_set_irq(int fd, struct interrupts *irq);
int ioctl_ring_doorbell(int fd, uint16_t sq_id);
int ioctl_cq_remain(int fd, uint16_t q_id);
int ioctl_cq_reap(int fd, uint16_t q_id, uint16_t remaining, uint8_t *buffer, uint32_t size);


#endif


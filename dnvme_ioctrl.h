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
int ioctl_create_iocq(int fd, struct nvme_admin_cmd *cmd, uint8_t *buffer);
int ioctl_create_iosq(int fd, struct nvme_admin_cmd *cmd, uint8_t *buffer);
int ioctl_delete_ioq(int fd, struct nvme_admin_cmd *cmd);

int ioctl_identify(int fd, struct nvme_admin_cmd *cmd);
int ioctl_get_log_page(int fd, struct nvme_admin_cmd *cmd);
int ioctl_abort(int fd, struct nvme_admin_cmd *cmd);
int ioctl_set_feature(int fd, struct nvme_admin_cmd *cmd, uint32_t buffer_size);
int ioctl_get_feature(int fd, struct nvme_admin_cmd *cmd, uint32_t buffer_size);
int ioctl_async_event_request(int fd, struct nvme_admin_cmd *cmd);
int ioctl_firmware_commit(int fd, struct nvme_admin_cmd *cmd);
int ioctl_firmware_download(int fd, struct nvme_admin_cmd *cmd, uint32_t buffer_size);
int ioctl_device_self_test(int fd, struct nvme_admin_cmd *cmd, uint32_t buffer_size);
int ioctl_format_nvm(int fd, struct nvme_admin_cmd *cmd);
int ioctl_sanitize(int fd, struct nvme_admin_cmd *cmd, uint32_t buffer_size);
int ioctl_compare(int fd, struct nvme_io_cmd *cmd, uint32_t buffer_size, uint16_t qid);
int ioctl_dataset_management(int fd, struct nvme_io_cmd *cmd, uint32_t buffer_size, uint16_t qid);
int ioctl_flush(int fd, struct nvme_io_cmd *cmd, uint16_t qid);
int ioctl_read(int fd, struct nvme_io_cmd *cmd, uint8_t *buffer, uint32_t buffer_size, uint16_t qid);
int ioctl_verify(int fd, struct nvme_io_cmd *cmd, uint32_t buffer_size, uint16_t qid);
int ioctl_write(int fd, struct nvme_io_cmd *cmd, uint32_t buffer_size, uint16_t qid);
int ioctl_write_uncorrectable(int fd, struct nvme_io_cmd *cmd, uint32_t buffer_size, uint16_t qid);
int ioctl_write_zeros(int fd, struct nvme_io_cmd *cmd, uint32_t buffer_size, uint16_t qid);

int ioctl_set_irq(int fd, struct interrupts *irq);
int ioctl_ring_doorbell(int fd, uint16_t sq_id);
int ioctl_cq_remain(int fd, uint16_t q_id);
int ioctl_cq_reap(int fd, uint16_t q_id, uint16_t remaining, uint8_t *buffer, uint32_t size);

void ioctl_drive_metrics(int fd);
void ioctl_device_metrics(int fd);
#endif


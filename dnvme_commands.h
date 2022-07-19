/*
 ************************************************************************
 * FileName: dnvme_commands.h
 * Description: dnvme commands.
 * Author: Xing Zou
 * Date: Jul-31-2020
 ************************************************************************
*/
#ifndef __DNVME_COMMANDS_H__
#define __DNVME_COMMANDS_H__
#include <stdint.h>
#include "inc/dnvme_interface.h"

enum status_type {
    SUCCESS = 0,
    MALLOC_BUFFER_ERROR,
    CREATE_ADMIN_CQ_BUFFER_ERROR,
    CREATE_ADMIN_SQ_BUFFER_ERROR,
    DISABLE_CONTROLLER_ERROR,
    ENABLE_IRQ_ERROR,
    CREATE_ADMIN_CQ_ERROR,
    CREATE_ADMIN_SQ_ERROR,
    ENABLE_CONTROLLER_ERROR,
    INIT_DRIVE_ERROR_MAX
};

int open_dev(char *dev);
int init_drive(int fd);
int dnvme_ring_doorbell(int fd, uint16_t sq_id);
int malloc_4k_aligned_buffer(void **buffer, uint32_t element_size, uint32_t elements);
void* create_buffer(uint32_t element_size, uint32_t elements);
int dump_data(void* buffer, int buffer_len, int index);

int dnvme_controller_enable(int fd);
int dnvme_controller_disable(int fd);
int dnvme_controller_reg_read_block(int fd, uint32_t offset, uint32_t bytes, uint8_t *data);
int dnvme_controller_reg_read_byte(int fd, uint32_t offset, uint8_t *data);
int dnvme_controller_reg_read_word(int fd, uint32_t offset, uint16_t *data);
int dnvme_controller_reg_read_dword(int fd, uint32_t offset, uint32_t *data);
int dnvme_controller_reg_write_block(int fd, uint32_t offset, uint32_t bytes, uint8_t *data);
int dnvme_controller_reg_write_byte(int fd, uint32_t offset, uint8_t *data);
int dnvme_controller_reg_write_word(int fd, uint32_t offset, uint16_t *data);
int dnvme_controller_reg_write_dword(int fd, uint32_t offset, uint32_t *data);
int dnvme_pcie_capability_read_block(int fd, uint32_t offset, uint32_t bytes, uint8_t *data);
int dnvme_pcie_capability_read_byte(int fd, uint32_t offset, uint8_t *data);
int dnvme_pcie_capability_read_word(int fd, uint32_t offset, uint16_t *data);
int dnvme_pcie_capability_read_dword(int fd, uint32_t offset, uint32_t *data);
int dnvme_pcie_capability_write_block(int fd, uint32_t offset, uint32_t bytes, uint8_t *data);
int dnvme_pcie_capability_write_byte(int fd, uint32_t offset, uint8_t *data);
int dnvme_pcie_capability_write_word(int fd, uint32_t offset, uint16_t *data);
int dnvme_pcie_capability_write_dword(int fd, uint32_t offset, uint32_t *data);
uint16_t dnvme_pcie_msix_capability(int fd);
uint16_t dnvme_pcie_msix_get_entry_count(int fd, uint16_t msix_cap);
void dnvme_pcie_msix_enable(int fd, uint16_t msix_cap);


int dnvme_admin_create_iocq(int fd, uint32_t nsid, uint16_t cq_id, uint16_t int_no, uint16_t qsize, uint8_t contig, void *buffer);
int dnvme_admin_create_iosq(int fd, uint32_t nsid, uint16_t sq_id, uint16_t cq_id, uint16_t qsize, uint8_t contig, void *buffer);
int dnvme_admin_identify(int fd, uint32_t nsid, uint16_t ctrl_id, int cns, uint8_t *buffer);
int dnvme_admin_identify_ctrl(int fd, uint32_t nsid, uint16_t ctrl_id, uint8_t *buffer);
int dnvme_admin_identify_ns(int fd, uint32_t nsid, uint16_t ctrl_id, uint8_t *buffer);
int dnvme_admin_abort(int fd, uint32_t nsid, uint16_t sq_id, uint16_t cmd_id);
int dnvme_admin_set_feature(
    int fd,
    uint32_t nsid,
    uint16_t feature_id,
    uint8_t save,
    uint32_t dw11,
    uint32_t dw12,
    uint32_t dw13,
    uint32_t dw14,
    uint32_t dw15,
    uint8_t *buffer,
    uint32_t buffer_size);
int dnvme_set_power_state(int fd, uint32_t nsid, uint8_t save, uint8_t ps, uint8_t wh);
int dnvme_admin_get_feature(
    int fd,
    uint32_t nsid,
    uint16_t feature_id,
    uint8_t select,
    uint32_t dw11,
    uint32_t dw12,
    uint32_t dw13,
    uint32_t dw14,
    uint32_t dw15,
    uint8_t *buffer,
    uint32_t buffer_size);
int dnvme_get_power_state(int fd, uint32_t nsid, uint8_t select);
int dnvme_admin_async_event_request(int fd, uint32_t nsid);
int dnvme_admin_firmware_commit(int fd, uint32_t nsid, uint32_t fs, uint32_t ca, uint32_t bpid);
int dnvme_admin_firmware_image_download(int fd, uint32_t nsid, uint32_t numd, uint32_t ofst, uint8_t *buffer, uint32_t buffer_size);
int dnvme_admin_device_self_test(int fd, uint32_t nsid, uint32_t stc, uint8_t *buffer, uint32_t buffer_size);


int dnvme_cq_remain(int fd, uint16_t q_id);
int dnvme_cq_reap(int fd, uint16_t q_id, uint16_t remaining, uint8_t *buffer, uint32_t size);


#endif


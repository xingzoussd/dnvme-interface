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

enum malloc_buffer_type {
    SUCCESS = 0,
    FAILURE = -1
};


int open_dev(char *dev);
int malloc_4k_aligned_buffer(void **buffer, uint32_t element_size, uint32_t elements);

int dnvme_controller_enable(int fd);
int dnvme_controller_disable(int fd);
int dnvme_controller_reg_read_block(int fd, uint32_t offset, uint32_t bytes, uint8_t *data);
int dnvme_controller_reg_read_byte(int fd, uint32_t offset, uint8_t *data);
int dnvme_controller_reg_read_word(int fd, uint32_t offset, uint8_t *data);
int dnvme_controller_reg_read_dword(int fd, uint32_t offset, uint8_t *data);
int dnvme_controller_reg_write_block(int fd, uint32_t offset, uint32_t bytes, uint8_t *data);
int dnvme_controller_reg_write_byte(int fd, uint32_t offset, uint8_t *data);
int dnvme_controller_reg_write_word(int fd, uint32_t offset, uint8_t *data);
int dnvme_controller_reg_write_dword(int fd, uint32_t offset, uint8_t *data);
int dnvme_pcie_capability_read_block(int fd, uint32_t offset, uint32_t bytes, uint8_t *data);
int dnvme_pcie_capability_read_byte(int fd, uint32_t offset, uint8_t *data);
int dnvme_pcie_capability_read_word(int fd, uint32_t offset, uint8_t *data);
int dnvme_pcie_capability_read_dword(int fd, uint32_t offset, uint8_t *data);
int dnvme_pcie_capability_write_block(int fd, uint32_t offset, uint32_t bytes, uint8_t *data);
int dnvme_pcie_capability_write_byte(int fd, uint32_t offset, uint8_t *data);
int dnvme_pcie_capability_write_word(int fd, uint32_t offset, uint8_t *data);
int dnvme_pcie_capability_write_dword(int fd, uint32_t offset, uint8_t *data);


int dnvme_create_admin_cq(int fd);
int dnvme_create_admin_sq(int fd);
int dnvme_create_iocq(int fd, uint16_t cq_id, uint16_t irq_no, uint16_t qsize, uint8_t contig, void *buffer);
int dnvme_create_iosq(int fd, uint16_t sq_id, uint16_t cq_id, uint16_t qsize, uint8_t contig, void *buffer);
int dnvme_identify_ctrl(int fd, uint16_t ctrl_id, uint8_t *buffer);
int dnvme_identify_ns(int fd, uint16_t ctrl_id, uint32_t nsid, uint8_t *buffer);

#endif


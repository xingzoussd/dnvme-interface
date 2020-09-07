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

int dnvme_create_admin_cq(int fd)
{
    return ioctl_create_admin_cq(fd);
}

int dnvme_create_admin_sq(int fd)
{
    return ioctl_create_admin_sq(fd);
}

int malloc_4k_aligned_buffer(void **buffer, uint32_t element_size, uint32_t elements)
{
    *buffer = NULL;
    posix_memalign(buffer, 4096, element_size*elements);
    if (*buffer)
    {
        return SUCCESS;
    }
    return FAILURE;
}

/************************************* Register commands ************************************/
int dnvme_controller_enable(int fd)
{
    return ioctl(fd, NVME_IOCTL_DEVICE_STATE, ST_ENABLE);
}

int dnvme_controller_disable(int fd)
{
    return ioctl(fd, NVME_IOCTL_DEVICE_STATE, ST_DISABLE);
}

int dnvme_controller_reg_read_block(int fd, uint32_t offset, uint32_t bytes, uint8_t *data)
{
    struct rw_generic read_param = {
        .type = NVMEIO_BAR01,
        .offset = offset,
        .nBytes = bytes,
        .acc_type = BYTE_LEN,
        .buffer = data,
    };
    return ioctl(fd, NVME_IOCTL_READ_GENERIC, &read_param);
}

int dnvme_controller_reg_read_byte(int fd, uint32_t offset, uint8_t *data)
{
    return dnvme_controller_reg_read_block(fd, offset, 1, data);
}

int dnvme_controller_reg_read_word(int fd, uint32_t offset, uint8_t *data)
{
    return dnvme_controller_reg_read_block(fd, offset, 2, data);
}

int dnvme_controller_reg_read_dword(int fd, uint32_t offset, uint8_t *data)
{
    return dnvme_controller_reg_read_block(fd, offset, 4, data);
}

int dnvme_controller_reg_write_block(int fd, uint32_t offset, uint32_t bytes, uint8_t *data)
{
    struct rw_generic write_param = {
        .type = NVMEIO_BAR01,
        .offset = offset,
        .nBytes = bytes,
        .acc_type = BYTE_LEN,
        .buffer = data,
    };
    return ioctl(fd, NVME_IOCTL_WRITE_GENERIC, &write_param);
}

int dnvme_controller_reg_write_byte(int fd, uint32_t offset, uint8_t *data)
{
    return dnvme_controller_reg_write_block(fd, offset, 1, data);
}

int dnvme_controller_reg_write_word(int fd, uint32_t offset, uint8_t *data)
{
    return dnvme_controller_reg_write_block(fd, offset, 2, data);
}

int dnvme_controller_reg_write_dword(int fd, uint32_t offset, uint8_t *data)
{
    return dnvme_controller_reg_write_block(fd, offset, 4, data);
}

int dnvme_pcie_capability_read_block(int fd, uint32_t offset, uint32_t bytes, uint8_t *data)
{
    struct rw_generic read_param = {
        .type = NVMEIO_PCI_HDR,
        .offset = offset,
        .nBytes = bytes,
        .acc_type = BYTE_LEN,
        .buffer = data,
    };
    return ioctl(fd, NVME_IOCTL_READ_GENERIC, &read_param);
}

int dnvme_pcie_capability_read_byte(int fd, uint32_t offset, uint8_t *data)
{
    return dnvme_pcie_capability_read_block(fd, offset, 1, data);
}

int dnvme_pcie_capability_read_word(int fd, uint32_t offset, uint8_t *data)
{
    return dnvme_pcie_capability_read_block(fd, offset, 2, data);
}

int dnvme_pcie_capability_read_dword(int fd, uint32_t offset, uint8_t *data)
{
    return dnvme_pcie_capability_read_block(fd, offset, 4, data);
}

int dnvme_pcie_capability_write_block(int fd, uint32_t offset, uint32_t bytes, uint8_t *data)
{
    struct rw_generic write_param = {
        .type = NVMEIO_PCI_HDR,
        .offset = offset,
        .nBytes = bytes,
        .acc_type = BYTE_LEN,
        .buffer = data,
    };
    return ioctl(fd, NVME_IOCTL_WRITE_GENERIC, &write_param);
}

int dnvme_pcie_capability_write_byte(int fd, uint32_t offset, uint8_t *data)
{
    return dnvme_pcie_capability_write_block(fd, offset, 1, data);
}

int dnvme_pcie_capability_write_word(int fd, uint32_t offset, uint8_t *data)
{
    return dnvme_pcie_capability_write_block(fd, offset, 2, data);
}

int dnvme_pcie_capability_write_dword(int fd, uint32_t offset, uint8_t *data)
{
    return dnvme_pcie_capability_write_block(fd, offset, 4, data);
}

/************************************** Admin commands **************************************/

int dnvme_admin_create_iocq(int fd, uint16_t cq_id, uint16_t irq_no, uint16_t qsize, uint8_t contig, void *buffer)
{
    struct nvme_create_cq cmd = {
        .opcode = NVME_ADMIN_CREATE_IOCQ,
        .flags = 0,
        .prp1 = (uint64_t)buffer,
        .cqid = cq_id,
        .qsize = qsize,
        .cq_flags = 1,
        .irq_no = irq_no,
    };
    int ret = ioctl_create_iocq(fd, &cmd, contig);
    return ret;
}

int dnvme_admin_create_iosq(int fd, uint16_t sq_id, uint16_t cq_id, uint16_t qsize, uint8_t contig, void *buffer)
{
    struct nvme_create_sq cmd = {
        .opcode = NVME_ADMIN_CREATE_IOSQ,
        .flags = 0,
        .prp1 = (uint64_t)buffer,
        .sqid = sq_id,
        .qsize = qsize,
        .sq_flags = 1,
        .cqid = cq_id,
    };
    int ret = ioctl_create_iosq(fd, &cmd, contig);
    return ret;
}

int dnvme_admin_delete_iosq(int fd, uint16_t sq_id)
{
    struct nvme_del_q cmd = {
        .opcode = NVME_ADMIN_DELETE_IOSQ,
        .qid = sq_id,
    };
    return ioctl_delete_ioq(fd, &cmd);
}

int dnvme_admin_delete_iocq(int fd, uint16_t cq_id)
{
    struct nvme_del_q cmd = {
        .opcode = NVME_ADMIN_DELETE_IOCQ,
        .qid = cq_id,
    };
    return ioctl_delete_ioq(fd, &cmd);
}

int dnvme_admin_get_log_page(int fd, struct nvme_64b_send *cmd)
{
    int ret = 0;
    return ret;
}

int dnvme_admin_identify_ctrl(int fd, uint16_t ctrl_id, uint8_t *buffer)
{
    struct nvme_identify cmd = {
        .opcode = NVME_ADMIN_IDENTIFY,
        .nsid = 0,
        .cns = NVME_ID_CNS_CTRL,
        .ctrl_id = 0,
        .prp1 = (uint64_t)buffer,
    };
    return ioctl_identify(fd, &cmd);
}

int dnvme_admin_identify_ns(int fd, uint16_t ctrl_id, uint32_t nsid, uint8_t *buffer)
{
    struct nvme_identify cmd = {
        .opcode = NVME_ADMIN_IDENTIFY,
        .nsid = nsid,
        .cns = NVME_ID_CNS_NS,
        .ctrl_id = ctrl_id,
        .prp1 = (uint64_t)buffer,
    };
    return ioctl_identify(fd, &cmd);
}

int dnvme_admin_abort(int fd, uint16_t sq_id, uint16_t cmd_id)
{
    struct nvme_abort cmd = {
        .opcode = NVME_ADMIN_ABORT,
        .sq_id = sq_id,
        .cmd_id = cmd_id,
    };
    return ioctl_abort(fd, &cmd);
}

int dnvme_admin_set_feature(int fd, uint32_t nsid, uint16_t feature_id, uint8_t save, uint32_t dw11, uint8_t *buffer, uint32_t buffer_size)
{
    struct nvme_set_feature cmd = {
        .opcode = NVME_ADMIN_SET_FEATURE,
        .nsid = nsid,
        .dw10 = {
            .feature_id = feature_id,
            .save = save,
        },
        .dw11 = dw11,
        .prp1 = (uint64_t)buffer,
    };
    return ioctl_set_feature(fd, &cmd, buffer_size);
}

int dnvme_set_power_state(int fd, uint32_t nsid, uint8_t save, uint8_t ps, uint8_t wh)
{
    uint32_t dw11 = (ps&0x1F)|((wh&0x7)<<5);
    return dnvme_admin_set_feature(fd, nsid, NVME_FEATURE_POWER_MANAGEMENT, save, dw11, NULL, 0);
}

int dnvme_admin_get_feature(int fd, uint32_t nsid, uint16_t feature_id, uint8_t select, uint8_t *buffer, uint32_t buffer_size)
{
    struct nvme_get_feature cmd = {
        .opcode = NVME_ADMIN_GET_FEATURE,
        .nsid = nsid,
        .dw10 = {
            .feature_id = feature_id,
            .select = select,
        },
        .prp1 = (uint64_t)buffer,
    };
    return ioctl_get_feature(fd, &cmd, buffer_size);
}

int dnvme_get_power_state(int fd, uint32_t nsid, uint8_t select)
{
    return dnvme_admin_get_feature(fd, nsid, NVME_FEATURE_POWER_MANAGEMENT, select, NULL, 0);
}

int dnvme_admin_async_event_request(int fd, struct nvme_64b_send *cmd)
{
    int ret = 0;
    return ret;
}

int dnvme_admin_namespace_management(int fd, struct nvme_64b_send *cmd)
{
    int ret = 0;
    return ret;
}

int dnvme_admin_firmware_commit(int fd, struct nvme_64b_send *cmd)
{
    int ret = 0;
    return ret;
}

int dnvme_admin_firmware_image_download(int fd, struct nvme_64b_send *cmd)
{
    int ret = 0;
    return ret;
}

int dnvme_admin_device_self_test(int fd, struct nvme_64b_send *cmd)
{
    int ret = 0;
    return ret;
}

int dnvme_admin_namespace_attachment(int fd, struct nvme_64b_send *cmd)
{
    int ret = 0;
    return ret;
}

int dnvme_admin_format_nvm(int fd, struct nvme_64b_send *cmd)
{
    int ret = 0;
    return ret;
}

int dnvme_admin_security_send(int fd, struct nvme_64b_send *cmd)
{
    int ret = 0;
    return ret;
}

int dnvme_admin_security_receive(int fd, struct nvme_64b_send *cmd)
{
    int ret = 0;
    return ret;
}

int dnvme_admin_sanitize_nvm(int fd, struct nvme_64b_send *cmd)
{
    int ret = 0;
    return ret;
}

/************************************** IO commands **************************************/

int dnvme_nvm_flush(int fd, struct nvme_64b_send *cmd)
{
    int ret = 0;
    return ret;
}

int dnvme_nvm_write(int fd, struct nvme_64b_send *cmd)
{
    int ret = 0;
    return ret;
}

int dnvme_nvm_read(int fd, struct nvme_64b_send *cmd)
{
    int ret = 0;
    return ret;
}

int dnvme_nvm_write_uncorrectable(int fd, struct nvme_64b_send *cmd)
{
    int ret = 0;
    return ret;
}

int dnvme_nvm_compare(int fd, struct nvme_64b_send *cmd)
{
    int ret = 0;
    return ret;
}

int dnvme_nvm_write_zeroes(int fd, struct nvme_64b_send *cmd)
{
    int ret = 0;
    return ret;
}

int dnvme_nvm_dataset_management(int fd, struct nvme_64b_send *cmd)
{
    int ret = 0;
    return ret;
}

int dnvme_cq_remain(int fd, uint16_t q_id)
{
    return ioctl_cq_remain(fd, q_id);
}

int dnvme_cq_reap(int fd, uint16_t q_id, uint16_t remaining, uint8_t *buffer, uint32_t size)
{
    return ioctl_cq_reap(fd, q_id, remaining, buffer, size);
}




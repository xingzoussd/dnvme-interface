/*
 ************************************************************************
 * FileName: dnvme.h
 * Description: nvme commands definition.
 * Author: Xing Zou
 * Date: Aug-1-2020
 ************************************************************************
*/

#ifndef __DNVME_H__
#define __DNVME_H__

#define NVME_NSID_ALL       0xffffffff
#define NVME_NSSR_VALUE     0x4E564D65

enum controller_reg {
    NVME_REG_CAP    = 0x0000,   /* Controller Capabilities */
    NVME_REG_VS     = 0x0008,   /* Version */
    NVME_REG_INTMS  = 0x000c,   /* Interrupt Mask Set */
    NVME_REG_INTMC  = 0x0010,   /* Interrupt Mask Clear */
    NVME_REG_CC     = 0x0014,   /* Controller Configuration */
    NVME_REG_CSTS   = 0x001c,   /* Controller Status */
    NVME_REG_NSSR   = 0x0020,   /* NVM Subsystem Reset */
    NVME_REG_AQA    = 0x0024,   /* Admin Queue Attributes */
    NVME_REG_ASQ    = 0x0028,   /* Admin SQ Base Address */
    NVME_REG_ACQ    = 0x0030,   /* Admin CQ Base Address */
    NVME_REG_CMBLOC = 0x0038,   /* Controller Memory Buffer Location */
    NVME_REG_CMBSZ  = 0x003c,   /* Controller Memory Buffer Size */
    NVME_REG_BPINFO = 0x0040,   /* Boot Partition Information */
    NVME_REG_BPRSEL = 0x0044,   /* Boot Partition Read Select */
    NVME_REG_BPMBL  = 0x0048,   /* Boot Partition Memory Buffer Location */
    NVME_REG_DBS    = 0x1000,   /* SQ 0 Tail Doorbell */
};

#define NVME_CAP_MQES(cap)      ((cap) & 0xffff)
#define NVME_CAP_TIMEOUT(cap)   (((cap) >> 24) & 0xff)
#define NVME_CAP_STRIDE(cap)    (((cap) >> 32) & 0xf)
#define NVME_CAP_NSSRC(cap)     (((cap) >> 36) & 0x1)
#define NVME_CAP_MPSMIN(cap)    (((cap) >> 48) & 0xf)
#define NVME_CAP_MPSMAX(cap)    (((cap) >> 52) & 0xf)

#define NVME_CMB_BIR(cmbloc)    ((cmbloc) & 0x7)
#define NVME_CMB_OFST(cmbloc)   (((cmbloc) >> 12) & 0xfffff)
#define NVME_CMB_SZ(cmbsz)      (((cmbsz) >> 12) & 0xfffff)
#define NVME_CMB_SZU(cmbsz)     (((cmbsz) >> 8) & 0xf)

#define NVME_CMB_WDS(cmbsz)     ((cmbsz) & 0x10)
#define NVME_CMB_RDS(cmbsz)     ((cmbsz) & 0x8)
#define NVME_CMB_LISTS(cmbsz)   ((cmbsz) & 0x4)
#define NVME_CMB_CQS(cmbsz)     ((cmbsz) & 0x2)
#define NVME_CMB_SQS(cmbsz)     ((cmbsz) & 0x1)

/*
 * Submission and Completion Queue Entry Sizes for the NVM command set.
 * (In bytes and specified as a power of two (2^n)).
 */
#define NVME_NVM_IOSQES     6
#define NVME_NVM_IOCQES     4

enum controller_control {
    NVME_CC_ENABLE          = (1 << 0),
    NVME_CC_CSS_NVM         = (0 << 4),
    NVME_CC_EN_SHIFT        = 0,
    NVME_CC_CSS_SHIFT       = 4,
    NVME_CC_MPS_SHIFT       = 7,
    NVME_CC_AMS_SHIFT       = 11,
    NVME_CC_SHN_SHIFT       = 14,
    NVME_CC_IOSQES_SHIFT    = 16,
    NVME_CC_IOCQES_SHIFT    = 20,
    NVME_CC_AMS_RR          = (0 << NVME_CC_AMS_SHIFT),
    NVME_CC_AMS_WRRU        = (1 << NVME_CC_AMS_SHIFT),
    NVME_CC_AMS_VS          = (7 << NVME_CC_AMS_SHIFT),
    NVME_CC_SHN_NONE        = (0 << NVME_CC_SHN_SHIFT),
    NVME_CC_SHN_NORMAL      = (1 << NVME_CC_SHN_SHIFT),
    NVME_CC_SHN_ABRUPT      = (2 << NVME_CC_SHN_SHIFT),
    NVME_CC_SHN_MASK        = (3 << NVME_CC_SHN_SHIFT),
    NVME_CC_IOSQES          = (NVME_NVM_IOSQES << NVME_CC_IOSQES_SHIFT),
    NVME_CC_IOCQES          = (NVME_NVM_IOCQES << NVME_CC_IOCQES_SHIFT),
};

enum controller_status {
    NVME_CSTS_RDY           = (1 << 0),
    NVME_CSTS_CFS           = (1 << 1),
    NVME_CSTS_NSSRO         = (1 << 4),
    NVME_CSTS_PP            = (1 << 5),
    NVME_CSTS_SHST_NORMAL   = (0 << 2),
    NVME_CSTS_SHST_OCCUR    = (1 << 2),
    NVME_CSTS_SHST_CMPLT    = (2 << 2),
    NVME_CSTS_SHST_MASK     = (3 << 2),
};

enum identify_cns {
    NVME_ID_CNS_NS              = 0x00,
    NVME_ID_CNS_CTRL            = 0x01,
    NVME_ID_CNS_NS_ACTIVE_LIST  = 0x02,
    NVME_ID_CNS_NS_DESC_LIST    = 0x03,
    NVME_ID_CNS_NVMSET_LIST     = 0x04,
    NVME_ID_CNS_NS_PRESENT_LIST = 0x10,
    NVME_ID_CNS_NS_PRESENT      = 0x11,
    NVME_ID_CNS_CTRL_NS_LIST    = 0x12,
    NVME_ID_CNS_CTRL_LIST       = 0x13,
};

enum {
    NVME_DIR_IDENTIFY           = 0x00,
    NVME_DIR_STREAMS            = 0x01,
    NVME_DIR_SND_ID_OP_ENABLE   = 0x01,
    NVME_DIR_SND_ST_OP_REL_ID   = 0x01,
    NVME_DIR_SND_ST_OP_REL_RSC  = 0x02,
    NVME_DIR_RCV_ID_OP_PARAM    = 0x01,
    NVME_DIR_RCV_ST_OP_PARAM    = 0x01,
    NVME_DIR_RCV_ST_OP_STATUS   = 0x02,
    NVME_DIR_RCV_ST_OP_RESOURCE = 0x03,
    NVME_DIR_ENDIR              = 0x01,
};

enum {
    NVME_NS_FEAT_THIN       = (1 << 0),
    NVME_NS_FLBAS_LBA_MASK  = 0xf,
    NVME_NS_FLBAS_META_EXT  = 0x10,
    NVME_LBAF_RP_BEST       = 0,
    NVME_LBAF_RP_BETTER     = 1,
    NVME_LBAF_RP_GOOD       = 2,
    NVME_LBAF_RP_DEGRADED   = 3,
    NVME_NS_DPC_PI_LAST     = (1 << 4),
    NVME_NS_DPC_PI_FIRST    = (1 << 3),
    NVME_NS_DPC_PI_TYPE3    = (1 << 2),
    NVME_NS_DPC_PI_TYPE2    = (1 << 1),
    NVME_NS_DPC_PI_TYPE1    = (1 << 0),
    NVME_NS_DPS_PI_FIRST    = (1 << 3),
    NVME_NS_DPS_PI_MASK     = 0x7,
    NVME_NS_DPS_PI_TYPE1    = 1,
    NVME_NS_DPS_PI_TYPE2    = 2,
    NVME_NS_DPS_PI_TYPE3    = 3,
};

enum nvme_async_event_type {
    NVME_AER_TYPE_ERROR     = 0,
    NVME_AER_TYPE_SMART     = 1,
    NVME_AER_TYPE_NOTICE    = 2,
};

/* I/O commands */

enum nvme_io_opcode {
    NVME_CMD_FLUSH                  = 0x00,
    NVME_CMD_WRITE                  = 0x01,
    NVME_CMD_READ                   = 0x02,
    NVME_CMD_WRITE_UNCORRECTABLE    = 0x04,
    NVME_CMD_COMPARE                = 0x05,
    NVME_CMD_WRITE_ZEROES           = 0x08,
    NVME_CMD_DATASET_MANAGEMENT     = 0x09,
    NVME_CMD_RESV_REGISTER          = 0x0D,
    NVME_CMD_RESV_REPORT            = 0x0E,
    NVME_CMD_RESV_ACQUIRE           = 0x11,
    NVME_CMD_RESV_RELEASE           = 0x15,
};


/* Admin commands */

enum nvme_admin_opcode {
    NVME_ADMIN_DELETE_IOSQ                  = 0x00,
    NVME_ADMIN_CREATE_IOSQ                  = 0x01,
    NVME_ADMIN_GET_LOG_PAGE                 = 0x02,
    NVME_ADMIN_DELETE_IOCQ                  = 0x04,
    NVME_ADMIN_CREATE_IOCQ                  = 0x05,
    NVME_ADMIN_IDENTIFY                     = 0x06,
    NVME_ADMIN_ABORT                        = 0x08,
    NVME_ADMIN_SET_FEATURE                  = 0x09,
    NVME_ADMIN_GET_FEATURE                  = 0x0A,
    NVME_ADMIN_ASYNC_EVENT_REQUEST          = 0x0C,
    NVME_ADMIN_NAMESPACE_MANAGEMENT         = 0x0D,
    NVME_ADMIN_FIRMWARE_COMMIT              = 0x10,
    NVME_ADMIN_FIRMWARE_IMAGE_DOWNLOAD      = 0x11,
    NVME_ADMIN_DEVICE_SELF_TEST             = 0x14,
    NVME_ADMIN_NAMESPACE_ATTACHMENT         = 0x15,
    NVME_ADMIN_KEEP_ALIVE                   = 0x18,
    NVME_ADMIN_DIRECTIVE_SEND               = 0x19,
    NVME_ADMIN_DIRECTIVE_RECEIVE            = 0x1A,
    NVME_ADMIN_VIRTUAL_MANAGEMENT           = 0x1C,
    NVME_ADMIN_NVME_MI_SEND                 = 0x1D,
    NVME_ADMIN_NVME_MI_RECEIVE              = 0x1E,
    NVME_ADMIN_DBBUF                        = 0x7C,
    NVME_ADMIN_FORMAT_NVM                   = 0x80,
    NVME_ADMIN_SECURITY_SEND                = 0x81,
    NVME_ADMIN_SECURITY_RECV                = 0x82,
    NVME_ADMIN_SANITIZE                     = 0x84,
};

enum {
    NVME_QUEUE_PHYS_CONTIG          = (1 << 0),
    NVME_CQ_IRQ_ENABLED             = (1 << 1),
};

enum queue_arbitration {
    NVME_SQ_PRIO_URGENT             = (0 << 1),
    NVME_SQ_PRIO_HIGH               = (1 << 1),
    NVME_SQ_PRIO_MEDIUM             = (2 << 1),
    NVME_SQ_PRIO_LOW                = (3 << 1),
};

enum feature_id {
    NVME_FEATURE_ARBITRATION                            = 0x01,
    NVME_FEATURE_POWER_MANAGEMENT                       = 0x02,
    NVME_FEATURE_LBA_RANGE                              = 0x03,
    NVME_FEATURE_TEMPERATURE_THRESHOLD                  = 0x04,
    NVME_FEATURE_ERROR_RECOVERY                         = 0x05,
    NVME_FEATURE_VOLATILE_WC                            = 0x06,
    NVME_FEATURE_NUM_QUEUES                             = 0x07,
    NVME_FEATURE_IRQ_COALESCE                           = 0x08,
    NVME_FEATURE_IRQ_CONFIG                             = 0x09,
    NVME_FEATURE_WRITE_ATOMIC                           = 0x0a,
    NVME_FEATURE_ASYNC_EVENT_CONFIG                     = 0x0b,
    NVME_FEATURE_AUTONOMOUS_POWER_STATE_TRANSITION      = 0x0c,
    NVME_FEATURE_HOST_MEMORY_BUFFER                     = 0x0d,
    NVME_FEATURE_TIMESTAMP                              = 0x0e,
    NVME_FEATURE_KATO                                   = 0x0f,
    NVME_FEATURE_HCTM                                   = 0X10,
    NVME_FEATURE_NON_OPERATIONAL_POWER_STATE_CONFIG     = 0X11,
    NVME_FEATURE_RRL                                    = 0x12,
    NVME_FEATURE_PLM_CONFIG                             = 0x13,
    NVME_FEATURE_PLM_WINDOW                             = 0x14,
    NVME_FEATURE_SW_PROGRESS                            = 0x80,
    NVME_FEATURE_HOST_ID                                = 0x81,
    NVME_FEATURE_RESV_MASK                              = 0x82,
    NVME_FEATURE_RESV_PERSIST                           = 0x83,
    NVME_FEATURE_WRITE_PROTECT                          = 0x84,
};

enum log_page_id {
    NVME_LOG_ERROR                  = 0x01,
    NVME_LOG_SMART                  = 0x02,
    NVME_LOG_FIRMWARE_SLOT          = 0x03,
    NVME_LOG_CHANGED_NAMESPACE      = 0x04,
    NVME_LOG_CMD_EFFECTS            = 0x05,
    NVME_LOG_DEVICE_SELF_TEST       = 0x06,
    NVME_LOG_TELEMETRY_HOST         = 0x07,
    NVME_LOG_TELEMETRY_CTRL         = 0x08,
    NVME_LOG_ENDURANCE_GROUP        = 0x09,
    NVME_LOG_ANA                    = 0x0c,
    NVME_LOG_DISC                   = 0x70,
    NVME_LOG_RESERVATION            = 0x80,
    NVME_LOG_SANITIZE               = 0x81,
};

enum firmware_commit_action {
    NVME_FIRMWARE_COMMIT_REPLACE            = (0 << 3),
    NVME_FIRMWARE_COMMIT_REPLACE_ACTIVE     = (1 << 3),
    NVME_FIRMWARE_COMMIT_ACTIVE             = (2 << 3),
};

enum {
    NVME_NO_LOG_LSP       = 0x0,
    NVME_NO_LOG_LPO       = 0x0,
    NVME_LOG_ANA_LSP_RGO  = 0x1,
    NVME_TELEM_LSP_CREATE = 0x1,
};

enum sanitize_action {
    NVME_SANITIZE_ACTION_EXIT           = 0x1,
    NVME_SANITIZE_ACTION_BLOCK_ERASE    = 0x2,
    NVME_SANITIZE_ACTION_OVERWRITE      = 0x3,
    NVME_SANITIZE_ACTION_CRYPTO_ERASE   = 0x4,
};


enum sanitize_parameter {
    NVME_SANITIZE_NO_DEALLOC                = 0x0200,
    NVME_SANITIZE_OIPBP                     = 0x0100,
    NVME_SANITIZE_OWPASS_SHIFT              = 0x0004,
    NVME_SANITIZE_AUSE                      = 0x0008,

    NVME_SANITIZE_LOG_DATA_LEN              = 0x0014,
    NVME_SANITIZE_LOG_GLOBAL_DATA_ERASED    = 0x0100,
    NVME_SANITIZE_LOG_NUM_CMPLTED_PASS_MASK = 0x00F8,
    NVME_SANITIZE_LOG_STATUS_MASK           = 0x0007,
    NVME_SANITIZE_LOG_NEVER_SANITIZED       = 0x0000,
    NVME_SANITIZE_LOG_COMPLETED_SUCCESS     = 0x0001,
    NVME_SANITIZE_LOG_IN_PROGESS            = 0x0002,
    NVME_SANITIZE_LOG_COMPLETED_FAILED      = 0x0003,
};

enum device_self_test_info {
    NVME_DEVICE_SELF_TEST_VALID_NSID   = (1 << 0),
    NVME_DEVICE_SELF_TEST_VALID_FLBA   = (1 << 1),
    NVME_DEVICE_SELF_TEST_VALID_SCT    = (1 << 2),
    NVME_DEVICE_SELF_TEST_VALID_SC     = (1 << 3),
    NVME_DEVICE_SELF_TEST_REPORTS      = 20,
};

enum {
    /* Generic Command Status */
    NVME_SC_SUCCESS                     = 0x0,
    NVME_SC_INVALID_OPCODE              = 0x1,
    NVME_SC_INVALID_FIELD               = 0x2,
    NVME_SC_CMDID_CONFLICT              = 0x3,
    NVME_SC_DATA_TRANSFER_ERROR         = 0x4,
    NVME_SC_POWER_LOSS                  = 0x5,
    NVME_SC_INTERNAL                    = 0x6,
    NVME_SC_ABORT_REQ                   = 0x7,
    NVME_SC_ABORT_QUEUE                 = 0x8,
    NVME_SC_FUSED_FAIL                  = 0x9,
    NVME_SC_FUSED_MISSING               = 0xa,
    NVME_SC_INVALID_NAMESPACE           = 0xb,
    NVME_SC_CMD_SEQ_ERROR               = 0xc,
    NVME_SC_SGL_INVALID_LAST            = 0xd,
    NVME_SC_SGL_INVALID_COUNT           = 0xe,
    NVME_SC_SGL_INVALID_DATA            = 0xf,
    NVME_SC_SGL_INVALID_METADATA        = 0x10,
    NVME_SC_SGL_INVALID_TYPE            = 0x11,
    NVME_SC_SGL_INVALID_OFFSET          = 0x16,
    NVME_SC_SGL_INVALID_SUBTYPE         = 0x17,
    NVME_SC_SANITIZE_FAILED             = 0x1C,
    NVME_SC_SANITIZE_IN_PROGRESS        = 0x1D,
    NVME_SC_NAMESPACE_WRITE_PROTECTED   = 0x20,
    NVME_SC_LBA_RANGE                   = 0x80,
    NVME_SC_CAP_EXCEEDED                = 0x81,
    NVME_SC_NAMESPACE_NOT_READY         = 0x82,
    NVME_SC_RESERVATION_CONFLICT        = 0x83,

    /* Command Specific Status */
    NVME_SC_CQ_INVALID                  = 0x100,
    NVME_SC_QID_INVALID                 = 0x101,
    NVME_SC_QUEUE_SIZE                  = 0x102,
    NVME_SC_ABORT_LIMIT                 = 0x103,
    NVME_SC_ABORT_MISSING               = 0x104,
    NVME_SC_ASYNC_LIMIT                 = 0x105,
    NVME_SC_FIRMWARE_SLOT               = 0x106,
    NVME_SC_FIRMWARE_IMAGE              = 0x107,
    NVME_SC_INVALID_VECTOR              = 0x108,
    NVME_SC_INVALID_LOG_PAGE            = 0x109,
    NVME_SC_INVALID_FORMAT              = 0x10a,
    NVME_SC_FIRMWARE_NEEDS_CONV_RESET   = 0x10b,
    NVME_SC_INVALID_QUEUE               = 0x10c,
    NVME_SC_FEATURE_NOT_SAVEABLE        = 0x10d,
    NVME_SC_FEATURE_NOT_CHANGEABLE      = 0x10e,
    NVME_SC_FEATURE_NOT_PER_NS          = 0x10f,
    NVME_SC_FIRMWARE_NEEDS_SUBSYS_RESET = 0x110,
    NVME_SC_FIRMWARE_NEEDS_RESET        = 0x111,
    NVME_SC_FIRMWARE_NEEDS_MAX_TIME     = 0x112,
    NVME_SC_FIRMWARE_ACIVATE_PROHIBITED = 0x113,
    NVME_SC_OVERLAPPING_RANGE           = 0x114,
    NVME_SC_NAMESPACE_INSUFFICENT_CAP   = 0x115,
    NVME_SC_NAMESPACE_ID_UNAVAILABLE    = 0x116,
    NVME_SC_NAMESPACE_ALREADY_ATTACHED  = 0x118,
    NVME_SC_NAMESPACE_IS_PRIVATE        = 0x119,
    NVME_SC_NAMESPACE_NOT_ATTACHED      = 0x11a,
    NVME_SC_THIN_PROV_NOT_SUPP          = 0x11b,
    NVME_SC_CTRL_LIST_INVALID           = 0x11c,
    NVME_SC_BP_WRITE_PROHIBITED         = 0x11e,

    /* I/O Command Set Specific */
    NVME_SC_BAD_ATTRIBUTES              = 0x180,
    NVME_SC_INVALID_PI                  = 0x181,
    NVME_SC_READ_ONLY                   = 0x182,
    NVME_SC_ONCS_NOT_SUPPORTED          = 0x183,

    /* I/O Command Set Specific - Fabrics commands */
    NVME_SC_CONNECT_FORMAT          = 0x180,
    NVME_SC_CONNECT_CTRL_BUSY       = 0x181,
    NVME_SC_CONNECT_INVALID_PARAM   = 0x182,
    NVME_SC_CONNECT_RESTART_DISC    = 0x183,
    NVME_SC_CONNECT_INVALID_HOST    = 0x184,
    NVME_SC_DISCOVERY_RESTART       = 0x190,
    NVME_SC_AUTH_REQUIRED           = 0x191,

    /* Media and Data Integrity Errors */
    NVME_SC_WRITE_FAULT         = 0x280,
    NVME_SC_READ_ERROR          = 0x281,
    NVME_SC_GUARD_CHECK         = 0x282,
    NVME_SC_APPTAG_CHECK        = 0x283,
    NVME_SC_REFTAG_CHECK        = 0x284,
    NVME_SC_COMPARE_FAILED      = 0x285,
    NVME_SC_ACCESS_DENIED       = 0x286,
    NVME_SC_UNWRITTEN_BLOCK     = 0x287,

    /* Path-related Errors */
    NVME_SC_ANA_PERSISTENT_LOSS = 0x301,
    NVME_SC_ANA_INACCESSIBLE    = 0x302,
    NVME_SC_ANA_TRANSITION      = 0x303,
    NVME_SC_DNR                 = 0x4000,
};

enum reset_type {
    NVME_RESET_CONTROLLER = 0,
    NVME_RESET_SUBSYSTEM,
    NVME_RESET_CONVENTIONAL_HOT,
    NVME_RESET_CONVENTIONAL_WARM,
    NVME_RESET_CONVENTIONAL_COLD,
    NVME_RESET_FUNCTION_LEVEL,
    NVME_RESET_LINK_DOWN
};


#endif


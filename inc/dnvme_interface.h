/*
 * NVM Express Compliance Suite
 * Copyright (c) 2011, Intel Corporation.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef _DNVME_INTERFACE_H_
#define _DNVME_INTERFACE_H_

/**
 * API version coordinates the tnvme binary to the dnvme binary. If the dnvme
 * interface changes at all, then this file will be modified and thus this
 * revision will be bumped up. Only when this file changes does this version
 * change. The dnvme driver version is registered by the contents of file
 * version.h. Although version.h will change whenever API_VERSION changes, the
 * API_VERSION won't necessarily change each time version.h changes. Rather
 * version.h changes whenever a new release of the driver logic has changed.
 *
 * Thus when this API changes, then tnvme will have to be recompiled against
 * this file to adhere to the new modification and requirements of the API.
 * tnvme refuses to execute when it detects a API version mismatch to dnvme.
 */
#define    API_VERSION          0x00010402          /* 1.4.2 */
#define    NVME_QUEUE_ELEMENTS  0x1000
#define    NVME_IOSQ_ELEMENT_SIZE  0x40
#define    NVME_IOCQ_ELEMENT_SIZE  0x10

/**
 * These are the enum types used for branching to
 * required offset as specified by either PCI space
 * or a NVME space enum value defined here.
 */
enum nvme_io_space {
    NVMEIO_PCI_HDR,
    NVMEIO_BAR01,
    NVMEIO_FENCE    /* always must be the last element */
};

/**
 * These are the enum types used for specifying the
 * required access width of registers or memory space.
 */
enum nvme_acc_type {
    BYTE_LEN,
    WORD_LEN,
    DWORD_LEN,
    QUAD_LEN,
    ACC_FENCE
};

/**
 * These enums define the type of interrupt scheme that the overall
 * system uses.
 */
enum nvme_irq_type {
    INT_MSI_SINGLE,
    INT_MSI_MULTI,
    INT_MSIX,
    INT_NONE,
    INT_FENCE    /* Last item to guard from loop run-overs */
};

/**
 * enums to define the q types.
 */
enum nvme_q_type {
    ADMIN_SQ,
    ADMIN_CQ,
};

/**
 * This struct is the basic structure which has important
 * parameter for the generic read  and write function to seek the correct
 * offset and length while reading or writing to nvme card.
 */
struct rw_generic {
    enum nvme_io_space type;
    uint32_t  offset;
    uint32_t  nBytes;
    enum nvme_acc_type acc_type;
    uint8_t *buffer;
};

/**
 * These enums are used while enabling or disabling or completely disabling the
 * controller.
 */
enum nvme_state {
    ST_ENABLE,              /* Set the NVME Controller to enable state */
    ST_DISABLE,             /* Controller reset without affecting Admin Q */
    ST_DISABLE_COMPLETELY   /* Completely destroy even Admin Q's */
};

/* Enum specifying bitmask passed on to IOCTL_SEND_64B */
enum send_64b_bitmask {
    MASK_NON_PRP = 0,
    MASK_PRP1_PAGE = 1, /* PRP1 can point to a physical page */
    MASK_PRP1_LIST = 2, /* PRP1 can point to a PRP list */
    MASK_PRP2_PAGE = 4, /* PRP2 can point to a physical page */
    MASK_PRP2_LIST = 8, /* PRP2 can point to a PRP list */
    MASK_MPTR = 16,     /* MPTR may be modified */
};

enum data_direction {
    DATA_DIR_NONE = 0,
    DATA_DIR_TO_DEVICE,
    DATA_DIR_FROM_DEVICE,
    DATA_DIR_BI_DIR,
    DATA_DIR_ILLEGAL
};

/**
 * This struct is the basic structure which has important parameter for
 * sending 64 Bytes command to both admin  and IO SQ's and CQ's
 */
struct nvme_64b_send {
    /* BIT MASK for PRP1,PRP2 and metadata pointer */
    enum send_64b_bitmask bit_mask;
    /* Data buffer or discontiguous CQ/SQ's user space address */
    uint8_t const *data_buf_ptr;
    /* 0=none; 1=to_device, 2=from_device, 3=bidirectional, others illegal */
    uint8_t data_dir;

    uint8_t *cmd_buf_ptr;   /* Virtual Address pointer to 64B command */
    uint32_t meta_buf_id;   /* Meta buffer ID when MASK_MPTR is set */
    uint32_t data_buf_size; /* Size of Data Buffer */
    uint16_t unique_id;     /* Value returned back to user space */
    uint16_t q_id;          /* Queue ID where the cmd_buf command should go */
};

/**
 * This structure defines the overall interrupt scheme used and
 * defined parameters to specify the driver version and application
 * version. A verification is performed by driver and application to
 * check if these versions match.
 */
struct metrics_driver {
    uint32_t driver_version;  /* dnvme driver version */
    uint32_t api_version;     /* tnvme test application version */
};

/**
 * This structure defines the parameters required for creating any CQ.
 * It supports both Admin CQ and IO CQ.
 */
struct nvme_gen_cq {
    uint16_t q_id;            /* even admin q's are supported here q_id = 0 */
    uint16_t tail_ptr;        /* The value calculated for respective tail_ptr */
    uint16_t head_ptr;        /* Actual value in CQxTDBL for this q_id */
    uint32_t elements;        /* pass the actual elements in this q */
    uint8_t  irq_enabled;     /* sets when the irq scheme is active */
    uint16_t irq_no;          /* idx in list; always 0 based */
    uint8_t  pbit_new_entry;  /* Indicates if a new entry is in CQ */
};

/**
 * This structure defines the parameters required for creating any SQ.
 * It supports both Admin SQ and IO SQ.
 */
struct nvme_gen_sq {
    uint16_t sq_id;         /* Admin SQ are supported with q_id = 0 */
    uint16_t cq_id;         /* The CQ ID to which this SQ is associated */
    uint16_t tail_ptr;      /* Actual value in SQxTDBL for this SQ id */
    uint16_t tail_ptr_virt; /* future SQxTDBL write value based on no.
        of new cmds copied to SQ */
    uint16_t head_ptr;      /* Calculate this value based on cmds reaped */
    uint32_t elements;      /* total number of elements in this Q */
};

/**
 * enum for metrics type. These enums are used when returning the device
 * metrics.
 */
enum metrics_type {
    METRICS_CQ,     /* Completion Q Metrics */
    METRICS_SQ,     /* Submission Q Metrics */
    MTERICS_FENCE,  /* Always last item */
};

/**
 * Interface structure for returning the Q metrics. The buffer is where the
 * data is stored for the user to copy from. This assumes that the user will
 * provide correct buffer space to store the required metrics.
 */
struct nvme_get_q_metrics {
    uint16_t          q_id;     /* Pass the Q id for which metrics is desired */
    enum metrics_type type;     /* SQ or CQ metrics desired */
    uint32_t          nBytes;   /* Number of bytes to copy into buffer */
    uint8_t *         buffer;   /* to store the required data */
};

/**
 * Interface structure for creating Admin Q's. The elements is a 1 based value.
 */
struct nvme_create_admn_q {
    enum nvme_q_type type;      /* Admin q type, ASQ or ACQ */
    uint32_t         elements;  /* No. of elements of size 64 B */
};

/**
 * Interface structure for allocating SQ memory. The elements are 1 based
 * values and the CC.IOSQES is 2^n based.
 */
struct nvme_prep_sq {
    uint32_t elements;   /* Total number of entries that need kernel mem */
    uint16_t sq_id;      /* The user specified unique SQ ID  */
    uint16_t cq_id;      /* Existing or non-existing CQ ID */
    uint8_t  contig;     /* Indicates if SQ is contig or not, 1 = contig */
};

/**
 * Interface structure for allocating CQ memory. The elements are 1 based
 * values and the CC.IOSQES is 2^n based.
 */
struct nvme_prep_cq {
    uint32_t    elements;   /* Total number of entries that need kernal mem */
    uint16_t    cq_id;      /* Existing or non-existing CQ ID. */
    uint8_t     contig;     /* Indicates if SQ is contig or not, 1 = contig */
};

/**
 * Interface structure for getting the metrics structure into a user file.
 * The filename and location are specified thought file_name parameter.
 */
struct nvme_file {
    uint16_t    flen;       /* Length of file name, it is not the total bytes */
    const char *file_name;  /* location and file name to copy metrics */
};

/**
 * Interface structure for reap inquiry ioctl. It works well for both admin
 * and IO Q's.
 */
struct nvme_reap_inquiry {
    uint16_t q_id;           /* CQ ID to reap commands for */
    uint32_t num_remaining;  /* return no of cmds waiting to be reaped */

    /* no of times isr was fired which is associated with cq reaped on */
    uint32_t isr_count;
};

/**
 * Interface structure for reap ioctl. Admin Q and all IO Q's are supported.
 */
struct nvme_reap {
    uint16_t q_id;          /* CQ ID to reap commands for */
    uint32_t elements;      /* Get the no. of elements to be reaped */
    uint32_t num_remaining; /* return no. of cmds waiting for this cq */
    uint32_t num_reaped;    /* Return no. of elements reaped */
    uint8_t  *buffer;       /* Buffer to copy reaped data */
    /* no of times isr was fired which is associated with cq reaped on */
    uint32_t isr_count;
    uint32_t size;          /* Size of buffer to fill data to */
};

/**
 * Format of general purpose nvme command DW0-DW9
 */
struct nvme_gen_cmd {
    uint8_t  opcode;
    uint8_t  flags;
    uint16_t command_id;
    uint32_t nsid;
    uint64_t rsvd2;
    uint64_t metadata;
    uint64_t prp1;
    uint64_t prp2;
};

/**
 * Format of nvme command DW0-DW15
 */
struct nvme_admin_cmd {
    uint8_t  opcode;
    uint8_t  flags;
    uint16_t command_id;
    uint32_t nsid;
    uint64_t rsvd2;
    uint64_t metadata;
    uint64_t prp1;
    uint64_t prp2;
    union {
        uint32_t value;
        struct {
            uint16_t qid;
            uint16_t qsize;
        } create_iocq;
        struct {
            uint16_t qid;
            uint16_t qsize;
        } create_iosq;
        struct {
            uint16_t qid;
            uint16_t rsvd;
        } del_ioq;
        struct {
            uint16_t sq_id;
            uint16_t cmd_id;
        } abort;
        struct {
            uint32_t test_code: 3;
            uint32_t rsvd: 29;
        } dst;
        struct {
            uint32_t num_dw;
        } dir_send_recv;
        struct {
            uint32_t fw_slot: 3;
            uint32_t action: 3;
            uint32_t rsvd: 25;
            uint32_t boot_part_id: 1;
        } fw_cmt;
        struct {
            uint32_t num_dw;
        } fw_dnld;
        struct {
            uint32_t fid: 8;
            uint32_t rsvd: 23;
            uint32_t save: 1;
        } set_feature;
        struct {
            uint32_t fid: 8;
            uint32_t select: 3;
            uint32_t rsvd: 21;
        } get_feature;
        struct {
            uint32_t lp_id: 8;
            uint32_t lp_field: 4;
            uint32_t rsvd: 3;
            uint32_t retain_async_event: 1;
            uint32_t num_dw_low: 16;
        } get_log_page;
        struct {
            uint8_t cns;
            uint8_t rsvd;
            uint16_t ctrl_id;
        } identify;
        struct {
            uint32_t select: 4;
            uint32_t rsvd: 28;
        } ns_attach;
        struct {
            uint32_t select: 4;
            uint32_t rsvd: 28;
        } ns_manage;
        struct {
            uint32_t act: 4;
            uint32_t rsvd4: 4;
            uint32_t rt: 3; //Resource Type
            uint32_t rsvd: 5;
            uint32_t ci: 16; //Controller Identifier
        } vm; //Virtualization Management command
        struct {
            uint32_t lf: 4; //LBA Format
            uint32_t ms: 1; //Metadata Settings
            uint32_t pi: 3; //Protection Information
            uint32_t pil: 1; //Protection Information Location
            uint32_t ses: 3; //Secure Erase Settings
            uint32_t rsvd: 20;
        } fmt;
        struct {
            uint32_t sa: 3; //Sanitize Action
            uint32_t ause: 1; //Allow Unrestricted Sanitize Exit
            uint32_t opc: 4; //Overwrite Pass Count
            uint32_t oipbp: 1; //Overwrite Invert Pattern Between Passes
            uint32_t ndas: 1; //No Deallocate After Sanitize
            uint32_t rsvd: 22;
        } sntz;
        struct {
            uint8_t nssf; //NVMe Security Specific Field
            uint8_t ss0; //SP Specific 0
            uint8_t ss1; //SP Specific 1
            uint8_t sp; //Security Protocol
        } sec_recv;
        struct {
            uint8_t nssf; //NVMe Security Specific Field
            uint8_t ss0; //SP Specific 0
            uint8_t ss1; //SP Specific 1
            uint8_t sp; //Security Protocol
        } sec_send;
        struct {
            uint32_t sll; //Starting LBA Lower
        } gls; //Get LBA Status
    } cdw10;
    union {
        uint32_t value;
        struct {
            uint32_t contig: 1;
            uint32_t int_en: 1;
            uint32_t rsvd: 14;
            uint32_t int_no: 16;
        } create_iocq;
        struct {
            uint32_t contig: 1;
            uint32_t qprio: 2;
            uint32_t rsvd: 13;
            uint32_t cq_id: 16;
        } create_iosq;
        struct {
            uint8_t dir_oper;
            uint8_t dir_type;
            uint16_t dir_spec;
        } dir_send_recv;
        struct {
            uint32_t offset;
        } fw_dnld;
        struct {
            uint16_t num_dw;
            uint16_t lp_id;
        } get_log_page;
        struct {
            uint16_t nvm_set_id;
            uint16_t rsvd;
        } identify;
        struct {
            uint32_t arb_burst: 3;
            uint32_t rsvd: 5;
            uint32_t lpw: 8;
            uint32_t mpw: 8;
            uint32_t hpw: 8;
        } feature_arb; //Arbitration (Feature Identifier 01h)
        struct {
            uint32_t ps: 5;
            uint32_t wh: 3;
            uint32_t rsvd: 24;
        } feature_pm; //Power Management (Feature Identifier 02h)
        struct {
            uint32_t num_lba_range: 6;
            uint32_t rsvd: 26;
        } feature_lrt; //LBA Range Type (Feature Identifier 03h)
        struct {
            uint32_t tmp_th: 16;
            uint32_t th_tmp_sel: 4;
            uint32_t th_type_sel: 2;
            uint32_t rsvd: 10;
        } feature_tt; //Temperature Threshold (Feature Identifier 04h)
        struct {
            uint32_t time_lim: 16;
            uint32_t enable: 1;
            uint32_t rsvd: 15;
        } feature_er; //Error Recovery (Feature Identifier 05h)
        struct {
            uint32_t enable: 1;
            uint32_t rsvd: 31;
        } feature_vwc; //Volatile Write Cache (Feature Identifier 06h)
        struct {
            uint16_t num_sq;
            uint16_t num_cq;
        } feature_noq; //Number of Queues (Feature Identifier 07h)
        struct {
            uint8_t threshold;
            uint8_t time;
            uint16_t rsvd;
        } feature_ic; //Interrupt Coalescing (Feature Identifier 08h)
        struct {
            uint32_t vector: 16;
            uint32_t disable: 1;
            uint32_t rsvd: 15;
        } feature_ivc; //Interrupt Vector Configuration (Feature Identifier 09h)
        struct {
            uint32_t disable: 1;
            uint32_t rsvd: 31;
        } feature_wan; //Write Atomicity Normal (Feature Identifier 0Ah)
        struct {
            uint32_t shcw: 8; //SMART / Health Critical Warnings
            uint32_t nan: 1; //Namespace Attribute Notices
            uint32_t fan: 1; //Firmware Activation Notices
            uint32_t tln: 1; //Telemetry Log Notices
            uint32_t anacn: 1; //Asymmetric Namespace Access Change Notices
            uint32_t plealcn: 1; //Predictable Latency Event Aggregate Log Change Notices
            uint32_t lsin: 1; //LBA Status Information Notices
            uint32_t egealcn: 1; //Endurance Group Event Aggregate Log Change Notices
            uint32_t rsvd: 13;
            uint32_t nof: 4; //Refer to the NVMe over Fabrics specification
        } feature_aec; //Asynchronous Event Configuration (Feature Identifier 0Bh)
        struct {
            uint32_t apste: 1; //Autonomous Power State Transition Enable
            uint32_t rsvd: 31;
        } feature_apst; //Autonomous Power State Transition (Feature Identifier 0Ch)
        struct {
            uint32_t ehm: 1; //Enable Host Memory
            uint32_t mr: 1; //Memory Return
            uint32_t rsvd: 30;
        } feature_hmb; //Host Memory Buffer (Feature Identifier 0Dh)
        struct {
            uint32_t kato; //Keep Alive Timeout
        } feature_kat; //Keep Alive Timer (Feature Identifier 0Fh)
        struct {
            uint16_t tmt2; //Thermal Management Temperature 2
            uint16_t tmt1; //Thermal Management Temperature 1
        } feature_hctm; //Host Controlled Thermal Management (Feature Identifier 10h)
        struct {
            uint32_t nopspme: 1; //Non-Operational Power State Permissive Mode Enable
            uint32_t rsvd: 31;
        } feature_nopsc; //Non-Operational Power State Config (Feature Identifier 11h)
        struct {
            uint16_t nsi; //NVM Set Identifier
            uint16_t rsvd;
        } feature_rrlc; //Read Recovery Level Config (Feature Identifier 12h)
        struct {
            uint16_t nsi; //NVM Set Identifier
            uint16_t rsvd;
        } feature_plmc; //Predictable Latency Mode Config (Feature Identifier 13h)
        struct {
            uint16_t nsi; //NVM Set Identifier
            uint16_t rsvd;
        } feature_plmw; //Predictable Latency Mode Window (Feature Identifier 14h)
        struct {
            uint16_t lsiri; //LBA Status Information Report Interval
            uint16_t lsipi; //LBA Status Information Poll Interval
        } feature_lsia; //LBA Status Information Attributes (Feature Identifier 15h)
        struct {
            uint32_t ndrm: 1; //No-Deallocate Response Mode
            uint32_t rsvd: 31;
        } feature_sc; //Sanitize Config (Feature Identifier 17h)
        struct {
            uint16_t egi; //Endurance Group Identifier
            uint8_t egcw; //Endurance Group Critical Warnings
            uint8_t rsvd;
        } feature_egec; //Endurance Group Event Configuration (Feature Identifier 18h)
        struct {
            uint32_t pbslc: 8; //Pre-boot Software Load Count
            uint32_t rsvd: 24;
        } feature_spm; //Software Progress Marker (Feature Identifier 80h)
        struct {
            uint32_t eehi: 1; //Enable Extended Host Identifier
            uint32_t rsvd: 31;
        } feature_hi; //Host Identifier (Feature Identifier 81h)
        struct {
            uint32_t rsvd0: 1;
            uint32_t mregpn: 1; //Mask Registration Preempted Notification
            uint32_t mrrn: 1; //Mask Reservation Released Notification
            uint32_t mrpn: 1; //Mask Reservation Preempted Notification
        } feature_rnm; //Reservation Notification Mask (Feature Identifier 82h)
        struct {
            uint32_t ptpl: 1; //Persist Through Power Loss
            uint32_t rsvd: 31;
        } feature_rp; //Reservation Persistence (Feature Identifier 83h)
        struct {
            uint32_t wps: 3; //Write Protection State
            uint32_t rsvd: 29;
        } feature_nwpc; //Namespace Write Protection Config (Feature Identifier 84h)
        struct {
            uint16_t ncr; //Number of Controller Resources
            uint16_t rsvd;
        } vm;
        struct {
            uint32_t op; //Overwrite Pattern
        } sntz;
        struct {
            uint32_t al; //Allocation Length
        } sec_recv;
        struct {
            uint32_t tl; //Transfer Length
        } sec_send;
        struct {
            uint32_t slu; //Starting LBA Upper
        } gls; //Get LBA Status
    } cdw11;
    union {
        uint32_t value;
        struct {
            uint16_t nvm_set_id;
            uint16_t rsvd;
        } create_iosq;
        struct {
            uint32_t lp_offset_low;
        } get_log_page;
        struct {
            uint32_t hmbs; //Host Memory Buffer Size
        } feature_hmb; //Host Memory Buffer (Feature Identifier 0Dh)
        struct {
            uint32_t rrl: 4; //Read Recovery Level
            uint32_t rsvd: 28;
        } feature_rrlc; //Read Recovery Level Config (Feature Identifier 12h)
        struct {
            uint32_t ple: 1; //Predictable Latency Enable
            uint32_t rsvd: 31;
        } feature_plmc; //Predictable Latency Mode Config (Feature Identifier 13h)
        struct {
            uint32_t ws: 3; //Window Select
            uint32_t rsvd: 29;
        } feature_plmw; //Predictable Latency Mode Window (Feature Identifier 14h)
        struct {
            uint32_t mnod; //Maximum Number of Dwords
        } gls; //Get LBA Status
    } cdw12;
    union {
        uint32_t value;
        struct {
            uint32_t lp_offset_up;
        } get_log_page;
        struct {
            uint32_t hmdlla; //Host Memory Descriptor List Lower Address
        } feature_hmb; //Host Memory Buffer (Feature Identifier 0Dh)
        struct {
            uint16_t rl; //Range Length
            uint8_t rsvd;
            uint8_t at; //Action Type
        } gls; //Get LBA Status
    } cdw13;
    union {
        uint32_t value;
        struct {
            uint32_t uuid: 7;
            uint32_t rsvd: 25;
        } set_feature;
        struct {
            uint32_t uuid: 7;
            uint32_t rsvd: 25;
        } get_feature;
        struct {
            uint32_t uuid: 7;
            uint32_t rsvd: 25;
        } get_log_page;
        struct {
            uint32_t uuid: 7;
            uint32_t rsvd: 25;
        } identify;
        struct {
            uint32_t hmdlua; //Host Memory Descriptor List Upper Address
        } feature_hmb; //Host Memory Buffer (Feature Identifier 0Dh)
    } cdw14;
    union {
        uint32_t value;
        struct {
            uint32_t hmdlec; //Host Memory Descriptor List Entry Count
        } feature_hmb; //Host Memory Buffer (Feature Identifier 0Dh)
    } cdw15;
};

/**
 * Specific structure for Create CQ command
 */
struct nvme_create_cq {
    uint8_t  opcode;
    uint8_t  flags;
    uint16_t command_id;
    uint32_t rsvd1[5];
    uint64_t prp1;
    uint64_t rsvd8;
    uint16_t cqid;
    uint16_t qsize;
    uint16_t cq_flags;
    uint16_t irq_no;
    uint32_t rsvd12[4];
};

/**
 * Specific structure for Create SQ command
 */
//struct nvme_create_sq {
//    uint8_t  opcode;
//    uint8_t  flags;
//    uint16_t command_id;
//    uint32_t rsvd1[5];
//    uint64_t prp1;
//    uint64_t rsvd8;
//    uint16_t sqid;
//    uint16_t qsize;
//    uint16_t sq_flags;
//    uint16_t cqid;
//    uint32_t rsvd12[4];
//};

/**
 * Specific structure for Delete Q command
 */
struct nvme_del_q {
    uint8_t  opcode;
    uint8_t  flags;
    uint16_t command_id;
    uint32_t rsvd1[9];
    uint16_t qid;
    uint16_t rsvd10;
    uint32_t rsvd11[5];
};

/**
 * structure for Get log page command
 */
struct nvme_get_log_page {
    uint8_t  opcode;
    uint8_t  flags;
    uint16_t command_id;
    uint32_t nsid;
    uint32_t rsvd1[4];
    uint64_t prp1;
    uint64_t prp2;
    uint8_t  log_id;
    uint8_t  log_field;
    uint16_t dw_num_low;
    uint16_t dw_num_high;
    uint16_t rsvd10;
    uint32_t page_offset_low;
    uint32_t page_offset_high;
    uint32_t rsvd11[2];
};

/**
 * power state descripter for identify controller
 */
struct ctrl_power_state_descripter {
    uint16_t max_power;  /* centiwatts */
    uint8_t  rsvd2;
    uint8_t  flags;
    uint32_t entry_lat;  /* microseconds */
    uint32_t exit_lat;   /* microseconds */
    uint8_t  read_tput;
    uint8_t  read_lat;
    uint8_t  write_tput;
    uint8_t  write_lat;
    uint16_t idle_power;
    uint8_t  idle_scale;
    uint8_t  rsvd19;
    uint16_t active_power;
    uint8_t  active_work_scale;
    uint8_t  rsvd23[9];
};

enum {
    NVME_PS_FLAGS_MAX_POWER_SCALE   = 1 << 0,
    NVME_PS_FLAGS_NON_OP_STATE  = 1 << 1,
};

/**
 * structure for identify command
 */
struct nvme_identify {
    uint8_t  opcode;
    uint8_t  flags;
    uint16_t command_id;
    uint32_t nsid;
    uint32_t rsvd1[4];
    uint64_t prp1;
    uint64_t rsvd8;
    uint16_t cns;
    uint16_t ctrl_id;
    uint16_t rsvd11[5];
};

/**
 * structure for identify controller response
 */
struct nvme_id_ctrl {
    uint16_t vid;
    uint16_t subsustem_vid;
    char sn[20];
    char mn[40];
    char fr[8];
    uint8_t rab;
    uint8_t ieee[3];
    uint8_t cmic;
    uint8_t mdts;
    uint16_t cntlid;
    uint32_t ver;
    uint32_t rtd3r;
    uint32_t rtd3e;
    uint32_t oaes;
    uint32_t ctratt;
    uint16_t rrls;
    uint8_t rsvd102[154];
    uint16_t oacs;
    uint8_t acl;
    uint8_t aerl;
    uint8_t frmw;
    uint8_t lpa;
    uint8_t elpe;
    uint8_t npss;
    uint8_t avscc;
    uint8_t apsta;
    uint16_t wctemp;
    uint16_t cctemp;
    uint16_t mtfa;
    uint32_t hmpre;
    uint32_t hmmin;
    uint8_t tnvmcap[16];
    uint8_t unvmcap[16];
    uint32_t rpmbs;
    uint16_t edstt;
    uint8_t dsto;
    uint8_t fwug;
    uint16_t kas;
    uint16_t hctma;
    uint16_t mntmt;
    uint16_t mxtmt;
    uint32_t sanicap;
    uint32_t hmminds;
    uint16_t hmmaxd;
    uint16_t nsetidmax;
    uint8_t rsvd340[2];
    uint8_t anatt;
    uint8_t anacap;
    uint32_t anagrpmax;
    uint32_t nanagrpid;
    uint8_t rsvd352[160];
    uint8_t sqes;
    uint8_t cqes;
    uint16_t maxcmd;
    uint32_t nn;
    uint16_t oncs;
    uint16_t fuses;
    uint8_t fna;
    uint8_t vwc;
    uint16_t awun;
    uint16_t awupf;
    uint8_t nvscc;
    uint8_t nwpc;
    uint16_t acwu;
    uint8_t rsvd534[2];
    uint32_t sgls;
    uint32_t mnan;
    uint8_t rsvd544[224];
    char subnqn[256];
    uint8_t rsvd1024[768];
    uint32_t ioccsz;
    uint32_t iorcsz;
    uint16_t icdoff;
    uint8_t ctrattr;
    uint8_t msdbd;
    uint8_t rsvd1804[244];
    struct ctrl_power_state_descripter psd[32];
    uint8_t vs[1024];
};

struct nvme_lbaf {
    uint16_t ms;
    uint8_t ds;
    uint8_t rp;
};

struct nvme_id_ns {
    uint64_t nsze;
    uint64_t ncap;
    uint64_t nuse;
    uint8_t  nsfeat;
    uint8_t  nlbaf;
    uint8_t  flbas;
    uint8_t  mc;
    uint8_t  dpc;
    uint8_t  dps;
    uint8_t  nmic;
    uint8_t  rescap;
    uint8_t  fpi;
    uint8_t  dlfeat;
    uint16_t nawun;
    uint16_t nawupf;
    uint16_t nacwu;
    uint16_t nabsn;
    uint16_t nabo;
    uint16_t nabspf;
    uint16_t noiob;
    uint8_t  nvmcap[16];
    uint8_t  rsvd64[28];
    uint32_t anagrpid;
    uint8_t  rsvd96[3];
    uint8_t  nsattr;
    uint16_t nvmsetid;
    uint16_t endgid;
    uint8_t  nguid[16];
    uint8_t  eui64[8];
    struct nvme_lbaf lbaf[16];
    uint8_t  rsvd192[192];
    uint8_t  vs[3712];
};

/**
 * structure for abort command
 */
struct nvme_abort {
    uint8_t  opcode;
    uint8_t  flags;
    uint16_t command_id;
    uint32_t nsid;
    uint32_t rsvd1[8];
    uint16_t sq_id;
    uint16_t cmd_id;
    uint32_t rsvd11[5];
};

/**
 * structure for get feature command
 */
struct nvme_get_feature {
    uint8_t  opcode;
    uint8_t  flags;
    uint16_t command_id;
    uint32_t nsid;
    uint32_t rsvd1[4];
    uint64_t prp1;
    uint64_t prp2;
    struct {
        uint32_t feature_id: 8;
        uint32_t select: 3;
        uint32_t rsvd10: 21;
    } dw10;
    uint32_t rsvd11[3];
    struct {
        uint32_t uuid: 7;
        uint32_t rsvd14: 25;
    } dw14;
    uint32_t rsvd15;
};

/**
 * structure for set feature command
 */
struct nvme_set_feature {
    uint8_t  opcode;
    uint8_t  flags;
    uint16_t command_id;
    uint32_t nsid;
    uint32_t rsvd1[4];
    uint64_t prp1;
    uint64_t prp2;
    struct {
        uint32_t feature_id:8;
        uint32_t rsvd10:23;
        uint32_t save:1;
    } dw10;
    uint32_t dw11;
    uint32_t rsvd11[2];
    struct {
        uint32_t uuid: 7;
        uint32_t rsvd14: 25;
    } dw14;
    uint32_t rsvd15;
};

/**
 * Interface structure for setting the desired IRQ type.
 * works for all type of interrupt scheme expect PIN based.
 */
struct interrupts {
    uint16_t           num_irqs;        /* total no. of irqs req by tnvme */
    enum nvme_irq_type irq_type;        /* Active IRQ scheme for this dev */
};

/**
 * Public interface for the nvme device parameters. These parameters are
 * copied to user on request through an IOCTL interface GET_DEVICE_METRICS.
 */
struct public_metrics_dev {
    struct interrupts irq_active;  /* Active IRQ state of the nvme device */
};

/**
 * Describes bits/bytes within an existing SQ indicating a new value for any
 * cmd dword. This is only allowed for those cmds for which the doorbell hasn't
 * already rung.
 */
struct backdoor_inject {
    uint16_t q_id;        /* SQ ID where the cmd is residing */
    uint16_t cmd_ptr;     /* [0 -> (CreateIOSQ.DW10.SIZE-1)] which cmd in SQ? */
    uint8_t  dword;       /* [0 -> (CC.IOSQES-1)] which DWORD in the cmd */
    uint32_t value_mask;  /* Bitmask indicates which 'value' bits to use */
    uint32_t value;       /* Extract spec'd bits; overwrite those exact bits */
};

/**
 * Interface structure for marking a unique string to the system log.
 */
struct nvme_logstr {
    uint16_t    slen;       /* sizeof(log_str) */
    const char *log_str;    /* NULl terminated ASCII logging statement */
};


#endif

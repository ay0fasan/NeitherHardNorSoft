#ifndef NHNS_STATUS_CODES_H_
#define NHNS_STATUS_CODES_H_

#include <stdint.h>

// Component allotment (256 errors for each module)
#define NHNS_STATUS_COMPONENT_ALLOTMENT 0x0100

// Component bases
#define NHNS_STATUS_BASE_GENERAL        0x0000
#define NHNS_STATUS_BASE_STM           (NHNS_STATUS_BASE_GENERAL + NHNS_STATUS_COMPONENT_ALLOTMENT)    // 0x0100

typedef enum nhns_status
{
    NHNS_STATUS_OK                    = NHNS_STATUS_BASE_GENERAL + 0x0,
    NHNS_STATUS_FAIL                  = NHNS_STATUS_BASE_GENERAL + 0x1,
    NHNS_STATUS_BUSY                  = NHNS_STATUS_BASE_GENERAL + 0x2,
    NHNS_STATUS_TIMEOUT               = NHNS_STATUS_BASE_GENERAL + 0x3,
    NHNS_STATUS_BAD_DATA              = NHNS_STATUS_BASE_GENERAL + 0x4,
    NHNS_STATUS_PROTOCOL_ERROR        = NHNS_STATUS_BASE_GENERAL + 0x5,
    NHNS_STATUS_NO_MEMORY             = NHNS_STATUS_BASE_GENERAL + 0x6,
    NHNS_STATUS_INVALID_ARGUMENT      = NHNS_STATUS_BASE_GENERAL + 0x7,
    NHNS_STATUS_BAD_ADDRESS           = NHNS_STATUS_BASE_GENERAL + 0x8,
    NHNS_STATUS_FORBIDDEN             = NHNS_STATUS_BASE_GENERAL + 0x9,
    NHNS_STATUS_NOT_FOUND             = NHNS_STATUS_BASE_GENERAL + 0xA,
    NHNS_STATUS_ALREADY_EXISTS        = NHNS_STATUS_BASE_GENERAL + 0xB,
    NHNS_STATUS_UNSUPPORTED           = NHNS_STATUS_BASE_GENERAL + 0xC,
    NHNS_STATUS_DATA_MISMATCH         = NHNS_STATUS_BASE_GENERAL + 0xD,
    NHNS_STATUS_VERSION_MISMATCH      = NHNS_STATUS_BASE_GENERAL + 0xE,
    NHNS_STATUS_MODULE_NOT_INIT       = NHNS_STATUS_BASE_GENERAL + 0x10,
    NHNS_STATUS_NULL_CALLBACK         = NHNS_STATUS_BASE_GENERAL + 0x11,
    NHNS_STATUS_UNKNOWN               = NHNS_STATUS_BASE_GENERAL + 0x12,
    NHNS_STATUS_INVALID_CONFIGURATION = NHNS_STATUS_BASE_GENERAL + 0x13,
} nhns_status_t;

#endif /* NHNS_STATUS_CODES_H_ */

#ifndef NHNS_STATUS_CODES_H_
#define NHNS_STATUS_CODES_H_

#include <stdint.h>

typedef enum nhns_status
{
    NHNS_STATUS_OK                    = 0x0,
    NHNS_STATUS_FAIL                  = 0x1,
    NHNS_STATUS_BUSY                  = 0x2,
    NHNS_STATUS_TIMEOUT               = 0x3,
    NHNS_STATUS_BAD_DATA              = 0x4,
    NHNS_STATUS_PROTOCOL_ERROR        = 0x5,
    NHNS_STATUS_NO_MEMORY             = 0x6,
    NHNS_STATUS_INVALID_ARGUMENT      = 0x7,
    NHNS_STATUS_BAD_ADDRESS           = 0x8,
    NHNS_STATUS_FORBIDDEN             = 0x9,
    NHNS_STATUS_NOT_FOUND             = 0xA,
    NHNS_STATUS_ALREADY_EXISTS        = 0xB,
    NHNS_STATUS_UNSUPPORTED           = 0xC,
    NHNS_STATUS_DATA_MISMATCH         = 0xD,
    NHNS_STATUS_VERSION_MISMATCH      = 0xE,
    NHNS_STATUS_MODULE_NOT_INIT       = 0x10,
    NHNS_STATUS_NULL_CALLBACK         = 0x11,
    NHNS_STATUS_UNKNOWN               = 0x12,
    NHNS_STATUS_INVALID_CONFIGURATION = 0x13,
} nhns_status_t;

#endif /* NHNS_STATUS_CODES_H_ */

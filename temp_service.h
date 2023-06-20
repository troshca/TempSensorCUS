#ifndef __TEMP_SERVICE_H__
#define __TEMP_SERVICE_H__

#include <stdint.h>
#include "main.h"
#include "ble.h"
#include "ble_srv_common.h"

#define BLE_UUID_TEMP_BASE_UUID                                                                        \
    {                                                                                                  \
        0x23, 0xD1, 0x13, 0xEF, 0x5F, 0x78, 0x23, 0x15, 0xDE, 0xEF, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00 \
    }                                // 128-bit base UUID
#define BLE_UUID_TEMP_SERVICE 0xABCD

// Defining 16-bit characteristic UUID
#define BLE_UUID_HUMIDIT_CHARACTERISTC_UUID 0xBEEF 
#define BLE_UUID_TEMPERATURE_CHARACTERISTC_UUID 0xB00B

/**
 * @brief This structure contains various status information for temp service.
 * It only holds one entry now, but will be populated with more items as we go.
 * The name is based on the naming convention used in Nordic's SDKs.
 * 'ble’ indicates that it is a Bluetooth Low Energy relevant structure and
 * ‘os’ is short for Temp Service).
 */
typedef struct
{
    uint16_t conn_handle;    /**< Handle of the current connection (as provided by the BLE stack, is BLE_CONN_HANDLE_INVALID if not in a connection).*/
    uint16_t service_handle; /**< Handle of Temp Service (as provided by the BLE stack). */
    ble_gatts_char_handles_t char_handles;
    ble_gatts_char_handles_t char_handles_temperature;
} ble_os_t;

/**@brief Function for handling BLE Stack events related to temp service and characteristic.
 *
 * @details Handles all events from the BLE stack of interest to Temp Service.
 *
 * @param[in]   p_temp_service       Temp Service structure.
 * @param[in]   p_ble_evt            Event received from the BLE stack.
 */
void ble_temp_service_on_ble_evt(ble_evt_t const *p_ble_evt, void *p_context);

/**@brief Function for initializing new custom temp service.
 *
 * @param[in]   p_temp_service       Pointer to Temp Service structure.
 */
void temp_service_init(ble_os_t *p_temp_service);

/**@brief Function for updating and sending new humidity characteristic values
 *
 *
 * @param[in]   p_temp_service                    Temp Service structure.
 * @param[in]   characteristic_value              New characteristic value.
 */
void humidity_characteristic_update(ble_os_t *p_temp_service, volatile uint8_t *humidity_value);

/**@brief Function for updating and sending new temperature characteristic values
 *
 *
 * @param[in]   p_temp_service                    Temp Service structure.
 * @param[in]   characteristic_value              New characteristic value.
 */
void temperature_characteristic_update(ble_os_t *p_temp_service, uint16_t *temperature_value);

#endif /* __TEMP_SERVICE_H__ */
#include <stdint.h>
#include <string.h>
#include "temp_service.h"
#include "ble_srv_common.h"
#include "app_error.h"

void ble_temp_service_on_ble_evt(ble_evt_t const *p_ble_evt, void *p_context)
{
    ble_os_t *p_temp_service = (ble_os_t *)p_context;
    switch (p_ble_evt->header.evt_id)
    {
    case BLE_GAP_EVT_CONNECTED:
        p_temp_service->conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
        break;
    case BLE_GAP_EVT_DISCONNECTED:
        p_temp_service->conn_handle = BLE_CONN_HANDLE_INVALID;
        break;
    default:
        break;
    }
}

/**@brief Function for adding new characterstic to "Temp service".
 *
 * @param[in]   p_temp_service        Temp Service structure.
 *
 */
static uint8_t humid_char_add(ble_os_t *p_temp_service)
{
    // Add a custom characteristic UUID
    uint32_t err_code;
    ble_uuid_t char_uuid;
    ble_uuid128_t base_uuid = BLE_UUID_TEMP_BASE_UUID;
    char_uuid.uuid = BLE_UUID_HUMIDIT_CHARACTERISTC_UUID;
    err_code = sd_ble_uuid_vs_add(&base_uuid, &char_uuid.type);
    APP_ERROR_CHECK(err_code);
    // Add read/write properties to characteristic
    ble_gatts_char_md_t char_md;
    memset(&char_md, 0, sizeof(char_md));
    char_md.char_props.read = 1;
    char_md.char_props.write = 1;
    // Configuring Client Characteristic Configuration Descriptor metadata and add to char_md structure
    ble_gatts_attr_md_t cccd_md;
    memset(&cccd_md, 0, sizeof(cccd_md));
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
    cccd_md.vloc = BLE_GATTS_VLOC_STACK;
    char_md.p_cccd_md = &cccd_md;
    char_md.char_props.notify = 1;
    // Configure the attribute metadata
    ble_gatts_attr_md_t attr_md;
    memset(&attr_md, 0, sizeof(attr_md));
    attr_md.vloc = BLE_GATTS_VLOC_STACK;
    // Set read/write security levels to characteristic
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
    // Configure the characteristic value attribute
    ble_gatts_attr_t attr_char_value;
    memset(&attr_char_value, 0, sizeof(attr_char_value));
    attr_char_value.p_uuid = &char_uuid;
    attr_char_value.p_attr_md = &attr_md;
    // Set characteristic length in number of bytes
    attr_char_value.max_len = 1;
    attr_char_value.init_len = 1;
    volatile uint16_t value = 0x10;
    attr_char_value.p_value = value;
    err_code = sd_ble_gatts_characteristic_add(p_temp_service->service_handle,
                                               &char_md,
                                               &attr_char_value,
                                               &p_temp_service->char_handles);
    APP_ERROR_CHECK(err_code);
    return NRF_SUCCESS;
}

/**@brief Function for adding new characterstic to "Temp service".
 *
 * @param[in]   p_temp_service        Temp Service structure.
 *
 */
static uint8_t temp_char_add(ble_os_t *p_temp_service)
{
    // Add a custom characteristic UUID
    uint32_t err_code;
    ble_uuid_t char_uuid;
    ble_uuid128_t base_uuid = BLE_UUID_TEMP_BASE_UUID;
    char_uuid.uuid = BLE_UUID_TEMPERATURE_CHARACTERISTC_UUID;
    err_code = sd_ble_uuid_vs_add(&base_uuid, &char_uuid.type);
    APP_ERROR_CHECK(err_code);
    // Add read/write properties to characteristic
    ble_gatts_char_md_t char_md;
    memset(&char_md, 0, sizeof(char_md));
    char_md.char_props.read = 1;
    char_md.char_props.write = 1;
    // Configuring Client Characteristic Configuration Descriptor metadata and add to char_md structure
    ble_gatts_attr_md_t cccd_md;
    memset(&cccd_md, 0, sizeof(cccd_md));
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
    cccd_md.vloc = BLE_GATTS_VLOC_STACK;
    char_md.p_cccd_md = &cccd_md;
    char_md.char_props.notify = 1;
    // Configure the attribute metadata
    ble_gatts_attr_md_t attr_md;
    memset(&attr_md, 0, sizeof(attr_md));
    attr_md.vloc = BLE_GATTS_VLOC_STACK;
    // Set read/write security levels to characteristic
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
    // Configure the characteristic value attribute
    ble_gatts_attr_t attr_char_value;
    memset(&attr_char_value, 0, sizeof(attr_char_value));
    attr_char_value.p_uuid = &char_uuid;
    attr_char_value.p_attr_md = &attr_md;
    // Set characteristic length in number of bytes
    attr_char_value.max_len = 2;
    attr_char_value.init_len = 2;
    uint8_t value[2] = {0xAA, 0xAA};
    attr_char_value.p_value = value;
    // Add new characteristic to the service
    err_code = sd_ble_gatts_characteristic_add(p_temp_service->service_handle,
                                               &char_md,
                                               &attr_char_value,
                                               &p_temp_service->char_handles_temperature);
    APP_ERROR_CHECK(err_code);
    return NRF_SUCCESS;
}

/**@brief Function for initiating new custom temp service.
 *
 * @param[in]   p_temp_service        Temp Service structure.
 *
 */
void temp_service_init(ble_os_t *p_temp_service)
{
    uint32_t err_code;
    // Declare 16-bit service and 128-bit base UUIDs and add them to the BLE stack
    ble_uuid_t service_uuid;
    ble_uuid128_t base_uuid = BLE_UUID_TEMP_BASE_UUID;
    service_uuid.uuid = BLE_UUID_TEMP_SERVICE;
    err_code = sd_ble_uuid_vs_add(&base_uuid, &service_uuid.type);
    APP_ERROR_CHECK(err_code);
    p_temp_service->conn_handle = BLE_CONN_HANDLE_INVALID;

    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY,
                                        &service_uuid,
                                        &p_temp_service->service_handle);
    APP_ERROR_CHECK(err_code);

    humid_char_add(p_temp_service);
    temp_char_add(p_temp_service);

    NRF_LOG_INFO("Executing temp_service_init().\n");
    NRF_LOG_INFO("Service UUID: 0x%#04x\n", service_uuid.uuid);
    NRF_LOG_FLUSH();
    NRF_LOG_INFO("Service UUID type: 0x%#02x\n", service_uuid.type);
    NRF_LOG_INFO("Service handle: 0x%#04x\n", p_temp_service->service_handle);
    NRF_LOG_FLUSH();
}

void humidity_characteristic_update(ble_os_t *p_temp_service, volatile uint8_t *humidity_value)
{
    // Update characteristic value
    if (p_temp_service->conn_handle != BLE_CONN_HANDLE_INVALID)
    {

        uint16_t len = 1;
        ble_gatts_hvx_params_t hvx_params;
        memset(&hvx_params, 0, sizeof(hvx_params));

        hvx_params.handle = p_temp_service->char_handles.value_handle;
        hvx_params.type = BLE_GATT_HVX_NOTIFICATION;
        hvx_params.offset = 0;
        hvx_params.p_len = &len;
        hvx_params.p_data = (volatile uint8_t *)humidity_value;

        sd_ble_gatts_hvx(p_temp_service->conn_handle, &hvx_params);
    }
}

void temperature_characteristic_update(ble_os_t *p_temp_service, uint16_t *temperature_value)
{
    // Update characteristic value
    if (p_temp_service->conn_handle != BLE_CONN_HANDLE_INVALID)
    {

        uint16_t len = 2;
        ble_gatts_hvx_params_t hvx_params;
        memset(&hvx_params, 0, sizeof(hvx_params));

        hvx_params.handle = p_temp_service->char_handles_temperature.value_handle;
        hvx_params.type = BLE_GATT_HVX_NOTIFICATION;
        hvx_params.offset = 0;
        hvx_params.p_len = &len;
        hvx_params.p_data = (uint8_t *)temperature_value;
        NRF_LOG_INFO("hvx_params.p_data: %d", hvx_params.p_data);
        NRF_LOG_FLUSH();

        sd_ble_gatts_hvx(p_temp_service->conn_handle, &hvx_params);
    }
}

#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#ifdef DOXYGEN
/**
 * @brief If set, enables scanning for extended advertisements
 * as specified by BLE 5.0. */
#define BLE_APP_USE_LEGACY_SCAN
#endif /* DOXYGEN */

#ifndef BLE_APP_SCAN_MODE
/**
 * @brief Used to set the scanning mode selected by the application. */
#define BLE_APP_SCAN_MODE DM_SCAN_TYPE_ACTIVE
#endif /* BLE_APP_SCAN_MODE */

#endif /* APP_CONFIG_H */

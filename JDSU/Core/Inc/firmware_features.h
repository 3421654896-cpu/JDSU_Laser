#ifndef FBG_FIRMWARE_FEATURES_H
#define FBG_FIRMWARE_FEATURES_H

/* Customer firmware omits independent features hidden in the desktop UI.
 * A developer build may override these with compiler definitions. The full
 * pre-trim source and binaries are also preserved in backups/. */
#ifndef FBG_FEATURE_LEGACY_PEAK_MODES
#define FBG_FEATURE_LEGACY_PEAK_MODES 0
#endif
#ifndef FBG_FEATURE_MQTT
#define FBG_FEATURE_MQTT 0
#endif
#ifndef FBG_FEATURE_NETWORK_OTA
#define FBG_FEATURE_NETWORK_OTA 0
#endif

/* USB firmware update, LAN RAW transport, dense/temporary point acquisition,
 * direct DAC calibration, thermal control and fail-dark protections are shared
 * by visible workflows and must not be excluded with the hidden features. */
#endif

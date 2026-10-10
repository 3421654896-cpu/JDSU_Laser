#ifndef __OTA_UPDATE_H
#define __OTA_UPDATE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void OtaUpdate_Init(uint32_t runtime_boot_id);

/* Physical CDC-only mailbox. Never route LAN FUSB1 commands into this API.
 * QueueUsbFrame only copies in the USB ISR; all flash work runs in main. */
uint8_t OtaUpdate_QueueUsbFrame(const uint8_t *frame, uint16_t length);
void OtaUpdate_ProcessUsb(void);

/* transport_kind: 1=LAN TCP, 2=WAN MQTT, 3=physical USB CDC. WAN END requires an
 * HMAC of the complete staged image; commands cannot cross an OTA owner. */
uint8_t OtaUpdate_HandleCommand(const char *payload,
                                uint32_t payload_length,
                                const char *authorization_key,
                                uint8_t transport_kind,
                                char *response,
                                uint16_t response_capacity,
                                uint8_t *reboot_after_response);

uint8_t OtaUpdate_IsActive(void);
uint8_t OtaUpdate_TransportKind(void);
uint8_t OtaUpdate_IsCommitted(void);
void OtaUpdate_AbortUncommittedSession(void);
uint32_t OtaUpdate_ReceivedBytes(void);
void OtaUpdate_OnClientDisconnected(void);
/* Release only the volatile transfer lock after a fully verified image has
 * already been committed to metadata but the coordinated radio/MCU restart
 * could not be confirmed.  The bootloader-visible pending image is preserved
 * for the next safe reset. */
void OtaUpdate_ReleaseCommittedSession(void);

#ifdef __cplusplus
}
#endif

#endif

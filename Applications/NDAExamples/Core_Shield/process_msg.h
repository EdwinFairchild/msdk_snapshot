
#define CMD_DONGLE_ID   0
#define CMD_RESET_STATE 1
#define CMD_ENCRYPT_BLK 2
#define CMD_RETR_SIG    3
#define CMD_DECRYPT_BLK 4
#define CMD_VERIFY_SIG  5

#define RESP_OK         0
#define RESP_DONGLE_ID  1
#define RESP_ENCRYPTED  2
#define RESP_SIGNATURE  3
#define RESP_DECRYPTED  4
#define RESP_SIG_GOOD   5
#define RESP_SIG_BAD    6
#define RESP_ERROR      255

uint8_t process_msg(dmConnId_t connId, uint16_t handle, uint8_t operation,
		    uint16_t offset, uint16_t len, uint8_t *pValue, attsAttr_t *pAttr);

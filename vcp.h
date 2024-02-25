#define ADDR	0x31
void write_byte(uint8_t byte);

void scan_bus();

int set_vcp(uint8_t op, uint16_t code);

uint16_t get_vcp(uint8_t op);

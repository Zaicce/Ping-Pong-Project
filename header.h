//Header file


//declare functions in functions.c

void init(void);
void menu(void);
void interruptInit(void);
void game(void);
void scoreScreen(void);


//declare functions in main.c
void delay(int);
void display_image(int x, const uint8_t *data);
void display_init(void);
void display_string(int line, char *s);
void display_update(void);
uint8_t spi_send_recv(uint8_t data);

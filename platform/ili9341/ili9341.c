#include "ili9341.h"

enum {
  COMMAND_MODE = 0,
  DATA_MODE = 1,
  DELAY_MODE = 2
};

enum {
  NOP = 0x00,     // No operation
  SWRESET = 0x01, // Software reset
  RDDIDIF = 0x04, // Read display identification information
  RDDST = 0x09,   // Read display status
  RDDPM = 0x0A,   // Read display power mode
  SLPOUT = 0x11,  // Sleep out
  DISPOFF = 0x28, // Display off
  DISPON = 0x29,  // Display on
  RAMWR = 0x2C,   // Memory write
  PIXSET = 0x3A,  // Pixel format set
  FRMCTR1 = 0xB1A // Frame rate control in normal/full-color mode
};

typedef struct {
  char mode;
  char payload;
} instruction_t;

#define COMMAND(c) (instruction_t){.mode = (char)COMMAND_MODE, .payload = (char)c}
#define DATA(d) (instruction_t){.mode = (char)DATA_MODE, .payload = (char)d}
#define DELAY(m) (instruction_t){.mode = (char)DELAY_MODE, .payload = (char)m}

static instruction_t initialisation_sequence[] = {
  COMMAND(SWRESET), DELAY(5), // Software reset, then wait 5ms
  COMMAND(DISPOFF),

  // FIXME: Ad-hoc skipping extended registers, we're not using it

  // Pixel format: 16bpp, both on RGB and SPI
  COMMAND(PIXSET), DATA(0x55),

  /* FIXME: Useless: extended
  // Framerate:
  COMMAND(FRMCTR1), DATA(0x00), DATA(0x1B),
  */

  // Gamma
  //FIXME

  // Display
  // Entry mode set, skipped
  COMMAND(SLPOUT), DELAY(100),

  COMMAND(DISPON), DELAY(20),

  COMMAND(NOP) // Used to mark the end of the init sequence
};

void perform_instruction(ili9341_t * c, instruction_t * instruction) {
  if (instruction->mode == DELAY_MODE) {
    // FIXME: Should sleep instruction->payload miliseconds
    for (int i = 0; i < 800*instruction->payload; i++) {
    }
  } else {
    c->data_command_pin_write(instruction->mode);
    c->spi_write(&instruction->payload, 1);
  }
}

void ili9341_initialize(ili9341_t * c) {
  // Falling edge on CSX
  c->chip_select_pin_write(0);

  // Send all the initialisation_sequence
  instruction_t * instruction = initialisation_sequence;
  while (!(instruction->mode == COMMAND_MODE && instruction->payload == NOP)) {
    perform_instruction(c, instruction++);
  }

#define FILL_SCREEN_UPON_INIT 0
#if FILL_SCREEN_UPON_INIT
  // This would draw stuff on the screen

  perform_instruction(c, &COMMAND(RAMWR));

  c->data_command_pin_write(DATA_MODE);

  char buffer[256];
  for (int i = 0;i<256;i++) {
    buffer[i] = i;
  }
  for (int i = 0; i<(320*240*2)/256; i++) {
    c->spi_write(buffer, 256);
  }
#endif
}

void ili9341_set_gamma(ili9341_t * c) {
}

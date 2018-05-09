#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include <joystick.h>
#include <nes.h>
#include "neslib/neslib.h"

uint8_t i, ix, iy;
uint8_t joy0;

const uint8_t PALETTE[] = {
	0x0D, 0x20, 0x11, 0x01, // Unused.
	0x0D, 0x00, 0x10, 0x20, // Star colors.
	0x0D, 0x27, 0x07, 0x17, // Asteroid colors.
	0x0D, 0x21, 0x21, 0x21, // Blue text.
	
	0x0D, 0x20, 0x11, 0x01, // P1 palette.
	0x0D, 0x20, 0x17, 0x07, // P2 palette.
	0x09, 0x06, 0x16, 0x19, // Enemy.
	0x0D, 0x00, 0x10, 0x20, // Unused.
};

static const char HEX[] = "0123456789ABCDEF";

/*
	This is something that I found useful for developing.
	Draw a table of all the tiles or sprites so you can
	look up their indexes easily.
	
	Also plays music because why not?
*/

void main (void) {
	joy_install(joy_static_stddrv);
	
	// Assign the two sprite atlasses to the sprites/tiles.
	bank_bg(0);
	bank_spr(1);
	
	// Clear the sprite memory.
	oam_clear();
	
	// Turn off the PPU to make batch uploads to VRAM.
	ppu_off(); {
		// Copy the palette to VRAM.
		pal_all(PALETTE);
		
		// Set VRAM writes to increment to the right. (1 for down)
		vram_inc(0);
		// Set the start address to the first nametable.
		vram_adr(NAMETABLE_A);
		// Fill it with 0's.
		vram_fill(0x00, 32*30);
		
		// Set start address to tile (8, 6)
		vram_adr(NTADR_A(8, 6));
		// Draw horizontal axis labels.
		for(i = 0; i < 16; ++i) vram_put(HEX[i]);
		
		// Increment downwards and draw the vertical labels.
		vram_inc(1);
		vram_adr(NTADR_A(6, 8));
		for(i = 0; i < 16; ++i) vram_put(HEX[i]);
		
		// Draw the "pipes" around the table.
		vram_adr(NTADR_A(7, 7));
		vram_put(0x14);
		vram_fill(0x0E, 16);
		vram_inc(0);
		vram_adr(NTADR_A(8, 7));
		vram_fill(0x0B, 16);
		
		// Loop over the table and draw the tiles.
		for(iy = 0; iy < 16; ++iy){
			vram_adr(NTADR_A(8, iy + 8));
			for(ix = 0; ix < 16; ++ix){
				vram_put((iy << 4) | ix);
			}
		}
	} ppu_on_all();
	
	// Play some music because why not?
	music_play(0);
	
	// Main loop.
	while(true){
		// Read the first player gamepad.
		joy0 = joy_read(0);
		
		// If they press the A/B buttons, switch the character bank.
		if(JOY_BTN_1(joy0)) bank_bg(0);
		if(JOY_BTN_2(joy0)) bank_bg(1);
		
		// Wait for the next frame.
		ppu_wait_nmi();
	}
}

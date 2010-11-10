/******************************************************************************/
/*									      */
/*	   QUIZ SEKAI WA SHOW BY SHOBAI (C) 1993 TAITO CORPORATION	      */
/*									      */
/******************************************************************************/

#include "gameinc.h"
#include "bsystem.h"
#include "tc180vcu.h"
#include "tc220ioc.h"
#include "taitosnd.h"
#include "cat93c46.h"
#include "sasound.h"            // sample support routines
#include "savegame.h"
#include "blit.h"

static struct DIR_INFO quiz_sekai_wa_show_by_shobai_dirs[] =
{
   { "quiz_sekai_wa_show_by_shobai", },
   { "qzshowby", },
   { NULL, },
};

static struct ROM_INFO quiz_sekai_wa_show_by_shobai_roms[] =
{
  LOAD8_16(  REGION_ROM1,  0x000000,  0x00080000,
              "d72-13.bin",  0xa867759f,   "d72-12.bin",  0x522c09a7),
   {   "d72-01.bin", 0x00200000, 0xb82b8830, 0, 0, 0, },
   {   "d72-02.bin", 0x00200000, 0xbf0da640, 0, 0, 0, },
   {   "d72-03.bin", 0x00200000, 0x1de257d0, 0, 0, 0, },
   {   "d72-11.bin", 0x00020000, 0x2ca046e2, 0, 0, 0, },
   {	       NULL,	      0,	  0, 0, 0, 0, },
};

static struct INPUT_INFO quiz_sekai_wa_show_by_shobai_inputs[] =
{
   INP0( COIN1, 0x026003, 0x10 ),
   INP0( COIN2, 0x026003, 0x20 ),
   INP0( COIN3, 0x026003, 0x40 ),
   INP0( COIN4, 0x026003, 0x80 ),
   INP0( TILT, 0x026005, 0x01 ),
   INP0( SERVICE, 0x026005, 0x02 ),
   INP0( TEST, 0x026003, 0x02 ),

   INP0( P1_START, 0x026005, 0x10 ),
   INP0( P1_B1, 0x02600F, 0x01 ),
   INP0( P1_B2, 0x02600F, 0x02 ),
   INP0( P1_B3, 0x02600F, 0x08 ),
   INP0( P1_B4, 0x02600F, 0x04 ),

   INP0( P2_START, 0x026005, 0x20 ),
   INP0( P2_B1, 0x02600F, 0x10 ),
   INP0( P2_B2, 0x02600F, 0x20 ),
   INP0( P2_B3, 0x02600F, 0x80 ),
   INP0( P2_B4, 0x02600F, 0x40 ),
   INP0( P3_START, 0x026005, 0x40 ),
   INP0( P3_B1, 0x02602F, 0x01 ),
   INP0( P3_B2, 0x02602F, 0x02 ),
   INP0( P3_B3, 0x02602F, 0x08 ),
   INP0( P3_B4, 0x02602F, 0x04 ),

   INP0( P4_START, 0x026005, 0x80 ),
   INP0( P4_B1, 0x02602F, 0x10 ),
   INP0( P4_B2, 0x02602F, 0x20 ),
   INP0( P4_B3, 0x02602F, 0x80 ),
   INP0( P4_B4, 0x02602F, 0x40 ),

   END_INPUT
};

GAME( quiz_sekai_wa_show_by_shobai ,
   quiz_sekai_wa_show_by_shobai_dirs,
   quiz_sekai_wa_show_by_shobai_roms,
   quiz_sekai_wa_show_by_shobai_inputs,
   NULL,
   NULL,

   load_quiz_sekai_wa_show_by_shobai,
   clear_quiz_sekai_wa_show_by_shobai,
   &b_system_video,
   execute_quiz_sekai_wa_show_by_shobai_frame,
   "qzshowby",
   "Quiz sekai wa show by shobai",
   "�N�C�Y���E�͂r�������@�����@�V���[�o�C",
   COMPANY_ID_TAITO,
   "D72",
   1993,
   taito_ym2610_sound,
   GAME_PUZZLE
);

static struct DIR_INFO puzzle_bobble_b_system_dirs[] =
{
   { "puzzle_bobble_b_system", },
   { "puzbobb", },
   { "pbobblbs", },
   { "pbobble", },
   { NULL, },
};

static struct ROM_INFO puzzle_bobble_b_system_roms[] =
{
  LOAD8_16(  REGION_ROM1,  0x000000,  0x00040000,
             "pb-1c18.bin",  0x5de14f49,   "pb-ic2.bin",  0x2abe07d1),
   {   "pb-ic9.bin", 0x00080000, 0x3253aac9, 0, 0, 0, },
   {  "pb-ic14.bin", 0x00080000, 0x55f90ea4, 0, 0, 0, },
   {  "pb-ic15.bin", 0x00100000, 0x0840cbc4, 0, 0, 0, },
   {  "pb-ic27.bin", 0x00020000, 0x26efa4c4, 0, 0, 0, },
   {	       NULL,	      0,	  0, 0, 0, 0, },
};

static struct INPUT_INFO puzzle_bobble_b_system_inputs[] =
{
   INP0( COIN1, 0x026002, 0x10 ),
   INP0( COIN2, 0x026002, 0x20 ),
   INP0( TILT, 0x026004, 0x01 ),
   INP0( SERVICE, 0x026004, 0x02 ),
   INP0( TEST, 0x026000, 0x80 ),

   INP0( P1_START, 0x026004, 0x10 ),
   INP0( P1_UP, 0x02600E, 0x01 ),
   INP0( P1_DOWN, 0x02600E, 0x02 ),
   INP0( P1_LEFT, 0x02600E, 0x04 ),
   INP0( P1_RIGHT, 0x02600E, 0x08 ),
   INP0( P1_B1, 0x026006, 0x01 ),
   INP0( P1_B2, 0x026006, 0x02 ),
   INP0( P1_B3, 0x026006, 0x04 ),

   INP0( P2_START, 0x026004, 0x20 ),
   INP0( P2_UP, 0x02600E, 0x10 ),
   INP0( P2_DOWN, 0x02600E, 0x20 ),
   INP0( P2_LEFT, 0x02600E, 0x40 ),
   INP0( P2_RIGHT, 0x02600E, 0x80 ),
   INP0( P2_B1, 0x026006, 0x10 ),
   INP0( P2_B2, 0x026006, 0x20 ),
   INP0( P2_B3, 0x026006, 0x40 ),

   END_INPUT
};

static struct ROMSW_DATA romsw_data_puzzle_bobble_b_system_0[] =
{
   { "Puzzle Bobble - Taito Japan",           0x01 },
   { "Bubble Buster - Taito America",         0x02 },
   { "Bubble Buster - Taito (World)",         0x03 },
   { "Puzzle Bobble - Taito (World)",         0x04 },
   { NULL,		      0    },
};

static struct ROMSW_INFO puzzle_bobble_b_system_romsw[] =
{
   { 0x07FFFF, 0x04, romsw_data_puzzle_bobble_b_system_0 },
   { 0,        0,    NULL },
};


GAME( puzzle_bobble_b_system ,
   puzzle_bobble_b_system_dirs,
   puzzle_bobble_b_system_roms,
   puzzle_bobble_b_system_inputs,
   NULL,
   puzzle_bobble_b_system_romsw,

   load_puzzle_bobble_b_system,
   clear_puzzle_bobble_b_system,
   &b_system_video,
   execute_puzzle_bobble_b_system_frame,
   "pbobble",
   "Puzzle Bobble (b system)",
   NULL,
   COMPANY_ID_TAITO,
   NULL,
   1994,
   taito_ym2610_sound,
   GAME_PUZZLE
);

static struct DIR_INFO space_invaders_dx_dirs[] =
{
   { "space_invaders_dx", },
   { "spacedx", },
   { "spacedxj", },
   { NULL, },
};

static struct ROM_INFO space_invaders_dx_roms[] =
{
   {	   "d89-06", 0x00040000, 0x7122751e, REGION_ROM1, 0x000000, LOAD_8_16, },
   {	   "d89-05", 0x00040000, 0xbe1638af, REGION_ROM1, 0x000001, LOAD_8_16, },
   {	   "d89-01.9", 0x00080000, 0xfffa0660, 0, 0, 0, },
   {	   "d89-02.14", 0x00080000, 0xc36544b9, 0, 0, 0, },
   {	   "d89-03.15", 0x00080000, 0x218f31a4, 0, 0, 0, },
   {	   "d89-07.27", 0x00010000, 0xbd743401, 0, 0, 0, },
   {	       NULL,	      0,	  0, 0, 0, 0, },
};

static struct INPUT_INFO space_invaders_dx_inputs[] =
{
   INP0( COIN1, 0x072002, 0x10 ),
   INP0( COIN2, 0x072002, 0x20 ),
   INP0( TILT, 0x072004, 0x01 ),
   INP0( SERVICE, 0x072004, 0x02 ),
   INP0( TEST, 0x072002, 0x02 ),

   INP0( P1_START, 0x072004, 0x10 ),
   INP0( P1_UP, 0x07200E, 0x01 ),
   INP0( P1_DOWN, 0x07200E, 0x02 ),
   INP0( P1_LEFT, 0x07200E, 0x04 ),
   INP0( P1_RIGHT, 0x07200E, 0x08 ),
   INP0( P1_B1, 0x072006, 0x01 ),

   INP0( P2_START, 0x072004, 0x20 ),
   INP0( P2_UP, 0x07200E, 0x10 ),
   INP0( P2_DOWN, 0x07200E, 0x20 ),
   INP0( P2_LEFT, 0x07200E, 0x40 ),
   INP0( P2_RIGHT, 0x07200E, 0x80 ),
   INP0( P2_B1, 0x072006, 0x10 ),

   END_INPUT
};

static struct ROMSW_DATA romsw_data_space_invaders_dx_0[] =
{
   { "Taito Japan",           0x01 },
   { "Taito America",         0x02 },
   { NULL,		      0    },
};

static struct ROMSW_INFO space_invaders_dx_romsw[] =
{
   { 0x03FFFF, 0x02, romsw_data_space_invaders_dx_0 },
   { 0,        0,    NULL },
};

static struct VIDEO_INFO space_invaders_dx_video =
{
   draw_space_dx,
   320,
   224,
   32,
   VIDEO_ROTATE_NORMAL,
};

GAME( space_invaders_dx ,
   space_invaders_dx_dirs,
   space_invaders_dx_roms,
   space_invaders_dx_inputs,
   NULL,
   space_invaders_dx_romsw,

   load_space_dx,
   clear_puzzle_bobble_b_system,
   &space_invaders_dx_video,
   execute_space_dx_frame,
   "spacedxj",
   "Space Invaders DX",
   "�X�y�[�X�C���x�[�_�[�c�w",
   COMPANY_ID_TAITO,
   "D89",
   1994,
   taito_ym2610_sound,
   GAME_SHOOT
);

static UINT8 *RAM_INPUT;
static UINT8 *RAM_VIDEO;
static UINT8 *RAM_VIDEO_2;
static UINT8 *RAM_COLOUR;

static UINT8 *GFX_BG0;
static UINT8 *GFX_BG0_SOLID;

static UINT8 *GFX_BG2;
static UINT8 *GFX_BG2_SOLID;

void load_quiz_sekai_wa_show_by_shobai(void)
{
   int ta,tb,tc;

   if(!(RAM=AllocateMem(0x200000))) return;
   if(!(GFX=AllocateMem(0x840000))) return;

   GFX_BG0 = GFX+0x000000;
   GFX_BG2 = GFX+0x800000;

   if(!load_rom("d72-02.bin", RAM, 0x200000)) return;
   tb=0x800000;
   for(ta=0;ta<0x10000;ta+=2){
      tc=RAM[ta+0x40001];
      GFX[tb+0]=((tc&0x80)>>7)<<0;
      GFX[tb+1]=((tc&0x40)>>6)<<0;
      GFX[tb+2]=((tc&0x20)>>5)<<0;
      GFX[tb+3]=((tc&0x10)>>4)<<0;
      GFX[tb+4]=((tc&0x08)>>3)<<0;
      GFX[tb+5]=((tc&0x04)>>2)<<0;
      GFX[tb+6]=((tc&0x02)>>1)<<0;
      GFX[tb+7]=((tc&0x01)>>0)<<0;
      tc=RAM[ta+0x40000];
      GFX[tb+0]|=((tc&0x80)>>7)<<1;
      GFX[tb+1]|=((tc&0x40)>>6)<<1;
      GFX[tb+2]|=((tc&0x20)>>5)<<1;
      GFX[tb+3]|=((tc&0x10)>>4)<<1;
      GFX[tb+4]|=((tc&0x08)>>3)<<1;
      GFX[tb+5]|=((tc&0x04)>>2)<<1;
      GFX[tb+6]|=((tc&0x02)>>1)<<1;
      GFX[tb+7]|=((tc&0x01)>>0)<<1;
      tb+=8;
   }
   tb=0;
   for(ta=0;ta<0x200000;ta+=2){
      tc=RAM[ta+1];
      GFX[tb+0]=((tc&0x80)>>7)<<0;
      GFX[tb+1]=((tc&0x40)>>6)<<0;
      GFX[tb+2]=((tc&0x20)>>5)<<0;
      GFX[tb+3]=((tc&0x10)>>4)<<0;
      GFX[tb+4]=((tc&0x08)>>3)<<0;
      GFX[tb+5]=((tc&0x04)>>2)<<0;
      GFX[tb+6]=((tc&0x02)>>1)<<0;
      GFX[tb+7]=((tc&0x01)>>0)<<0;
      tc=RAM[ta];
      GFX[tb+0]|=((tc&0x80)>>7)<<1;
      GFX[tb+1]|=((tc&0x40)>>6)<<1;
      GFX[tb+2]|=((tc&0x20)>>5)<<1;
      GFX[tb+3]|=((tc&0x10)>>4)<<1;
      GFX[tb+4]|=((tc&0x08)>>3)<<1;
      GFX[tb+5]|=((tc&0x04)>>2)<<1;
      GFX[tb+6]|=((tc&0x02)>>1)<<1;
      GFX[tb+7]|=((tc&0x01)>>0)<<1;
      tb+=16;
      if((tb&0x7F)==0){tb-=0x78;}
      else{if((tb&0x7F)==8){tb-=0x08;}}
   }
   if(!load_rom("d72-03.bin", RAM, 0x200000)) return;
   tb=0x800000;
   for(ta=0;ta<0x10000;ta+=2){
      tc=RAM[ta+0x40001];
      GFX[tb+0]|=((tc&0x80)>>7)<<2;
      GFX[tb+1]|=((tc&0x40)>>6)<<2;
      GFX[tb+2]|=((tc&0x20)>>5)<<2;
      GFX[tb+3]|=((tc&0x10)>>4)<<2;
      GFX[tb+4]|=((tc&0x08)>>3)<<2;
      GFX[tb+5]|=((tc&0x04)>>2)<<2;
      GFX[tb+6]|=((tc&0x02)>>1)<<2;
      GFX[tb+7]|=((tc&0x01)>>0)<<2;
      tc=RAM[ta+0x40000];
      GFX[tb+0]|=((tc&0x80)>>7)<<3;
      GFX[tb+1]|=((tc&0x40)>>6)<<3;
      GFX[tb+2]|=((tc&0x20)>>5)<<3;
      GFX[tb+3]|=((tc&0x10)>>4)<<3;
      GFX[tb+4]|=((tc&0x08)>>3)<<3;
      GFX[tb+5]|=((tc&0x04)>>2)<<3;
      GFX[tb+6]|=((tc&0x02)>>1)<<3;
      GFX[tb+7]|=((tc&0x01)>>0)<<3;
      tb+=8;
   }
   tb=0;
   for(ta=0;ta<0x200000;ta+=2){
      tc=RAM[ta+1];
      GFX[tb+0]|=((tc&0x80)>>7)<<2;
      GFX[tb+1]|=((tc&0x40)>>6)<<2;
      GFX[tb+2]|=((tc&0x20)>>5)<<2;
      GFX[tb+3]|=((tc&0x10)>>4)<<2;
      GFX[tb+4]|=((tc&0x08)>>3)<<2;
      GFX[tb+5]|=((tc&0x04)>>2)<<2;
      GFX[tb+6]|=((tc&0x02)>>1)<<2;
      GFX[tb+7]|=((tc&0x01)>>0)<<2;
      tc=RAM[ta];
      GFX[tb+0]|=((tc&0x80)>>7)<<3;
      GFX[tb+1]|=((tc&0x40)>>6)<<3;
      GFX[tb+2]|=((tc&0x20)>>5)<<3;
      GFX[tb+3]|=((tc&0x10)>>4)<<3;
      GFX[tb+4]|=((tc&0x08)>>3)<<3;
      GFX[tb+5]|=((tc&0x04)>>2)<<3;
      GFX[tb+6]|=((tc&0x02)>>1)<<3;
      GFX[tb+7]|=((tc&0x01)>>0)<<3;
      tb+=16;
      if((tb&0x7F)==0){tb-=0x78;}
      else{if((tb&0x7F)==8){tb-=0x08;}}
   }

   FreeMem(RAM);

   RAMSize=0x40000+0x20000;

   if(!(RAM=AllocateMem(0x80000))) return;

   /*-----[Sound Setup]-----*/

   Z80ROM=RAM+0x40000;
   if(!load_rom("d72-11.bin", Z80ROM, 0x20000)) return;         // Z80 SOUND ROM

   if(!(PCMROM=AllocateMem(0x200000))) return;
   if(!load_rom("d72-01.bin",PCMROM,0x200000)) return;          // ADPCM A rom
   YM2610SetBuffers(PCMROM, PCMROM, 0x200000, 0x200000);

   AddTaitoYM2610(0x017F, 0x0152, 0x20000);

   /*-----------------------*/

   memset(RAM+0x00000,0x00,0x40000);
   memset(RAM+0x26000,0xFF,0x00100);

   RAM_INPUT  = RAM+0x26000;
   RAM_VIDEO  = RAM+0x10000;
   RAM_COLOUR = RAM+0x24000;
   RAM_VIDEO_2= RAM+0x26100;

   GFX_BG0_SOLID = make_solid_mask_16x16(GFX_BG0, 0x8000);
   GFX_BG2_SOLID = make_solid_mask_8x8	(GFX_BG2, 0x1000);

   InitPaletteMap(RAM_COLOUR, 0x100, 0x10, 0x1000);

   set_colour_mapper(&col_map_rrrr_gggg_bbbb_xxxx);

/*
   // EEPROM Access Hacks

   WriteLong68k(&ROM[0x81EE],0x13FC0000);	// move.b #$00,$BB0000
   WriteLong68k(&ROM[0x81F2],0x00BB0000);	// (EEP-ROM READ HACK)
   WriteWord68k(&ROM[0x81F6],0x4E75);

   WriteLong68k(&ROM[0x826A],0x13FC0001);	// move.b #$01,$BB0000
   WriteLong68k(&ROM[0x826E],0x00BB0000);	// (EEP-ROM WRITE HACK)
   WriteWord68k(&ROM[0x8272],0x4E75);

   // Fix ROM Checksum

   WriteWord68k(&ROM[0x1AF2],0x4E75);		// rts
*/
   // speed hack

   WriteLong68k(&ROM[0x09C0],0x13FC0000);	// move.b #$00,$AA0000
   WriteLong68k(&ROM[0x09C4],0x00AA0000);	//


   // Init tc0180vcu emulation
   // ------------------------

   tc0180vcu.RAM	= RAM_VIDEO;
   tc0180vcu.RAM_2	= RAM_VIDEO_2;
   tc0180vcu.GFX_BG0	= GFX_BG0;
   tc0180vcu.GFX_BG0_MSK= GFX_BG0_SOLID;
   tc0180vcu.GFX_BG2	= GFX_BG2;
   tc0180vcu.GFX_BG2_MSK= GFX_BG2_SOLID;
   tc0180vcu.tile_mask	= 0x7FFF;
   tc0180vcu.bmp_x	= 32;
   tc0180vcu.bmp_y	= 32;
   tc0180vcu.bmp_w	= 320;
   tc0180vcu.bmp_h	= 224;
   tc0180vcu.scr_x	= 0;
   tc0180vcu.scr_y	= 16;

   vcu_make_col_bankmap(0x40,0x00,0x40,0x80,0xC0);

/*
 *  StarScream Stuff follows
 */

   ByteSwap(ROM,0x100000);
   ByteSwap(RAM,0x02C000);

   AddMemFetch(0x000000, 0x0FFFFF, ROM+0x000000-0x000000);	// 68000 ROM
   AddMemFetch(-1, -1, NULL);

   AddReadByte(0x000000, 0x0FFFFF, NULL, ROM+0x000000); 		// 68000 ROM
   AddReadByte(0x900000, 0x90FFFF, NULL, RAM+0x000000); 		// 68000 RAM
   AddReadByte(0x400000, 0x413FFF, NULL, RAM_VIDEO);			// SCREEN RAM
   AddReadByte(0x200000, 0x2000FF, NULL, RAM_INPUT);			// INPUT
   AddReadByte(0x800000, 0x801FFF, NULL, RAM_COLOUR);			// COLOR RAM
   AddReadByte(0x418000, 0x41801F, NULL, RAM_VIDEO_2);			// SCREEN RAM
   AddReadByte(0x600000, 0x600003, tc0140syt_read_main_68k, NULL);	// SOUND COMM
   AddReadByte(0x000000, 0xFFFFFF, DefBadReadByte, NULL);		// <Bad Reads>
   AddReadByte(-1, -1, NULL, NULL);

   AddReadWord(0x000000, 0x0FFFFF, NULL, ROM+0x000000); 		// 68000 ROM
   AddReadWord(0x900000, 0x90FFFF, NULL, RAM+0x000000); 		// 68000 RAM
   AddReadWord(0x400000, 0x413FFF, NULL, RAM_VIDEO);			// SCREEN RAM
   AddReadWord(0x800000, 0x801FFF, NULL, RAM_COLOUR);			// COLOR RAM
   AddReadWord(0x418000, 0x41801F, NULL, RAM_VIDEO_2);			// SCREEN RAM
   AddReadWord(0x200000, 0x2000FF, NULL, RAM_INPUT);			// INPUT
   AddReadWord(0x000000, 0xFFFFFF, DefBadReadWord, NULL);		// <Bad Reads>
   AddReadWord(-1, -1,NULL, NULL);

   AddWriteByte(0x900000, 0x90FFFF, NULL, RAM+0x000000);		// 68000 RAM
   AddWriteByte(0x400000, 0x413FFF, NULL, RAM_VIDEO);			// SCREEN RAM
   AddWriteByte(0x800000, 0x801FFF, NULL, RAM_COLOUR);			// COLOR RAM
   AddWriteByte(0x600000, 0x600003, tc0140syt_write_main_68k, NULL);	// SOUND COMM
 //AddWriteByte(0x200000, 0x20000F, tc0220ioc_wb, NULL);		// INPUT
   AddWriteByte(0x418000, 0x41801F, NULL, RAM_VIDEO_2); 		// SCREEN RAM
   AddWriteByte(0xAA0000, 0xAA0001, Stop68000, NULL);			// Trap Idle 68000
   AddWriteByte(0x000000, 0xFFFFFF, DefBadWriteByte, NULL);		// <Bad Writes>
   AddWriteByte(-1, -1, NULL, NULL);

   AddWriteWord(0x900000, 0x90FFFF, NULL, RAM+0x000000);		// 68000 RAM
   AddWriteWord(0x400000, 0x413FFF, NULL, RAM_VIDEO);			// SCREEN RAM
   AddWriteWord(0x800000, 0x801FFF, NULL, RAM_COLOUR);			// COLOR RAM
   AddWriteWord(0x418000, 0x41801F, NULL, RAM_VIDEO_2); 		// SCREEN RAM
 //AddWriteWord(0x200000, 0x20000F, tc0220ioc_ww, NULL);		// INPUT
   AddWriteWord(0x000000, 0xFFFFFF, DefBadWriteWord, NULL);		// <Bad Writes>
   AddWriteWord(-1, -1, NULL, NULL);

   AddInitMemory();	// Set Starscream mem pointers...
}

void clear_quiz_sekai_wa_show_by_shobai(void)
{
   RemoveTaitoYM2610();

#ifdef RAINE_DEBUG
      save_debug("ROM.bin",ROM,0x100000,1);
      save_debug("RAM.bin",RAM,0x040000,1);
#endif
}

void execute_quiz_sekai_wa_show_by_shobai_frame(void)
{
#ifdef RAINE_DEBUG
   vcu_debug_info();
#endif

   cpu_execute_cycles(CPU_68K_0, CPU_FRAME_MHz(12,60)); // M68000 12MHz (60fps)

   cpu_interrupt(CPU_68K_0, 5);
   cpu_interrupt(CPU_68K_0, 3); 	// <rte>

   Taito2610_Frame();			// Z80 and YM2610
}

static UINT8 pb_input_rb(UINT32 offset)
{
   switch(offset & 0x3E){
      case 0x02:
	 return (RAM_INPUT[2] & 0xFE) | eeprom_93c46_rb();
      break;
      default:
	 return RAM_INPUT[offset & 0x3E];
      break;
   }
}

static UINT16 pb_input_rw(UINT32 offset)
{
   return pb_input_rb(offset);
}

static void pb_input_wb(UINT32 offset, UINT8 data)
{
   switch(offset & 0x3E){
      case 0x26:

	 if(data&0x08)

	    eeprom_93c46_wb((data & 0x04) >> 2);

      break;
      default:
#ifdef RAINE_DEBUG
	    //print_debug("pb_input_wb(%02x,%02x)\n", offset & 0x3E, data);
#endif
      break;
   }

}

static void pb_input_ww(UINT32 offset, UINT16 data)
{
   pb_input_wb(offset, data);
}

void load_puzzle_bobble_b_system(void)
{
   int ta,tb,tc;

   if(!(RAM=AllocateMem(0x080000))) return;
   if(!(GFX=AllocateMem(0x240000))) return;

   GFX_BG0 = GFX+0x000000;
   GFX_BG2 = GFX+0x200000;

   if(!load_rom("pb-ic9.bin", RAM, 0x080000)) return;
   tb=0x200000;
   for(ta=0;ta<0x10000;ta+=2){
      tc=RAM[ta+0x00001];
      GFX[tb+0]=((tc&0x80)>>7)<<0;
      GFX[tb+1]=((tc&0x40)>>6)<<0;
      GFX[tb+2]=((tc&0x20)>>5)<<0;
      GFX[tb+3]=((tc&0x10)>>4)<<0;
      GFX[tb+4]=((tc&0x08)>>3)<<0;
      GFX[tb+5]=((tc&0x04)>>2)<<0;
      GFX[tb+6]=((tc&0x02)>>1)<<0;
      GFX[tb+7]=((tc&0x01)>>0)<<0;
      tc=RAM[ta+0x00000];
      GFX[tb+0]|=((tc&0x80)>>7)<<1;
      GFX[tb+1]|=((tc&0x40)>>6)<<1;
      GFX[tb+2]|=((tc&0x20)>>5)<<1;
      GFX[tb+3]|=((tc&0x10)>>4)<<1;
      GFX[tb+4]|=((tc&0x08)>>3)<<1;
      GFX[tb+5]|=((tc&0x04)>>2)<<1;
      GFX[tb+6]|=((tc&0x02)>>1)<<1;
      GFX[tb+7]|=((tc&0x01)>>0)<<1;
      tb+=8;
   }
   tb=0;
   for(ta=0;ta<0x080000;ta+=2){
      tc=RAM[ta+1];
      GFX[tb+0]=((tc&0x80)>>7)<<0;
      GFX[tb+1]=((tc&0x40)>>6)<<0;
      GFX[tb+2]=((tc&0x20)>>5)<<0;
      GFX[tb+3]=((tc&0x10)>>4)<<0;
      GFX[tb+4]=((tc&0x08)>>3)<<0;
      GFX[tb+5]=((tc&0x04)>>2)<<0;
      GFX[tb+6]=((tc&0x02)>>1)<<0;
      GFX[tb+7]=((tc&0x01)>>0)<<0;
      tc=RAM[ta];
      GFX[tb+0]|=((tc&0x80)>>7)<<1;
      GFX[tb+1]|=((tc&0x40)>>6)<<1;
      GFX[tb+2]|=((tc&0x20)>>5)<<1;
      GFX[tb+3]|=((tc&0x10)>>4)<<1;
      GFX[tb+4]|=((tc&0x08)>>3)<<1;
      GFX[tb+5]|=((tc&0x04)>>2)<<1;
      GFX[tb+6]|=((tc&0x02)>>1)<<1;
      GFX[tb+7]|=((tc&0x01)>>0)<<1;
      tb+=16;
      if((tb&0x7F)==0){tb-=0x78;}
      else{if((tb&0x7F)==8){tb-=0x08;}}
   }
   if(!load_rom("pb-ic14.bin", RAM, 0x080000)) return;
   tb=0x200000;
   for(ta=0;ta<0x10000;ta+=2){
      tc=RAM[ta+0x00001];
      GFX[tb+0]|=((tc&0x80)>>7)<<2;
      GFX[tb+1]|=((tc&0x40)>>6)<<2;
      GFX[tb+2]|=((tc&0x20)>>5)<<2;
      GFX[tb+3]|=((tc&0x10)>>4)<<2;
      GFX[tb+4]|=((tc&0x08)>>3)<<2;
      GFX[tb+5]|=((tc&0x04)>>2)<<2;
      GFX[tb+6]|=((tc&0x02)>>1)<<2;
      GFX[tb+7]|=((tc&0x01)>>0)<<2;
      tc=RAM[ta+0x00000];
      GFX[tb+0]|=((tc&0x80)>>7)<<3;
      GFX[tb+1]|=((tc&0x40)>>6)<<3;
      GFX[tb+2]|=((tc&0x20)>>5)<<3;
      GFX[tb+3]|=((tc&0x10)>>4)<<3;
      GFX[tb+4]|=((tc&0x08)>>3)<<3;
      GFX[tb+5]|=((tc&0x04)>>2)<<3;
      GFX[tb+6]|=((tc&0x02)>>1)<<3;
      GFX[tb+7]|=((tc&0x01)>>0)<<3;
      tb+=8;
   }
   tb=0;
   for(ta=0;ta<0x080000;ta+=2){
      tc=RAM[ta+1];
      GFX[tb+0]|=((tc&0x80)>>7)<<2;
      GFX[tb+1]|=((tc&0x40)>>6)<<2;
      GFX[tb+2]|=((tc&0x20)>>5)<<2;
      GFX[tb+3]|=((tc&0x10)>>4)<<2;
      GFX[tb+4]|=((tc&0x08)>>3)<<2;
      GFX[tb+5]|=((tc&0x04)>>2)<<2;
      GFX[tb+6]|=((tc&0x02)>>1)<<2;
      GFX[tb+7]|=((tc&0x01)>>0)<<2;
      tc=RAM[ta];
      GFX[tb+0]|=((tc&0x80)>>7)<<3;
      GFX[tb+1]|=((tc&0x40)>>6)<<3;
      GFX[tb+2]|=((tc&0x20)>>5)<<3;
      GFX[tb+3]|=((tc&0x10)>>4)<<3;
      GFX[tb+4]|=((tc&0x08)>>3)<<3;
      GFX[tb+5]|=((tc&0x04)>>2)<<3;
      GFX[tb+6]|=((tc&0x02)>>1)<<3;
      GFX[tb+7]|=((tc&0x01)>>0)<<3;
      tb+=16;
      if((tb&0x7F)==0){tb-=0x78;}
      else{if((tb&0x7F)==8){tb-=0x08;}}
   }

   FreeMem(RAM);

   RAMSize=0x40000+0x20000;

   if(!(RAM=AllocateMem(0x80000))) return;

   /*-----[Sound Setup]-----*/

   Z80ROM=RAM+0x40000;
   if(!load_rom("pb-ic27.bin", Z80ROM, 0x20000)) return;        // Z80 SOUND ROM

   if(!(PCMROM=AllocateMem(0x100000))) return;
   if(!load_rom("pb-ic15.bin",PCMROM,0x100000)) return;         // ADPCM A rom
   YM2610SetBuffers(PCMROM, PCMROM, 0x100000, 0x100000);

   AddTaitoYM2610(0x017F, 0x0152, 0x20000);

   /*-----------------------*/

   memset(RAM+0x00000,0x00,0x40000);
   memset(RAM+0x26000,0xFF,0x00100);

   RAM_INPUT  = RAM+0x26000;
   RAM_VIDEO  = RAM+0x10000;
   RAM_COLOUR = RAM+0x24000;
   RAM_VIDEO_2= RAM+0x26100;

   GFX_BG0_SOLID = make_solid_mask_16x16(GFX_BG0, 0x2000);
   GFX_BG2_SOLID = make_solid_mask_8x8	(GFX_BG2, 0x1000);

   InitPaletteMap(RAM_COLOUR, 0x100, 0x10, 0x1000);

   set_colour_mapper(&col_map_rrrr_gggg_bbbb_xxxx);

   EEPROM = RAM + 0x26200;

   eeprom_data = (UINT16 *) EEPROM;

   add_eeprom(EEPROM, 0x80, EPR_INVALIDATE_ON_ROM_CHANGE);
   load_eeprom();

/*
   // Fix ROM Checksum

   WriteWord68k(&ROM[0x1AF2],0x4E75);		// rts
*/
   // speed hack
/*
   WriteLong68k(&ROM[0x09C0],0x13FC0000);	// move.b #$00,$AA0000
   WriteLong68k(&ROM[0x09C4],0x00AA0000);	//
*/

   // Init tc0180vcu emulation
   // ------------------------

   tc0180vcu.RAM	= RAM_VIDEO;
   tc0180vcu.RAM_2	= RAM_VIDEO_2;
   tc0180vcu.GFX_BG0	= GFX_BG0;
   tc0180vcu.GFX_BG0_MSK= GFX_BG0_SOLID;
   tc0180vcu.GFX_BG2	= GFX_BG2;
   tc0180vcu.GFX_BG2_MSK= GFX_BG2_SOLID;
   tc0180vcu.tile_mask	= 0x1FFF;
   tc0180vcu.bmp_x	= 32;
   tc0180vcu.bmp_y	= 32;
   tc0180vcu.bmp_w	= 320;
   tc0180vcu.bmp_h	= 224;
   tc0180vcu.scr_x	= 0;
   tc0180vcu.scr_y	= 16;

   vcu_make_col_bankmap(0x40,0x00,0x40,0x80,0xC0);

/*
 *  StarScream Stuff follows
 */

   ByteSwap(ROM,0x080000);
   ByteSwap(RAM,0x02C000);

   AddMemFetch(0x000000, 0x07FFFF, ROM+0x000000-0x000000);	// 68000 ROM
   AddMemFetch(-1, -1, NULL);

   AddReadByte(0x000000, 0x07FFFF, NULL, ROM+0x000000); 		// 68000 ROM
   AddReadByte(0x900000, 0x90FFFF, NULL, RAM+0x000000); 		// 68000 RAM
   AddReadByte(0x400000, 0x413FFF, NULL, RAM_VIDEO);			// SCREEN RAM
   AddReadByte(0x800000, 0x801FFF, NULL, RAM_COLOUR);			// COLOR RAM
   AddReadByte(0x418000, 0x41801F, NULL, RAM_VIDEO_2);			// SCREEN RAM
   AddReadByte(0x700000, 0x700003, tc0140syt_read_main_68k, NULL);	// SOUND COMM
   AddReadByte(0x500000, 0x50003F, pb_input_rb, NULL);			// input
   AddReadByte(0x000000, 0xFFFFFF, DefBadReadByte, NULL);		// <Bad Reads>
   AddReadByte(-1, -1, NULL, NULL);

   AddReadWord(0x000000, 0x07FFFF, NULL, ROM+0x000000); 		// 68000 ROM
   AddReadWord(0x900000, 0x90FFFF, NULL, RAM+0x000000); 		// 68000 RAM
   AddReadWord(0x400000, 0x413FFF, NULL, RAM_VIDEO);			// SCREEN RAM
   AddReadWord(0x800000, 0x801FFF, NULL, RAM_COLOUR);			// COLOR RAM
   AddReadWord(0x418000, 0x41801F, NULL, RAM_VIDEO_2);			// SCREEN RAM
   AddReadWord(0x500000, 0x50003F, pb_input_rw, NULL);			// input
   AddReadWord(0x000000, 0xFFFFFF, DefBadReadWord, NULL);		// <Bad Reads>
   AddReadWord(-1, -1,NULL, NULL);

   AddWriteByte(0x900000, 0x90FFFF, NULL, RAM+0x000000);		// 68000 RAM
   AddWriteByte(0x400000, 0x413FFF, NULL, RAM_VIDEO);			// SCREEN RAM
   AddWriteByte(0x800000, 0x801FFF, NULL, RAM_COLOUR);			// COLOR RAM
   AddWriteByte(0x700000, 0x700003, tc0140syt_write_main_68k, NULL);	// SOUND COMM
   AddWriteByte(0x418000, 0x41801F, NULL, RAM_VIDEO_2); 		// SCREEN RAM
   AddWriteByte(0xAA0000, 0xAA0001, Stop68000, NULL);			// Trap Idle 68000
   AddWriteByte(0x500000, 0x50003F, pb_input_wb, NULL); 		// input
   AddWriteByte(0x000000, 0xFFFFFF, DefBadWriteByte, NULL);		// <Bad Writes>
   AddWriteByte(-1, -1, NULL, NULL);

   AddWriteWord(0x900000, 0x90FFFF, NULL, RAM+0x000000);		// 68000 RAM
   AddWriteWord(0x400000, 0x413FFF, NULL, RAM_VIDEO);			// SCREEN RAM
   AddWriteWord(0x800000, 0x801FFF, NULL, RAM_COLOUR);			// COLOR RAM
   AddWriteWord(0x418000, 0x41801F, NULL, RAM_VIDEO_2); 		// SCREEN RAM
   AddWriteWord(0x500000, 0x50003F, pb_input_ww, NULL); 		// input
   AddWriteWord(0x000000, 0xFFFFFF, DefBadWriteWord, NULL);		// <Bad Writes>
   AddWriteWord(-1, -1, NULL, NULL);

   AddInitMemory();	// Set Starscream mem pointers...
}

void clear_puzzle_bobble_b_system(void)
{
   save_eeprom();

   RemoveTaitoYM2610();

#ifdef RAINE_DEBUG
      save_debug("RAM.bin",RAM,0x040000,1);
#endif
}

void execute_space_dx_frame(void)
{
#ifdef RAINE_DEBUG
   vcu_debug_info();
#endif

   cpu_execute_cycles(CPU_68K_0, CPU_FRAME_MHz(12,60)); // M68000 12MHz (60fps)

   cpu_interrupt(CPU_68K_0, 5);
   cpu_interrupt(CPU_68K_0, 3);

   Taito2610_Frame();			// Z80 and YM2610
}

void execute_puzzle_bobble_b_system_frame(void)
{
#ifdef RAINE_DEBUG
   vcu_debug_info();
#endif

   cpu_execute_cycles(CPU_68K_0, CPU_FRAME_MHz(10,60)); // M68000 12MHz (60fps)

   cpu_interrupt(CPU_68K_0, 3);

   cpu_execute_cycles(CPU_68K_0, CPU_FRAME_MHz(2,60));	// M68000 12MHz (60fps)
   cpu_interrupt(CPU_68K_0, 5);

   Taito2610_Frame();			// Z80 and YM2610
}

void load_space_dx(void)
{
   int ta,tb,tc;

   if(!(RAM=AllocateMem(0x80000+0x10000))) return;
   if(!(GFX=AllocateMem(0x140000))) return;

   GFX_BG0 = GFX+0x000000;
   GFX_BG2 = GFX+0x100000;

   if(!load_rom("d89-02.14", RAM, 0x40000)) return;
   tb=0;
   for(ta=0;ta<0x40000;ta+=2){
      tc=RAM[ta+0];
      GFX[tb+0]=((tc&0x80)>>7)<<3;
      GFX[tb+1]=((tc&0x40)>>6)<<3;
      GFX[tb+2]=((tc&0x20)>>5)<<3;
      GFX[tb+3]=((tc&0x10)>>4)<<3;
      GFX[tb+4]=((tc&0x08)>>3)<<3;
      GFX[tb+5]=((tc&0x04)>>2)<<3;
      GFX[tb+6]=((tc&0x02)>>1)<<3;
      GFX[tb+7]=((tc&0x01)>>0)<<3;
      tc=RAM[ta+1];
      GFX[tb+0]|=((tc&0x80)>>7)<<2;
      GFX[tb+1]|=((tc&0x40)>>6)<<2;
      GFX[tb+2]|=((tc&0x20)>>5)<<2;
      GFX[tb+3]|=((tc&0x10)>>4)<<2;
      GFX[tb+4]|=((tc&0x08)>>3)<<2;
      GFX[tb+5]|=((tc&0x04)>>2)<<2;
      GFX[tb+6]|=((tc&0x02)>>1)<<2;
      GFX[tb+7]|=((tc&0x01)>>0)<<2;
      tb+=16;
      if((tb&0x7F)==0){tb-=0x78;}
      else{if((tb&0x7F)==8){tb-=8;}}
   }
   for(ta=0;ta<0x10000;ta+=2){
      tc=RAM[ta+0];
      GFX[tb+0]=((tc&0x80)>>7)<<3;
      GFX[tb+1]=((tc&0x40)>>6)<<3;
      GFX[tb+2]=((tc&0x20)>>5)<<3;
      GFX[tb+3]=((tc&0x10)>>4)<<3;
      GFX[tb+4]=((tc&0x08)>>3)<<3;
      GFX[tb+5]=((tc&0x04)>>2)<<3;
      GFX[tb+6]=((tc&0x02)>>1)<<3;
      GFX[tb+7]=((tc&0x01)>>0)<<3;
      tc=RAM[ta+1];
      GFX[tb+0]|=((tc&0x80)>>7)<<2;
      GFX[tb+1]|=((tc&0x40)>>6)<<2;
      GFX[tb+2]|=((tc&0x20)>>5)<<2;
      GFX[tb+3]|=((tc&0x10)>>4)<<2;
      GFX[tb+4]|=((tc&0x08)>>3)<<2;
      GFX[tb+5]|=((tc&0x04)>>2)<<2;
      GFX[tb+6]|=((tc&0x02)>>1)<<2;
      GFX[tb+7]|=((tc&0x01)>>0)<<2;
      tb+=8;
   }
   if(!load_rom("d89-01.9", RAM, 0x40000)) return;
   tb=0;
   for(ta=0;ta<0x40000;ta+=2){
      tc=RAM[ta+0];
      GFX[tb+0]|=((tc&0x80)>>7)<<1;
      GFX[tb+1]|=((tc&0x40)>>6)<<1;
      GFX[tb+2]|=((tc&0x20)>>5)<<1;
      GFX[tb+3]|=((tc&0x10)>>4)<<1;
      GFX[tb+4]|=((tc&0x08)>>3)<<1;
      GFX[tb+5]|=((tc&0x04)>>2)<<1;
      GFX[tb+6]|=((tc&0x02)>>1)<<1;
      GFX[tb+7]|=((tc&0x01)>>0)<<1;
      tc=RAM[ta+1];
      GFX[tb+0]|=((tc&0x80)>>7)<<0;
      GFX[tb+1]|=((tc&0x40)>>6)<<0;
      GFX[tb+2]|=((tc&0x20)>>5)<<0;
      GFX[tb+3]|=((tc&0x10)>>4)<<0;
      GFX[tb+4]|=((tc&0x08)>>3)<<0;
      GFX[tb+5]|=((tc&0x04)>>2)<<0;
      GFX[tb+6]|=((tc&0x02)>>1)<<0;
      GFX[tb+7]|=((tc&0x01)>>0)<<0;
      tb+=16;
      if((tb&0x7F)==0){tb-=0x78;}
      else{if((tb&0x7F)==8){tb-=8;}}
   }
   for(ta=0;ta<0x10000;ta+=2){
      tc=RAM[ta+0];
      GFX[tb+0]|=((tc&0x80)>>7)<<1;
      GFX[tb+1]|=((tc&0x40)>>6)<<1;
      GFX[tb+2]|=((tc&0x20)>>5)<<1;
      GFX[tb+3]|=((tc&0x10)>>4)<<1;
      GFX[tb+4]|=((tc&0x08)>>3)<<1;
      GFX[tb+5]|=((tc&0x04)>>2)<<1;
      GFX[tb+6]|=((tc&0x02)>>1)<<1;
      GFX[tb+7]|=((tc&0x01)>>0)<<1;
      tc=RAM[ta+1];
      GFX[tb+0]|=((tc&0x80)>>7)<<0;
      GFX[tb+1]|=((tc&0x40)>>6)<<0;
      GFX[tb+2]|=((tc&0x20)>>5)<<0;
      GFX[tb+3]|=((tc&0x10)>>4)<<0;
      GFX[tb+4]|=((tc&0x08)>>3)<<0;
      GFX[tb+5]|=((tc&0x04)>>2)<<0;
      GFX[tb+6]|=((tc&0x02)>>1)<<0;
      GFX[tb+7]|=((tc&0x01)>>0)<<0;
      tb+=8;
   }

   /*-----[Sound Setup]-----*/

   Z80ROM=RAM+0x80000;
   if(!load_rom("d89-07.27", Z80ROM, 0x10000)) return;          // Z80 SOUND ROM

   if(!(PCMROM=AllocateMem(0x80000))) return;
   if(!load_rom("d89-03.15",PCMROM,0x80000)) return;            // ADPCM A rom
   YM2610SetBuffers(PCMROM, PCMROM, 0x80000, 0x80000);

   AddTaitoYM2610(0x017F, 0x0152, 0x10000);

   /*-----------------------*/

   memset(RAM+0x00000,0x00,0x80000);
   memset(RAM+0x72000,0xFF,0x00100);

   RAMSize=0x80000+0x10000;

   EEPROM = RAM + 0x72200;

   eeprom_data = (UINT16 *) EEPROM;

   add_eeprom(EEPROM, 0x80, EPR_INVALIDATE_ON_ROM_CHANGE);
   load_eeprom();

   RAM_VIDEO  = RAM+0x50000;
   RAM_INPUT  = RAM+0x72000;
   RAM_COLOUR = RAM+0x70000;

   GFX_BG0_SOLID = make_solid_mask_16x16(GFX_BG0, 0x1000);
   GFX_BG2_SOLID = make_solid_mask_8x8	(GFX_BG2, 0x1000);

   InitPaletteMap(RAM_COLOUR, 0x100, 0x10, 0x1000);

   set_colour_mapper(&col_map_rrrr_gggg_bbbb_xxxx);

   // EEPROM Access Hacks
/*
   WriteLong68k(&ROM[0x81EE],0x13FC0000);	// move.b #$00,$BB0000
   WriteLong68k(&ROM[0x81F2],0x00BB0000);	// (EEP-ROM READ HACK)
   WriteWord68k(&ROM[0x81F6],0x4E75);

   WriteLong68k(&ROM[0x826A],0x13FC0001);	// move.b #$01,$BB0000
   WriteLong68k(&ROM[0x826E],0x00BB0000);	// (EEP-ROM WRITE HACK)
   WriteWord68k(&ROM[0x8272],0x4E75);
*/
   // Fix ROM Checksum

   WriteWord68k(&ROM[0x1AF2],0x4E75);		// rts

   // Speed Hacking

   WriteLong68k(&ROM[0x08F2],0x4E714E71);	// nop; nop
   WriteLong68k(&ROM[0x08F6],0x4E714E71);	// nop; nop
   WriteLong68k(&ROM[0x08FA],0x4E714E71);	// nop; nop

   WriteLong68k(&ROM[0x05DA],0x13FC0000);	// move.b #$00,$AA0000
   WriteLong68k(&ROM[0x05DE],0x00AA0000);	// (SPEED HACK)
   WriteLong68k(&ROM[0x05E2],0x4E714E71);	// nop; nop

   // Init tc0180vcu emulation
   // ------------------------

   tc0180vcu.RAM	= RAM_VIDEO;
   tc0180vcu.RAM_2	= RAM_VIDEO+0x18000;
   tc0180vcu.GFX_BG0	= GFX_BG0;
   tc0180vcu.GFX_BG0_MSK= GFX_BG0_SOLID;
   tc0180vcu.GFX_BG2	= GFX_BG2;
   tc0180vcu.GFX_BG2_MSK= GFX_BG2_SOLID;
   tc0180vcu.tile_mask	= 0x0FFF;
   tc0180vcu.bmp_x	= 32;
   tc0180vcu.bmp_y	= 32;
   tc0180vcu.bmp_w	= 320;
   tc0180vcu.bmp_h	= 224;
   tc0180vcu.scr_x	= 0;
   tc0180vcu.scr_y	= 16;

   vcu_make_col_bankmap(0x40,0x00,0x40,0x80,0xC0);

/*
 *  StarScream Stuff follows
 */

   ByteSwap(ROM,0x40000);
   ByteSwap(RAM,0x72000);

   AddMemFetch(0x000000, 0x03FFFF, ROM+0x000000-0x000000);	// 68000 ROM
   AddMemFetch(-1, -1, NULL);

   AddReadByte(0x000000, 0x03FFFF, NULL, ROM+0x000000); 		// 68000 ROM
   AddReadByte(0x900000, 0x90FFFF, NULL, RAM+0x000000); 		// 68000 RAM
   AddReadByte(0x440000, 0x47FFFF, NULL, RAM+0x010000); 		// SCREEN RAM
   AddReadByte(0x400000, 0x41FFFF, NULL, RAM_VIDEO);			// SCREEN RAM
   AddReadByte(0x800000, 0x801FFF, NULL, RAM_COLOUR);			// COLOR RAM
   AddReadByte(0x500000, 0x50003F, pb_input_rb, NULL);			// input
   AddReadByte(0x700000, 0x700003, tc0140syt_read_main_68k, NULL);	// SOUND COMM
   AddReadByte(0x000000, 0xFFFFFF, DefBadReadByte, NULL);		// <Bad Reads>
   AddReadByte(-1, -1, NULL, NULL);

   AddReadWord(0x000000, 0x03FFFF, NULL, ROM+0x000000); 		// 68000 ROM
   AddReadWord(0x900000, 0x90FFFF, NULL, RAM+0x000000); 		// 68000 RAM
   AddReadWord(0x440000, 0x47FFFF, NULL, RAM+0x010000); 		// SCREEN RAM
   AddReadWord(0x400000, 0x41FFFF, NULL, RAM_VIDEO);			// SCREEN RAM
   AddReadWord(0x800000, 0x801FFF, NULL, RAM_COLOUR);			// COLOR RAM
   AddReadWord(0x500000, 0x50003F, pb_input_rw, NULL);			// input
   AddReadWord(0x000000, 0xFFFFFF, DefBadReadWord, NULL);		// <Bad Reads>
   AddReadWord(-1, -1,NULL, NULL);

   AddWriteByte(0x900000, 0x90FFFF, NULL, RAM+0x000000);		// 68000 RAM
   AddWriteByte(0x440000, 0x47FFFF, NULL, RAM+0x010000);		// SCREEN RAM
   AddWriteByte(0x400000, 0x41FFFF, NULL, RAM_VIDEO);			// SCREEN RAM
   AddWriteByte(0x800000, 0x801FFF, NULL, RAM_COLOUR);			// COLOR RAM
   AddWriteByte(0x700000, 0x700003, tc0140syt_write_main_68k, NULL);	// SOUND COMM
   AddWriteByte(0xAA0000, 0xAA0001, Stop68000, NULL);			// Trap Idle 68000
   AddWriteByte(0x500000, 0x50003F, pb_input_wb, NULL); 		// input
   AddWriteByte(0x000000, 0xFFFFFF, DefBadWriteByte, NULL);		// <Bad Writes>
   AddWriteByte(-1, -1, NULL, NULL);

   AddWriteWord(0x900000, 0x90FFFF, NULL, RAM+0x000000);		// 68000 RAM
   AddWriteWord(0x440000, 0x47FFFF, NULL, RAM+0x010000);		// SCREEN RAM
   AddWriteWord(0x400000, 0x41FFFF, NULL, RAM_VIDEO);			// SCREEN RAM
   AddWriteWord(0x800000, 0x801FFF, NULL, RAM_COLOUR);			// COLOR RAM
   AddWriteWord(0x500000, 0x50003F, pb_input_ww, NULL); 		// input
   AddWriteWord(0x000000, 0xFFFFFF, DefBadWriteWord, NULL);		// <Bad Writes>
   AddWriteWord(-1, -1, NULL, NULL);

   AddInitMemory();	// Set Starscream mem pointers...
}

void draw_space_dx(void)
{
   static int do_clear;

   int x,y,ta,zz;
   UINT8 *bit,*map;

   ClearPaletteMap();

   // Init tc0180vcu emulation
   // ------------------------

   tc0180vcu_layer_count = 0;

   // BG0
   // ---

   vcu_render_bg0();

   // BG0
   // ---

   vcu_render_bg1();

   // PIXEL
   // -----

   if(1) { // (RAM_VIDEO[0x1800F]&0x80)!=0){

   if((RAM_VIDEO[0x1800F]&0x40)!=0)
      zz=0x00000;
   else
      zz=0x20000;

   zz+=(16*512);
   zz&=0x3FFFF;

   for(ta=15;ta>=0;ta--){

      MAP_PALETTE_MAPPED_NEW(
	 ta|0x80,
	 16,
	 map
      );

   }

   for(y=0;y<224;y++){
   bit = GameViewBitmap->line[y];
   zz+=0x10000;
   for(x=0;x<320;x+=2){
      if(RAM[zz]!=0){bit[x+1] = map[RAM[zz]];}
      if(RAM[zz+1]!=0){bit[x] = map[RAM[zz+1]];}
      zz+=2;
   }
   zz-=0x10000;
   zz+=(512-320);
   zz&=0x3FFFF;
   }

   do_clear=1;
   }
   else{
      if(do_clear){
	 do_clear=0;
	 memset(RAM+0x10000,0x00,0x40000);
      }
   }

   // BG2
   // ---

   vcu_render_bg2();

}


/******************************************************************************/
/*                                                                            */
/*                  BUBBLE BOBBLE (C) 1984 TAITO CORPORATION.                 */
/*                                                                            */
/*  Driver by Richard Mitton (richard.mitton@bigfoot.com)                     */
/*  Similiar hardware is used by Bobble Bobble, and Tokio/Scramble Formation. */
/******************************************************************************/

#include "gameinc.h"
#include "bublbobl.h"
#include "sasound.h"		// sample support routines
#include "taitosnd.h"
#include "2203intf.h"
#include "3812intf.h"
#include "savegame.h"
#include "blit.h" // clear_game_screen
#include "timer.h"

/*

changes/spindizzy:

- put English/Japanese language switch back (?!)
- fixed 2P controls
- fixed EXTEND letters

- removed VIDEO_INFO struct and inserted SetScreenBitmap() :-P
- added sound (YM3812 doesn't work when using Raine functions though,
               only using inport/outport directly)
- well bugger me. The sound stops once you lose the game. Bummer.
- cleaned some bits up
- /me thinks it's time Antiriad had a lesson in whitespace/indentation...
- AAARGH!! Raine wants the MCU to be able to save it's state! (it doesn't)
           Moon on a friggin' stick, that's what you want...
- swapped buttons 1/2 for the inputs

*/

/*

changes/antiriad:

- remove SetScreenBitmap() and insert VIDEO_INFO sturct
- fixed it drawing over the edges of the screen bitmap (naughty)! :)
- removed procedure calls for common ram access (use raine modified mz80 for
  direct access)
- changed a few lines here and there (looking for the bug)...
- hmm, added 100 cpu slices per frame, and the game seems to run now =)
- improved the screen update (flipping, clipping, solid mask data, refresh
  palette when needed)
- er, stuck in some dsw and inputs, the inputs are far from complete, but
  the joystick will work too now anyway :).
- fixed ym3812 playing, you have to set multiple soundchips with saaddsound().
  don't ask me why, hiroshi coded it this way ;).
- added mz80 raine-customized bank switching, no more memcpy :).
- bankswitch is updated in loadgame callback.

27-August-99:

- Added Bobble Bobble and Super Bobble Bobble bootleg sets.
- Changed date from 1984 to 1986 (oops!).
- Removed up/down from inputs (unused, like rainbow islands).
- Cpu slices is now 16 per frame, or the music breaks on the bootlegs.
- I think it's safe to remove the FAKE_MCU stuff now ｦ_ｦ.

08-September-99:

- Added Bubble Bobble (romstar). That's all.
- The FAKE_MCU stuff is removed!

*/

/* This is a hack for the MCU. Increase it if the controls don't
 * seem responsive enough.
 */

#define BIH_COUNT 60

/******************************************************************************/

/*****************
   BUBBLE BOBBLE
 *****************/

static struct DIR_INFO bubble_bobble_dirs[] =
{
   { "bubble_bobble", },
   { "bublbobl", },
   { "bublbob1", },
   { NULL, },
};

static struct ROM_INFO bubble_bobble_roms[] =
{
   {   "a78-05.52", 0x00010000, 0x53f4bc6e, 0, 0, 0, },
   {   "a78-06.51", 0x00008000, 0x32c8305b, 0, 0, 0, },
   {   "a78-07.46", 0x00008000, 0x4f9a26e8, 0, 0, 0, },
   {   "a78-08.37", 0x00008000, 0xae11a07b, 0, 0, 0, },
   {   "a78-09.12", 0x00008000, 0x20358c22, 0, 0, 0, },
   {   "a78-10.13", 0x00008000, 0x930168a9, 0, 0, 0, },
   {   "a78-11.14", 0x00008000, 0x9773e512, 0, 0, 0, },
   {   "a78-12.15", 0x00008000, 0xd045549b, 0, 0, 0, },
   {   "a78-13.16", 0x00008000, 0xd0af35c5, 0, 0, 0, },
   {   "a78-14.17", 0x00008000, 0x7b5369a8, 0, 0, 0, },
   {   "a78-15.30", 0x00008000, 0x6b61a413, 0, 0, 0, },
   {   "a78-16.31", 0x00008000, 0xb5492d97, 0, 0, 0, },
   {   "a78-17.32", 0x00008000, 0xd69762d5, 0, 0, 0, },
   {   "a78-18.33", 0x00008000, 0x9f243b68, 0, 0, 0, },
   {   "a78-19.34", 0x00008000, 0x66e9438c, 0, 0, 0, },
   {   "a78-20.35", 0x00008000, 0x9ef863ad, 0, 0, 0, },
   {    "68705.bin", 0x00000800, 0x78caa635, 0, 0, 0, },
   {           NULL,          0,          0, 0, 0, 0, },
};

static struct INPUT_INFO bubble_bobble_inputs[] =
{
   { KB_DEF_COIN1,        MSG_COIN1,               0x010000, 0xFF, BIT_ACTIVE_1 },
   { KB_DEF_COIN2,        MSG_COIN2,               0x010001, 0xFF, BIT_ACTIVE_1 },
   { KB_DEF_TILT,         MSG_TILT,                0x010002, 0xFF, BIT_ACTIVE_1 },
   { KB_DEF_SERVICE,      MSG_SERVICE,             0x010003, 0xFF, BIT_ACTIVE_1 },

   { KB_DEF_P1_START,     MSG_P1_START,            0x010010, 0xFF, BIT_ACTIVE_1 },
   { KB_DEF_P1_LEFT,      MSG_P1_LEFT,             0x010013, 0xFF, BIT_ACTIVE_1 },
   { KB_DEF_P1_RIGHT,     MSG_P1_RIGHT,            0x010014, 0xFF, BIT_ACTIVE_1 },
   { KB_DEF_P1_B1,        MSG_P1_B1,               0x010016, 0xFF, BIT_ACTIVE_1 },
   { KB_DEF_P1_B2,        MSG_P1_B2,               0x010015, 0xFF, BIT_ACTIVE_1 },

   { KB_DEF_P2_START,     MSG_P2_START,            0x010020, 0xFF, BIT_ACTIVE_1 },
   { KB_DEF_P2_LEFT,      MSG_P2_LEFT,             0x010023, 0xFF, BIT_ACTIVE_1 },
   { KB_DEF_P2_RIGHT,     MSG_P2_RIGHT,            0x010024, 0xFF, BIT_ACTIVE_1 },
   { KB_DEF_P2_B1,        MSG_P2_B1,               0x010026, 0xFF, BIT_ACTIVE_1 },
   { KB_DEF_P2_B2,        MSG_P2_B2,               0x010025, 0xFF, BIT_ACTIVE_1 },

   { 0,                   NULL,                    0,        0,    0            },
};

struct DSW_DATA dsw_data_bubble_bobble_0[] =
{
   { "Language",              0x01, 0x02 },
   { "Japanese",              0x01},
   { "English",               0x00},
   { MSG_SCREEN,              0x02, 0x02 },
   { MSG_NORMAL,              0x02},
   { MSG_INVERT,              0x00},
   { MSG_TEST_MODE,           0x04, 0x02 },
   { MSG_OFF,                 0x04},
   { MSG_ON,                  0x00},
   { MSG_DEMO_SOUND,          0x08, 0x02 },
   { MSG_ON,                  0x08},
   { MSG_OFF,                 0x00},
   { MSG_COIN1,               0x30, 0x04 },
   { MSG_1COIN_1PLAY,         0x30},
   { MSG_1COIN_2PLAY,         0x20},
   { MSG_2COIN_1PLAY,         0x10},
   { MSG_2COIN_3PLAY,         0x00},
   { MSG_COIN2,               0xC0, 0x04 },
   { MSG_1COIN_1PLAY,         0xC0},
   { MSG_1COIN_2PLAY,         0x80},
   { MSG_2COIN_1PLAY,         0x40},
   { MSG_2COIN_3PLAY,         0x00},
   { NULL,                    0,   },
};

struct DSW_DATA dsw_data_bubble_bobble_1[] =
{
   { MSG_DIFFICULTY,          0x03, 0x04 },
   { MSG_NORMAL,              0x03},
   { MSG_EASY,                0x02},
   { MSG_HARD,                0x01},
   { MSG_HARDEST,             0x00},
   { MSG_EXTRA_LIFE,          0x0c, 0x04 },
   { "30k and 100k",          0x0C},
   { "20k and 80k",           0x08},
   { "40k and 200k",          0x04},
   { "50k and 250k",          0x00},
   { MSG_LIVES,               0x30, 0x04 },
   { "3",                     0x30},
   { "5",                     0x20},
   { "1",                     0x10},
   { "2",                     0x00},
   { MSG_DSWB_BIT7,           0x40, 0x02 },
   { MSG_OFF,                 0x40},
   { MSG_ON,                  0x00},
   { MSG_DSWB_BIT8,           0x80, 0x02 },
   { MSG_OFF,                 0x80},
   { MSG_ON,                  0x00},
   { NULL,                    0,   },
};

static struct DSW_INFO bubble_bobble_dsw[] =
{
   { 0x010030, 0xFE, dsw_data_bubble_bobble_0 },
   { 0x010040, 0xFF, dsw_data_bubble_bobble_1 },
   { 0,        0,    NULL,      },
};

static struct VIDEO_INFO bubble_bobble_video =
{
   draw_bubble_bobble,
   256,
   224,
   32,
   VIDEO_ROTATE_NORMAL |
   VIDEO_ROTATABLE,
};

static struct YM2203interface ym2203_interface =
{
     1,
     3000000,
     // { YM2203_VOL(255,255) },
     { 0x00ff20c0 },
   { 0 },
   { 0 },
   { 0 },
   { 0 },
   { z80_irq_handler },
};

static struct YM3812interface ym3526_interface =
{
   1,              // 1 chip
   3000000,        // 3.0 MHz
   { 255 },        // Volume (emu only)
   { NULL }
};

static struct SOUND_INFO bubble_bobble_sound[] =
{
   { SOUND_YM2203,  &ym2203_interface,    },
   { SOUND_YM3812,  &ym3526_interface,    },
   { 0,             NULL,                 },
};

GAME( bubble_bobble ,
   bubble_bobble_dirs,
   bubble_bobble_roms,
   bubble_bobble_inputs,
   bubble_bobble_dsw,
   NULL,

   load_bubble_bobble,
   clear_bubble_bobble,
   &bubble_bobble_video,
   execute_bubble_bobble_frame,
   "bublbobl",
   "Bubble Bobble",
   "バブルボブル",
   COMPANY_ID_TAITO,
   "A78",
   1986,
   bubble_bobble_sound,
   GAME_PLATFORM
);

/*************************************
   BUBBLE BOBBLE US with mode select
 *************************************/

static struct DIR_INFO bubble_bobble_romstar_dirs[] =
{
   { "bubble_bobble_romstar", },
   { "bublbobr", },
   { ROMOF("bublbobl"), },
   { CLONEOF("bublbobl"), },
   { NULL, },
};

static struct ROM_INFO bubble_bobble_romstar_roms[] =
{
   {       "a78-24.52", 0x00010000, 0xb7afedc4, 0, 0, 0, },
   {       "a78-25.51", 0x00008000, 0x2d901c9d, 0, 0, 0, },
   {   "a78-07.46", 0x00008000, 0x4f9a26e8, 0, 0, 0, },
   {   "a78-08.37", 0x00008000, 0xae11a07b, 0, 0, 0, },
   {   "a78-09.12", 0x00008000, 0x20358c22, 0, 0, 0, },
   {   "a78-10.13", 0x00008000, 0x930168a9, 0, 0, 0, },
   {   "a78-11.14", 0x00008000, 0x9773e512, 0, 0, 0, },
   {   "a78-12.15", 0x00008000, 0xd045549b, 0, 0, 0, },
   {   "a78-13.16", 0x00008000, 0xd0af35c5, 0, 0, 0, },
   {   "a78-14.17", 0x00008000, 0x7b5369a8, 0, 0, 0, },
   {   "a78-15.30", 0x00008000, 0x6b61a413, 0, 0, 0, },
   {   "a78-16.31", 0x00008000, 0xb5492d97, 0, 0, 0, },
   {   "a78-17.32", 0x00008000, 0xd69762d5, 0, 0, 0, },
   {   "a78-18.33", 0x00008000, 0x9f243b68, 0, 0, 0, },
   {   "a78-19.34", 0x00008000, 0x66e9438c, 0, 0, 0, },
   {   "a78-20.35", 0x00008000, 0x9ef863ad, 0, 0, 0, },
   {    "68705.bin", 0x00000800, 0x78caa635, 0, 0, 0, },
   {           NULL,          0,          0, 0, 0, 0, },
};

GAME( bubble_bobble_romstar ,
   bubble_bobble_romstar_dirs,
   bubble_bobble_romstar_roms,
   bubble_bobble_inputs,
   bubble_bobble_dsw,
   NULL,

   load_bubble_bobble,
   clear_bubble_bobble,
   &bubble_bobble_video,
   execute_bubble_bobble_frame,
   "bublbobr",
   "Bubble Bobble (US mode select)",
   "バブルボブル (US mode select)",
   COMPANY_ID_TAITO,
   "A78",
   1986,
   bubble_bobble_sound,
   GAME_PLATFORM
);

/********************
   BUBBLE BOBBLE US
 ********************/

static struct DIR_INFO bubble_bobble_romstar2_dirs[] =
{
   { "bubble_bobble_romstar2", },
   { "bubbobr1", },
   { ROMOF("bublbobl"), },
   { CLONEOF("bublbobl"), },
   { NULL, },
};

static struct ROM_INFO bubble_bobble_romstar2_roms[] =
{
   {    "a78-21.52", 0x00010000, 0x2844033d, 0, 0, 0, },
   {   "a78-06.51", 0x00008000, 0x32c8305b, 0, 0, 0, },
   {   "a78-07.46", 0x00008000, 0x4f9a26e8, 0, 0, 0, },
   {   "a78-08.37", 0x00008000, 0xae11a07b, 0, 0, 0, },
   {   "a78-09.12", 0x00008000, 0x20358c22, 0, 0, 0, },
   {   "a78-10.13", 0x00008000, 0x930168a9, 0, 0, 0, },
   {   "a78-11.14", 0x00008000, 0x9773e512, 0, 0, 0, },
   {   "a78-12.15", 0x00008000, 0xd045549b, 0, 0, 0, },
   {   "a78-13.16", 0x00008000, 0xd0af35c5, 0, 0, 0, },
   {   "a78-14.17", 0x00008000, 0x7b5369a8, 0, 0, 0, },
   {   "a78-15.30", 0x00008000, 0x6b61a413, 0, 0, 0, },
   {   "a78-16.31", 0x00008000, 0xb5492d97, 0, 0, 0, },
   {   "a78-17.32", 0x00008000, 0xd69762d5, 0, 0, 0, },
   {   "a78-18.33", 0x00008000, 0x9f243b68, 0, 0, 0, },
   {   "a78-19.34", 0x00008000, 0x66e9438c, 0, 0, 0, },
   {   "a78-20.35", 0x00008000, 0x9ef863ad, 0, 0, 0, },
   {    "68705.bin", 0x00000800, 0x78caa635, 0, 0, 0, },
   {           NULL,          0,          0, 0, 0, 0, },
};

GAME( bubble_bobble_romstar2 ,
   bubble_bobble_romstar2_dirs,
   bubble_bobble_romstar2_roms,
   bubble_bobble_inputs,
   bubble_bobble_dsw,
   NULL,

   load_bubble_bobble,
   clear_bubble_bobble,
   &bubble_bobble_video,
   execute_bubble_bobble_frame,
   "bubbobr1",
   "Bubble Bobble (US)",
   "バブルボブル (US)",
   COMPANY_ID_TAITO,
   "A78",
   1986,
   bubble_bobble_sound,
   GAME_PLATFORM
);

/*****************
   BOBBLE BOBBLE
 *****************/

static struct DIR_INFO bobble_bobble_dirs[] =
{
   { "bobble_bobble", },
   { "boblbobl", },
   { ROMOF("bublbobl"), },
   { CLONEOF("bublbobl"), },
   { NULL, },
};

static struct ROM_INFO bobble_bobble_roms[] =
{
   {          "bb3", 0x00008000, 0x01f81936, 0, 0, 0, },
   {          "bb4", 0x00008000, 0xafda99d8, 0, 0, 0, },
   {          "bb5", 0x00008000, 0x13118eb1, 0, 0, 0, },
   {   "a78-07.46", 0x00008000, 0x4f9a26e8, 0, 0, 0, },
   {   "a78-08.37", 0x00008000, 0xae11a07b, 0, 0, 0, },
   {   "a78-09.12", 0x00008000, 0x20358c22, 0, 0, 0, },
   {   "a78-10.13", 0x00008000, 0x930168a9, 0, 0, 0, },
   {   "a78-11.14", 0x00008000, 0x9773e512, 0, 0, 0, },
   {   "a78-12.15", 0x00008000, 0xd045549b, 0, 0, 0, },
   {   "a78-13.16", 0x00008000, 0xd0af35c5, 0, 0, 0, },
   {   "a78-14.17", 0x00008000, 0x7b5369a8, 0, 0, 0, },
   {   "a78-15.30", 0x00008000, 0x6b61a413, 0, 0, 0, },
   {   "a78-16.31", 0x00008000, 0xb5492d97, 0, 0, 0, },
   {   "a78-17.32", 0x00008000, 0xd69762d5, 0, 0, 0, },
   {   "a78-18.33", 0x00008000, 0x9f243b68, 0, 0, 0, },
   {   "a78-19.34", 0x00008000, 0x66e9438c, 0, 0, 0, },
   {   "a78-20.35", 0x00008000, 0x9ef863ad, 0, 0, 0, },
   {    "68705.bin", 0x00000800, 0x78caa635, 0, 0, 0, },
   {           NULL,          0,          0, 0, 0, 0, },
};

static struct INPUT_INFO bobble_bobble_inputs[] =
{
   { KB_DEF_COIN1,        MSG_COIN1,               0x00FF02, 0x08, BIT_ACTIVE_0 },
   { KB_DEF_COIN2,        MSG_COIN2,               0x00FF02, 0x04, BIT_ACTIVE_0 },
   { KB_DEF_TILT,         MSG_TILT,                0x00FF03, 0x04, BIT_ACTIVE_0 },
   { KB_DEF_SERVICE,      MSG_SERVICE,             0x00FF03, 0x08, BIT_ACTIVE_0 },

   { KB_DEF_P1_START,     MSG_P1_START,            0x00FF02, 0x40, BIT_ACTIVE_0 },
   { KB_DEF_P1_LEFT,      MSG_P1_LEFT,             0x00FF02, 0x01, BIT_ACTIVE_0 },
   { KB_DEF_P1_RIGHT,     MSG_P1_RIGHT,            0x00FF02, 0x02, BIT_ACTIVE_0 },
   { KB_DEF_P1_B1,        MSG_P1_B1,               0x00FF02, 0x20, BIT_ACTIVE_0 },
   { KB_DEF_P1_B2,        MSG_P1_B2,               0x00FF02, 0x10, BIT_ACTIVE_0 },

   { KB_DEF_P2_START,     MSG_P2_START,            0x00FF03, 0x40, BIT_ACTIVE_0 },
   { KB_DEF_P2_LEFT,      MSG_P2_LEFT,             0x00FF03, 0x01, BIT_ACTIVE_0 },
   { KB_DEF_P2_RIGHT,     MSG_P2_RIGHT,            0x00FF03, 0x02, BIT_ACTIVE_0 },
   { KB_DEF_P2_B1,        MSG_P2_B1,               0x00FF03, 0x20, BIT_ACTIVE_0 },
   { KB_DEF_P2_B2,        MSG_P2_B2,               0x00FF03, 0x10, BIT_ACTIVE_0 },

   { 0,                   NULL,                    0,        0,    0            },
};

struct DSW_DATA dsw_data_bobble_bobble_1[] =
{
   { MSG_DIFFICULTY,          0x03, 0x04 },
   { MSG_NORMAL,              0x03},
   { MSG_EASY,                0x02},
   { MSG_HARD,                0x01},
   { MSG_HARDEST,             0x00},
   { MSG_EXTRA_LIFE,          0x0c, 0x04 },
   { "30k and 100k",          0x0C},
   { "20k and 80k",           0x08},
   { "40k and 200k",          0x04},
   { "50k and 250k",          0x00},
   { MSG_LIVES,               0x30, 0x04 },
   { "3",                     0x30},
   { "5",                     0x20},
   { "1",                     0x10},
   { "2",                     0x00},
   { "Monster Speed",         0xC0, 0x04 },
   { "Very High",             0xC0},
   { "High",                  0x80},
   { "Medium",                0x40},
   { MSG_NORMAL,              0x00},
   { NULL,                    0,   },
};

static struct DSW_INFO bobble_bobble_dsw[] =
{
   { 0x00FF00, 0xFE, dsw_data_bubble_bobble_0 },
   { 0x00FF01, 0xFF, dsw_data_bobble_bobble_1 },
   { 0,        0,    NULL,      },
};

GAME( bobble_bobble ,
   bobble_bobble_dirs,
   bobble_bobble_roms,
   bobble_bobble_inputs,
   bobble_bobble_dsw,
   NULL,

   load_bobble_bobble,
   clear_bubble_bobble,
   &bubble_bobble_video,
   execute_bubble_bobble_frame,
   "boblbobl",
   "Bobble Bobble",
   "ボブルボブル",
   COMPANY_ID_BOOTLEG,
   NULL,
   1986,
   bubble_bobble_sound,
   GAME_PLATFORM
);

/***********************
   SUPER BOBBLE BOBBLE
 ***********************/

static struct DIR_INFO super_bobble_bobble_dirs[] =
{
   { "super_bobble_bobble", },
   { "sboblbob", },
   { ROMOF("bublbobl"), },
   { CLONEOF("bublbobl"), },
   //{ ROMOF("boblbobl"), },	// mulitple romof/cloneof if we want full merging
   //{ CLONEOF("boblbobl"), },	// but that will probably break rom merge tools ;)
   { NULL, },
};

static struct ROM_INFO super_bobble_bobble_roms[] =
{
   {    "bbb-3.rom", 0x00008000, 0xf304152a, 0, 0, 0, },
   {    "bbb-4.rom", 0x00008000, 0x94c75591, 0, 0, 0, },
   {          "bb5", 0x00008000, 0x13118eb1, 0, 0, 0, },
   {   "a78-07.46", 0x00008000, 0x4f9a26e8, 0, 0, 0, },
   {   "a78-08.37", 0x00008000, 0xae11a07b, 0, 0, 0, },
   {   "a78-09.12", 0x00008000, 0x20358c22, 0, 0, 0, },
   {   "a78-10.13", 0x00008000, 0x930168a9, 0, 0, 0, },
   {   "a78-11.14", 0x00008000, 0x9773e512, 0, 0, 0, },
   {   "a78-12.15", 0x00008000, 0xd045549b, 0, 0, 0, },
   {   "a78-13.16", 0x00008000, 0xd0af35c5, 0, 0, 0, },
   {   "a78-14.17", 0x00008000, 0x7b5369a8, 0, 0, 0, },
   {   "a78-15.30", 0x00008000, 0x6b61a413, 0, 0, 0, },
   {   "a78-16.31", 0x00008000, 0xb5492d97, 0, 0, 0, },
   {   "a78-17.32", 0x00008000, 0xd69762d5, 0, 0, 0, },
   {   "a78-18.33", 0x00008000, 0x9f243b68, 0, 0, 0, },
   {   "a78-19.34", 0x00008000, 0x66e9438c, 0, 0, 0, },
   {   "a78-20.35", 0x00008000, 0x9ef863ad, 0, 0, 0, },
   {    "68705.bin", 0x00000800, 0x78caa635, 0, 0, 0, },
   {           NULL,          0,          0, 0, 0, 0, },
};

struct DSW_DATA dsw_data_super_bobble_bobble_0[] =
{
   { "Game",                  0x01, 0x02 },
   { "Super Bobble Bobble",   0x00},
   { "Bobble Bobble",         0x01},
   { MSG_SCREEN,              0x02, 0x02 },
   { MSG_NORMAL,              0x02},
   { MSG_INVERT,              0x00},
   { MSG_TEST_MODE,           0x04, 0x02 },
   { MSG_OFF,                 0x04},
   { MSG_ON,                  0x00},
   { MSG_DEMO_SOUND,          0x08, 0x02 },
   { MSG_ON,                  0x08},
   { MSG_OFF,                 0x00},
   { MSG_COIN1,               0x30, 0x04 },
   { MSG_1COIN_1PLAY,         0x30},
   { MSG_1COIN_2PLAY,         0x20},
   { MSG_2COIN_1PLAY,         0x10},
   { MSG_2COIN_3PLAY,         0x00},
   { MSG_COIN2,               0xC0, 0x04 },
   { MSG_1COIN_1PLAY,         0xC0},
   { MSG_1COIN_2PLAY,         0x80},
   { MSG_2COIN_1PLAY,         0x40},
   { MSG_2COIN_3PLAY,         0x00},
   { NULL,                    0,   },
};

struct DSW_DATA dsw_data_super_bobble_bobble_1[] =
{
   { MSG_DIFFICULTY,          0x03, 0x04 },
   { MSG_NORMAL,              0x03},
   { MSG_EASY,                0x02},
   { MSG_HARD,                0x01},
   { MSG_HARDEST,             0x00},
   { MSG_EXTRA_LIFE,          0x0c, 0x04 },
   { "30k and 100k",          0x0C},
   { "20k and 80k",           0x08},
   { "40k and 200k",          0x04},
   { "50k and 250k",          0x00},
   { MSG_LIVES,               0x30, 0x04 },
   { "3",                     0x30},
   { "100",                   0x20},
   { "1",                     0x10},
   { "2",                     0x00},
   { "Monster Speed",         0xC0, 0x04 },
   { "Very High",             0xC0},
   { "High",                  0x80},
   { "Medium",                0x40},
   { MSG_NORMAL,              0x00},
   { NULL,                    0,   },
};

static struct DSW_INFO super_bobble_bobble_dsw[] =
{
   { 0x00FF00, 0xFE, dsw_data_super_bobble_bobble_0 },
   { 0x00FF01, 0xFF, dsw_data_super_bobble_bobble_1 },
   { 0,        0,    NULL,      },
};

GAME( super_bobble_bobble ,
   super_bobble_bobble_dirs,
   super_bobble_bobble_roms,
   bobble_bobble_inputs,
   super_bobble_bobble_dsw,
   NULL,

   load_super_bobble_bobble,
   clear_bubble_bobble,
   &bubble_bobble_video,
   execute_bubble_bobble_frame,
   "sboblbob",
   "Super Bobble Bobble",
   "スーパーボブルボブル",
   COMPANY_ID_BOOTLEG,
   NULL,
   1986,
   bubble_bobble_sound,
   GAME_PLATFORM
);

/******************************************************************************/

static UINT8 *RAM_INPUT;

static UINT8 *GFX_BG0;
static UINT8 *GFX_BG0_SOLID;

static int mcu_addr;
static UINT8 mcu_latch, mcu_port_a, mcu_old_data;
static UINT8 *RAM_MCU, *ROM_2ND, *ROM_SND, *RAM_SND;
static int nmi_enable, nmi_trigger;
static UINT8 sound_cmd;

void BubbleBobble_mcu(int bih_count);
void BubbleBobble_mcu_reset(void);

UINT8 BubbleBobble_MCU_RDMEM(int a)
{
   if (a == 0x02) /* COIN + TILT */
   {
      return 0xff - (RAM_INPUT[0x00] ? 4 : 0);
   }
   return RAM_MCU[a];
}

void BubbleBobble_MCU_WRMEM(int a, UINT8 v)
{
   UINT8 rising,falling;

   RAM_MCU[a] = v;
   if (a == 0x00) mcu_port_a = v;
   else if (a == 0x01) /* I/O control */
   {
      rising = (v ^ mcu_old_data) & v; /* only the rising-edge triggers these */
      falling = (v ^ mcu_old_data) & mcu_old_data; /* only the falling-edge triggers these */
      mcu_old_data = v;

      if (falling & 0x01) RAM_MCU[0x00] = mcu_latch;
      if (rising & 0x02) mcu_addr = (mcu_addr & 0xff00) | mcu_port_a;
      if (rising & 0x04) mcu_addr = (mcu_addr & 0x00ff) | ((mcu_port_a & 0x0f) << 8);
      if (falling & 0x10)
      {
         if (v & 0x08) /* read */
         {
            if ((mcu_addr & 0x0f00) == 0x0000)
            {
               switch(mcu_addr & 3)
               {
               case 0:        /* DSW0 */
                  mcu_latch = get_dsw(0); break;
               case 1:        /* DSW1 */
                  mcu_latch = get_dsw(1); break;
               case 2:        /* IN1 */
                  mcu_latch = 0xff - (RAM_INPUT[0x10] ? 0x40 : 0)
                                   - (RAM_INPUT[0x13] ? 0x01 : 0)
                                   - (RAM_INPUT[0x14] ? 0x02 : 0)
                                   - (RAM_INPUT[0x15] ? 0x10 : 0)
                                   - (RAM_INPUT[0x16] ? 0x20 : 0);
                  break;
               case 3:        /* IN2 */
                  mcu_latch = 0xff - (RAM_INPUT[0x20] ? 0x40 : 0)
                                   - (RAM_INPUT[0x23] ? 0x01 : 0)
                                   - (RAM_INPUT[0x24] ? 0x02 : 0)
                                   - (RAM_INPUT[0x25] ? 0x10 : 0)
                                   - (RAM_INPUT[0x26] ? 0x20 : 0);
                  break;
               }
            }
            else if ((mcu_addr & 0x0f00) == 0x0c00)
            {
               mcu_latch = RAM[0xfc00 + (mcu_addr & 0xff)];
            } else {
               /* reading from UNKNOWN? */
                  print_debug("Read from Unknown 0x%x\n", mcu_addr);
            }
         } else {    /* write */
            if ((mcu_addr & 0x0f00) == 0x0c00)
            {
               RAM[0xfc00 + (mcu_addr & 0xff)] = RAM_MCU[0x00];
            } else {
               /* writing to UNKNOWN? */
                  print_debug("Write to Unknown 0x%x=0x%x\n", mcu_addr, RAM_MCU[0x00]);
            }
         }
      }
      //if (falling & 0x20) set_IRQ_line((falling >> 5) & 1);
   }
}

/******************************************************************************/
/* Main Z80 read/write functions */
#if 0
/* A piece of history : how to update a real 256 colors palette in real time */
static void BublBobl_PalRAMWrite(UINT16 addr, UINT8 value)
{
   int n;
   UINT8 *p;

   if (RAM[addr] != value) {
     n = addr & 0x1fe;
     p = &RAM[0xf800 + n];
     n = (n >> 1) ^ 0x0f;

     RAM[addr] = value;
     pal[n].r = ((*p) & 0xf0) >> 2;
     pal[n].g = ((*p) & 0x0f) << 2;
     pal[n].b = ((*(p+1)) & 0xf0) >> 2;
   }
}
#endif

static void BublBobl_SoundCmd(UINT16 offset, UINT8 data)
{
	(void)(offset);
   sound_cmd = data;
   nmi_trigger = 1;
}

static UINT8 BublBobl_SoundCmd_read(UINT16 offset)
{
   int ta;
	(void)(offset);

   ta = sound_cmd;
   sound_cmd = 0;
   nmi_trigger = 0;

   return ta;
}

/******************************************************************************/
/* Bubble Bobble Z80 A Bankswitching                                          */
/******************************************************************************/

static UINT32 bank_sw;

static void init_bank_rom(UINT8 *src, UINT8 *dst)
{
   UINT32 ta;

   setup_z80_frame(CPU_Z80_2,CPU_FRAME_MHz(8,60));

   for(ta=0;ta<4;ta++){
      ROM_BANK[ta] = dst+(ta*0xC000);
      memcpy(ROM_BANK[ta]+0x0000,src+0x0000,0x8000);
      memcpy(ROM_BANK[ta]+0x8000,src+0x8000+(ta*0x4000),0x4000);
   }

   bank_sw = 0;
}

static void BublBobl_BankSwitch(UINT16 addr, UINT8 value)
{
	(void)(addr);
   if (bank_sw != (UINT32)(value & 3))
   {
      bank_sw = value & 3;
      Z80ASetBank( ROM_BANK[bank_sw] );
   }
}

/******************************************************************************/
/* Bubble Bobble YM2203                                                       */
/******************************************************************************/

static UINT8 ym2203_reg, ym3526_reg;

// YM2203

static UINT8 BB_YM2203Read(UINT16 offset)
{
   /* Timer A controls SFX
    * Timer B controls MUSIC
    */
   if(!(offset&1)){
     return YM2203_status_port_0_r(offset); // Set timers ON
   }
   else{
      return YM2203_read_port_0_r(0);
   }
}

static void BB_YM2203Write(UINT16 offset, UINT8 data)
{
   if(!(offset&1)){
     YM2203_control_port_0_w(offset,data);
     //ym2203_reg = data;
   }
   else{
     //YM2203_control_port_0_w(offset,ym2203_reg);
      YM2203_write_port_0_w(offset,data);
   }
}

// YM3526

static UINT8 BB_YM3526Read(UINT16 offset)
{
	(void)(offset);
  //int status =  YM3526_status_port_0_r(offset);
  //return status;
  return 0x80;  //It ignores the timers anyway? Just set the IRQ bit.
}

static void BB_YM3526Write(UINT16 offset, UINT8 data)
{
   if(!(offset&1)){
     YM3526_control_port_0_w(offset,data);//      ym3526_reg = data;
   }
   else{
     //YM3812_control_port_0_w(offset,ym3526_reg);
      YM3812_write_port_0_w(offset,data);
   }
}

static UINT8 BB_Unknown(UINT16 offset)
{
	(void)(offset);
   return 0x00;   /* b001 needs bit 0 to be clear */
}

static void SelectNMI(UINT16 offset, UINT8 data)
{
	(void)(data);
   switch(offset & 3)
   {
   case 0: break;
   case 1: nmi_enable = 1; break;
   case 2: nmi_enable = 0; break;
   }
}

/******************************************************************************/
/* Bubble Bobble Load/Save Extra Stuff                                        */
/******************************************************************************/

static void BublBoblLoadUpdate(void)
{
   UINT32 ta;

   // force a bankswitch refresh

   ta = bank_sw;
   bank_sw = 0xFF;
   BublBobl_BankSwitch(0,ta);
}

static void BublBoblAddSaveData(void)
{
   AddLoadCallback(BublBoblLoadUpdate);

   AddSaveData(SAVE_USER_0, (UINT8 *)&bank_sw,        sizeof(bank_sw));
   AddSaveData(SAVE_USER_1, (UINT8 *)&mcu_addr,       sizeof(mcu_addr));
   AddSaveData(SAVE_USER_2, (UINT8 *)&mcu_latch,      sizeof(mcu_latch));
   AddSaveData(SAVE_USER_3, (UINT8 *)&mcu_port_a,     sizeof(mcu_port_a));
   AddSaveData(SAVE_USER_4, (UINT8 *)&mcu_old_data,   sizeof(mcu_old_data));
   AddSaveData(SAVE_USER_5, (UINT8 *)&ym2203_reg,     sizeof(ym2203_reg));
   AddSaveData(SAVE_USER_6, (UINT8 *)&ym3526_reg,     sizeof(ym3526_reg));
   AddSaveData(SAVE_USER_7, (UINT8 *)&sound_cmd,      sizeof(sound_cmd));
   AddSaveData(SAVE_USER_8, (UINT8 *)&nmi_enable,     sizeof(nmi_enable));
   AddSaveData(SAVE_USER_9, (UINT8 *)&nmi_trigger,    sizeof(nmi_trigger));
   AddSaveData(SAVE_USER_10, RAM_MCU,                 0x80);
}

/******************************************************************************/

static void DrawNibble0(UINT8 *out, UINT8 c)
{
   int count, t;

   count = 4;
   do {
      t = c & 1;
      *out = t;
      out++;
      c >>= 1;
   } while(--count);
}

static void DrawNibble(UINT8 *out, int plane, UINT8 c)
{
   int count, t;

   count = 4;
   do {
      t = c & 1;
      *out |= t << plane;
      out++;
      c >>= 1;
   } while(--count);
}

static UINT8 HACK[3][2];

void load_bubble_bobble(void)
{
   int ta, tb;
   UINT8 *TMP;

/*ﾄﾄﾄﾄ Set up ROM/RAM ﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄ*/

   RAMSize=0x10000 + 0x100 + 0x1000;

   if(!(ROM =AllocateMem(0xC000*4))) return;

   if(!(ROM_2ND =AllocateMem(0x10000))) return;

   if(!(ROM_SND =AllocateMem(0x10000))) return;

   if(!(RAM_MCU =AllocateMem(0x800))) return;

   if(!(RAM=AllocateMem(RAMSize))) return;

   RAM_INPUT = RAM + 0x10000;
   RAM_SND   = RAM + 0x10100;

   if(!(TMP=AllocateMem(0x18000))) return;

   if(!load_rom_index(1,TMP+0x00000, 0x08000)) return;  // Z80 base rom
   if(!load_rom_index(0,TMP+0x08000, 0x10000)) return;  // Z80 bank rom

   // Skip Idle Z80

   HACK[0][0] = TMP[0x01ED];
   HACK[0][1] = TMP[0x01EE];
   TMP[0x01ED]=0xD3;  // OUTA (AAh)
   TMP[0x01EE]=0xAA;  //

   SetStopZ80Mode2(0x01ED);

   init_bank_rom(TMP,ROM);

   nmi_enable = 1;
   nmi_trigger = 0;

   set_colour_mapper(&col_map_rrrr_gggg_bbbb_xxxx_68k); // col_map_12bit_RRRRGGGGBBBBxxxx_Rev );
   InitPaletteMap(RAM+0xf800, 0x10, 0x10, 0x1000);

   /* Copy ROM code into our virtual image (copy 1st bank as well) */
   memcpy(RAM+0x0000, ROM, 0xC000);
   memset(RAM+0xC000, 0x00, 0x4000);

   AddZ80AROMBase(RAM, 0x38, 0x66);

   AddZ80AReadByte(0x01ed, 0x01ee, NULL, HACK[0]); // hide speed hack
   AddZ80AReadByte(0x0000, 0xBFFF, NULL,                    NULL);         // Z80 ROM (bank switched)
   AddZ80AReadByte(0xC000, 0xFFFF, NULL,                    RAM+0xC000);   // Z80 RAM
   AddZ80AReadByte(-1, -1, NULL, NULL);

   AddZ80AWriteByte(0xC000, 0xF7FF, NULL,                   RAM+0xC000);   // Z80 RAM
   AddZ80AWriteByte(0xFC00, 0xFFFF, NULL,                   RAM+0xFC00);   // Z80/MCU RAM
   AddZ80AWriteByte(0xF800, 0xF9FF, NULL /* BublBobl_PalRAMWrite */,   RAM+0xf800);         // Color RAM
   AddZ80AWriteByte(0xFB40, 0xFB40, BublBobl_BankSwitch,    NULL);         // Bank switch
   AddZ80AWriteByte(0xFA00, 0xFA00, BublBobl_SoundCmd,      NULL);         // Sound command
   AddZ80AWriteByte(0xFA40, 0xFA40, NULL,                   RAM+0xFA40);   // ???
   AddZ80AWriteByte(0xFA80, 0xFA80, NULL,                   RAM+0xFA80);   // Watchdog
   AddZ80AWriteByte(0x0000, 0xFFFF, DefBadWriteZ80,         NULL);         // <bad writes>
   AddZ80AWriteByte(-1, -1, NULL, NULL);

   AddZ80AReadPort(0x00, 0xFF, DefBadReadZ80,		NULL);
   AddZ80AReadPort(  -1,   -1, NULL,			NULL);

   AddZ80AWritePort(0xAA, 0xAA, StopZ80Mode2,		NULL);
   AddZ80AWritePort(0x00, 0xFF, DefBadWriteZ80,		NULL);
   AddZ80AWritePort(  -1,   -1, NULL,			NULL);

   AddZ80AInit();

   FreeMem(TMP);

   if(!load_rom("a78-08.37",ROM_2ND,    0x8000)) {return;}  // 2nd Z80 code

   // Fix Checksum

   ROM_2ND[0x018C]=0xC9;

   // Skip Idle Z80

   HACK[1][0] = ROM_2ND[0x000A];
   HACK[1][1] = ROM_2ND[0x000B];
   ROM_2ND[0x000A]=0xD3;  // OUTA (AAh)
   ROM_2ND[0x000B]=0xAA;  //

   SetStopZ80BMode2(0x000A);

   /* 2nd Z80 */
   AddZ80BROMBase(ROM_2ND, 0x38, 0x66);

   AddZ80BReadByte(0x000a, 0x000b, NULL, HACK[1]); // hide speed hack
   AddZ80BReadByte(0x0000, 0x7FFF, NULL,                   ROM_2ND+0x0000);  // ROM
   AddZ80BReadByte(0xE000, 0xF7FF, NULL,                   RAM+0xE000);  // COMMON RAM
   AddZ80BReadByte(0x8000, 0xFFFF, DefBadReadZ80,          NULL);  // <bad reads>
   AddZ80BReadByte(-1, -1, NULL, NULL);

   AddZ80BWriteByte(0xE000, 0xF7FF, NULL,                   RAM+0xE000);  // COMMON RAM
   AddZ80BWriteByte(0x0000, 0xFFFF, DefBadWriteZ80,         NULL);   // <bad writes>
   AddZ80BWriteByte(-1, -1, NULL, NULL);

   AddZ80BReadPort(0x00, 0xFF, DefBadReadZ80,			NULL);		// <bad reads>
   AddZ80BReadPort(-1, -1, NULL, NULL);

   AddZ80BWritePort(0xAA, 0xAA, StopZ80BMode2,			NULL);		// Trap Idle Z80
   AddZ80BWritePort(0x00, 0xFF, DefBadWriteZ80,			NULL);		// <bad reads>
   AddZ80BWritePort(-1, -1, NULL, NULL);

   AddZ80BInit();

   /* Hell, lets have a 3rd Z80, why not... */

   if(!load_rom("a78-07.46",ROM_SND,    0x8000)) {return;}  // Sound Z80 code

   // Skip Idle Z80
   // -------------

   HACK[2][0] = ROM_SND[0x0178];
   HACK[2][1] = ROM_SND[0x0179];
   ROM_SND[0x0178]=0xD3;  // OUT (AAh), A
   ROM_SND[0x0179]=0xAA;  //

   SetStopZ80CMode2(0x016D);

   AddZ80CROMBase(ROM_SND, 0x38, 0x66);

   AddZ80CReadByte(0x0178, 0x0179, NULL, HACK[2]); // hide speed hack
   AddZ80CReadByte(0x0000, 0x7FFF, NULL,              ROM_SND);      // ROM
   AddZ80CReadByte(0x8000, 0x8FFF, NULL,              RAM_SND);      // SOUND RAM
   AddZ80CReadByte(0x9000, 0x9001, BB_YM2203Read,     NULL);         // YM2203
   AddZ80CReadByte(0xA000, 0xA000, BB_YM3526Read,     NULL);         // YM3526
   //AddZ80CReadByte(0xB000, 0xB000, NULL,              &sound_cmd);   // SOUND COMMAND
   AddZ80CReadByte(0xB000, 0xB000, BublBobl_SoundCmd_read, NULL);   // SOUND COMMAND
   AddZ80CReadByte(0xB001, 0xB001, BB_Unknown,        NULL);         // ???
   AddZ80CReadByte(0xE000, 0xE000, BB_Unknown,        NULL);         // Diagnostic ROM (not used)
   AddZ80CReadByte(0x0000, 0xFFFF, DefBadReadZ80,     NULL);         // <bad writes>
   AddZ80CReadByte(-1, -1, NULL, NULL);

   AddZ80CWriteByte(0x8000, 0x8FFF, NULL,             RAM_SND);      // SOUND RAM
   AddZ80CWriteByte(0x9000, 0x9001, BB_YM2203Write,   NULL);         // YM2203
   AddZ80CWriteByte(0xA000, 0xA001, BB_YM3526Write,   NULL);         // YM3526
   AddZ80CWriteByte(0xB000, 0xB002, SelectNMI,        NULL);         // NMI enable
   AddZ80CWriteByte(0x0000, 0xFFFF, DefBadWriteZ80,   NULL);         // <bad writes>
   AddZ80CWriteByte(-1, -1, NULL, NULL);

   AddZ80CReadPort(0x00, 0xFF, DefBadReadZ80,         NULL);         // <bad reads>
   AddZ80CReadPort(-1, -1, NULL, NULL);

   AddZ80CWritePort(0xAA, 0xAA, StopZ80CMode2,        NULL);         // Trap Idle Z80
   AddZ80CWritePort(0x00, 0xFF, DefBadWriteZ80,       NULL);         // <bad reads>
   AddZ80CWritePort(-1, -1, NULL, NULL);

   AddZ80CInit();

   // MCU

   if(!load_rom("68705.bin", RAM_MCU,    0x800)) return;  // MCU code

   mcu_addr = 0;
   BubbleBobble_mcu_reset();

/*ﾄﾄﾄﾄ Set up graphics ﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄ*/
   if(!(GFX=AllocateMem(0x100000))) return;
   if(!(TMP=AllocateMem(0x080000))) return;

   GFX_BG0 = GFX+0x000000;

   /*
    * The hardware has space for 8 pairs of 32K GFX roms.
    * Bubble Bobble only uses the first 6.
    */

   if(!load_rom("a78-15.30", TMP+0x00000, 0x8000)) return;	// 8x8 BG0/OBJ
   if(!load_rom("a78-16.31", TMP+0x08000, 0x8000)) return;
   if(!load_rom("a78-17.32", TMP+0x10000, 0x8000)) return;
   if(!load_rom("a78-18.33", TMP+0x18000, 0x8000)) return;
   if(!load_rom("a78-19.34", TMP+0x20000, 0x8000)) return;
   if(!load_rom("a78-20.35", TMP+0x28000, 0x8000)) return;
   memset(TMP+0x30000,0x00,0x10000);				// 2 ROMs empty here

   if(!load_rom("a78-09.12", TMP+0x40000, 0x8000)) return;	// 8x8 BG0/OBJ
   if(!load_rom("a78-10.13", TMP+0x48000, 0x8000)) return;
   if(!load_rom("a78-11.14", TMP+0x50000, 0x8000)) return;
   if(!load_rom("a78-12.15", TMP+0x58000, 0x8000)) return;
   if(!load_rom("a78-13.16", TMP+0x60000, 0x8000)) return;
   if(!load_rom("a78-14.17", TMP+0x68000, 0x8000)) return;
   memset(TMP+0x70000,0x00,0x10000);				// 2 ROMs empty here

   for(ta=0,tb=0;ta<0x40000;ta+=2,tb+=8){
      DrawNibble0(&GFX[tb+0], TMP[ta+0]&15);
      DrawNibble0(&GFX[tb+4], TMP[ta+1]&15);
      DrawNibble(&GFX[tb+0], 1, TMP[ta+0]>>4);
      DrawNibble(&GFX[tb+4], 1, TMP[ta+1]>>4);
      DrawNibble(&GFX[tb+0], 2, TMP[ta+0x40000]&15);
      DrawNibble(&GFX[tb+4], 2, TMP[ta+0x40001]&15);
      DrawNibble(&GFX[tb+0], 3, TMP[ta+0x40000]>>4);
      DrawNibble(&GFX[tb+4], 3, TMP[ta+0x40001]>>4);
   }

   GFX_BG0_SOLID = make_solid_mask_8x8(GFX_BG0, 0x4000);

   BublBoblAddSaveData();

   FreeMem(TMP);
}

void load_bobble_bobble(void)
{
   int ta, tb;
   UINT8 *TMP;

/*ﾄﾄﾄﾄ Set up ROM/RAM ﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄ*/

   RAMSize=0x10000 + 0x100 + 0x1000;

   if(!(ROM =AllocateMem(0xC000*4))) return;

   if(!(ROM_2ND =AllocateMem(0x10000))) return;

   if(!(ROM_SND =AllocateMem(0x10000))) return;

   if(!(RAM_MCU =AllocateMem(0x800))) return;

   if(!(RAM=AllocateMem(RAMSize))) return;

   RAM_INPUT = RAM + 0x10000;
   RAM_SND   = RAM + 0x10100;

   if(!(TMP=AllocateMem(0x18000))) return;

   if(!load_rom("bb3",TMP+0x00000, 0x08000)) return;  // Z80 base rom
   if(!load_rom("bb5",TMP+0x08000, 0x08000)) return;  // Z80 bank rom
   if(!load_rom("bb4",TMP+0x10000, 0x08000)) return;  // Z80 bank rom

   // Skip Idle Z80

   HACK[0][0] = TMP[0x01ED];
   HACK[0][1] = TMP[0x01EE];
   TMP[0x01ED]=0xD3;  // OUTA (AAh)
   TMP[0x01EE]=0xAA;  //

   SetStopZ80Mode2(0x01ED);

   // What are these ? Apparently banks modifications ???
   // Don't know if they are counted in rom checks...
   TMP[0x9a71 + 0xC000]=0x00;
   TMP[0x9a72 + 0xC000]=0x00;
   TMP[0x9a73 + 0xC000]=0x00;

   TMP[0xa4af + 0xC000]=0x00;
   TMP[0xa4b0 + 0xC000]=0x00;
   TMP[0xa4b1 + 0xC000]=0x00;

   TMP[0xa55d + 0xC000]=0x00;
   TMP[0xa55e + 0xC000]=0x00;
   TMP[0xa55f + 0xC000]=0x00;

   TMP[0xb561 + 0xC000]=0x00;
   TMP[0xb562 + 0xC000]=0x00;
   TMP[0xb563 + 0xC000]=0x00;

   init_bank_rom(TMP,ROM);

   nmi_enable = 1;
   nmi_trigger = 0;

   set_colour_mapper(&col_map_rrrr_gggg_bbbb_xxxx_68k); // col_map_12bit_RRRRGGGGBBBBxxxx_Rev );
   InitPaletteMap(RAM+0xf800, 0x10, 0x10, 0x1000);

   /* Copy ROM code into our virtual image (copy 1st bank as well) */
   memcpy(RAM+0x0000, ROM, 0xC000);
   memset(RAM+0xC000, 0x00, 0x4000);

   AddZ80AROMBase(RAM, 0x38, 0x66);

   AddZ80AReadByte(0x01ed, 0x01ee, NULL, HACK[0]); // hide speed hack
   AddZ80AReadByte(0x0000, 0xBFFF, NULL,                    NULL);         // Z80 ROM (bank switched)
   AddZ80AReadByte(0xC000, 0xFFFF, NULL,                    RAM+0xC000);   // Z80 RAM
   AddZ80AReadByte(-1, -1, NULL, NULL);

   AddZ80AWriteByte(0xC000, 0xF7FF, NULL,                   RAM+0xC000);   // Z80 RAM
   AddZ80AWriteByte(0xFC00, 0xFFFF, NULL,                   RAM+0xFC00);   // Z80/MCU RAM
   AddZ80AWriteByte(0xF800, 0xF9FF, NULL /* BublBobl_PalRAMWrite */, RAM+0xf800);         // Color RAM
   AddZ80AWriteByte(0xFB40, 0xFB40, BublBobl_BankSwitch,    NULL);         // Bank switch
   AddZ80AWriteByte(0xFA00, 0xFA00, BublBobl_SoundCmd,      NULL);         // Sound command
   AddZ80AWriteByte(0xFA40, 0xFA40, NULL,                   RAM+0xFA40);   // ???
   AddZ80AWriteByte(0xFA80, 0xFA80, NULL,                   RAM+0xFA80);   // Watchdog
   AddZ80AWriteByte(0x0000, 0xFFFF, DefBadWriteZ80,         NULL);         // <bad writes>
   AddZ80AWriteByte(-1, -1, NULL, NULL);

   AddZ80AReadPort(0x00, 0xFF, DefBadReadZ80,		NULL);
   AddZ80AReadPort(  -1,   -1, NULL,			NULL);

   AddZ80AWritePort(0xAA, 0xAA, StopZ80Mode2,		NULL);
   AddZ80AWritePort(0x00, 0xFF, DefBadWriteZ80,		NULL);
   AddZ80AWritePort(  -1,   -1, NULL,			NULL);

   AddZ80AInit();

   FreeMem(TMP);

   if(!load_rom("a78-08.37",ROM_2ND,    0x8000)) {return;}  // 2nd Z80 code

   // Fix Checksum

   ROM_2ND[0x018C]=0xC9;

   // Skip Idle Z80

   HACK[1][0] = ROM_2ND[0x000A];
   HACK[1][0] = ROM_2ND[0x000b];

   ROM_2ND[0x000A]=0xD3;  // OUTA (AAh)
   ROM_2ND[0x000B]=0xAA;  //

   SetStopZ80BMode2(0x000A);

   /* 2nd Z80 */
   AddZ80BROMBase(ROM_2ND, 0x38, 0x66);

   AddZ80BReadByte(0x000a, 0x000b, NULL, HACK[1]); // hide speed hack
   AddZ80BReadByte(0x0000, 0x7FFF, NULL,                   ROM_2ND+0x0000);  // ROM
   AddZ80BReadByte(0xE000, 0xF7FF, NULL,                   RAM+0xE000);  // COMMON RAM
   AddZ80BReadByte(0x8000, 0xFFFF, DefBadReadZ80,          NULL);  // <bad reads>
   AddZ80BReadByte(-1, -1, NULL, NULL);

   AddZ80BWriteByte(0xE000, 0xF7FF, NULL,                   RAM+0xE000);  // COMMON RAM
   AddZ80BWriteByte(0x0000, 0xFFFF, DefBadWriteZ80,         NULL);   // <bad writes>
   AddZ80BWriteByte(-1, -1, NULL, NULL);

   AddZ80BReadPort(0x00, 0xFF, DefBadReadZ80,			NULL);		// <bad reads>
   AddZ80BReadPort(-1, -1, NULL, NULL);

   AddZ80BWritePort(0xAA, 0xAA, StopZ80BMode2,			NULL);		// Trap Idle Z80
   AddZ80BWritePort(0x00, 0xFF, DefBadWriteZ80,			NULL);		// <bad reads>
   AddZ80BWritePort(-1, -1, NULL, NULL);

   AddZ80BInit();

   /* Hell, lets have a 3rd Z80, why not... */

   if(!load_rom("a78-07.46",ROM_SND,    0x8000)) {return;}  // Sound Z80 code

   // Skip Idle Z80
   // -------------

   HACK[2][0] = ROM_SND[0x0178];
   HACK[2][1] = ROM_SND[0x0179];

   ROM_SND[0x0178]=0xD3;  // OUT (AAh), A
   ROM_SND[0x0179]=0xAA;  //

   SetStopZ80CMode2(0x016D);

   AddZ80CROMBase(ROM_SND, 0x38, 0x66);

   AddZ80CReadByte(0x0178, 0x0179, NULL,              HACK[2]);      // ROM
   AddZ80CReadByte(0x0000, 0x7FFF, NULL,              ROM_SND);      // ROM
   AddZ80CReadByte(0x8000, 0x8FFF, NULL,              RAM_SND);      // SOUND RAM
   AddZ80CReadByte(0x9000, 0x9001, BB_YM2203Read,     NULL);         // YM2203
   AddZ80CReadByte(0xA000, 0xA000, BB_YM3526Read,     NULL);         // YM3526
   AddZ80CReadByte(0xB000, 0xB000, BublBobl_SoundCmd_read, NULL);    // SOUND COMMAND
   AddZ80CReadByte(0xB001, 0xB001, BB_Unknown,        NULL);         // ???
   AddZ80CReadByte(0xE000, 0xE000, BB_Unknown,        NULL);         // Diagnostic ROM (not used)
   AddZ80CReadByte(0x0000, 0xFFFF, DefBadReadZ80,     NULL);         // <bad writes>
   AddZ80CReadByte(-1, -1, NULL, NULL);

   AddZ80CWriteByte(0x8000, 0x8FFF, NULL,             RAM_SND);      // SOUND RAM
   AddZ80CWriteByte(0x9000, 0x9001, BB_YM2203Write,   NULL);         // YM2203
   AddZ80CWriteByte(0xA000, 0xA001, BB_YM3526Write,   NULL);         // YM3526
   AddZ80CWriteByte(0xB000, 0xB002, SelectNMI,        NULL);         // NMI enable
   AddZ80CWriteByte(0x0000, 0xFFFF, DefBadWriteZ80,   NULL);         // <bad writes>
   AddZ80CWriteByte(-1, -1, NULL, NULL);

   AddZ80CReadPort(0x00, 0xFF, DefBadReadZ80,         NULL);         // <bad reads>
   AddZ80CReadPort(-1, -1, NULL, NULL);

   AddZ80CWritePort(0xAA, 0xAA, StopZ80CMode2,        NULL);         // Trap Idle Z80
   AddZ80CWritePort(0x00, 0xFF, DefBadWriteZ80,       NULL);         // <bad reads>
   AddZ80CWritePort(-1, -1, NULL, NULL);

   AddZ80CInit();

   // MCU

   if(!load_rom("68705.bin", RAM_MCU,    0x800)) return;  // MCU code

   mcu_addr = 0;
   BubbleBobble_mcu_reset();

/*ﾄﾄﾄﾄ Set up graphics ﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄ*/
   if(!(GFX=AllocateMem(0x100000))) return;
   if(!(TMP=AllocateMem(0x080000))) return;

   GFX_BG0 = GFX+0x000000;

   /*
    * The hardware has space for 8 pairs of 32K GFX roms.
    * Bubble Bobble only uses the first 6.
    */

   if(!load_rom("a78-15.30", TMP+0x00000, 0x8000)) return;	// 8x8 BG0/OBJ
   if(!load_rom("a78-16.31", TMP+0x08000, 0x8000)) return;
   if(!load_rom("a78-17.32", TMP+0x10000, 0x8000)) return;
   if(!load_rom("a78-18.33", TMP+0x18000, 0x8000)) return;
   if(!load_rom("a78-19.34", TMP+0x20000, 0x8000)) return;
   if(!load_rom("a78-20.35", TMP+0x28000, 0x8000)) return;
   memset(TMP+0x30000,0x00,0x10000);				// 2 ROMs empty here

   if(!load_rom("a78-09.12", TMP+0x40000, 0x8000)) return;	// 8x8 BG0/OBJ
   if(!load_rom("a78-10.13", TMP+0x48000, 0x8000)) return;
   if(!load_rom("a78-11.14", TMP+0x50000, 0x8000)) return;
   if(!load_rom("a78-12.15", TMP+0x58000, 0x8000)) return;
   if(!load_rom("a78-13.16", TMP+0x60000, 0x8000)) return;
   if(!load_rom("a78-14.17", TMP+0x68000, 0x8000)) return;
   memset(TMP+0x70000,0x00,0x10000);				// 2 ROMs empty here

   for(ta=0,tb=0;ta<0x40000;ta+=2,tb+=8){
      DrawNibble0(&GFX[tb+0], TMP[ta+0]&15);
      DrawNibble0(&GFX[tb+4], TMP[ta+1]&15);
      DrawNibble(&GFX[tb+0], 1, TMP[ta+0]>>4);
      DrawNibble(&GFX[tb+4], 1, TMP[ta+1]>>4);
      DrawNibble(&GFX[tb+0], 2, TMP[ta+0x40000]&15);
      DrawNibble(&GFX[tb+4], 2, TMP[ta+0x40001]&15);
      DrawNibble(&GFX[tb+0], 3, TMP[ta+0x40000]>>4);
      DrawNibble(&GFX[tb+4], 3, TMP[ta+0x40001]>>4);
   }

   GFX_BG0_SOLID = make_solid_mask_8x8(GFX_BG0, 0x4000);

   BublBoblAddSaveData();

   FreeMem(TMP);
}

void load_super_bobble_bobble(void)
{
   int ta, tb;
   UINT8 *TMP;

/*ﾄﾄﾄﾄ Set up ROM/RAM ﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄ*/

   RAMSize=0x10000 + 0x100 + 0x1000;

   if(!(ROM =AllocateMem(0xC000*4))) return;

   if(!(ROM_2ND =AllocateMem(0x10000))) return;

   if(!(ROM_SND =AllocateMem(0x10000))) return;

   if(!(RAM_MCU =AllocateMem(0x800))) return;

   if(!(RAM=AllocateMem(RAMSize))) return;

   RAM_INPUT = RAM + 0x10000;
   RAM_SND   = RAM + 0x10100;

   if(!(TMP=AllocateMem(0x18000))) return;

   if(!load_rom("bbb-3.rom",TMP+0x00000, 0x08000)) return;  // Z80 base rom
   if(!load_rom("bb5",      TMP+0x08000, 0x08000)) return;  // Z80 bank rom
   if(!load_rom("bbb-4.rom",TMP+0x10000, 0x08000)) return;  // Z80 bank rom

   // Skip Idle Z80

   HACK[0][0] = TMP[0x01ED];
   HACK[0][1] = TMP[0x01EE];
   TMP[0x01ED]=0xD3;  // OUTA (AAh)
   TMP[0x01EE]=0xAA;  //

   SetStopZ80Mode2(0x01ED);

   init_bank_rom(TMP,ROM);

   nmi_enable = 1;
   nmi_trigger = 0;

   /* Copy ROM code into our virtual image (copy 1st bank as well) */
   memcpy(RAM+0x0000, ROM, 0xC000);
   memset(RAM+0xC000, 0x00, 0x4000);

   set_colour_mapper(&col_map_rrrr_gggg_bbbb_xxxx_68k); // col_map_12bit_RRRRGGGGBBBBxxxx_Rev );
   InitPaletteMap(RAM+0xf800, 0x10, 0x10, 0x1000);

   AddZ80AROMBase(RAM, 0x38, 0x66);

   AddZ80AReadByte(0x01ed, 0x01ee, NULL, HACK[0]); // hide speed hack
   AddZ80AReadByte(0x0000, 0xBFFF, NULL,                    NULL);         // Z80 ROM (bank switched)
   AddZ80AReadByte(0xC000, 0xFFFF, NULL,                    RAM+0xC000);   // Z80 RAM
   AddZ80AReadByte(-1, -1, NULL, NULL);

   AddZ80AWriteByte(0xC000, 0xF7FF, NULL,                   RAM+0xC000);   // Z80 RAM
   AddZ80AWriteByte(0xFC00, 0xFFFF, NULL,                   RAM+0xFC00);   // Z80/MCU RAM
   AddZ80AWriteByte(0xF800, 0xF9FF, NULL, /* BublBobl_PalRAMWrite, */   RAM+0xf800);         // Color RAM
   AddZ80AWriteByte(0xFB40, 0xFB40, BublBobl_BankSwitch,    NULL);         // Bank switch
   AddZ80AWriteByte(0xFA00, 0xFA00, BublBobl_SoundCmd,      NULL);         // Sound command
   AddZ80AWriteByte(0xFA40, 0xFA40, NULL,                   RAM+0xFA40);   // ???
   AddZ80AWriteByte(0xFA80, 0xFA80, NULL,                   RAM+0xFA80);   // Watchdog
   AddZ80AWriteByte(0x0000, 0xFFFF, DefBadWriteZ80,         NULL);         // <bad writes>
   AddZ80AWriteByte(-1, -1, NULL, NULL);

   AddZ80AReadPort(0x00, 0xFF, DefBadReadZ80,		NULL);
   AddZ80AReadPort(  -1,   -1, NULL,			NULL);

   AddZ80AWritePort(0xAA, 0xAA, StopZ80Mode2,		NULL);
   AddZ80AWritePort(0x00, 0xFF, DefBadWriteZ80,		NULL);
   AddZ80AWritePort(  -1,   -1, NULL,			NULL);

   AddZ80AInit();

   FreeMem(TMP);

   if(!load_rom("a78-08.37",ROM_2ND,    0x8000)) {return;}  // 2nd Z80 code

   // Fix Checksum

   ROM_2ND[0x018C]=0xC9;

   // Skip Idle Z80

   HACK[1][0] = ROM_2ND[0x000A];
   HACK[1][1] = ROM_2ND[0x000B];
   ROM_2ND[0x000A]=0xD3;  // OUTA (AAh)
   ROM_2ND[0x000B]=0xAA;  //

   SetStopZ80BMode2(0x000A);

   /* 2nd Z80 */
   AddZ80BROMBase(ROM_2ND, 0x38, 0x66);

   AddZ80BReadByte(0x000a, 0x000b, NULL, HACK[1]); // hide speed hack
   AddZ80BReadByte(0x0000, 0x7FFF, NULL,                   ROM_2ND+0x0000);  // ROM
   AddZ80BReadByte(0xE000, 0xF7FF, NULL,                   RAM+0xE000);  // COMMON RAM
   AddZ80BReadByte(0x8000, 0xFFFF, DefBadReadZ80,          NULL);  // <bad reads>
   AddZ80BReadByte(-1, -1, NULL, NULL);

   AddZ80BWriteByte(0xE000, 0xF7FF, NULL,                   RAM+0xE000);  // COMMON RAM
   AddZ80BWriteByte(0x0000, 0xFFFF, DefBadWriteZ80,         NULL);   // <bad writes>
   AddZ80BWriteByte(-1, -1, NULL, NULL);

   AddZ80BReadPort(0x00, 0xFF, DefBadReadZ80,			NULL);		// <bad reads>
   AddZ80BReadPort(-1, -1, NULL, NULL);

   AddZ80BWritePort(0xAA, 0xAA, StopZ80BMode2,			NULL);		// Trap Idle Z80
   AddZ80BWritePort(0x00, 0xFF, DefBadWriteZ80,			NULL);		// <bad reads>
   AddZ80BWritePort(-1, -1, NULL, NULL);

   AddZ80BInit();

   /* Hell, lets have a 3rd Z80, why not... */

   if(!load_rom("a78-07.46",ROM_SND,    0x8000)) {return;}  // Sound Z80 code

   // Skip Idle Z80
   // -------------

   HACK[2][0] = ROM_SND[0x0178];
   HACK[2][1] = ROM_SND[0x0179];
   ROM_SND[0x0178]=0xD3;  // OUT (AAh), A
   ROM_SND[0x0179]=0xAA;  //

   SetStopZ80CMode2(0x016D);

   AddZ80CROMBase(ROM_SND, 0x38, 0x66);

   AddZ80CReadByte(0x0178, 0x0179, NULL, HACK[2]); // hide speed hack
   AddZ80CReadByte(0x0000, 0x7FFF, NULL,              ROM_SND);      // ROM
   AddZ80CReadByte(0x8000, 0x8FFF, NULL,              RAM_SND);      // SOUND RAM
   AddZ80CReadByte(0x9000, 0x9001, BB_YM2203Read,     NULL);         // YM2203
   AddZ80CReadByte(0xA000, 0xA000, BB_YM3526Read,     NULL);         // YM3526
   //AddZ80CReadByte(0xB000, 0xB000, NULL,              &sound_cmd);   // SOUND COMMAND
   AddZ80CReadByte(0xB000, 0xB000, BublBobl_SoundCmd_read, NULL);   // SOUND COMMAND
   AddZ80CReadByte(0xB001, 0xB001, BB_Unknown,        NULL);         // ???
   AddZ80CReadByte(0xE000, 0xE000, BB_Unknown,        NULL);         // Diagnostic ROM (not used)
   AddZ80CReadByte(0x0000, 0xFFFF, DefBadReadZ80,     NULL);         // <bad writes>
   AddZ80CReadByte(-1, -1, NULL, NULL);

   AddZ80CWriteByte(0x8000, 0x8FFF, NULL,             RAM_SND);      // SOUND RAM
   AddZ80CWriteByte(0x9000, 0x9001, BB_YM2203Write,   NULL);         // YM2203
   AddZ80CWriteByte(0xA000, 0xA001, BB_YM3526Write,   NULL);         // YM3526
   AddZ80CWriteByte(0xB000, 0xB002, SelectNMI,        NULL);         // NMI enable
   AddZ80CWriteByte(0x0000, 0xFFFF, DefBadWriteZ80,   NULL);         // <bad writes>
   AddZ80CWriteByte(-1, -1, NULL, NULL);

   AddZ80CReadPort(0x00, 0xFF, DefBadReadZ80,         NULL);         // <bad reads>
   AddZ80CReadPort(-1, -1, NULL, NULL);

   AddZ80CWritePort(0xAA, 0xAA, StopZ80CMode2,        NULL);         // Trap Idle Z80
   AddZ80CWritePort(0x00, 0xFF, DefBadWriteZ80,       NULL);         // <bad reads>
   AddZ80CWritePort(-1, -1, NULL, NULL);

   AddZ80CInit();

   // MCU

   if(!load_rom("68705.bin", RAM_MCU,    0x800)) return;  // MCU code

   mcu_addr = 0;
   BubbleBobble_mcu_reset();

/*ﾄﾄﾄﾄ Set up graphics ﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄﾄ*/
   if(!(GFX=AllocateMem(0x100000))) return;
   if(!(TMP=AllocateMem(0x080000))) return;

   GFX_BG0 = GFX+0x000000;

   /*
    * The hardware has space for 8 pairs of 32K GFX roms.
    * Bubble Bobble only uses the first 6.
    */

   if(!load_rom("a78-15.30", TMP+0x00000, 0x8000)) return;	// 8x8 BG0/OBJ
   if(!load_rom("a78-16.31", TMP+0x08000, 0x8000)) return;
   if(!load_rom("a78-17.32", TMP+0x10000, 0x8000)) return;
   if(!load_rom("a78-18.33", TMP+0x18000, 0x8000)) return;
   if(!load_rom("a78-19.34", TMP+0x20000, 0x8000)) return;
   if(!load_rom("a78-20.35", TMP+0x28000, 0x8000)) return;
   memset(TMP+0x30000,0x00,0x10000);				// 2 ROMs empty here

   if(!load_rom("a78-09.12", TMP+0x40000, 0x8000)) return;	// 8x8 BG0/OBJ
   if(!load_rom("a78-10.13", TMP+0x48000, 0x8000)) return;
   if(!load_rom("a78-11.14", TMP+0x50000, 0x8000)) return;
   if(!load_rom("a78-12.15", TMP+0x58000, 0x8000)) return;
   if(!load_rom("a78-13.16", TMP+0x60000, 0x8000)) return;
   if(!load_rom("a78-14.17", TMP+0x68000, 0x8000)) return;
   memset(TMP+0x70000,0x00,0x10000);				// 2 ROMs empty here

   for(ta=0,tb=0;ta<0x40000;ta+=2,tb+=8){
      DrawNibble0(&GFX[tb+0], TMP[ta+0]&15);
      DrawNibble0(&GFX[tb+4], TMP[ta+1]&15);
      DrawNibble(&GFX[tb+0], 1, TMP[ta+0]>>4);
      DrawNibble(&GFX[tb+4], 1, TMP[ta+1]>>4);
      DrawNibble(&GFX[tb+0], 2, TMP[ta+0x40000]&15);
      DrawNibble(&GFX[tb+4], 2, TMP[ta+0x40001]&15);
      DrawNibble(&GFX[tb+0], 3, TMP[ta+0x40000]>>4);
      DrawNibble(&GFX[tb+4], 3, TMP[ta+0x40001]>>4);
   }

   GFX_BG0_SOLID = make_solid_mask_8x8(GFX_BG0, 0x4000);

   BublBoblAddSaveData();

   FreeMem(TMP);
}

void clear_bubble_bobble(void)
{
}

void execute_bubble_bobble_frame(void)
{
   int ta;

   BubbleBobble_mcu(BIH_COUNT);

   /*
    * This makes the EXTEND letters random. Neither the game nor the MCU
    * appears to be doing this (why not???), so we do...
    */
   RAM[0xfc7c] ++;
   RAM[0xfc7c] %= 6;

   /*
    * 4 slices is enough for the orignal, but the bootlegs need more (lousy
    * programming?)
    */
   #define CPU_SLICE 16

   for(ta=0;ta<CPU_SLICE;ta++)
   {
      if(cpu_get_pc(CPU_Z80_0) != 0x01ED)
      {
         cpu_execute_cycles(CPU_Z80_0, CPU_FRAME_MHz(8,60)/CPU_SLICE);  // Main Z80 8MHz (60fps)
      }

      if(cpu_get_pc(CPU_Z80_1) != 0x000A)
      {
	cpu_execute_cycles(CPU_Z80_1, CPU_FRAME_MHz(8,60)/CPU_SLICE);  // Sub Z80 8MHz (60fps)
      }

      if (nmi_trigger && nmi_enable)
      {
         cpu_int_nmi(CPU_Z80_2);
         //nmi_trigger = 0;
      }

      //      if(cpu_get_pc(CPU_Z80_2) != 0x016D)
      //{
         cpu_execute_cycles(CPU_Z80_2, CPU_FRAME_MHz(8,60)/CPU_SLICE);  // Sound Z80 8MHz (60fps)
	 //}

	 triger_timers();
   }
   print_debug("Z80PC_MAIN:%04x\n",cpu_get_pc(CPU_Z80_0));
   cpu_interrupt(CPU_Z80_0, RAM[0xfc00]);

   print_debug("Z80PC_SUB:%04x\n",cpu_get_pc(CPU_Z80_1));
   cpu_interrupt(CPU_Z80_1, 0xff);


   print_debug("Z80PC_SOUND:%04x\n",cpu_get_pc(CPU_Z80_2));

#if 0
   // Piece of history, I keep it here !
   /* The timers expire, on average, every 51.4 Hz.
    * This can be achieved by doing an interrupt 6 out of every 7 frames
    * from a 60Hz timer (51.4 = 60*6/7)
    *
    * NOTE: 51.4Hz is carefully calculated, and wasn't just obtained
    *       by twiddling it till it worked. Oh no. Honest, guv. ;-)
    */
   if (tq) cpu_interrupt(CPU_Z80_2, 0xff);
   tq++; tq %= 7;
#endif
}

void draw_bubble_bobble(void)
{
   int ta,x,y;
   UINT8 *map;

   int offs, height, goffs;
   int sx,sy,xc,yc;
   int gfx_num,gfx_attr,gfx_offs;

   // Palette needs updating (eg. loadgame)

   // No Solid BG0

   ClearPaletteMap();
   MAP_PALETTE_MAPPED_NEW(
			  0xe,
			  16,
			  map
			  );

   clear_game_screen(ReadLong(&map[0]));

   // OBJECT

   sx = 0;
   for (offs=0;offs<0x300;offs+=4)
   {
      /* skip empty sprites */
      if (*(UINT32 *)(&RAM[offs+0xdd00]) == 0) continue;

      gfx_num = RAM[0xdd00 + offs + 1];
      gfx_attr = RAM[0xdd00 + offs + 3];

      if ((gfx_num & 0x80) == 0)      /* 16x16 sprites */
      {
         gfx_offs = ((gfx_num & 0x1f) * 0x80) + ((gfx_num & 0x60) >> 1) + 12;
         height = 2;
      }
      else    /* tilemaps (each sprite is a 16x256 column) */
      {
         gfx_offs = ((gfx_num & 0x3f) * 0x80);
         height = 32;
      }

      if ((gfx_num & 0xc0) == 0xc0)
         sx += 16;
      else
      {
         sx = RAM[0xdd00 + offs + 2];
         if (gfx_attr & 0x40) sx -= 256;
      }
      sy = 256 - height*8 - (RAM[0xdd00 + offs + 0]);

      for (xc=0;xc<2;xc++)
      {
         goffs = gfx_offs;

         for (yc=0;yc<height;yc++)
         {
            ta = (ReadWord(&RAM[0xc000+goffs]) & 0x3FF) | ((gfx_attr << 10) & 0x3C00);

            if (GFX_BG0_SOLID[ta])        // No pixels; skip
            {
	      MAP_PALETTE_MAPPED_NEW(
			       ((RAM[0xc001 + goffs] >> 2) & 0xF),
			       16,
			       map
			       );

               x = 32 + (sx + xc * 8);
               y = 32 + (((sy + yc * 8) & 0xff) - 16);

               if((x>16) && (y>16) && (x<256+32) && (y<224+32))
               {
                  if(GFX_BG0_SOLID[ta]==1)
                  {        // Some pixels; trans

                  Draw8x8_Trans_Mapped_flip_Rot(&GFX_BG0[ta<<6], x, y, map, (RAM[0xc001+goffs]>>6) & 3);

                  } else {    // all pixels; solid

                  Draw8x8_Mapped_flip_Rot(&GFX_BG0[ta<<6], x, y, map, (RAM[0xc001+goffs]>>6) & 3);

                  }
               }
            }

            goffs += 2;
         }

         gfx_offs += 64;
      }
   }
}


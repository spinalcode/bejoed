#define PROJ_SCREENMODE TASMODE
#define PROJ_FPS 255
#define PROJ_HIGH_RAM HIGH_RAM_ON
//#define PROJ_LINE_FILLERS TAS::NOPFiller, TAS::NOPFiller, TAS::NOPFiller
#define PROJ_MAX_SPRITES 255
// sound stuff
#define PROJ_ENABLE_SOUND       1       // 0 = all sound functions disabled
#define PROJ_STREAMING_MUSIC    1       // 1 = enable streaming music from SD card
#define PROJ_AUD_FREQ           22050
#define PROJ_STREAM_TO_DAC      1       // 1 use DAC for stream, 0 = use PWM for stream
#define PROJ_GBSOUND            0       // 1 = use Gamebuino-compatible sound interrupt 
#define PROJ_ENABLE_SYNTH       0       // 1 = use Rboy-compatible sound interrupt
#define PROJ_USE_PWM            0       // 0 = Use only DAC for output

/*
* REEEEEEEE stolen codeeeeeeeeeee (not rly)
*/
#include <fluidsynth.h>

typedef struct {
    PyObject_HEAD
    fluid_settings_t* settings;
    fluid_synth_t* synth;
    fluid_player_t* player;
    fluid_audio_driver_t* adriver;
} songbirdControl;

/*
* ooga booga write all the functions down here ok
*/
static int *songbirdControl_init_player(PyObject *self, char** arg);
static int *songbirdControl_start_playing(PyObject *self, int tick);
static int *songbirdControl_stop_playing(PyObject *self);
static int *songbirdControl_adjust_volume(PyObject *self, int vol);
static int *songbirdControl_adjust_tempo(PyObject *self, int bpm);
static int songbirdControl_init(PyObject *self, PyObject *args, PyObject *kwds);
static void songbirdControl_dealloc(songbirdControl *self);
static PyObject *songbirdControl_repr(PyObject *self);
static PyObject *songbirdControl_new(PyTypeObject *type, PyObject *args, PyObject *kwds);
static PyObject *songbirdControl_new(PyTypeObject *type, PyObject *args, PyObject *kwds);


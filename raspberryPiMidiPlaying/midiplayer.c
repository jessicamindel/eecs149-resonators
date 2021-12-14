
/* usage: (program) (sf2 file) (midi file) (tick to start at)
    basically play it until you want to, it listens at every tick for a stop signal, and then it stops.
    if you want to resume, main computer simply sends the tick to restart at (which is useful, since stops can be delayed due to latency)

    in order to schedule interrupts, there has to be some external program also connected to this that can send in inputs during execution.
    Interrupt handler is already setup from backend.
*/

#include <fluidsynth.h>
#include <stdio.h>
#include <Python.h>
#include "structmember.h"
#include "midiplayer.h"

typedef struct {
    PyObject_HEAD
    fluid_settings_t* settings;
    fluid_synth_t* synth;
    fluid_player_t* player;
    fluid_audio_driver_t* adriver;
}songbirdControl;

static PyTypeObject midiplayerType;

static int songbirdControl_init(PyObject *self, PyObject *soundfont, PyObject *midifile) {
    //fill this with the structure init
    songbirdControl *new_songbird;
    new_songbird = malloc(sizeof(songbirdControl));

    (*new_songbird).settings = new_fluid_settings();
    (*new_songbird).synth = new_fluid_synth((*new_songbird).settings);
    (*new_songbird).player = new_fluid_player((*new_songbird).synth);
    fluid_settings_setstr((*new_songbird).settings, "audio.driver", "alsa");


    if(fluid_synth_sfload((*new_songbird).synth, "sftarget.sf2", 1) < 0){
        printf("Soundfont failed to load.\n");
        fflush(stdout);
        return -1;
    }
    if(fluid_player_add((*new_songbird).player, "miditarget.mid") < 0) {
        printf("MIDI file failed to load.\n");
        fflush(stdout);
        return -1;
    }

    (*new_songbird).adriver = new_fluid_audio_driver((*new_songbird).settings, (*new_songbird).synth);
    printf("Initialization of songbird object complete.\n");
    fflush(stdout);
    
    return 0;
}

static int *songbirdControl_start_playing(PyObject *self, PyObject *pyTick) {
    //starts it (uses fluid_player_seek, and then fluid_player_play)
    int tick = PyLong_AsLong(pyTick);
    printf("Now starting play at tick %i.\n", tick);
    fflush(stdout);
    fluid_player_seek(((songbirdControl *)self)->player, tick);
    fluid_player_play(((songbirdControl *)self)->player);
    return 0;
}

static int *songbirdControl_stop_playing(PyObject *self) {
    //stops it (uses fluid_player_stop, and associated cleanups)
    printf("Now attempting to stop play.\n");
    fflush(stdout);
    fluid_player_stop(((songbirdControl *)self)->player);
    delete_fluid_audio_driver(((songbirdControl *)self)->adriver);
    delete_fluid_player(((songbirdControl *)self)->player);
    delete_fluid_synth(((songbirdControl *)self)->synth);
    delete_fluid_settings(((songbirdControl *)self)->settings);
    return 0;
}

static int *songbirdControl_adjust_volume(PyObject *self, PyObject *pyVol) {
    //increase or decrease volume based on input (uses fluid_synth_set_gain)
    printf("Adjusting volume of songbird.\n");
    fflush(stdout);
    float vol = PyFloat_AsDouble(pyVol);
    fluid_synth_set_gain(((songbirdControl *)self)->synth, vol);
    return 0;
}

static int *songbirdControl_adjust_tempo(PyObject *self, PyObject *pyBPM) {
    //increase or decrease tempo based on input (uses fluidsynth_set_bpm)
    printf("Adjusting tempo of songbird.\n");
    fflush(stdout);
    int bpm = PyLong_AsLong(pyBPM);
    fluid_player_set_bpm(((songbirdControl *)self)->player, bpm);
    return 0;
}

static void songbirdControl_dealloc(songbirdControl *self) {
    printf("Deallocating songbird.\n");
    fflush(stdout);
    Py_TYPE(self)->tp_free(self);
}

static PyObject *songbirdControl_repr(PyObject *self) {
    //too lazy to implement actual repr
    return NULL;
}

static PyObject *songbirdControl_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    /* size of allocated memory is tp_basicsize + nitems*tp_itemsize*/
    songbirdControl *self = (songbirdControl *)type->tp_alloc(type, 0);
    return (PyObject *)self;
}

static PyMethodDef songbirdControl_methods[] = {
    /* TODO: YOUR CODE HERE */
    {"start_playing", (PyCFunction) songbirdControl_start_playing, METH_VARARGS, NULL},
    {"stop_playing", (PyCFunction) songbirdControl_stop_playing, METH_VARARGS, NULL},
    {"adjust_volume", (PyCFunction) songbirdControl_adjust_volume, METH_VARARGS, NULL},
    {"adjust_tempo", (PyCFunction) songbirdControl_adjust_tempo, METH_VARARGS, NULL},
    {NULL, NULL, 0, NULL}
};

static PyTypeObject midiplayerType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "midiplayer.songbirdControl",
    .tp_basicsize = sizeof(songbirdControl),
    .tp_itemsize = 0,
    .tp_dealloc = (destructor)songbirdControl_dealloc,
    .tp_repr = (reprfunc)songbirdControl_repr,
    .tp_flags = Py_TPFLAGS_DEFAULT |
        Py_TPFLAGS_BASETYPE,
    .tp_doc = "midiplayer.songbirdControl objects",
    .tp_methods = songbirdControl_methods,
    .tp_init = (initproc)songbirdControl_init,
    .tp_new = songbirdControl_new
};

static struct PyModuleDef midiplayermodule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "midiplayer",
    .m_doc = "Midiplayer operations",
    .m_size = -1,
};

/* Initialize the midiplayer module */
PyMODINIT_FUNC PyInit_midiplayer(void) {
    PyObject* m;
    if (PyType_Ready(&midiplayerType) < 0)
        return NULL;

    m = PyModule_Create(&midiplayermodule);
    if (m == NULL)
        return NULL;
    Py_INCREF(&midiplayerType);

    if (PyModule_AddObject(m, "songbirdControl", (PyObject *)&midiplayerType) < 0) {
        Py_DECREF(&midiplayerType);
        Py_DECREF(m);
        return NULL;
    }
    printf("Successful import of Midi Player!\n");
    fflush(stdout);
    return m;
}
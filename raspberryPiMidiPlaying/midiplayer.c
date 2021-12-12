
/* usage: (program) (sf2 file) (midi file) (tick to start at)
    basically play it until you want to, it listens at every tick for a stop signal, and then it stops.
    if you want to resume, main computer simply sends the tick to restart at (which is useful, since stops can be delayed due to latency)

    in order to schedule interrupts, there has to be some external program also connected to this that can send in inputs during execution.
    Interrupt handler is already setup from backend.
*/

#include <fluidsynth.h>
#include <Python.h>
#include "midiplayer.h"


static int *songbirdControl_init_player(PyObject *self, char** arg) {
    //does preloading to minimize latency on start.
    return 0
}

static int *songbirdControl_start_playing(PyObject *self, int tick) {
    //starts it (uses fluid_player_seek, and then fluid_player_play)
    return 0
}

static int *songbirdControl_stop_playing(PyObject *self) {
    //stops it (uses fluid_player_stop, and associated cleanups)
    return 0
}

static int *songbirdControl_adjust_volume(PyObject *self, int vol) {
    //increase or decrease volume based on input (uses fluid_synth_set_gain)
    return 0
}

static int *songbirdControl_adjust_tempo(PyObject *self, int bpm) {
    //increase or decrease tempo based on input (uses fluidsynth_set_bpm)
    return 0
}


int main(int argc, char** argv) 
{
    int i;
    fluid_settings_t* settings;
    fluid_synth_t* synth;
    fluid_player_t* player;
    fluid_audio_driver_t* adriver;
    settings = new_fluid_settings();
    synth = new_fluid_synth(settings);
    player = new_fluid_player(synth);
    /* process command line arguments */
    fluid_synth_sfload(synth, argv[0], 1);
    //add in the midi player
    fluid_player_add(player, argv[1]);
    //add in the tick seek
    fluid_player_seek(player, argv[2]);
    //start the interrupt handler (so basically when we need to stop it)
    fluid_player_set_tick_callback(player, interrupt_handler, player, settings, synth, adriver)
    /* start the synthesizer thread */
    adriver = new_fluid_audio_driver(settings, synth);
    /* play the midi files, if any */
    fluid_player_play(player);
    /* wait for playback termination */
    fluid_player_join(player);
    /* cleanup */
    delete_fluid_audio_driver(adriver);
    delete_fluid_player(player);
    delete_fluid_synth(synth);
    delete_fluid_settings(settings);
    return 0;
}

//this function runs every single MIDI tick so it basically insta stops once the stop code is recieved
void interrupt_handler(fluid_player_t* player, fluid_settings_t* settings, fluid_synth_t* synth, fluid_audio_driver_t* adriver) {
    //TODO: find a way to send a signal over BLE/Wifi, once that's done, just run the code block below lol
    if(false) {
        fluid_player_stop(player)
        delete_fluid_audio_driver(adriver);
        delete_fluid_player(player);
        delete_fluid_synth(synth);
        delete_fluid_settings(settings);
    }
    //this one could handle drumstick imu inputs for volume control
    if(false) {
        fluid_synth_set_gain(synth, 0)
    }
    //this one could handle drumstick imu input for mod... i actually dunno what it does but maybe yall can figure it out
    if(false) {
        fluid_synth_add_mod(synth, mod, 0)
    }
}

/*
 * for reference - Jet
 * Create an array of PyMethodDef structs to hold the instance methods.
 * Name the python function corresponding to Matrix61c_get_value as "get" and Matrix61c_set_value
 * as "set"
 * You might find this link helpful: https://docs.python.org/3.6/c-api/structures.html
 */
static PyMethodDef Matrix61c_methods[] = {
    /* TODO: YOUR CODE HERE */
    {"init_player", (PyCFunction) songbirdControl_init_player, METH_VARARGS, NULL},
    {"start_playing", (PyCFunction) songbirdControl_start_playing, METH_VARARGS, NULL},
    {"stop_playing", (PyCFunction) songbirdControl_stop_playing, METH_VARARGS, NULL},
    {"adjust_volume", (PyCFunction) songbirdControl_adjust_volume, METH_VARARGS, NULL},
    {"adjust_tempo", (PyCFunction) songbirdControl_adjust_tempo, METH_VARARGS, NULL},
    {NULL, NULL, 0, NULL}
};

static PyMemberDef songbirdControl_members[] = {
    {NULL}  /* Sentinel */
};

static PyTypeObject midiplayerType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "songbirdControl",
    .tp_basicsize = sizeof(songbirdControl),
    .tp_dealloc = (destructor)songbirdControl_dealloc,
    .tp_repr = (reprfunc)songbirdControl_repr,
    .tp_as_number = &songbirdControl_as_number,
    .tp_flags = Py_TPFLAGS_DEFAULT |
        Py_TPFLAGS_BASETYPE,
    .tp_doc = "songbirdControl objects",
    .tp_methods = songbirdControl_methods,
    .tp_members = songbirdControl_members,
    .tp_as_mapping = &songbirdControl_mapping,
    .tp_init = (initproc)songbirdControl_init,
    .tp_new = songbirdControl_new
};

static struct PyModuleDef midiplayermodule = {
    PyModuleDef_HEAD_INIT,
    "midiplayer",
    "Midiplayer operations",
    -1,
    songbirdControl_class_methods
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
    PyModule_AddObject(m, "Midi Player", (PyObject *)&midiplayerType);
    printf("Successful import of Midi Player!\n");
    fflush(stdout);
    return m;
}
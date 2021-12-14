#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdio.h>
#include <fluidsynth.h>
#include <structmember.h>


typedef struct {
    PyObject_HEAD
    fluid_settings_t* settings;
    fluid_synth_t* synth;
    fluid_player_t* player;
    fluid_audio_driver_t* adriver;
} Songbird;

static void Songbird_dealloc(Songbird *self) {
    printf("Deallocating Songbird.\n");
    fflush(stdout);
    delete_fluid_audio_driver(self->adriver);
    delete_fluid_player(self->player);
    delete_fluid_synth(self->synth);
    delete_fluid_settings(self->settings);
    Py_TYPE(self)->tp_free((PyObject *) self);
}

static PyObject *Songbird_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    Songbird *self;
    self = (Songbird *) type->tp_alloc(type, 0);
    return (PyObject *) self;
}

static int Songbird_init(Songbird *self, PyObject *args, PyObject *kwds)
{

    self->settings = new_fluid_settings();
    self->synth = new_fluid_synth(self->settings);
    self->player = new_fluid_player(self->synth);
    fluid_settings_setstr(self->settings, "audio.driver", "alsa");
    self->adriver = new_fluid_audio_driver(self->settings, self->synth);

    if(fluid_synth_sfload(self->synth, "sftarget.sf2", 1) < 0){
        printf("Soundfont failed to load.\n");
        fflush(stdout);
        return -1;
    }
    if(fluid_player_add(self->player, "midtarget.mid") < 0) {
        printf("MIDI file failed to load.\n");
        fflush(stdout);
        return -1;
    }

    printf("Initialization of songbird object complete.\n");
    fflush(stdout);
    return 0;
}

static PyMemberDef Songbird_members[] = {
    {NULL}  /* Sentinel */
};

static void *Songbird_start_playing(Songbird *self, PyObject *pyTick) {
    fluid_player_seek(self->player, PyLong_AsLong(pyTick));
    fluid_player_play(self->player);
    return 0;
}

static void *Songbird_stop_playing(Songbird *self) {
    fluid_player_stop(self->player);
    return 0;
}

static void *Songbird_adjust_volume(Songbird *self, PyObject *pyVol) {
    fluid_synth_set_gain(self->synth, PyFloat_AsDouble(pyVol));
    return 0;
}

static void *Songbird_adjust_tempo(Songbird *self, PyObject *pyBpm) {
    fluid_player_set_bpm(self->player, PyLong_AsLong(pyBpm));
    return 0;
}

static PyMethodDef Songbird_methods[] = {
    {"start", (PyCFunction) Songbird_start_playing, METH_VARARGS,"start"},
    {"stop", (PyCFunction) Songbird_stop_playing, METH_VARARGS,"stop"},
    {"vol", (PyCFunction) Songbird_adjust_volume, METH_VARARGS,"vol"},
    {"bpm", (PyCFunction) Songbird_adjust_tempo, METH_VARARGS,"bpm"},
    {NULL}  /* Sentinel */
};

static PyTypeObject songbirdType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "songbird.Songbird",
    .tp_doc = "songbird objects",
    .tp_basicsize = sizeof(Songbird),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_new = Songbird_new,
    .tp_init = (initproc) Songbird_init,
    .tp_dealloc = (destructor) Songbird_dealloc,
    .tp_members = Songbird_members,
    .tp_methods = Songbird_methods,
};

static PyModuleDef songbirdmodule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "songbird",
    .m_doc = "songbird module",
    .m_size = -1,
};

PyMODINIT_FUNC PyInit_songbird(void)
{
    PyObject *m;
    if (PyType_Ready(&songbirdType) < 0)
        return NULL;

    m = PyModule_Create(&songbirdmodule);
    if (m == NULL)
        return NULL;

    Py_INCREF(&songbirdType);
    if (PyModule_AddObject(m, "Songbird", (PyObject *) &songbirdType) < 0) {
        Py_DECREF(&songbirdType);
        Py_DECREF(m);
        return NULL;
    }
    printf("Successful import of songbird!\n");
    fflush(stdout);
    return m;
}
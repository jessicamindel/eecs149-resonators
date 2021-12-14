/*
* REEEEEEEE stolen codeeeeeeeeeee (not rly)
*/

/*
* ooga booga write all the functions down here ok
*/
static int *songbirdControl_start_playing(PyObject *self, PyObject *pyTick);
static int *songbirdControl_stop_playing(PyObject *self);
static int *songbirdControl_adjust_volume(PyObject *self, PyObject *pyVol);
static int *songbirdControl_adjust_tempo(PyObject *self, PyObject *pyBPM);
static int songbirdControl_init(PyObject *self, PyObject *args, PyObject *kwds);
static void songbirdControl_dealloc(songbirdControl *self);
static PyObject *songbirdControl_repr(PyObject *self);
static PyObject *songbirdControl_new(PyTypeObject *type, PyObject *args, PyObject *kwds);


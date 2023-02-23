#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <cpr-detector.hpp>
#include <cstddef>
#include <iostream>

#ifdef __cplusplus
extern "C" {
#endif

  static PyObject *
  detector_find_matches(PyObject *self, PyObject *args) {
    const char* content;

    if (!PyArg_ParseTuple(args, "s", &content))
      return NULL;

    std::cout << "Called find_matches with: " << content << '\n';

    Py_RETURN_NONE;
  }

  static PyMethodDef DetectorMethods[] = {
    {"find_matches", detector_find_matches, METH_VARARGS,
     "Find matches in a text."},
    {NULL, NULL, 0, NULL} /* Sentinel */
  };

  static struct PyModuleDef detectormodule = {
    PyModuleDef_HEAD_INIT,
    "detector", /* name of module */
    NULL, /* No documentation, yet... */
    -1,
    DetectorMethods
  };

  PyMODINIT_FUNC PyInit_detector(void) {
    return PyModule_Create(&detectormodule);
  }

#ifdef __cplusplus
}
#endif

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <cpr-detector.hpp>
#include <cstddef>
#include <iostream>
#include <string>

#ifdef __cplusplus
extern "C" {
#endif

  static PyObject *
  detector_find_matches(PyObject *self, PyObject *args) {
    const char* content;

    if (!PyArg_ParseTuple(args, "s", &content))
      return NULL;

    CPRDetector::CPRDetector detector(false);
    std::string search_space(content);

    auto results = detector.find_matches(search_space);
    Py_ssize_t len = Py_ssize_t(results.size());

    PyObject* list_of_results = PyList_New(len);

    for (Py_ssize_t i = 0; i < len; ++i) {
      auto res = results[i];
      PyObject* obj = Py_BuildValue("{s:s, s:i, s:i}", "cpr", res.cpr_.c_str(), "start", res.start_, "end", res.end_);
      PyList_SetItem(list_of_results, i, obj);
    }

    return list_of_results;
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
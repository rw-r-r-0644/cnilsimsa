/* nilsimsa.c - A C implementation of Nilsimsa for Python. */

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#define PY_SSIZE_T_CLEAN
#include <Python.h>


static const unsigned char popcount[256] = {
  0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
  1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
  1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
  2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
  1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
  2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
  2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
  3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8,
};


int native_compare_hexdigest(const char *a, const char *b) {
  int i, ia, ib, bits = 0;
  char ha[3] = "  \0", hb[3] = "  \0";
  for (i = 0; i < 63; i += 2) {
    memcpy(ha, &a[i], 2);
    memcpy(hb, &b[i], 2);
    ia = strtol(&ha[0], NULL, 16);
    ib = strtol(&hb[0], NULL, 16);
    bits += popcount[255 & (ia ^ ib)];
  }
  return 128 - bits;
}


static PyObject * compare_hexdigests(PyObject * self, PyObject * args) {
  const char *a, *b;
  Py_ssize_t a_len, b_len;
  int i;
  if (!PyArg_ParseTuple(args, "s#s#", &a, &a_len, &b, &b_len)) {
    PyErr_SetString(PyExc_ValueError, "64 character hex digests required");
    return NULL;
  }
  if (!(a_len == 64 && b_len == 64)) {
    PyErr_SetString(PyExc_ValueError, "64 character hex digests required");
    return NULL;
  }
  i = native_compare_hexdigest(a, b);
  return PyLong_FromLong(i);
}


static const char _nilsimsa_docs[] =
  "compare_hexdigest(digest1, digest2): similarity score -128 to 128\n";


static PyMethodDef _nilsimsa_funcs[] = {
  {"compare_hexdigests",
   (PyCFunction) compare_hexdigests,
   METH_VARARGS,
   _nilsimsa_docs},
  {NULL}
};

static PyModuleDef _nilsimsa_module = {
  PyModuleDef_HEAD_INIT,
  "_nilsimsa",
  "Python C nilsimsa extension module.",
  0,
  _nilsimsa_funcs
};


PyMODINIT_FUNC PyInit__nilsimsa(void) {
  return PyModule_Create(&_nilsimsa_module);
}

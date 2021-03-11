// This file contains helper functions that are automatically created from
// templates.

#include "nuitka/prelude.h"

extern PyObject *callPythonFunction( PyObject *func, PyObject **args, int count );


PyObject *CALL_FUNCTION_WITH_ARGS2(PyObject *called, PyObject **args) {
    CHECK_OBJECT(called);

    // Check if arguments are valid objects in debug mode.
#ifndef __NUITKA_NO_ASSERT__
    for (size_t i = 0; i < 2; i++) {
        CHECK_OBJECT(args[i]);
    }
#endif

    if (Nuitka_Function_Check(called)) {
        if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
            return NULL;
        }

        struct Nuitka_FunctionObject *function = (struct Nuitka_FunctionObject *)called;
        PyObject *result;

        if (function->m_args_simple && 2 == function->m_args_positional_count){
            for (Py_ssize_t i = 0; i < 2; i++) {
                Py_INCREF(args[i]);
            }

            result = function->m_c_code(function, args);
        } else if (function->m_args_simple && 2 + function->m_defaults_given == function->m_args_positional_count) {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_positional_count);
#else
            PyObject *python_pars[function->m_args_positional_count];
#endif
            memcpy(python_pars, args, 2 * sizeof(PyObject *));
            memcpy(python_pars + 2, &PyTuple_GET_ITEM(function->m_defaults, 0), function->m_defaults_given * sizeof(PyObject *));

            for (Py_ssize_t i = 0; i < function->m_args_positional_count; i++) {
                Py_INCREF(python_pars[i]);
            }

            result = function->m_c_code(function, python_pars);
        } else {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_overall_count);
#else
            PyObject *python_pars[function->m_args_overall_count];
#endif
            memset(python_pars, 0, function->m_args_overall_count * sizeof(PyObject *));

            if (parseArgumentsPos(function, python_pars, args, 2)) {
                result = function->m_c_code(function, python_pars);
            } else {
                result = NULL;
            }
        }

        Py_LeaveRecursiveCall();

        return result;
    } else if (Nuitka_Method_Check(called)) {
        struct Nuitka_MethodObject *method = (struct Nuitka_MethodObject *)called;

        // Unbound method without arguments, let the error path be slow.
        if (method->m_object != NULL) {
            if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
                return NULL;
            }

            struct Nuitka_FunctionObject *function = method->m_function;

            PyObject *result;

            if (function->m_args_simple && 2 + 1 == function->m_args_positional_count) {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_positional_count);
#else
                PyObject *python_pars[function->m_args_positional_count];
#endif
                python_pars[0] = method->m_object;
                Py_INCREF(method->m_object);

                for (Py_ssize_t i = 0; i < 2; i++) {
                    python_pars[i + 1] = args[i];
                    Py_INCREF(args[i]);
                }

                result = function->m_c_code(function, python_pars);
            } else if ( function->m_args_simple && 2 + 1 + function->m_defaults_given == function->m_args_positional_count ) {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_positional_count);
#else
                PyObject *python_pars[function->m_args_positional_count];
#endif
                python_pars[0] = method->m_object;
                Py_INCREF(method->m_object);

                memcpy(python_pars+1, args, 2 * sizeof(PyObject *));
                memcpy(python_pars+1 + 2, &PyTuple_GET_ITEM(function->m_defaults, 0), function->m_defaults_given * sizeof(PyObject *));

                for (Py_ssize_t i = 1; i < function->m_args_overall_count; i++) {
                    Py_INCREF(python_pars[i]);
                }

                result = function->m_c_code(function, python_pars);
            } else {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_overall_count);
#else
                PyObject *python_pars[function->m_args_overall_count];
#endif
                memset(python_pars, 0, function->m_args_overall_count * sizeof(PyObject *));

                if (parseArgumentsMethodPos(function, python_pars, method->m_object, args, 2)) {
                    result = function->m_c_code(function, python_pars);
                } else {
                    result = NULL;
                }
            }

            Py_LeaveRecursiveCall();

            return result;
        }
    } else if (PyCFunction_Check(called)) {
        // Try to be fast about wrapping the arguments.
        int flags = PyCFunction_GET_FLAGS(called) & ~(METH_CLASS | METH_STATIC | METH_COEXIST);

        if (flags & METH_NOARGS) {
#if 2 == 0
            PyCFunction method = PyCFunction_GET_FUNCTION(called);
            PyObject *self = PyCFunction_GET_SELF(called);

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, NULL );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if (result != NULL) {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            } else {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely(!ERROR_OCCURRED())) {
                    SET_CURRENT_EXCEPTION_TYPE0_STR(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            SET_CURRENT_EXCEPTION_TYPE0_FORMAT1(
                PyExc_TypeError,
                "%s() takes no arguments (2 given)",
                ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        } else if (flags & METH_O) {
#if 2 == 1
            PyCFunction method = PyCFunction_GET_FUNCTION(called);
            PyObject *self = PyCFunction_GET_SELF(called);

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, args[0]);

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if (result != NULL) {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            } else {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely(!ERROR_OCCURRED())) {
                    SET_CURRENT_EXCEPTION_TYPE0_STR(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            SET_CURRENT_EXCEPTION_TYPE0_FORMAT1(PyExc_TypeError,
                "%s() takes exactly one argument (2 given)",
                 ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        } else if (flags & METH_VARARGS) {
            PyCFunction method = PyCFunction_GET_FUNCTION(called);
            PyObject *self = PyCFunction_GET_SELF(called);

            PyObject *pos_args = MAKE_TUPLE(args, 2);

            PyObject *result;

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
                return NULL;
            }
#endif

#if PYTHON_VERSION < 0x360
            if (flags & METH_KEYWORDS) {
                result = (*(PyCFunctionWithKeywords)method)(self, pos_args, NULL);
            } else {
                result = (*method)(self, pos_args);
            }
#else
            if (flags == (METH_VARARGS|METH_KEYWORDS)) {
                result = (*(PyCFunctionWithKeywords)method)(self, pos_args, NULL);
            } else if (flags == METH_FASTCALL) {
#if PYTHON_VERSION < 0x370
                result = (*(_PyCFunctionFast)method)(self, &PyTuple_GET_ITEM(pos_args, 0), 2, NULL);
#else
                result = (*(_PyCFunctionFast)method)(self, &pos_args, 2);
#endif
            } else {
                result = (*method)(self, pos_args);
            }
#endif

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if (result != NULL) {
                // Some buggy C functions do set an error, but do not indicate it
                // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                Py_DECREF(pos_args);
                return result;
            } else {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely(!ERROR_OCCURRED())) {
                    SET_CURRENT_EXCEPTION_TYPE0_STR(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                Py_DECREF(pos_args);
                return NULL;
            }
        }
    } else if (PyFunction_Check(called)) {
        return callPythonFunction(
            called,
            args,
            2
        );
    }

    PyObject *pos_args = MAKE_TUPLE(args, 2);

    PyObject *result = CALL_FUNCTION(called, pos_args, NULL);

    Py_DECREF(pos_args);

    return result;
}

PyObject *CALL_FUNCTION_WITH_ARGS3(PyObject *called, PyObject **args) {
    CHECK_OBJECT(called);

    // Check if arguments are valid objects in debug mode.
#ifndef __NUITKA_NO_ASSERT__
    for (size_t i = 0; i < 3; i++) {
        CHECK_OBJECT(args[i]);
    }
#endif

    if (Nuitka_Function_Check(called)) {
        if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
            return NULL;
        }

        struct Nuitka_FunctionObject *function = (struct Nuitka_FunctionObject *)called;
        PyObject *result;

        if (function->m_args_simple && 3 == function->m_args_positional_count){
            for (Py_ssize_t i = 0; i < 3; i++) {
                Py_INCREF(args[i]);
            }

            result = function->m_c_code(function, args);
        } else if (function->m_args_simple && 3 + function->m_defaults_given == function->m_args_positional_count) {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_positional_count);
#else
            PyObject *python_pars[function->m_args_positional_count];
#endif
            memcpy(python_pars, args, 3 * sizeof(PyObject *));
            memcpy(python_pars + 3, &PyTuple_GET_ITEM(function->m_defaults, 0), function->m_defaults_given * sizeof(PyObject *));

            for (Py_ssize_t i = 0; i < function->m_args_positional_count; i++) {
                Py_INCREF(python_pars[i]);
            }

            result = function->m_c_code(function, python_pars);
        } else {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_overall_count);
#else
            PyObject *python_pars[function->m_args_overall_count];
#endif
            memset(python_pars, 0, function->m_args_overall_count * sizeof(PyObject *));

            if (parseArgumentsPos(function, python_pars, args, 3)) {
                result = function->m_c_code(function, python_pars);
            } else {
                result = NULL;
            }
        }

        Py_LeaveRecursiveCall();

        return result;
    } else if (Nuitka_Method_Check(called)) {
        struct Nuitka_MethodObject *method = (struct Nuitka_MethodObject *)called;

        // Unbound method without arguments, let the error path be slow.
        if (method->m_object != NULL) {
            if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
                return NULL;
            }

            struct Nuitka_FunctionObject *function = method->m_function;

            PyObject *result;

            if (function->m_args_simple && 3 + 1 == function->m_args_positional_count) {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_positional_count);
#else
                PyObject *python_pars[function->m_args_positional_count];
#endif
                python_pars[0] = method->m_object;
                Py_INCREF(method->m_object);

                for (Py_ssize_t i = 0; i < 3; i++) {
                    python_pars[i + 1] = args[i];
                    Py_INCREF(args[i]);
                }

                result = function->m_c_code(function, python_pars);
            } else if ( function->m_args_simple && 3 + 1 + function->m_defaults_given == function->m_args_positional_count ) {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_positional_count);
#else
                PyObject *python_pars[function->m_args_positional_count];
#endif
                python_pars[0] = method->m_object;
                Py_INCREF(method->m_object);

                memcpy(python_pars+1, args, 3 * sizeof(PyObject *));
                memcpy(python_pars+1 + 3, &PyTuple_GET_ITEM(function->m_defaults, 0), function->m_defaults_given * sizeof(PyObject *));

                for (Py_ssize_t i = 1; i < function->m_args_overall_count; i++) {
                    Py_INCREF(python_pars[i]);
                }

                result = function->m_c_code(function, python_pars);
            } else {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_overall_count);
#else
                PyObject *python_pars[function->m_args_overall_count];
#endif
                memset(python_pars, 0, function->m_args_overall_count * sizeof(PyObject *));

                if (parseArgumentsMethodPos(function, python_pars, method->m_object, args, 3)) {
                    result = function->m_c_code(function, python_pars);
                } else {
                    result = NULL;
                }
            }

            Py_LeaveRecursiveCall();

            return result;
        }
    } else if (PyCFunction_Check(called)) {
        // Try to be fast about wrapping the arguments.
        int flags = PyCFunction_GET_FLAGS(called) & ~(METH_CLASS | METH_STATIC | METH_COEXIST);

        if (flags & METH_NOARGS) {
#if 3 == 0
            PyCFunction method = PyCFunction_GET_FUNCTION(called);
            PyObject *self = PyCFunction_GET_SELF(called);

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, NULL );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if (result != NULL) {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            } else {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely(!ERROR_OCCURRED())) {
                    SET_CURRENT_EXCEPTION_TYPE0_STR(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            SET_CURRENT_EXCEPTION_TYPE0_FORMAT1(
                PyExc_TypeError,
                "%s() takes no arguments (3 given)",
                ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        } else if (flags & METH_O) {
#if 3 == 1
            PyCFunction method = PyCFunction_GET_FUNCTION(called);
            PyObject *self = PyCFunction_GET_SELF(called);

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, args[0]);

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if (result != NULL) {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            } else {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely(!ERROR_OCCURRED())) {
                    SET_CURRENT_EXCEPTION_TYPE0_STR(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            SET_CURRENT_EXCEPTION_TYPE0_FORMAT1(PyExc_TypeError,
                "%s() takes exactly one argument (3 given)",
                 ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        } else if (flags & METH_VARARGS) {
            PyCFunction method = PyCFunction_GET_FUNCTION(called);
            PyObject *self = PyCFunction_GET_SELF(called);

            PyObject *pos_args = MAKE_TUPLE(args, 3);

            PyObject *result;

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
                return NULL;
            }
#endif

#if PYTHON_VERSION < 0x360
            if (flags & METH_KEYWORDS) {
                result = (*(PyCFunctionWithKeywords)method)(self, pos_args, NULL);
            } else {
                result = (*method)(self, pos_args);
            }
#else
            if (flags == (METH_VARARGS|METH_KEYWORDS)) {
                result = (*(PyCFunctionWithKeywords)method)(self, pos_args, NULL);
            } else if (flags == METH_FASTCALL) {
#if PYTHON_VERSION < 0x370
                result = (*(_PyCFunctionFast)method)(self, &PyTuple_GET_ITEM(pos_args, 0), 3, NULL);
#else
                result = (*(_PyCFunctionFast)method)(self, &pos_args, 3);
#endif
            } else {
                result = (*method)(self, pos_args);
            }
#endif

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if (result != NULL) {
                // Some buggy C functions do set an error, but do not indicate it
                // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                Py_DECREF(pos_args);
                return result;
            } else {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely(!ERROR_OCCURRED())) {
                    SET_CURRENT_EXCEPTION_TYPE0_STR(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                Py_DECREF(pos_args);
                return NULL;
            }
        }
    } else if (PyFunction_Check(called)) {
        return callPythonFunction(
            called,
            args,
            3
        );
    }

    PyObject *pos_args = MAKE_TUPLE(args, 3);

    PyObject *result = CALL_FUNCTION(called, pos_args, NULL);

    Py_DECREF(pos_args);

    return result;
}

PyObject *CALL_FUNCTION_WITH_ARGS4(PyObject *called, PyObject **args) {
    CHECK_OBJECT(called);

    // Check if arguments are valid objects in debug mode.
#ifndef __NUITKA_NO_ASSERT__
    for (size_t i = 0; i < 4; i++) {
        CHECK_OBJECT(args[i]);
    }
#endif

    if (Nuitka_Function_Check(called)) {
        if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
            return NULL;
        }

        struct Nuitka_FunctionObject *function = (struct Nuitka_FunctionObject *)called;
        PyObject *result;

        if (function->m_args_simple && 4 == function->m_args_positional_count){
            for (Py_ssize_t i = 0; i < 4; i++) {
                Py_INCREF(args[i]);
            }

            result = function->m_c_code(function, args);
        } else if (function->m_args_simple && 4 + function->m_defaults_given == function->m_args_positional_count) {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_positional_count);
#else
            PyObject *python_pars[function->m_args_positional_count];
#endif
            memcpy(python_pars, args, 4 * sizeof(PyObject *));
            memcpy(python_pars + 4, &PyTuple_GET_ITEM(function->m_defaults, 0), function->m_defaults_given * sizeof(PyObject *));

            for (Py_ssize_t i = 0; i < function->m_args_positional_count; i++) {
                Py_INCREF(python_pars[i]);
            }

            result = function->m_c_code(function, python_pars);
        } else {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_overall_count);
#else
            PyObject *python_pars[function->m_args_overall_count];
#endif
            memset(python_pars, 0, function->m_args_overall_count * sizeof(PyObject *));

            if (parseArgumentsPos(function, python_pars, args, 4)) {
                result = function->m_c_code(function, python_pars);
            } else {
                result = NULL;
            }
        }

        Py_LeaveRecursiveCall();

        return result;
    } else if (Nuitka_Method_Check(called)) {
        struct Nuitka_MethodObject *method = (struct Nuitka_MethodObject *)called;

        // Unbound method without arguments, let the error path be slow.
        if (method->m_object != NULL) {
            if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
                return NULL;
            }

            struct Nuitka_FunctionObject *function = method->m_function;

            PyObject *result;

            if (function->m_args_simple && 4 + 1 == function->m_args_positional_count) {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_positional_count);
#else
                PyObject *python_pars[function->m_args_positional_count];
#endif
                python_pars[0] = method->m_object;
                Py_INCREF(method->m_object);

                for (Py_ssize_t i = 0; i < 4; i++) {
                    python_pars[i + 1] = args[i];
                    Py_INCREF(args[i]);
                }

                result = function->m_c_code(function, python_pars);
            } else if ( function->m_args_simple && 4 + 1 + function->m_defaults_given == function->m_args_positional_count ) {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_positional_count);
#else
                PyObject *python_pars[function->m_args_positional_count];
#endif
                python_pars[0] = method->m_object;
                Py_INCREF(method->m_object);

                memcpy(python_pars+1, args, 4 * sizeof(PyObject *));
                memcpy(python_pars+1 + 4, &PyTuple_GET_ITEM(function->m_defaults, 0), function->m_defaults_given * sizeof(PyObject *));

                for (Py_ssize_t i = 1; i < function->m_args_overall_count; i++) {
                    Py_INCREF(python_pars[i]);
                }

                result = function->m_c_code(function, python_pars);
            } else {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_overall_count);
#else
                PyObject *python_pars[function->m_args_overall_count];
#endif
                memset(python_pars, 0, function->m_args_overall_count * sizeof(PyObject *));

                if (parseArgumentsMethodPos(function, python_pars, method->m_object, args, 4)) {
                    result = function->m_c_code(function, python_pars);
                } else {
                    result = NULL;
                }
            }

            Py_LeaveRecursiveCall();

            return result;
        }
    } else if (PyCFunction_Check(called)) {
        // Try to be fast about wrapping the arguments.
        int flags = PyCFunction_GET_FLAGS(called) & ~(METH_CLASS | METH_STATIC | METH_COEXIST);

        if (flags & METH_NOARGS) {
#if 4 == 0
            PyCFunction method = PyCFunction_GET_FUNCTION(called);
            PyObject *self = PyCFunction_GET_SELF(called);

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, NULL );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if (result != NULL) {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            } else {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely(!ERROR_OCCURRED())) {
                    SET_CURRENT_EXCEPTION_TYPE0_STR(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            SET_CURRENT_EXCEPTION_TYPE0_FORMAT1(
                PyExc_TypeError,
                "%s() takes no arguments (4 given)",
                ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        } else if (flags & METH_O) {
#if 4 == 1
            PyCFunction method = PyCFunction_GET_FUNCTION(called);
            PyObject *self = PyCFunction_GET_SELF(called);

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, args[0]);

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if (result != NULL) {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            } else {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely(!ERROR_OCCURRED())) {
                    SET_CURRENT_EXCEPTION_TYPE0_STR(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            SET_CURRENT_EXCEPTION_TYPE0_FORMAT1(PyExc_TypeError,
                "%s() takes exactly one argument (4 given)",
                 ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        } else if (flags & METH_VARARGS) {
            PyCFunction method = PyCFunction_GET_FUNCTION(called);
            PyObject *self = PyCFunction_GET_SELF(called);

            PyObject *pos_args = MAKE_TUPLE(args, 4);

            PyObject *result;

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
                return NULL;
            }
#endif

#if PYTHON_VERSION < 0x360
            if (flags & METH_KEYWORDS) {
                result = (*(PyCFunctionWithKeywords)method)(self, pos_args, NULL);
            } else {
                result = (*method)(self, pos_args);
            }
#else
            if (flags == (METH_VARARGS|METH_KEYWORDS)) {
                result = (*(PyCFunctionWithKeywords)method)(self, pos_args, NULL);
            } else if (flags == METH_FASTCALL) {
#if PYTHON_VERSION < 0x370
                result = (*(_PyCFunctionFast)method)(self, &PyTuple_GET_ITEM(pos_args, 0), 4, NULL);
#else
                result = (*(_PyCFunctionFast)method)(self, &pos_args, 4);
#endif
            } else {
                result = (*method)(self, pos_args);
            }
#endif

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if (result != NULL) {
                // Some buggy C functions do set an error, but do not indicate it
                // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                Py_DECREF(pos_args);
                return result;
            } else {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely(!ERROR_OCCURRED())) {
                    SET_CURRENT_EXCEPTION_TYPE0_STR(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                Py_DECREF(pos_args);
                return NULL;
            }
        }
    } else if (PyFunction_Check(called)) {
        return callPythonFunction(
            called,
            args,
            4
        );
    }

    PyObject *pos_args = MAKE_TUPLE(args, 4);

    PyObject *result = CALL_FUNCTION(called, pos_args, NULL);

    Py_DECREF(pos_args);

    return result;
}

PyObject *CALL_FUNCTION_WITH_ARGS5(PyObject *called, PyObject **args) {
    CHECK_OBJECT(called);

    // Check if arguments are valid objects in debug mode.
#ifndef __NUITKA_NO_ASSERT__
    for (size_t i = 0; i < 5; i++) {
        CHECK_OBJECT(args[i]);
    }
#endif

    if (Nuitka_Function_Check(called)) {
        if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
            return NULL;
        }

        struct Nuitka_FunctionObject *function = (struct Nuitka_FunctionObject *)called;
        PyObject *result;

        if (function->m_args_simple && 5 == function->m_args_positional_count){
            for (Py_ssize_t i = 0; i < 5; i++) {
                Py_INCREF(args[i]);
            }

            result = function->m_c_code(function, args);
        } else if (function->m_args_simple && 5 + function->m_defaults_given == function->m_args_positional_count) {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_positional_count);
#else
            PyObject *python_pars[function->m_args_positional_count];
#endif
            memcpy(python_pars, args, 5 * sizeof(PyObject *));
            memcpy(python_pars + 5, &PyTuple_GET_ITEM(function->m_defaults, 0), function->m_defaults_given * sizeof(PyObject *));

            for (Py_ssize_t i = 0; i < function->m_args_positional_count; i++) {
                Py_INCREF(python_pars[i]);
            }

            result = function->m_c_code(function, python_pars);
        } else {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_overall_count);
#else
            PyObject *python_pars[function->m_args_overall_count];
#endif
            memset(python_pars, 0, function->m_args_overall_count * sizeof(PyObject *));

            if (parseArgumentsPos(function, python_pars, args, 5)) {
                result = function->m_c_code(function, python_pars);
            } else {
                result = NULL;
            }
        }

        Py_LeaveRecursiveCall();

        return result;
    } else if (Nuitka_Method_Check(called)) {
        struct Nuitka_MethodObject *method = (struct Nuitka_MethodObject *)called;

        // Unbound method without arguments, let the error path be slow.
        if (method->m_object != NULL) {
            if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
                return NULL;
            }

            struct Nuitka_FunctionObject *function = method->m_function;

            PyObject *result;

            if (function->m_args_simple && 5 + 1 == function->m_args_positional_count) {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_positional_count);
#else
                PyObject *python_pars[function->m_args_positional_count];
#endif
                python_pars[0] = method->m_object;
                Py_INCREF(method->m_object);

                for (Py_ssize_t i = 0; i < 5; i++) {
                    python_pars[i + 1] = args[i];
                    Py_INCREF(args[i]);
                }

                result = function->m_c_code(function, python_pars);
            } else if ( function->m_args_simple && 5 + 1 + function->m_defaults_given == function->m_args_positional_count ) {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_positional_count);
#else
                PyObject *python_pars[function->m_args_positional_count];
#endif
                python_pars[0] = method->m_object;
                Py_INCREF(method->m_object);

                memcpy(python_pars+1, args, 5 * sizeof(PyObject *));
                memcpy(python_pars+1 + 5, &PyTuple_GET_ITEM(function->m_defaults, 0), function->m_defaults_given * sizeof(PyObject *));

                for (Py_ssize_t i = 1; i < function->m_args_overall_count; i++) {
                    Py_INCREF(python_pars[i]);
                }

                result = function->m_c_code(function, python_pars);
            } else {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_overall_count);
#else
                PyObject *python_pars[function->m_args_overall_count];
#endif
                memset(python_pars, 0, function->m_args_overall_count * sizeof(PyObject *));

                if (parseArgumentsMethodPos(function, python_pars, method->m_object, args, 5)) {
                    result = function->m_c_code(function, python_pars);
                } else {
                    result = NULL;
                }
            }

            Py_LeaveRecursiveCall();

            return result;
        }
    } else if (PyCFunction_Check(called)) {
        // Try to be fast about wrapping the arguments.
        int flags = PyCFunction_GET_FLAGS(called) & ~(METH_CLASS | METH_STATIC | METH_COEXIST);

        if (flags & METH_NOARGS) {
#if 5 == 0
            PyCFunction method = PyCFunction_GET_FUNCTION(called);
            PyObject *self = PyCFunction_GET_SELF(called);

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, NULL );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if (result != NULL) {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            } else {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely(!ERROR_OCCURRED())) {
                    SET_CURRENT_EXCEPTION_TYPE0_STR(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            SET_CURRENT_EXCEPTION_TYPE0_FORMAT1(
                PyExc_TypeError,
                "%s() takes no arguments (5 given)",
                ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        } else if (flags & METH_O) {
#if 5 == 1
            PyCFunction method = PyCFunction_GET_FUNCTION(called);
            PyObject *self = PyCFunction_GET_SELF(called);

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, args[0]);

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if (result != NULL) {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            } else {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely(!ERROR_OCCURRED())) {
                    SET_CURRENT_EXCEPTION_TYPE0_STR(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            SET_CURRENT_EXCEPTION_TYPE0_FORMAT1(PyExc_TypeError,
                "%s() takes exactly one argument (5 given)",
                 ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        } else if (flags & METH_VARARGS) {
            PyCFunction method = PyCFunction_GET_FUNCTION(called);
            PyObject *self = PyCFunction_GET_SELF(called);

            PyObject *pos_args = MAKE_TUPLE(args, 5);

            PyObject *result;

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
                return NULL;
            }
#endif

#if PYTHON_VERSION < 0x360
            if (flags & METH_KEYWORDS) {
                result = (*(PyCFunctionWithKeywords)method)(self, pos_args, NULL);
            } else {
                result = (*method)(self, pos_args);
            }
#else
            if (flags == (METH_VARARGS|METH_KEYWORDS)) {
                result = (*(PyCFunctionWithKeywords)method)(self, pos_args, NULL);
            } else if (flags == METH_FASTCALL) {
#if PYTHON_VERSION < 0x370
                result = (*(_PyCFunctionFast)method)(self, &PyTuple_GET_ITEM(pos_args, 0), 5, NULL);
#else
                result = (*(_PyCFunctionFast)method)(self, &pos_args, 5);
#endif
            } else {
                result = (*method)(self, pos_args);
            }
#endif

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if (result != NULL) {
                // Some buggy C functions do set an error, but do not indicate it
                // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                Py_DECREF(pos_args);
                return result;
            } else {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely(!ERROR_OCCURRED())) {
                    SET_CURRENT_EXCEPTION_TYPE0_STR(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                Py_DECREF(pos_args);
                return NULL;
            }
        }
    } else if (PyFunction_Check(called)) {
        return callPythonFunction(
            called,
            args,
            5
        );
    }

    PyObject *pos_args = MAKE_TUPLE(args, 5);

    PyObject *result = CALL_FUNCTION(called, pos_args, NULL);

    Py_DECREF(pos_args);

    return result;
}


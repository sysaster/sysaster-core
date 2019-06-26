#ifndef _SYSASTER_TELLO_IMGSOURCE_
#define _SYSASTER_TELLO_IMGSOURCE_

#include <Python.h>
#include "sysaster/core/imgsource/ImageSource.h"
#include "numpy/arrayobject.h"
#include <thread>
#include <chrono>

/**
 * Capture images from a tello drone
 * by calling an external Python class.
 *
 * @author Vitor Greati
 * */
class TelloImageSource : public ImageSource {

    private:

        std::string local_ip;
        std::string tello_ip;
        int local_port;
        int tello_port;
        bool imperial;
        float response_timeout;

        PyObject *module_name, *module, *dict, *python_class, *object;

        void init_numpy() {
            import_array();
        }

    protected:

        bool establish() override {
    
            setenv("PYTHONPATH", ".", 1);

            Py_SetProgramName("sysaster-tello");
            Py_Initialize();

            PyObject* pArgs = PyTuple_New(6);
            PyTuple_SetItem(pArgs, 0, PyString_FromString(local_ip.c_str()));
            PyTuple_SetItem(pArgs, 1, PyInt_FromLong(local_port));
            PyTuple_SetItem(pArgs, 2, PyBool_FromLong(imperial));
            PyTuple_SetItem(pArgs, 3, PyFloat_FromDouble(0.5));
            PyTuple_SetItem(pArgs, 4, PyString_FromString(tello_ip.c_str()));
            PyTuple_SetItem(pArgs, 5, PyInt_FromLong(tello_port));
            
            module_name = PyString_FromString("tello.tello");

            module = PyImport_Import(module_name);
            if (module == nullptr) {
                PyErr_Print();
                std::cerr << "Fails to import the module.\n";
            }
            Py_DECREF(module_name);

            // dict is a borrowed reference.
            dict = PyModule_GetDict(module);
            if (dict == nullptr) {
              PyErr_Print();
              std::cerr << "Fails to get the dictionary.\n";
            }
            Py_DECREF(module);

            // builds the name of a callable class
            python_class = PyDict_GetItemString(dict, "Tello");
            if (python_class == nullptr) {
              PyErr_Print();
              std::cerr << "Fails to get the Python class.\n";
            }
            Py_DECREF(dict);

            // creates an instance of the class
            if (PyCallable_Check(python_class)) {
              object = PyObject_CallObject(python_class, pArgs);
              Py_DECREF(python_class);
            } else {
              std::cout << "Cannot instantiate the Python class" << std::endl;
              Py_DECREF(python_class);
            }
        }

        bool get_image(cv::Mat& image) override { 
            init_numpy();
            PyObject * img = Py_None;

            int i = 0;
            while (img == Py_None)
                img = PyObject_CallMethod(object, "read", NULL);

            if (img == Py_None)
                return false;

            PyArrayObject *np_ret = reinterpret_cast<PyArrayObject*>(img);
            npy_intp nrows = PyArray_DIM(np_ret, 0); // number of rows
            npy_intp ncols = PyArray_DIM(np_ret, 1); // number of columns

            cv::Mat mat (nrows, ncols, CV_8UC3, PyArray_DATA(np_ret));
            image = mat.clone();

            Py_DECREF(np_ret);
            Py_DECREF(img);
            return true;       
        }

    public:

        TelloImageSource(const decltype(local_ip)& local_ip,
                int local_port,
                const decltype(tello_ip)& tello_ip,
                int tello_port=8889,
                bool imperial = false,
                float response_timeout=0.3)
        : local_ip {local_ip}, local_port {local_port},
          tello_ip {tello_ip}, tello_port {tello_port}, 
          imperial {imperial}, response_timeout{response_timeout} {/** empty */} 

        ~TelloImageSource() {
            Py_DECREF(object);
            Py_Finalize();
        }
};

#endif


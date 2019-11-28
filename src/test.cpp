#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/iostream.h>
#include "life.h"

template <typename... Args>
using overload_cast_ = pybind11::detail::overload_cast_impl<Args...>;

namespace py = pybind11;

PYBIND11_MODULE(_life, m) {
    py::class_<life>(m, "life")
    .def(py::init<const int, const int>())
    .def(py::init<const int, const int, int, const long>())
    .def("update", overload_cast_<void>()(&life::update))
    .def("update", overload_cast_<int>()(&life::update))
    .def_readwrite("data", &life::currectLife);



#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "0.0.1";
#endif
}
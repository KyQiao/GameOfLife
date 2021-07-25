#include <pybind11/iostream.h>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "life.h"

namespace py = pybind11;
using intArray_t = py::array_t<int, py::array::c_style | py::array::forcecast>;

PYBIND11_MODULE(_life, m) {
  py::class_<life>(m, "life")
      .def(py::init<>())
      .def(py::init<const int&, const int&>())
      .def(py::init<const int&, const int&,
                    const int&, const long&>())
      .def(py::init<const int&, const int&, const std::vector<int>&>())
      .def(py::init<const std::string&, int>())
      .def("read", (&life::read))
      .def("update", (&life::update), py::arg("n") = 1, py::arg("bound") = false)
      .def("print", (&life::print))
      .def("getData", [](life& self) { return intArray_t({self._size()}, self.getData()); })
      .def("getH", (&life::getH))
      .def("getW", (&life::getW));

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "0.0.1";
#endif
}
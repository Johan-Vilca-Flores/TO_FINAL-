#pragma once

#include <string>

struct Consulta {
    int id;
    std::string paciente;
    std::string especialidad;
    std::string aseguradora;
    double montoBase;
    double montoFinal;
};

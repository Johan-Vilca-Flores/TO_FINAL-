#pragma once

#include "model/Consulta.hpp"
#include "persistence/ArchivoConsultas.hpp"
#include "strategy/TarifaStrategy.hpp"

#include <algorithm>
#include <cmath>
#include <cctype>
#include <filesystem>
#include <limits>
#include <numeric>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

class ClinicaController {
public:
    explicit ClinicaController(
        std::filesystem::path ruta = std::filesystem::path("data") / "consultas.csv")
        : archivo_(std::move(ruta)), consultas_(archivo_.cargar()) {}

    const std::vector<Consulta>& consultas() const noexcept {
        return consultas_;
    }

    Consulta registrarConsulta(const std::string& paciente,
                               const std::string& especialidad,
                               double montoBase,
                               const TarifaStrategy& tarifa) {
        if (estaVacio(paciente) || estaVacio(especialidad)) {
            throw std::invalid_argument("Paciente y especialidad son obligatorios.");
        }
        if (!std::isfinite(montoBase) || montoBase <= 0.0) {
            throw std::invalid_argument("El monto debe ser un número mayor que cero.");
        }

        const int id = siguienteId();
        const double montoFinal = tarifa.calcular(montoBase);
        if (!std::isfinite(montoFinal) || montoFinal < 0.0 ||
            montoFinal > montoBase) {
            throw std::logic_error("La estrategia de tarifa calculó un monto inválido.");
        }

        Consulta consulta{id, paciente, especialidad, tarifa.nombre(),
                          montoBase, montoFinal};
        archivo_.guardar(consulta);
        consultas_.push_back(consulta);
        return consulta;
    }

    double totalFacturado() const noexcept {
        return std::accumulate(
            consultas_.begin(), consultas_.end(), 0.0,
            [](double total, const Consulta& consulta) {
                return total + consulta.montoFinal;
            });
    }

private:
    static bool estaVacio(const std::string& texto) {
        return texto.empty() ||
               std::all_of(texto.begin(), texto.end(), [](unsigned char caracter) {
                   return std::isspace(caracter) != 0;
               });
    }

    int siguienteId() const {
        int mayorId = 0;
        for (const auto& consulta : consultas_) {
            mayorId = std::max(mayorId, consulta.id);
        }
        if (mayorId == std::numeric_limits<int>::max()) {
            throw std::overflow_error("Se alcanzó el límite de identificadores.");
        }
        return mayorId + 1;
    }

    ArchivoConsultas archivo_;
    std::vector<Consulta> consultas_;
};

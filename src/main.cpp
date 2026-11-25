#include "controller/ClinicaController.hpp"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>

namespace {

std::string leerTexto(const std::string& indicacion) {
    std::cout << indicacion;
    std::string texto;
    if (!std::getline(std::cin, texto)) {
        throw std::runtime_error("Se cerró la entrada de datos.");
    }
    return texto;
}

std::string leerTextoObligatorio(const std::string& indicacion) {
    while (true) {
        std::string texto = leerTexto(indicacion);
        const bool vacio = std::all_of(
            texto.begin(), texto.end(), [](unsigned char caracter) {
                return std::isspace(caracter) != 0;
            });
        if (!vacio) {
            return texto;
        }
        std::cout << "Este campo es obligatorio.\n";
    }
}

int leerOpcion(const std::string& indicacion, int minimo, int maximo) {
    while (true) {
        const std::string texto = leerTexto(indicacion);
        std::istringstream entrada(texto);
        int valor = 0;
        char sobrante = '\0';
        if ((entrada >> valor) && !(entrada >> sobrante) &&
            valor >= minimo && valor <= maximo) {
            return valor;
        }
        std::cout << "Opción no válida. Intenta de nuevo.\n";
    }
}

double leerMonto() {
    while (true) {
        const std::string texto = leerTexto("Tarifa base (S/): ");
        std::istringstream entrada(texto);
        double monto = 0.0;
        char sobrante = '\0';
        if ((entrada >> monto) && !(entrada >> sobrante) &&
            std::isfinite(monto) && monto > 0.0) {
            return std::round(monto * 100.0) / 100.0;
        }
        std::cout << "Ingresa un monto válido mayor que cero, por ejemplo 80.00.\n";
    }
}

std::unique_ptr<TarifaStrategy> seleccionarTarifa() {
    std::cout << "\nTipo de seguro:\n"
              << "  1. Particular (sin descuento)\n"
              << "  2. Seguro privado (20% de descuento)\n"
              << "  3. SIS (50% de descuento)\n";
    switch (leerOpcion("Selecciona una opción: ", 1, 3)) {
    case 1:
        return std::make_unique<TarifaParticular>();
    case 2:
        return std::make_unique<TarifaSeguroPrivado>();
    default:
        return std::make_unique<TarifaSeguroSIS>();
    }
}

void mostrarMenu() {
    std::cout << "\n========================================\n"
              << "   SISTEMA DE GESTIÓN CLÍNICA - DEMO\n"
              << "========================================\n"
              << "  1. Registrar consulta\n"
              << "  2. Ver consultas\n"
              << "  3. Ver resumen de caja\n"
              << "  0. Salir\n";
}

void registrarConsulta(ClinicaController& controller) {
    const std::string paciente = leerTextoObligatorio("Nombre del paciente: ");
    const std::string especialidad = leerTextoObligatorio("Especialidad: ");
    const double montoBase = leerMonto();
    const auto tarifa = seleccionarTarifa();
    const Consulta consulta = controller.registrarConsulta(
        paciente, especialidad, montoBase, *tarifa);

    std::cout << "\nConsulta registrada correctamente.\n"
              << "  Código: " << consulta.id << '\n'
              << "  Seguro: " << consulta.aseguradora << '\n'
              << std::fixed << std::setprecision(2)
              << "  Tarifa base: S/ " << consulta.montoBase << '\n'
              << "  A pagar:     S/ " << consulta.montoFinal << '\n';
}

void mostrarConsultas(const ClinicaController& controller) {
    const auto& consultas = controller.consultas();
    if (consultas.empty()) {
        std::cout << "\nTodavía no hay consultas registradas.\n";
        return;
    }

    std::cout << '\n' << std::left << std::setw(6) << "ID"
              << std::setw(24) << "PACIENTE"
              << std::setw(20) << "ESPECIALIDAD"
              << std::setw(26) << "SEGURO"
              << std::right << std::setw(12) << "BASE"
              << std::setw(12) << "A PAGAR" << '\n'
              << std::string(100, '-') << '\n'
              << std::fixed << std::setprecision(2);

    for (const auto& consulta : consultas) {
        std::cout << std::left << std::setw(6) << consulta.id
                  << std::setw(24) << consulta.paciente.substr(0, 22)
                  << std::setw(20) << consulta.especialidad.substr(0, 18)
                  << std::setw(26) << consulta.aseguradora.substr(0, 24)
                  << std::right << std::setw(12) << consulta.montoBase
                  << std::setw(12) << consulta.montoFinal << '\n';
    }
}

void mostrarResumen(const ClinicaController& controller) {
    const auto cantidad = controller.consultas().size();
    std::cout << "\nResumen de caja\n"
              << "  Consultas registradas: " << cantidad << '\n'
              << std::fixed << std::setprecision(2)
              << "  Total facturado: S/ " << controller.totalFacturado() << '\n';
}

} // namespace

int main() {
    try {
        ClinicaController controller;
        while (true) {
            mostrarMenu();
            const int opcion = leerOpcion("Elige una opción: ", 0, 3);
            switch (opcion) {
            case 1:
                registrarConsulta(controller);
                break;
            case 2:
                mostrarConsultas(controller);
                break;
            case 3:
                mostrarResumen(controller);
                break;
            case 0:
                std::cout << "Hasta luego.\n";
                return 0;
            default:
                break;
            }
        }
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }
}

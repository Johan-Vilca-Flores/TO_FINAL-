#pragma once

#include "model/Consulta.hpp"

#include <charconv>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>
#include <system_error>
#include <utility>
#include <vector>

class ArchivoConsultas {
public:
    explicit ArchivoConsultas(std::filesystem::path ruta)
        : ruta_(std::move(ruta)) {}

    std::vector<Consulta> cargar() const {
        if (!std::filesystem::exists(ruta_)) {
            return {};
        }

        std::ifstream archivo(ruta_);
        if (!archivo) {
            throw std::runtime_error("No se pudo abrir " + ruta_.string());
        }

        std::string linea;
        if (!std::getline(archivo, linea)) {
            return {};
        }
        if (linea != encabezado()) {
            throw std::runtime_error("El encabezado de " + ruta_.string() +
                                     " no tiene el formato esperado.");
        }

        std::vector<Consulta> consultas;
        std::size_t numeroLinea = 1;
        while (std::getline(archivo, linea)) {
            ++numeroLinea;
            if (linea.empty()) {
                continue;
            }

            try {
                const auto campos = separarCsv(linea);
                if (campos.size() != 6) {
                    throw std::runtime_error("cantidad de columnas incorrecta");
                }

                Consulta consulta{
                    convertirId(campos[0]),
                    campos[1],
                    campos[2],
                    campos[3],
                    convertirMonto(campos[4]),
                    convertirMonto(campos[5])};

                if (consulta.id <= 0 || consulta.paciente.empty() ||
                    consulta.especialidad.empty() || consulta.aseguradora.empty() ||
                    consulta.montoBase <= 0.0 || consulta.montoFinal < 0.0 ||
                    consulta.montoFinal > consulta.montoBase) {
                    throw std::runtime_error("valores fuera de rango");
                }
                consultas.push_back(std::move(consulta));
            } catch (const std::exception& error) {
                throw std::runtime_error("Error en " + ruta_.string() + ", línea " +
                                         std::to_string(numeroLinea) + ": " +
                                         error.what());
            }
        }

        if (archivo.bad()) {
            throw std::runtime_error("Error al leer " + ruta_.string());
        }
        return consultas;
    }

    void guardar(const Consulta& consulta) const {
        const auto directorio = ruta_.parent_path();
        if (!directorio.empty()) {
            std::filesystem::create_directories(directorio);
        }

        const bool necesitaEncabezado =
            !std::filesystem::exists(ruta_) || std::filesystem::file_size(ruta_) == 0;
        std::ofstream archivo(ruta_, std::ios::app);
        if (!archivo) {
            throw std::runtime_error("No se pudo escribir en " + ruta_.string());
        }

        if (necesitaEncabezado) {
            archivo << encabezado() << '\n';
        }
        archivo << consulta.id << ','
                << escaparCsv(consulta.paciente) << ','
                << escaparCsv(consulta.especialidad) << ','
                << escaparCsv(consulta.aseguradora) << ','
                << std::fixed << std::setprecision(2) << consulta.montoBase << ','
                << consulta.montoFinal << '\n';
        archivo.flush();
        if (!archivo) {
            throw std::runtime_error("Error al guardar en " + ruta_.string());
        }
    }

private:
    static const std::string& encabezado() {
        static const std::string valor =
            "id,paciente,especialidad,aseguradora,monto_base,monto_final";
        return valor;
    }

    static std::string escaparCsv(const std::string& valor) {
        if (valor.find_first_of(",\"\r\n") == std::string::npos) {
            return valor;
        }

        std::string escapado = "\"";
        for (const char caracter : valor) {
            if (caracter == '"') {
                escapado += "\"\"";
            } else {
                escapado += caracter;
            }
        }
        escapado += '"';
        return escapado;
    }

    static std::vector<std::string> separarCsv(const std::string& linea) {
        std::vector<std::string> campos;
        std::string campo;
        bool entreComillas = false;

        for (std::size_t indice = 0; indice < linea.size(); ++indice) {
            const char caracter = linea[indice];
            if (entreComillas) {
                if (caracter == '"' && indice + 1 < linea.size() &&
                    linea[indice + 1] == '"') {
                    campo += '"';
                    ++indice;
                } else if (caracter == '"') {
                    entreComillas = false;
                } else {
                    campo += caracter;
                }
            } else if (caracter == ',') {
                campos.push_back(std::move(campo));
                campo.clear();
            } else if (caracter == '"' && campo.empty()) {
                entreComillas = true;
            } else if (caracter == '"') {
                throw std::runtime_error("comillas CSV no válidas");
            } else {
                campo += caracter;
            }
        }

        if (entreComillas) {
            throw std::runtime_error("campo CSV sin comillas de cierre");
        }
        campos.push_back(std::move(campo));
        return campos;
    }

    static int convertirId(const std::string& texto) {
        int valor = 0;
        const auto resultado =
            std::from_chars(texto.data(), texto.data() + texto.size(), valor);
        if (resultado.ec != std::errc{} ||
            resultado.ptr != texto.data() + texto.size()) {
            throw std::runtime_error("identificador no válido");
        }
        return valor;
    }

    static double convertirMonto(const std::string& texto) {
        std::istringstream entrada(texto);
        double valor = 0.0;
        char sobrante = '\0';
        if (!(entrada >> valor) || (entrada >> sobrante) || !std::isfinite(valor)) {
            throw std::runtime_error("monto no válido");
        }
        return valor;
    }

    std::filesystem::path ruta_;
};

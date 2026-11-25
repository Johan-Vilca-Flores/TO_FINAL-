#pragma once

#include <cmath>
#include <string>

class TarifaStrategy {
public:
    virtual ~TarifaStrategy() = default;
    virtual std::string nombre() const = 0;
    virtual double calcular(double montoBase) const = 0;

protected:
    static double redondear(double monto) {
        return std::round(monto * 100.0) / 100.0;
    }
};

class TarifaParticular final : public TarifaStrategy {
public:
    std::string nombre() const override {
        return "Particular (0%)";
    }

    double calcular(double montoBase) const override {
        return redondear(montoBase);
    }
};

class TarifaSeguroPrivado final : public TarifaStrategy {
public:
    std::string nombre() const override {
        return "Seguro privado (20%)";
    }

    double calcular(double montoBase) const override {
        return redondear(montoBase * 0.80);
    }
};

class TarifaSeguroSIS final : public TarifaStrategy {
public:
    std::string nombre() const override {
        return "SIS (50%)";
    }

    double calcular(double montoBase) const override {
        return redondear(montoBase * 0.50);
    }
};

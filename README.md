# Sistema Integral de Gestión Clínica

**ClinicaMVC-CPP** es un prototipo académico en C++17 para registrar consultas y calcular sus tarifas según el seguro. La versión actual es una aplicación de consola: permite probar el flujo sin instalar bibliotecas gráficas.

> **Demo:** usa únicamente datos ficticios. El prototipo no está preparado para almacenar información médica real.

## Lo que ya funciona

- Registrar consultas con paciente, especialidad y tarifa base.
- Aplicar estrategias de tarifa: particular (0%), seguro privado (20%) y SIS (50%).
- Listar consultas y ver un resumen de caja.
- Guardar y cargar registros desde `data/consultas.csv`.
- Validar opciones y montos ingresados.

Los datos se guardan en la carpeta desde la que se ejecuta el programa. No ingreses información real de pacientes.

## Compilar y ejecutar en Windows

Necesitas un compilador C++17, por ejemplo MinGW-w64/MSYS2.

```powershell
New-Item -ItemType Directory -Force build
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude src/main.cpp -o build\ClinicaMVC.exe
.\build\ClinicaMVC.exe
```

Al iniciar, usa el menú para registrar una consulta, listar los registros o revisar el resumen de caja. Para una prueba rápida, registra una tarifa de `100`, selecciona seguro privado y verifica que el monto a pagar sea `80.00`.

## Estructura actual

```text
include/
├── controller/ClinicaController.hpp
├── model/Consulta.hpp
├── persistence/ArchivoConsultas.hpp
└── strategy/TarifaStrategy.hpp
src/
└── main.cpp
```

La consola es la vista inicial; `ClinicaController` coordina el flujo, `Consulta` representa el modelo, las estrategias calculan tarifas y `ArchivoConsultas` gestiona el CSV. Aún no hay una ventana gráfica; podemos añadir una más adelante, por ejemplo con Qt, cuando el flujo base esté estable.

## Siguientes pasos

1. Separar la vista de consola en su propia clase y añadir pruebas.
2. Incorporar historial por paciente y filtros de consultas.
3. Implementar deshacer con Command y `std::stack`.
4. Evaluar una interfaz gráfica para la demostración final.

## GitHub Pages

La documentación está preparada para [GitHub Pages](https://johan-vilca-flores.github.io/TO_FINAL-/). Si aún no aparece publicada, abre **Settings → Pages**, selecciona **GitHub Actions** como origen y vuelve a ejecutar **Deploy GitHub Pages** en la pestaña **Actions**.

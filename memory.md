# Estado del proyecto — ClinicaMVC-CPP

## Objetivo

Prototipo académico en C++17 para demostrar el registro básico de consultas clínicas, cálculo de tarifas por seguro y persistencia local. La demostración actual usa una interfaz de consola para poder compilarse y probarse sin instalar dependencias gráficas.

## Estado actual

Implementado:

- Menú de consola para registrar consultas, listar registros y consultar el resumen de caja.
- Datos de consulta: código, paciente, especialidad, aseguradora, tarifa base y total a pagar.
- Patrón Strategy con tarifas Particular (0%), Seguro privado (20%) y SIS (50%).
- Guardado y carga de consultas en CSV, con comillas escapadas para campos que incluyen comas o comillas.
- Validación de opciones, montos y datos cargados; los errores de archivo se informan explícitamente.
- Compilación con C++17 y compilador MinGW-w64/MSYS2.

No implementado todavía: ventana gráfica, historial de pacientes, deshacer, Observer, base de datos ni pruebas automatizadas.

## Arquitectura inicial

```text
src/main.cpp                              Interfaz de consola y ciclo principal
include/model/Consulta.hpp                Entidad de consulta
include/controller/ClinicaController.hpp  Registro y resumen de consultas
include/strategy/TarifaStrategy.hpp       Estrategias de cálculo por seguro
include/persistence/ArchivoConsultas.hpp  Persistencia CSV
```

La capa de consola es la vista inicial; el controlador aplica reglas y coordina modelo, tarifas y almacenamiento. El prototipo está deliberadamente acotado: sirve para presentar y verificar el flujo principal, no como sistema clínico listo para producción.

## Cómo probarlo

```powershell
New-Item -ItemType Directory -Force build
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude src/main.cpp -o build\ClinicaMVC.exe
.\build\ClinicaMVC.exe
```

Prueba sugerida: registra una consulta de S/ 100 con seguro privado; el total debe ser S/ 80.00. Cierra y vuelve a abrir el programa para confirmar que el registro permanece en `data/consultas.csv`.

## Seguridad y alcance

Usar solo datos ficticios en esta etapa. El CSV local no cifra información y este prototipo no implementa autenticación, permisos ni controles necesarios para almacenar datos reales de salud.

## Próximos hitos

1. Añadir pruebas automatizadas para tarifas y persistencia.
2. Separar la vista de consola y agregar búsquedas e historial por paciente.
3. Implementar Command/undo y reportes.
4. Decidir si se incorpora una GUI con Qt después de validar el flujo de consola.

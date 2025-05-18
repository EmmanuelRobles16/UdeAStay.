# UdeAStay.
**Informe desafio 2**
**Materia:** Informática II  
**Universidad:** Universidad de Antioquia  
**Estudiantes:** Emmanuel Robles  
**Profesores:** Aníbal Guerra y Augusto Salazar  
**Semestre:** 2025-1 

# Introducción

El contenido que sigue a continuación se ocupa de explicar el proceso de análisis, diseño y construcción de una solución computacional para el Desafío II que se propone en la asignatura Informática II (2025-1). Este desafío consiste en modelar un sistema de gestión de estadías hogareñas para el entorno ficticio propuesto UdeAStay, y lo tomamos como referente a aplicaciones reales de alojamiento tradicional como lo puede ser Airbnb, con una arquitectura orientada a objetos construida en C++.
El sistema permite la gestión eficiente de alojamientos, reservas, anfitriones y huéspedes; todo ello respetando las propiedades esenciales de la Programación Orientada a Objetos como son el encapsulamiento, la abstracción, la modularidad. La función básica a realizar es la automatización de operaciones de búsqueda de hospedajes, la gestión de reservaciones, la actualización de históricos y la visualización de los registros de tipo huespedes, utilizando para ello estructuras de datos diseñadas manualmente, sin el uso de la STL.

# Analisis del problema

UdeAStay funciona como un mercado digital de estadías hogareñas. En este contexto, se identifican dos tipos de usuarios: huéspedes (personas que buscan y reservan alojamientos) y anfitriones (personas que ofrecen alojamientos). Cada alojamiento tiene atributos asociados como nombre, tipo, ubicación, precio y un conjunto de amenidades. Las reservaciones también cuentan con múltiples atributos como fechas, duración, método de pago, anotaciones y relación directa con huéspedes y alojamientos.

Se requiere que el sistema cumpla con funcionalidades específicas:

  -Ingreso con perfil de usuario.

  -Búsqueda y reserva de alojamientos aplicando filtros.

  -Anulación de reservas activas.

  -Consulta de reservaciones por parte del anfitrión.

  -Actualización automática del histórico de reservas vencidas.

  -Medición de consumo de recursos (memoria e iteraciones).

  # Diseño de la solución

El diseño del sistema se basó en una arquitectura orientada a objetos que agrupa las entidades clave del dominio: Huesped, Anfitrion, Alojamiento, Reservacion y Fecha. Cada clase fue diseñada para encapsular los datos y comportamientos relevantes, permitiendo una implementación modular, extensible y clara.

Para la interacción con el usuario, se diseñó una única clase llamada Menu, encargada de mostrar las opciones disponibles, recibir las decisiones del usuario y dirigir el flujo del programa hacia las funcionalidades correspondientes. Esta clase contiene la lógica para cargar los datos desde archivos, autenticar el ingreso (tanto de anfitriones como de huéspedes), y acceder a operaciones como reservas, consultas, anulaciones y actualizaciones del historico.

La clase Fecha cumple un rol clave en el control de disponibilidad, manejo de fechas límite, cálculo de duración y formato de presentación. Incluye funcionalidades como comparación entre fechas, suma de días y formateo completo tipo "jueves 15 de mayo del 2025".

El flujo del sistema puede resumirse así:

  1-Se carga la información desde los archivos al inicio.

  2-El usuario ingresa como huésped o como anfitrión.

  3-Según su tipo, el Menu presenta las opciones habilitadas.

  4-El sistema ejecuta la acción seleccionada y actualiza los archivos si es necesario.

  5-Se permite medir los recursos consumidos (memoria e iteraciones).

  6-El programa permanece en ejecución hasta que se indique salir.

# Estructuras de datos utilizadas

**Descripción general**

Para representar las diferentes entidades del sistema, se implementaron estructuras de datos manuales, La decisión de estructura para cada caso se basó en su comportamiento esperado: si la cantidad de elementos es fija, si puede crecer, o si requiere inserciones/eliminaciones frecuentes. A continuación se describen las estructuras utilizadas:

Para huéspedes y anfitriones, se usaron arreglos dinámicos de punteros dobles. La cantidad se conoce al cargar los archivos, y se accede por índice o búsqueda secuencial según el documento.

Para los alojamientos, también se usó un arreglo dinámico, ya que la cantidad es fija al leer desde archivo. Cada elemento contiene atributos y una lista propia de reservas.

Las amenidades se almacenaron en un arreglo estático de tamaño 10 dentro de cada alojamiento. No se modifican en tiempo de ejecución, por lo tanto no requieren estructuras dinámicas.

Las reservas vigentes y reservas históricas se gestionaron mediante arreglos semi-dinámicos, que se duplican manualmente al alcanzar su capacidad. Esto permite agregar y eliminar elementos según las operaciones del sistema.

Para controlar la disponibilidad diaria de los alojamientos, se utilizó un arreglo estático de 365 booleanos por alojamiento, donde cada posición representa un día del año.

Esta selección de estructuras permitió un equilibrio entre eficiencia, simplicidad y cumplimiento estricto de los requisitos del reto

# Estructura de los archivos txt

Los archivos .txt utilizados actúan como almacenamiento permanente. Cada uno representa una entidad clave del sistema, y cada línea del archivo contiene un objeto codificado usando el separador | para facilitar su lectura y descomposición.

📁 huespedes.txt
# documento | antigüedad (meses) | puntuación
1122334455|18|4.5

📁 anfitriones.txt
# documento | antigüedad | puntuación | alojamientos separados por coma
1004567890|24|4.7|A001,A002

📁 alojamientos.txt
# código | nombre | doc anfitrión | depto | municipio | tipo | dirección | precio | amenidades separadas por coma
A001|Casa El Retiro|1004567890|Antioquia|El Retiro|Casa|Cra 10 #15-30|120000|Piscina,Parqueadero,Aire

📁 reservas_vigentes.txt y reservas_historico.txt
# código | alojamiento | huésped | fecha entrada | duración | pago | fecha pago | monto | anotación
R001|A001|1122334455|2025-06-10|3|PSE|2025-05-25|360000|Llegamos en la noche

Estos archivos son leídos al iniciar el programa y actualizados en cada operación relevante. El sistema ignora líneas comentadas con #.

# Conclusiones

El desarrollo del sistema UdeAStay representó un ejercicio riguroso de análisis, diseño e implementación orientado a objetos, aplicando los conceptos fundamentales de estructuras de datos manuales en C++. Forzó un mayor control sobre la gestión de memoria, la organización modular del código y la correcta aplicación de punteros y arreglos personalizados.

Se logró modelar correctamente el comportamiento de un sistema de reservas real, permitiendo operaciones complejas como la verificación de disponibilidad, filtrado de alojamientos, manejo de historial, y validación de conflictos, todo ello sobre estructuras propias como arreglos dinámicos, semi-dinámicos, listas enlazadas y estructuras auxiliares como la clase Fecha.

El diseño centrado en la clase Menu como controlador permitió simplificar el flujo del sistema y separar responsabilidades, mientras que el uso de esquemas de memoria y métodos dedicados por clase facilitó la depuración y extensibilidad del proyecto.

En resumen, el proyecto permitió fortalecer habilidades esenciales en programación de sistemas estructurados, fomentando buenas prácticas de diseño, lógica algorítmica clara y un enfoque disciplinado en el desarrollo de software modular y escalable.

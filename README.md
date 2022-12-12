# Sistema-de-control
Control y monitoreo del nivel en un contenedor cilindrico 

El código en lenguaje C es una interfaz para el control y monitoreo de una válvula a traves de un controlador (simulado en Arduino)
El desarrollo de la interfaz permite:
- La conexión del controlador
- La configuración del controlador 
  - Modo de operación 
  - Límite mínimo y máximo
- Configuración de válvulas
  - Válvula de entrada 
    - Abrir válvula 
    - Cerrar válvula
  - Válvula de salida 
    - Abrir válvula 
    - Cerrar válvula
- Monitoreo de variables
- Salir
    
+ Comunicación SR-232 con una placa de Desarrollo Arduino para la escritura y lectura de datos
+ Aplicación del principio de columna hidrostática
+ Instrumentos de cada válvula
  - Sensores de final de carrera (ZSx)
  - Válvulas selenoides (SVx)
  - Pulsadores (PBx)

Los siguientes archivos fueron proporcionados por el Doctor Carlos Javier Morales Pérez
Archivos:
- com_port.c
- com_port.h

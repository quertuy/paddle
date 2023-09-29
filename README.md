Id: 369010
#### Jhonier Stiven Mosquera Blandon
# Proyecto juego [Pong](https://youtu.be/fiShX2pTz9A?si=YvYoQbYoKognsDfv).

## Evaluacion unidad 2

En esta evaluacion se encontrara los siguientes conceptos en lenguaje C:

- Lectura de comandos de entrada desde el teclado.
- Lectura de archivos en memoria usando el `HEAP`.
- Procesamiento de la información y manipulación de la memoria.
- Escritura de archivos.
## Instalacion

Para configurar el entorno de trabajo para cualquier sistema operativo se siguio este curso de [udemy](https://www.udemy.com/course/game-loop-c-sdl/), donde podemos encontrar la configuracion inicial en los distintos sistemas operativos Ubuntu, Mac y Windows

## Objectivo
El objetivo es manejar una pala posicionada en la parte inferior de la pantalla, evitando que la pelota haga contacto con el borde inferior. La pelota rebotará de manera predefinida o aleatoria contra los bordes de la pantalla.
### Mecanicas
1. El jugador podra moverse a traves de las teclas de izquierda ← y derecha →.
2. La pelota inicia en diagonal desde la esquina superior izquierda hasta la esquina inferior derecha.
3.  La pala debe desplazarse horizontalmente para interceptar la pelota y enviarla hacia arriba.
4. El juego terminará si la pelota llega a tocar el borde inferior de la pantalla.
5. Una puntuación puede calcularse con base en los rebotes exitosos de la pelota contra la pala. Por ejemplo, cada vez que se contabilicen 3 rebotes. No se permite puntuación por cada rebote, se debe realizar algún tipo de cálculo.

## Etapas de desarrollo
1. Configuracion basica con SDL2 :heavy_check_mark:
2. Movimiento de la pelota y rebotes :heavy_check_mark:
3. Colisiones con la paleta y puntuación :heavy_check_mark:
4. Aumento de velocidad :heavy_check_mark:
5. Historial de eventos :heavy_multiplication_x:
6. Registro de eventos
:heavy_multiplication_x:
7. Entrada del jugador :heavy_multiplication_mark:
8. Almacenamiento en archivo
:heavy_multiplication_x:
9. Carga de partidas
:heavy_multiplication_x:
10. Reproducción de partidas
:heavy_multiplication_x:
11. Optimización y mejoras :heavy_check_mark:
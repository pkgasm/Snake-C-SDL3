# Snake en C con SDL3

Un clon clásico del juego Snake desarrollado en C utilizando la biblioteca SDL3.

![Gameplay del juego Snake](placeholder.gif)
*(Sugerencia: Reemplaza `placeholder.gif` con una captura de pantalla o un GIF animado de tu juego)*

## Características

- Movimiento clásico de la serpiente.
- Sistema de puntuación y guardado de la puntuación más alta.
- Pantalla de título, pausa y fin de juego.
- Power-ups que afectan la jugabilidad:
  - **Manzana Dorada**: Otorga una bonificación de 50 puntos.
  - **Cubo de Hielo**: Ralentiza temporalmente a la serpiente.
- Aumento gradual de la velocidad a medida que aumenta la puntuación.
- Controles por teclado (WASD y flechas) y botones en la interfaz.

## Prerrequisitos

Para compilar y ejecutar este juego, necesitarás:

- Un compilador de C (como GCC o Clang).
- La biblioteca **SDL3** instalada en tu sistema.
- La biblioteca **SDL3_ttf** (para SDL3) instalada en tu sistema.

Este proyecto incluye las bibliotecas SDL3 y SDL3_ttf en el repositorio para facilitar la compilación en Windows. Para otros sistemas operativos, es posible que necesites instalarlas manualmente.

## Cómo Compilar y Ejecutar

Abre una terminal en el directorio raíz del proyecto y ejecuta el siguiente comando. Este comando compilará todos los archivos `.c` y los enlazará con las bibliotecas SDL3 y SDL_ttf.

### En Windows (usando MinGW/GCC)

```bash
gcc src/*.c -o snake.exe -I./SDL3/include -I./SDL3_ttf/include -L./SDL3/lib/x64 -L./SDL3_ttf/lib/x64 -lSDL3 -lSDL3_ttf -mwindows
```

### En Linux/macOS

Asegúrate de tener SDL3 y SDL3_ttf instalados a través de tu gestor de paquetes.

```bash
gcc src/*.c -o snake $(sdl3-config --cflags --libs) -lSDL3_ttf
```

Después de la compilación, ejecuta el juego:

```bash
# En Windows
.\snake.exe

# En Linux/macOS
./snake
```

## Controles

- **Flechas de dirección** o **WASD**: Mover la serpiente.
- **P**: Pausar / Reanudar el juego.
- **R**: Reiniciar el juego.
- **ESPACIO**: Reiniciar el juego en la pantalla de "Game Over".
- **ESC**: Salir del juego.
- **Ratón**: Interactuar con los botones del menú y del juego.

## Licencia

Este proyecto está bajo la Licencia MIT. Consulta el archivo `LICENSE` para más detalles.
# GameEngine2D

![](Obrazy/Menu.png)

## Opis

Projekt silnika voxelowego stworzony z użyciem biblioteki GLEW. Praca skupia się na generowaniu różnorodnych terenów oraz optymalizacji renderowania w celu poprawy wydajności.

![](Obrazy/Odbicie.png)

## Generator

W grze znalazł się generator terenu wykorzystujący kilka technik, takich jak Perlin noise oraz Wave Collapse Function.

![](Obrazy/Pustynia.png)

![](Obrazy/śnieg.png)

![](Obrazy/Swiatło.png)

## Optymalizacja

W projekcie zostało użytych kilka sposobów optymalizacji. Jednym z nich jest greedy meshing, polegający na redukcji liczby przesyłanych wierzchołków do karty graficznej, a także redukcji liczby trójkątów poprzez usunięcie przesłoniętych ścian oraz redukcję danych przesyłanych do GPU.

![](Obrazy/GrredMesh.png)

![](Obrazy/Obraz pod ziemią.png)

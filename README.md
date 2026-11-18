# Tarea #6: Árboles y Grafos 
**Alumna:** Camila Araceli Alfaro Chuquino  
**Universidad:** Universidad de Ingeniería y Tecnología (UTEC)  
**Ciclo:** 2026-2
---
## Compilación y Ejecución
```bash
g++ -std=c++17 -O2 main.cpp -o tarea6
./tarea6
```
---
## 1. Ejercicio 1: Demostración matemática de la degradación del BST a $O(n)$
Sea $S = (x_1, x_2, \dots, x_n)$ una secuencia estrictamente creciente ($x_1 < x_2 < \dots < x_n$) insertada en un Árbol Binario de Búsqueda (BST) inicialmente vacío:
1. **Caso base ($k = 1$):** El primer elemento $x_1$ se convierte en la raíz $r$. La altura del árbol es $h(1) = 0$ y la profundidad de $x_1$ es $d(x_1) = 0$.
2. **Paso inductivo:** Supóngase que tras insertar $k-1$ elementos, el árbol forma una cadena lineal derecha de longitud $k-2$, donde $x_{k-1}$ es el único nodo hoja a profundidad $d(x_{k-1}) = k - 2$ y ningún nodo posee hijo izquierdo ($\forall i < k-1,\; \text{left}(x_i) = \text{null}$ y $\text{right}(x_i) = x_{i+1}$).  
   Al insertar $x_k$: dado que $x_k > x_i$ para todo $1 \le i \le k-1$, el algoritmo de inserción desciende exclusivamente por los punteros derechos atravesando los $k-1$ nodos existentes hasta ubicar $x_k$ como hijo derecho de $x_{k-1}$, a profundidad $d(x_k) = k - 1$.
3. **Costo total de construcción y altura:**
    - La altura final tras $n$ inserciones es $h(n) = n - 1 \in \Theta(n)$.
    - El número total de comparaciones realizadas durante las $n$ inserciones es:
      $$T(n) = \sum_{k=1}^{n} (k - 1) = \frac{n(n - 1)}{2} \in \Theta(n^2)$$
    - Cualquier operación posterior (búsqueda, inserción o eliminación) sobre el nodo más profundo recorre toda la cadena de $n$ nodos, requiriendo $h(n) + 1 = n \in O(n)$ operaciones en lugar de $O(\log n)$.
---
## 2. Problema 2: Demostración formal de la cota $O(\log n)$ en Red-Black Trees
Sea $bh(x)$ la **altura negra** de un nodo $x$, definida como el número de nodos negros en cualquier camino simple desde $x$ (sin contar a $x$) hasta una hoja nula (`NIL`). Por la Propiedad 4, $bh(x)$ está bien definida para todo nodo $x$.
### Lema
*Todo subárbol con raíz en un nodo $x$ contiene al menos $2^{bh(x)} - 1$ nodos internos.*
**Demostración por inducción sobre la altura $h(x)$:**
* **Caso base ($h(x) = 0$):** $x$ es una hoja `NIL`, por lo que $bh(x) = 0$. El número de nodos internos es $2^0 - 1 = 0$. Se cumple.
* **Paso inductivo:** Sea $x$ un nodo interno con $h(x) > 0$ y dos hijos $y, z$ (que pueden ser internos o `NIL`). Cada hijo tiene altura negra $bh(x)$ (si el hijo es rojo) o $bh(x) - 1$ (si el hijo es negro). Como $h(y) < h(x)$ y $h(z) < h(x)$, aplicamos la hipótesis inductiva a ambos hijos:
  $$\text{Nodos}(x) = 1 + \text{Nodos}(y) + \text{Nodos}(z) \ge 1 + \left(2^{bh(x)-1} - 1\right) + \left(2^{bh(x)-1} - 1\right) = 2 \cdot 2^{bh(x)-1} - 1 = 2^{bh(x)} - 1$$
### Teorema (Cota superior de la altura)
*Un Red-Black Tree con $n$ nodos internos tiene altura $h \le 2\log_2(n + 1) \in O(\log n)$.*
**Demostración:**
Por la Propiedad 3 (ningún nodo rojo tiene hijo rojo), en cualquier camino simple de longitud $h$ desde la raíz hasta una hoja `NIL`, al menos la mitad de los nodos (excluyendo la raíz) deben ser negros. Por lo tanto:
$$bh(\text{root}) \ge \frac{h}{2}$$
Aplicando el Lema a la raíz del árbol ($n$ nodos internos):
$$n \ge 2^{bh(\text{root})} - 1 \ge 2^{h/2} - 1$$
Despejando $h$:
$$n + 1 \ge 2^{h/2} \implies \log_2(n + 1) \ge \frac{h}{2} \implies h \le 2\log_2(n + 1) \in O(\log n)$$
### Comparación experimental (20 elementos ascendentes $1 \dots 20$)

| Estructura | Altura obtenida | Complejidad real | Observación |
| :--- | :---: | :---: | :--- |
| **BST Tradicional** | `19` | $O(n)$ | Degradado en cadena lineal derecha |
| **Red-Black Tree** | `5` | $O(\log n)$ | Cumple $5 \le 2\log_2(21) \approx 8.78$ |

#### Recorrido Pre-Order del Red-Black Tree (`R` = Rojo, `B` = Negro):
    8(B) 4(B) 2(B) 1(B) 3(B) 6(B) 5(B) 7(B) 12(R) 10(B) 9(B) 11(B) 16(B) 14(R) 13(B) 15(B) 18(R) 17(B) 19(B) 20(R)
---
## 3. Problema 3: ¿Por qué DFS es adecuado para caminos raíz-hoja de suma $K$?
1. **Alineación natural con la estructura de caminos:** Un camino raíz-hoja es una secuencia lineal de ancestros desde la raíz hasta un nodo terminal. La pila explícita (`stack`) de DFS mantiene en todo momento la rama activa en exploración, permitiendo acumular la suma parcial y almacenar el camino exacto sin procesar subárboles hermanos simultáneamente.
2. **Complejidad espacial óptima:** Mientras que BFS almacena en cola toda la frontera horizontal del árbol —que en un árbol balanceado alcanza $\Theta(n/2) = \Theta(n)$ nodos en el último nivel—, DFS solo almacena nodos a lo largo de la rama actual, requiriendo $O(h)$ espacio en pila ($O(\log n)$ en árboles balanceados).
3. **Terminación temprana (Early Exit):** Al profundizar primero hasta las hojas, DFS encuentra y retorna el primer camino válido tan pronto como alcanza una hoja con suma $K$, evitando expandir el resto del árbol.
---
## 4. Problema 4: Justificación formal de BFS para completitud, ancho máximo y profundidad mínima
Sea $L_d = \{v \in V : \text{dist}(\text{root}, v) = d\}$ el conjunto de nodos a profundidad $d$ procesados secuencialmente por la cola FIFO (`queue`) de BFS:
1. **Profundidad mínima $(\min_{v \in \text{Hojas}} d(v))$:**
   Por inducción sobre el orden de extracción de la cola FIFO, BFS visita todos los nodos de $L_d$ antes de extraer cualquier nodo de $L_{d+1}$ (monotonicidad de distancias). En consecuencia, el **primer** nodo hoja extraído de la cola pertenece necesariamente al nivel $d^*$ más pequeño posible, garantizando optimalidad y corte temprano con complejidad $O(b^{d^*})$.
2. **Ancho máximo ($\max_{d \ge 0} |L_d|$):**
   Al iniciar cada iteración del bucle externo, la cola contiene exacta y exclusivamente todos los nodos del nivel $L_d$. Así, `q.size()` mide directamente $|L_d|$ sin estructuras auxiliares de agrupación por nivel, obteniendo $\max_d |L_d|$ en una única pasada $O(n)$.
3. **Verificación de árbol binario completo:**
   Por definición, un árbol binario de altura $h$ es completo si todos los niveles $0, \dots, h-1$ están totalmente llenos y todos los nodos del nivel $h$ están alineados lo más a la izquierda posible. El recorrido BFS enumera los nodos exactamente en el orden canónico de índices de un arreglo ($1, 2, \dots, n$). Por tanto, el árbol es completo si y solo si en el flujo FIFO de hijos nunca aparece un puntero no nulo después de haber observado un puntero `nullptr`.
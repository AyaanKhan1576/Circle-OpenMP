
---

# 🌀 OpenMP Circle Drawing with Taylor Series and OpenGL

This project implements a C++ program that draws a circle using both **serial** and **OpenMP-parallelized** Taylor series approximations of `sin(x)` and `cos(x)`, and visualizes the result using **OpenGL**. The goal is to analyze parallel performance and accuracy in a visual and mathematical context.

---

## 📌 Objective

- Implement a circle using parametric equations with Taylor series.
- Parallelize computation using OpenMP to accelerate performance.
- Visualize both serial and parallel outputs using OpenGL.
- Measure and compare performance between the two methods.

---

## 📐 Approach

### 1. Mathematical Model
The circle is drawn using parametric equations:
- `x(t) = r * cos(t) + j`
- `y(t) = r * sin(t) + k`

Where:
- `t` = angle (in degrees, converted to radians)
- `r` = radius
- `(j, k)` = circle center

### 2. Taylor Series Approximation

Implemented from scratch (no `std::sin/cos`):
- `sin(x) ≈ Σ [(-1)^n * x^(2n+1)] / (2n+1)!`
- `cos(x) ≈ Σ [(-1)^n * x^(2n)] / (2n)!`

Used up to **50 terms** for high precision.

### 3. Parallelization with OpenMP

- `#pragma omp parallel for` applied to the angle loop
- Inner Taylor series loop also tested with parallelism
- Performance measured using `omp_get_wtime()`

### 4. Visualization with OpenGL

- Used `GL_LINE_LOOP` to draw connected smooth lines
- Used `glVertex2f` for sub-pixel accuracy
- Enabled anti-aliasing for smoother rendering

---

## ⚠️ Challenges Faced

### 1. Parallel Overhead
- For smaller point counts or term counts, parallel execution was **slower** due to threading overhead.

### 2. Nested Parallelism
- Initial implementation parallelized both the outer angle loop and inner Taylor loop.
- OpenMP doesn’t handle nested parallel regions efficiently by default, which degraded performance.

### 3. Circle Smoothness
- At 1° (361 points), the circle appeared jagged.
- Increasing resolution to 0.1° (3601 points) improved smoothness drastically.

### 4. Accuracy of Taylor Approximation
- Lower-order Taylor expansions (5–10 terms) caused distortions.
- 15–50 terms produced accurate circles.

---

## 📊 Results & Observations

| Configuration          | Serial Time (s) | Parallel Time (s) | Notes                        |
|------------------------|-----------------|-------------------|------------------------------|
| 360 Points, 15 Terms   | 0.00100017      | 0.00199986        | Parallel slower (overhead)   |
| 3600 Points, 15 Terms  | 0.00499988      | 0.0139999         | Parallel slower (overhead)   |
| 360 Points, 50 Terms   | 0.00499988      | 0.00200009        | Parallel 50% faster          |
| 3600 Points, 50 Terms  | 0.0439999       | 0.00999999        | Parallel **much faster**     |

### Key Takeaways

- Parallel speedup is **only noticeable** with larger workloads (more points × more terms).
- Visualization confirmed that serial and parallel outputs matched perfectly (visually overlapping).
- Float-based coordinates and anti-aliasing created high-quality graphics.

---

## 📌 Conclusion

OpenMP parallelization offers **clear benefits for large workloads**, but may degrade performance in small-scale computations due to overhead. This project demonstrates:

- How **granularity matters** in parallel programming
- The value of combining **math + visualization**
- Trade-offs between **accuracy**, **smoothness**, and **speed**

---

## 🛠️ How to Compile & Run

**Tested on Windows using MSYS2 (MINGW64)**

### Compilation:
```bash
g++ Q2_OpenMP.cpp -o Q2_OpenMP -fopenmp -lfreeglut -lopengl32 -lglu32
```

### Run:
```bash
./Q2_OpenMP
```

---

## 📁 File Structure

```
openmp-circle-drawing/
├── Q2_OpenMP.cpp        # Main C++ source code
├── README.md            # This file
```

## 🔗 Author & Credits

Made by Ayaan Khan as part of the Parallel & Distributed Computing Assignment  
FAST-NUCES — Spring 2025


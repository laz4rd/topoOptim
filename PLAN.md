# Topology Optimizer

A C17 topology optimization tool with a 3D interactive visualization.

The goal is to take a solid structure, understand how forces travel through
it, and iteratively remove material that contributes the least to structural
performance.

---

## 01 — Project Goal

Build a topology optimization engine capable of:

- Defining a 3D rectangular design domain
- Defining material properties
- Defining loads and boundary conditions
- Generating a finite-element mesh
- Solving the structural system using FEM
- Running SIMP topology optimization
- Visualizing material density in 3D
- Showing the optimization process iteration-by-iteration
- Exporting the final optimized geometry

### Core Optimization Objective

Minimize structural compliance while satisfying a target material
volume fraction.

In simple terms:

> Remove as much material as possible while maintaining structural
> stiffness.

---

# 02 — Architecture

The project is split into independent systems:

```text
                    USER INPUT
                        │
                        ▼
                 ┌──────────────┐
                 │   DOMAIN     │
                 │   MATERIAL   │
                 │ LOADS/SUPPORT│
                 └──────┬───────┘
                        │
                        ▼
                 ┌──────────────┐
                 │     MESH     │
                 │   GENERATOR  │
                 └──────┬───────┘
                        │
                        ▼
                 ┌──────────────┐
                 │     FEM      │
                 │    SOLVER    │
                 └──────┬───────┘
                        │
                        ▼
                 ┌──────────────┐
                 │     SIMP     │
                 │ OPTIMIZATION │
                 └──────┬───────┘
                        │
                  density update
                        │
                        └──────────┐
                                   │
                                   ▼
                              next iteration


                    SIMULATION DATA
                          │
                          ▼
                 ┌─────────────────┐
                 │  3D VISUALIZER  │
                 │     OpenGL      │
                 └─────────────────┘

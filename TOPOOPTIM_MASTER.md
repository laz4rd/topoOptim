# topoOptim --- Project Master Plan

> **Project:** `topoOptim`\
> **Version:** `0.1.0` (early development)\
> **Language:** C17\
> **Platform:** Arch Linux / Omarchy\
> **Graphics API:** OpenGL 3.3 Core\
> **Windowing/input:** GLFW\
> **OpenGL loader:** GLAD 2\
> **Project goal:** Build a 3D topology-optimization visualizer that can
> optimize a solid metal block to use less material while maintaining
> specified structural performance.

------------------------------------------------------------------------

## 1. Project Goal

`topoOptim` is intended to be an engineering visualization application
for **3D structural topology optimization**.

The user should eventually be able to:

1.  Define a metal block's dimensions: length, width, and height.
2.  Specify material properties, such as Young's modulus and Poisson's
    ratio.
3.  Define supports and applied loads.
4.  Set a target material/volume fraction (for example, retain 40% of
    the original design volume).
5.  Run a topology-optimization algorithm.
6.  Watch the material distribution update in a 3D viewport as the
    algorithm iterates.
7.  Inspect the resulting structure and, eventually, export the
    optimized geometry.

The engineering objective should be described precisely as:

> **Minimize structural compliance subject to a prescribed volume
> fraction and mechanical boundary conditions.**

This is more precise than saying "keep exactly the same strength." In a
typical compliance-minimization setup, the solver seeks a stiff
structure under the specified loads and supports while limiting the
amount of material. Results depend on the model, loads, constraints,
mesh, and material assumptions.

### Intended algorithm

The planned method is **SIMP** (Solid Isotropic Material with
Penalization), supported by a finite-element method (FEM) solver.

At a high level:

1.  Create a mesh of the design domain.
2.  Assign an initial material density to each element.
3.  Solve the FEM system for displacement under the applied loads and
    supports.
4.  Calculate structural compliance and element sensitivities.
5.  Update element densities while respecting the volume constraint.
6.  Repeat until the design converges or reaches the iteration limit.
7.  Visualize the final density distribution.

------------------------------------------------------------------------

## 2. Current Status

### Confirmed working

-   [x] C project initialized.
-   [x] CMake build system created.
-   [x] C17 selected.
-   [x] GLFW installed and used to create a 1280 × 720 window.
-   [x] GLAD 2 generated for OpenGL 3.3 Core.
-   [x] OpenGL context initialized.
-   [x] Renderer separated from `main.c`.
-   [x] Vertex and fragment shader files created.
-   [x] Shader-loading, compilation, and linking module implemented.
-   [x] VBO/VAO created for the initial triangle.
-   [x] Triangle renders successfully.
-   [x] Depth testing added after successful GLAD initialization.
-   [x] Application confirmed working with NVIDIA PRIME offload.

### Current graphics/runtime notes

The machine has hybrid AMD/NVIDIA graphics. The application previously
selected the AMD renderer by default, while this command selected the
NVIDIA GPU successfully:

``` bash
__NV_PRIME_RENDER_OFFLOAD=1 __GLX_VENDOR_LIBRARY_NAME=nvidia ./build/topo
```

Keep this as a per-application launch option for now; do not force the
entire desktop to use the NVIDIA GPU.

**OpenGL initialization order matters:**

1.  Initialize GLFW.
2.  Create the GLFW window.
3.  Make the OpenGL context current.
4.  Initialize GLAD using `gladLoadGL`.
5.  Call OpenGL functions such as `glEnable`.

Calling OpenGL functions through GLAD before GLAD has loaded the
function pointers can cause a crash.

### Work that has been started or discussed but is not yet confirmed complete

-   [ ] Replacing the triangle's vertex array with cube geometry.
-   [ ] Updating the vertex shader to use `model`, `view`, and
    `projection` matrices.
-   [ ] Creating the custom `math3d` module (`math3d.h` and `math3d.c`).
-   [ ] Implementing perspective projection, camera view, and model
    rotation.
-   [ ] Rendering a cube successfully with a perspective camera.

Treat these as pending until they have been built and visually verified.

------------------------------------------------------------------------

## 3. Current Project Structure

The project has been developed toward this structure:

``` text
topoOptim/
├── CMakeLists.txt
├── PLAN.md
├── README.md
├── .gitignore
├── glad/
│   ├── include/
│   │   ├── glad/gl.h
│   │   └── KHR/khrplatform.h
│   └── src/gl.c
├── include/
│   └── graphics/
│       ├── renderer.h
│       ├── shader.h
│       └── math3d.h          # planned / in progress
├── shaders/
│   ├── basic.vert
│   └── basic.frag
├── src/
│   ├── main.c
│   └── graphics/
│       ├── renderer.c
│       ├── shader.c
│       └── math3d.c          # planned / in progress
└── tests/
```

The exact filesystem may temporarily differ while the cube and math
module are being implemented.

### Module responsibilities

  -----------------------------------------------------------------------
  Module                              Responsibility
  ----------------------------------- -----------------------------------
  `main.c`                            Initialize GLFW/OpenGL, create the
                                      window, run the event/render loop,
                                      and clean up.

  `renderer.c`                        Own rendering resources and issue
                                      draw calls.

  `renderer.h`                        Expose the renderer's public
                                      interface.

  `shader.c`                          Read shader files, compile shaders,
                                      link shader programs, and report
                                      errors.

  `shader.h`                          Expose shader functions.

  `basic.vert`                        Transform vertex positions.

  `basic.frag`                        Choose the fragment/pixel colour.

  `math3d.c/.h`                       Provide the matrix/vector
                                      operations needed for model
                                      transforms and camera projection.

  `glad/`                             Load OpenGL functions at runtime.
  -----------------------------------------------------------------------

------------------------------------------------------------------------

## 4. Dependencies and Build Environment

### Current dependencies

-   **C compiler** supporting C17
-   **CMake**
-   **GLFW**
-   **OpenGL implementation/driver**
-   **GLAD 2 generated source and headers**

The GLAD package available on this Arch setup is a generator. The
generated C source and headers are stored inside the project, so CMake
compiles `glad/src/gl.c` directly.

### cglm note

An attempt to find `cglm` using `pacman -Ss cglm` returned no results in
the current package database. The project therefore plans to begin with
a small in-house `math3d` module rather than block progress on
installing a math library. A library can be reconsidered later.

### Build

From the project root:

``` bash
cmake -S . -B build
cmake --build build
```

### Run using the NVIDIA GPU

``` bash
__NV_PRIME_RENDER_OFFLOAD=1 __GLX_VENDOR_LIBRARY_NAME=nvidia ./build/topo
```

### Git hygiene

The `build/` directory should remain untracked. The current `.gitignore`
includes build output, CMake-generated files, editor folders, and
`.DS_Store`.

------------------------------------------------------------------------

## 5. Development Roadmap

## Phase 0 --- Project foundation

**Status: Complete**

-   [x] Create repository and C source layout.
-   [x] Initialize Git and CMake.
-   [x] Establish a C17 executable.
-   [x] Add `.gitignore`.
-   [x] Build and run a basic console program.

**Milestone:** CMake successfully builds `topo`.

## Phase 1 --- OpenGL foundation

**Status: Complete**

-   [x] Install GLFW and required OpenGL packages.
-   [x] Generate GLAD 2 for OpenGL 3.3 Core.
-   [x] Create the GLFW window.
-   [x] Load OpenGL functions through GLAD.
-   [x] Separate renderer and shader modules.
-   [x] Compile and link vertex/fragment shaders.
-   [x] Clear the screen each frame.
-   [x] Create a VAO and VBO.
-   [x] Render a triangle.
-   [x] Enable depth testing after GLAD initialization.

**Milestone:** A working OpenGL window renders a triangle.

## Phase 2 --- 3D rendering foundation

**Status: In progress**

-   [ ] Replace the triangle with cube geometry.
-   [ ] Finish and verify the `math3d` module.
-   [ ] Implement a 4 × 4 identity matrix.
-   [ ] Implement model rotation.
-   [ ] Implement a look-at camera matrix.
-   [ ] Implement perspective projection.
-   [ ] Upload the `model`, `view`, and `projection` uniforms to the
    shader.
-   [ ] Verify depth-buffer clearing every frame.
-   [ ] Render a cube in perspective.
-   [ ] Add basic keyboard/mouse camera controls.
-   [ ] Handle framebuffer resizing and viewport updates.

**Milestone:** A perspective 3D cube is visible and can be inspected
interactively.

## Phase 3 --- Domain and mesh

**Status: Pending**

-   [ ] Add user-configurable block length, width, and height.
-   [ ] Define a consistent unit system.
-   [ ] Create a structured 3D element grid.
-   [ ] Represent nodes, elements, connectivity, and element densities
    in C data structures.
-   [ ] Render the block and mesh in the viewport.
-   [ ] Add mesh-resolution controls.
-   [ ] Validate coordinate conventions and indexing.

**Milestone:** The application displays a configurable block divided
into finite elements.

## Phase 4 --- Mechanical model

**Status: Pending**

-   [ ] Define material properties, including Young's modulus and
    Poisson's ratio.
-   [ ] Define loads and support/boundary conditions.
-   [ ] Allow the user to choose or configure load and support
    locations.
-   [ ] Establish the finite-element formulation and element stiffness
    calculation.
-   [ ] Assemble the global stiffness system.
-   [ ] Apply boundary conditions correctly.
-   [ ] Solve for nodal displacements.
-   [ ] Calculate compliance and check basic equilibrium/solver
    behavior.
-   [ ] Add small test cases with known or independently verified
    results.

**Milestone:** A simple structural model produces plausible, validated
displacement results.

## Phase 5 --- Topology optimization (SIMP)

**Status: Pending**

-   [ ] Initialize element densities.
-   [ ] Implement the density-dependent material interpolation used by
    SIMP.
-   [ ] Solve the FEM system for each iteration.
-   [ ] Calculate compliance and sensitivities.
-   [ ] Implement a density update method subject to the volume
    constraint.
-   [ ] Add density filtering or another appropriate stabilization
    method.
-   [ ] Track compliance, volume fraction, and iteration count.
-   [ ] Add convergence and maximum-iteration conditions.
-   [ ] Validate against standard benchmark cases, starting with a
    cantilever beam.

**Milestone:** The solver produces a repeatable optimized density field
for a benchmark problem.

## Phase 6 --- Optimization visualization

**Status: Pending**

-   [ ] Map element densities to a clear visual representation.
-   [ ] Display the evolving density field during optimization.
-   [ ] Add iteration controls (start, pause, step, reset).
-   [ ] Display iteration number, compliance, and current volume
    fraction.
-   [ ] Distinguish loads and supports visually.
-   [ ] Add wireframe/solid and mesh display modes.
-   [ ] Keep rendering separate from the numerical solver.

**Milestone:** The user can watch material distribution evolve during
the optimization process.

## Phase 7 --- User interface and usability

**Status: Pending**

-   [ ] Add a proper UI for geometry, material, load, support, and
    optimization settings.
-   [ ] Validate user input and report errors clearly.
-   [ ] Add progress and solver-status information.
-   [ ] Save/load project configurations.
-   [ ] Improve camera navigation and viewport interaction.

Dear ImGui is a possible future UI choice, but it is not required for
the current rendering milestone.

## Phase 8 --- Export and polish

**Status: Pending**

-   [ ] Export optimized results or geometry, with STL as an initial
    candidate.
-   [ ] Document assumptions, units, limitations, and solver settings.
-   [ ] Add automated tests for math, mesh, FEM, and optimization
    components.
-   [ ] Profile CPU and GPU performance.
-   [ ] Improve numerical stability and error handling.
-   [ ] Prepare examples and a clear README.

------------------------------------------------------------------------

## 6. Proposed Architecture

Keep the numerical solver independent of the renderer. The renderer
should visualize solver state; it should not contain FEM or optimization
logic.

``` text
User configuration
       |
       v
Geometry + material + loads + supports
       |
       v
3D mesh and element data
       |
       v
Finite-element solver (FEM)
       |
       v
Compliance + sensitivity analysis
       |
       v
SIMP density update
       |
       +------> Repeat until convergence
       |
       v
Final element density field
       |
       v
OpenGL visualization / export
```

A likely future separation is:

``` text
src/
├── main.c
├── graphics/
│   ├── renderer.c
│   ├── shader.c
│   └── math3d.c
├── mesh/
├── fem/
├── optimization/
└── io/
```

Introduce these modules when their responsibilities become concrete
rather than creating empty abstractions prematurely.

------------------------------------------------------------------------

## 7. Engineering Decisions and Guardrails

1.  **Validate before scaling up.** Start with a simple benchmark, such
    as a 2D cantilever for the numerical algorithm, even though the
    eventual application is 3D.
2.  **Keep the 3D viewport as a separate subsystem.** The optimizer
    should be testable without opening a window.
3.  **Use precise engineering language.** State the load cases, boundary
    conditions, material model, objective, and volume constraint; do not
    promise identical strength for every possible load.
4.  **Make coordinate conventions explicit.** Document matrix layout,
    multiplication order, handedness, units, and camera conventions.
5.  **Check return values and errors.** Shader compilation, shader
    linking, file reading, allocation, window creation, and solver
    operations should fail with useful diagnostics.
6.  **Avoid premature complexity.** First make the cube render, then
    make the block and mesh, then implement and validate the numerical
    solver.
7.  **Do not assume a GPU makes FEM fast automatically.** The first
    solver can run on the CPU; optimize performance after correctness is
    established.
8.  **Use version control at each milestone.** Commit known-working
    states before adding major features.

------------------------------------------------------------------------

## 8. Definition of the First Complete MVP

The first meaningful MVP should let a user:

-   Set block dimensions.
-   Set a mesh resolution.
-   Specify at least one load case and support configuration.
-   Set material properties and a target volume fraction.
-   Run a validated SIMP-based compliance-minimization loop.
-   Watch the 3D density field update.
-   Inspect the final result and see basic solver metrics.

Export, advanced meshing, multiple load cases, sophisticated materials,
and polished UI can follow after this core workflow works reliably.

------------------------------------------------------------------------

## 9. Immediate Next Actions

Work on these in order:

1.  Confirm the current `renderer.c` cube vertex data is present and
    contains the intended 36 vertices.
2.  Create `include/graphics/math3d.h` and `src/graphics/math3d.c` if
    they do not exist yet.
3.  Add `src/graphics/math3d.c` to the `topo` executable in
    `CMakeLists.txt`.
4.  Build and fix any compilation errors.
5.  Implement perspective, look-at, and rotation matrices.
6.  Update the vertex shader and set its matrix uniforms.
7.  Render and verify the cube before adding camera controls.

**Current immediate goal:** a working, perspective-rendered 3D cube.\
**Ultimate project goal:** a validated 3D topology-optimization tool
written primarily in C, using FEM + SIMP and an interactive OpenGL
visualization.

------------------------------------------------------------------------

## 10. Current Build/Run Checklist

From the project root:

``` bash
cmake -S . -B build
cmake --build build
__NV_PRIME_RENDER_OFFLOAD=1 __GLX_VENDOR_LIBRARY_NAME=nvidia ./build/topo
```

If the application crashes:

1.  Check that `glfwMakeContextCurrent(window)` happens before
    `gladLoadGL(...)`.

2.  Check that all OpenGL calls happen only after `gladLoadGL(...)`
    succeeds.

3.  Rebuild with debugging symbols:

    ``` bash
    cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
    cmake --build build
    ```

4.  Run through GDB and inspect the backtrace:

    ``` bash
    gdb --args ./build/topo
    ```

    In GDB:

    ``` gdb
    set environment __NV_PRIME_RENDER_OFFLOAD 1
    set environment __GLX_VENDOR_LIBRARY_NAME nvidia
    run
    bt
    ```

------------------------------------------------------------------------

*Keep this file updated as milestones are completed. Mark an item
complete only after it has been built or tested, not merely planned.*

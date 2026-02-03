## Optimization analysis

### Runtime measurement per stage 

a new class `Timer` (file `timer.h`) is used to wrap a `chrono` clock to function as a stopwatch with methods `start()` and `stop()` to measure time in code stages. If a code block is executed multiple times, the timer accumulates and compute the average duration for each run. 

Four stages are investigated: build tree, compute force, position + velocity update, and write results. 

Prior to optimization (commit hash `e9f0ef8dae292459538e57a65b8a18d52e6f6301`)
```
Simulation BH Classic Gravity,   202 bodies 

Stage: Build Tree     Samples: 5000,     Mean Duration: 0.201381 ms 
Stage: Compute Force  Samples: 5000,     Mean Duration: 1.707795 ms 
Stage: Write CSV      Samples: 5000,     Mean Duration: 0.226476 ms 
Stage: P + V Update   Samples: 5000,     Mean Duration: 0.007064 ms 
```

We identified bottlenecks in the stages Compute Force and Write CSV. 

### Optimization 1: Reduce expensive calls in Compute Force + inlining small frequent functions
(commit hash `650842b7560b87e32ddf67ed7d1a883cd9fd0607`)

For Compute Force, initial optimization was done by removing the expensive `sqrt()` calls as much as possible. Memory access like `array[i]` is also reduced.  

Small frequent function calls like vector addition and multiplication is inlined.

```
Simulation BH Classic Gravity,   202 bodies 

Stage: Build Tree     Samples: 5000,     Mean Duration: 0.198227 ms 
Stage: Compute Force  Samples: 5000,     Mean Duration: 1.369512 ms  <--   -0.34ms
Stage: Write CSV      Samples: 5000,     Mean Duration: 0.226993 ms 
Stage: P + V Update   Samples: 5000,     Mean Duration: 0.007046 ms 
```

### Optimization 2: Multithreading for force computation
(commit hash `4d42321fcfab4e86b03d945f4b09eb356eb443b0`)

Since force computation does not change the state of the tree and can be computed concurrently for each object, ` #pragma omp parallel` is used to enable multithreading in force computation. 

```
Simulation BH Classic Gravity,   202 bodies 

Stage: Build Tree      Samples: 5000,     Mean Duration: 0.301930 ms 
Stage: Compute Force   Samples: 5000,     Mean Duration: 0.301688 ms  <- -1.06 ms
Stage: Write CSV       Samples: 5000,     Mean Duration: 0.406467 ms 
Stage: P + V Update    Samples: 5000,     Mean Duration: 0.002008 ms 
```

### Optimization 3: Vectorization for position & velocity updates
(commit hash `4d42321fcfab4e86b03d945f4b09eb356eb443b0`)

The `Vec3` operations `+` and `*` is unrolled in the update of body position and velocity. Then `#pragma omp simd` is used to enable vectorized simd execution. 

```
Simulation BH Classic Gravity,   202 bodies 

Stage: Build Tree      Samples: 5000,     Mean Duration: 0.301930 ms 
Stage: Compute Force   Samples: 5000,     Mean Duration: 0.301688 ms  
Stage: Write CSV       Samples: 5000,     Mean Duration: 0.406467 ms 
Stage: P + V Update    Samples: 5000,     Mean Duration: 0.002008 ms <-- -0.005 ms
```


### Optimization 4: Get rid of expensive CSV formatting

(Commit hash `6b46193b9fa3ecc8a632e7624a1af58455b21694`)

`fprintf` into a file is an expensive call due to formatting. This is especially expensive because every object at every timestep is a CSV line that has to be written.  Instead of writing CSV, we directly write the result in binary form. 

```
Simulation BH Classic Gravity,   202 bodies 

Stage: Build Tree     Samples: 5000,     Mean Duration: 0.304438 ms 
Stage: Compute Force  Samples: 5000,     Mean Duration: 0.271902 ms 
Stage: Write Results  Samples: 5000,     Mean Duration: 0.055521 ms  <-- -0.35 ms
Stage: P + V Update   Samples: 5000,     Mean Duration: 0.002009 ms 
```
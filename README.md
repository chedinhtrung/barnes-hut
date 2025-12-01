# Barnes-Hut Simulation

## Building and running the simulation

//TODO for Zed: define concretelyy

**Requirements**
- gcc/g++ build tool
- Make & CMake
- python >= 3.10

**Building**

Clone the repo. In the project root directory run:
```
mkdir build && cd build && cmake .. && make
```

**Run the simulation**

Run the simulation with:
```
./simulation
```

**Visualize**

Install the requirements with
```
pip install -r requirements.txt
```
Then run the notebook 
`visualize.ipynb`

## N-Body Simulation

An `N-body simulation` is a way to model how N bodies interact with each other through forces. In this case, we want to approximate effect of `gravity` in the formation of gas clouds and clustering of stars over long periods of time. This will be done by simulating the `attractive forces between thousands of particles`.

Given two bodies with the mass of $m_1$ and $m_2$, the `gravitational attraction` between them is going to be: 

$$
F_g = \frac{Gm_1m_2}{r^2}
$$

with `G` being the universal gravitational constant and `r` being the distance between the center of masses of the two bodies. 

If we have `N` bodies, in every frame, we will need to apply the forces on each body based on all the other bodies, meaning $O(N^2)$ calculations per frame. This is way too many calculations, so we need a faster algorithm for simulating the gravity between N bodies.  

## Barnes-Hut Algorithm

This faster algorithm is called the `Barnes-Hut algorithm`. The following explanation is based on the work of `Tom Ventimiglia` and `Kevin Wayne`: https://arborjs.org/docs/barnes-hut. 

The crucial idea in speeding up the brute force N-body algorithm is to `group nearby bodies` and `approximate them as a single body`. Basically, if the group is `sufficiently far away` from the planet that we are looking at, we can approximate its gratitational effects by using its `center of mass`. Formally, for a system of $N$ bodies with masses $m_i$ located at $r_i$, the total mass $m$ and the coordinate of the center of mass $r_G$ are given by:

$$
m = \sum_{i=1}^N m_i
$$

$$
r_G = \frac{ \sum_{i=1}^N m_i r_i}{\sum_{i=1}^N m_i}
$$

The `Barnes-Hut algorithm` is a clever scheme for `grouping together bodies that are sufficiently nearby`. It recursively divides the set of bodies into groups by storing them into an `Octree`. The `topmost node` represents the whole space, and its `eight children` represent the `eight quadrants of the space`. The space is recursively subdivided until `each subdivision contains 0 or 1 bodies`. 

Each `external node (leaf node)` represents `a single body`. Each `internal node` represents `the group of bodies beneath it`, and stores the `center of mass` and the `total mass of all its children bodies`. 

To calculate the net force on a particular body, traverse the nodes of the tree, starting from the root. If the center of mass of an internal node is sufficiently far from the body, approximate the bodies contained in that part of the tree as `a single body`, whose position is the `group's center of mass` and whose mass is the `group's total mass`. The Barnes-Hut algorithm is fast because we do not need to individually examine any of the bodies in the group.

If the internal node is not sufficiently far from the body, `recursively traverse each of its subtrees`. To determine if a node is sufficiently far away, compute the quotient $\frac{s}{d}$, where `s` is the width of the region represented by the internal node, and `d` is the distance between the body and the node's center of mass. Then, compare this ratio again a threshold value θ. If $\frac{s}{d} < θ$, then the internal node is sufficiently far away. By adjusting the θ parameter, we can change the speed and accuracy of the simulation. A commonly used value for θ in practice is `0.5`. 

### Constructing the Barnes-Hut Tree

To construct the Barnes-Hut tree, insert the bodies one after another. To insert a body `b` into the tree rooted at node `x`, use the following recursive procedure:

1. If node `x` does not contain a body, put the new body `b` here.

2. If node `x` is an internal node, update the center-of-mass and total mass of `x`. Recursively insert the body `b` in the appropriate quadrant.

3. If node `x` is an external node, say containing a body named `c`, then there are two bodies `b` and `c` in the same region. Subdivide the region further by creating eight children. Then, recursively insert both `b` and `c` into the appropriate quadrant(s). Since `b` and `c` may still end up in the same quadrant, there may be several subdivisions during a single insertion. Finally, update the center-of-mass and total mass of `x`.

### Calculating the Force Acting on a Body

To calculate the net force acting on body `b`, use the following recursive procedure, starting with the root of the quad-tree:

1. If the current node is an external node (and it is not body `b`), calculate the force exerted by the current node on `b`, and add this amount to b’s net force.

2. Otherwise, calculate the ratio $\frac{s}{d}$. If $\frac{s}{d} < θ$, treat this internal node as a single body, and calculate the force it exerts on body `b`, and add this amount to `b`’s net force.

3. Otherwise, run the procedure recursively on each of the current node’s children.


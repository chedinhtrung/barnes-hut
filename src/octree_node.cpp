#include "octree_node.hpp"
#include <algorithm>
#include <limits>
#include <cmath>
#include <iostream>

void OctreeNode::insert(Body* b) {
    /*
    Case 1: Leaf node that is currently empty
    */
    if (isLeaf() && body == nullptr) {
        // Store the body in this node
        body = b;

        // For a single body: Mass = body mass, center of mass = body position
        mass = b->mass;
        centerOfMass = b->position;
        return;
    }

    /*
    Case 2: Node is a leaf, but already has one body

    We subdivide this region into 8 children and move bodies downwards into the correct child octants
    */
    if (isLeaf() && body != nullptr) {
        // Save the existing body
        Body* oldBody = body;

        // This node will become an internal node, so it no longer directly stores a body pointer
        body = nullptr;

        // 1. Create 8 children, one for each octant
        for (int i = 0; i < 8; ++i) {
            Region childReg = bounds.childRegion(i);
            children[i] = new OctreeNode(childReg);
        }

        // 2. Insert the old body into the appropriate child
        int oldIndex = getChildIndex(bounds, oldBody->position);
        children[oldIndex]->insert(oldBody);

        // 3. Insert the new body into the appropriate child
        int newIndex = getChildIndex(bounds, b->position);
        children[newIndex]->insert(b);

        // 4. Update the mass and center of mass of this node based on the children
        mass = 0.0;
        centerOfMass = Vec3{0.0, 0.0, 0.0};

        for (int i = 0; i < 8; ++i) {
            if (children[i] != nullptr && children[i]->mass > 0.0) {
                mass += children[i]->mass;
                centerOfMass += children[i]->centerOfMass * children[i]->mass;
            }
        }

        if (mass > 0.0) {
            centerOfMass *= (1.0 / mass);
        }

        return;
    }

    /*
    Case 3: Node is an internal node (has children)

    We have already subdivided this node before, so we simply choose the correct child and recurse
    */
    {
        // Determine which child should receive this body
        int idx = getChildIndex(bounds, b->position);

        // If that child doesn't exist yet, create it
        if (children[idx] == nullptr) {
            Region childReg = bounds.childRegion(idx);
            children[idx] = new OctreeNode(childReg);
        }

        // Insert into the child node recursively
        children[idx]->insert(b);

        // After inserting into a child, update this node's mass & center of mass
        mass = 0.0;
        centerOfMass = Vec3{0.0, 0.0, 0.0};

        for (int i = 0; i < 8; ++i) {
            OctreeNode* child_i = children[i];
            if (child_i != nullptr && child_i->mass > 0.0) {
                mass += child_i->mass;
                centerOfMass += child_i->centerOfMass * child_i->mass;
            }
        }

        if (mass > 0.0) {
            centerOfMass *= (1.0 / mass);
        }

        return;
    }
}


OctreeNode::~OctreeNode(){
    // recursively delete all the nodes in the tree (dont delete Bodies!)
    for (OctreeNode* child:children){
        delete(child);
    }
}

Region computeRootRegion(const std::vector<Body>& bodies) {
    double minX =  std::numeric_limits<double>::infinity();
    double minY =  std::numeric_limits<double>::infinity();
    double minZ =  std::numeric_limits<double>::infinity();
    double maxX = -std::numeric_limits<double>::infinity();
    double maxY = -std::numeric_limits<double>::infinity();
    double maxZ = -std::numeric_limits<double>::infinity();

    // 1. Find the min & max in each dimension
    for (const Body& body : bodies) {
        const Vec3& p = body.position;

        if (p.x < minX) minX = p.x;
        if (p.x > maxX) maxX = p.x;

        if (p.y < minY) minY = p.y;
        if (p.y > maxY) maxY = p.y;

        if (p.z < minZ) minZ = p.z;
        if (p.z > maxZ) maxZ = p.z;
    }

    // 2. Compute box center
    double cx = 0.5 * (minX + maxX);
    double cy = 0.5 * (minY + maxY);
    double cz = 0.5 * (minZ + maxZ);

    Vec3 center{cx, cy, cz};

    // 3. Compute the maximum span in all directions
    double spanX = maxX - minX;
    double spanY = maxY - minY;
    double spanZ = maxZ - minZ;

    double maxSpan = std::max({spanX, spanY, spanZ});

    // Avoid zero-size region when all bodies are at the same position
    if (maxSpan == 0.0) {
        maxSpan = 1.0;
    }

    double halfSize = 0.5 * maxSpan;

    return Region{center, halfSize};
}

/*
Decide which child octant 0 -> 7 of the region contains the given point

We use the following convention for the 8 child cubes:

        ^ z
        |      
        4-----6
       /|    /|
      5-----7 |
      | 0---|-2 --> y 
      |/    |/
      1-----3               
     /
     x

Start bottom left back. binary coords zyx. move in that direction -> that coord is 1. For example, 111_bin = 7_dec
*/
int getChildIndex(const Region& region, const Vec3& point) {
    bool dx = (point.x >= region.center.x);
    bool dy   = (point.y >= region.center.y);
    bool dz = (point.z >= region.center.z);

    int index = 0;
    index |= (dz << 2); 
    index |= (dy << 1);
    index |= dx;
    return index;
    
}

OctreeNode* buildOctree(std::vector<Body>& bodies) {
    if (bodies.empty()) {
        return nullptr;
    }

    // 1. Compute bounding region of all bodies
    Region rootRegion = computeRootRegion(bodies);

    // 2. Create root node
    OctreeNode* root = new OctreeNode(rootRegion);

    // 3. Insert each body into the octree
    for (Body& body: bodies) {
        root->insert(&body);
    }

    // 4. Return the root
    return root;
}

void computeForceFromNode(const OctreeNode* node, Body& b, double theta, ForceField* forcefield) {
    if (!node || !forcefield) return;

    // Only care about gravity in this run, avoid using switch

    const double m = node->mass;
    if (m <= 0.0) return;

    // Vector from node's center of mass to body
    const Vec3 r = b.position - node->centerOfMass;

    // Calculate distance from node to body b
    const double softening = 1e-5; // Avoid division by 0 
    double dist2 = norm2(r) + softening * softening;

    // Avoid division by 0 if node is in the exact same position as b
    if (dist2 == 0.0) {
        return;
    }

    /*
    Case 1: Node is leaf node

    Calculate the force exerted by the current node on 'b' and add this amount to b’s net force
    */
    if (node->isLeaf()) {
        const Body* other = node->body;
        // If this leaf's body is exactly the same as 'b', skip it because a body does not exert force on itself
        if (!other || other == &b) return; 

        // Treat the node as a single body at centerOfMass with mass = node->mass & compute gravitational force in vector form: fVec = (G * m1 * m2 / |r|^3) * r
        
        // Compute force
        switch (forcefield->type){
            case GRAVITY:
                b.force += forcefield->gravity(b.mass, node->mass, r);
        }
        return;
    }

    /*
    Case 2: Node is internal node

    Calculate the ratio s / d. If s / d < theta, treat this internal node as a single body, and calculate the force it exerts on body 'b', 
    and add this amount to b’s net force.
    */
    double s = 2.0 * node->bounds.halfSize;
    if (s*s < theta*theta*dist2) {
        // Treat the whole cell as one body at its center of mass
        // Compute force
        switch (forcefield->type){
            case GRAVITY:
                b.force += forcefield->gravity(b.mass, node->mass, r);
        }

        return;
    }

    // If s / d > theta, recurse to children
    for (int i = 0; i < 8; ++i) {
        const OctreeNode* child = node->children[i];
        if (child) computeForceFromNode(child, b, theta, forcefield);
    }
}
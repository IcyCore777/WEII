// Supported with union (c) 2020 Union team

// User API for zCMesh
// Add your methods here

zCVertex* Vertex(int i) const { _ASSERT(0 <= i && i < numVert&& vertList); return vertList[i]; }
zCPolygon* Poly(int i) const { _ASSERT(0 <= i && i < numPoly&& polyList); return polyList[i]; }
zCVertFeature* Feature(int i) const { _ASSERT(0 <= i && i < numFeat&& featList); return featList[i]; }
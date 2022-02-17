// Supported with union (c) 2020 Union team

// User API for zCPolygon
// Add your methods here

zCMaterial*						GetMaterial() const { return material; };
const zVEC3&					GetNormal() const { return polyPlane.normal; };
zBOOL							GetSectorFlag() const { return flags.sectorPoly; };
void							SetPlane(const zTPlane& plane) { polyPlane = plane; };
zCVertex* __fastcall			GetVertexPtr(int index)
{
	return vertex[index];
}
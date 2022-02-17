// Supported with union (c) 2020 Union team

// User API for zVEC3
// Add your methods here

zBOOL IsEqualEps(const zVEC3& b) const {								// fuzzy compare with small epsilon
	return ((fabs(zREAL(n[0] - b.n[0])) < zREAL(0.001)) &&
		(fabs(zREAL(n[1] - b.n[1])) < zREAL(0.001)) &&
		(fabs(zREAL(n[2] - b.n[2])) < zREAL(0.001)));
};
zSTRING GetString() const { return zSTRING(" (" + zSTRING(n[0]) + ", " + zSTRING(n[1]) + ", " + zSTRING(n[2]) + ") "); };
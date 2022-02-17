// Supported with union (c) 2020 Union team

// User API for zCWorld
// Add your methods here

zCWorld*                zCWorld_Union();

void	                SetProgressBar(zCViewProgressBar* progressBar) { this->progressBar = progressBar; };
void					SearchVobByVisual(zSTRING&, zCTree<zCVob>* vobNode);
zCWayNet*				GetWayNet() const { return wayNet; };
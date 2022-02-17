// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
#define TV_PFX 5
#define TV_OBJ 3

	HTREEITEM InsertTreeFolder(LPSTR str, HTREEITEM parent)
	{
		TVINSERTSTRUCT  lpItem;
		TVITEMEX tvi;
		tvi.mask = TVIF_TEXT | TVIF_IMAGE
			| TVIF_SELECTEDIMAGE | TVIF_PARAM | TVIF_EXPANDEDIMAGE;
		tvi.pszText = str;
		tvi.cchTextMax = sizeof(tvi.pszText) / sizeof(tvi.pszText[0]);

		tvi.iImage = TV_IT;
		tvi.iSelectedImage = TV_ITSEL;
		tvi.iExpandedImage = TV_ITSEL;
		tvi.hwnd = FILEVIEW.m_hWnd;

		lpItem.itemex = tvi;

		lpItem.hParent = parent;
		return FILEVIEW.InsertItem(&lpItem);
	}


	Common::string GetVobSectorName(zCVob* vob)
	{
		zSTRING sectorName = vob->GetSectorNameVobIsIn();
		if (sectorName.Length())
			return sectorName;

		return "GLOBAL";
	}


	VirtualVobTree::VirtualVobTree()
	{
		TreeNode = nullptr;
	}

	VirtualVobTree::VirtualVobTree(InterfaceTreeItem mfcItem, GothicTreeNode treeNode, Common::string reserved)
	{
		TreeNode = nullptr;
		Insert(mfcItem, treeNode);
	}

	void VirtualVobTree::Insert(InterfaceTreeItem mfcItem, GothicTreeNode treeNode)
	{
		TreeNode = treeNode;

		zCVob* vob = treeNode->GetData();
		if (dynamic_cast<zCParticleFX*>(vob->visual))
			MFCTreeItem = FILEVIEW.InsertItem(zSTRING(dynamic_cast<zCParticleFX*>(vob->visual)->emitter->particleFXName + zSTRING(".pfx")).ToChar(), TV_PFX, TV_PFX, mfcItem);
		else if (vob->visual)
			MFCTreeItem = FILEVIEW.InsertItem(vob->visual->GetVisualName().ToChar(), TV_OBJ, TV_OBJ, mfcItem);
		else
			MFCTreeItem = FILEVIEW.InsertItem(vob->objectName.Length() ? vob->objectName.ToChar() : vob->GetClassName_().ToChar(), TV_OBJ, TV_OBJ, mfcItem);

		GothicTreeNode childNode = treeNode->GetFirstChild();
		while (childNode)
		{
			VirtualVobTree* childVirtualTree = new VirtualVobTree(MFCTreeItem, childNode);
			ChildList.Insert(childVirtualTree);
			childNode = childNode->GetNextChild();
		}
	}

	void VirtualVobTree::InsertAsChild(GothicTreeNode treeNode)
	{
		VirtualVobTree* vobTree = new VirtualVobTree(MFCTreeItem, treeNode);
		ChildList.Insert(vobTree);
	}

	void VirtualVobTree::InsertAsChild(zCVob* vob)
	{
		GothicTreeNode tree = vob->globalVobTreeNode;
		InsertAsChild(tree);
	}

	inline bool CheckIntersection(GothicTreeNode treeNode, GothicTreeNode targetTreeNode)
	{
		GothicTreeNode targetNodeList = targetTreeNode->GetParent();
		while (targetNodeList)
		{
			if (treeNode == targetNodeList)
				return true;

			targetNodeList = targetNodeList->GetParent();
		}

		return false;
	}

	void VirtualVobTree::Remove(GothicTreeNode treeNode)
	{
		/* TODO
		 * Сделать перебор списка, удалить элемент из вобтри,
		 * Удалить все связанные с ним группы, а также папки.
		 */
		 //Log(zSTRING("2 " + AHEX32(treeNode)) );
		for (uint i = 0; i < ChildList.GetNum(); i++)
		{
			//Log(zSTRING("2.1 " + AHEX32(ChildList[i]->TreeNode)) );
			if (treeNode == ChildList[i]->TreeNode)
			{
				//Log("3" );
				ChildList.DeleteAt(i);
				break;
			}
			else if (CheckIntersection(treeNode->GetParent(), ChildList[i]->TreeNode))
				return Remove(treeNode->GetParent());
		}
	}

	void VirtualVobTree::Remove(InterfaceTreeItem treeNode)
	{
		// PASS
	}

	void VirtualVobTree::Remove(zCVob* vob)
	{
		GothicTreeNode tree = vob->globalVobTreeNode;
		//Log("1");
		Remove(tree);

	}

	VirtualVobTree* VirtualVobTree::FindNodeByInterfaceTreeItem(InterfaceTreeItem item)
	{
		if (MFCTreeItem == item)
			return this;

		for (uint i = 0; i < ChildList.GetNum(); i++)
		{
			VirtualVobTree* tree = ChildList[i]->FindNodeByInterfaceTreeItem(item);
			if (tree)
				return tree;
		}

		return nullptr;
	}

	VirtualVobTree* VirtualVobTree::FindParentVobTreeForVob(zCVob* vob)
	{
		auto parent = vob->globalVobTreeNode->GetParent();
		for (uint i = 0; i < ChildList.GetNum(); i++)
		{
			auto childTreeNode = ChildList[i]->TreeNode;
			if (childTreeNode == parent)
				return ChildList[i];

			if (CheckIntersection(childTreeNode, parent))
				return ChildList[i]->FindParentVobTreeForVob(vob);
		}

		return nullptr;
	}

	HTREEITEM VirtualVobTree::FindTreeItemForVob(zCVob* vob)
	{
		auto parent = vob->globalVobTreeNode;
		for (uint i = 0; i < ChildList.GetNum(); i++)
		{
			auto childTreeNode = ChildList[i]->TreeNode;
			if (childTreeNode == parent)
				return ChildList[i]->MFCTreeItem;

			if (CheckIntersection(childTreeNode, parent))
				return ChildList[i]->FindTreeItemForVob(vob);
		}

		return nullptr;
	}

	bool VirtualVobTree::IsIn(zCVob* vob)
	{
		auto parent = vob->globalVobTreeNode;
		for (uint i = 0; i < ChildList.GetNum(); i++)
		{
			auto childTreeNode = ChildList[i]->TreeNode;
			if (childTreeNode == parent)
				return true;

			if (CheckIntersection(childTreeNode, parent))
				return ChildList[i]->FindParentVobTreeForVob(vob);
		}

		return false;
	}

	void VirtualVobTree::DeleteChilds()
	{
		ChildList.DeleteData();
	}

	void VirtualVobTree::Clear()
	{
		ChildList.DeleteData();
		FILEVIEW.DeleteItem(MFCTreeItem);
	}

	VirtualVobTree::~VirtualVobTree()
	{
		Clear();
	}






	InterfaceTreeItem SectorList::GetClassTree(Common::string className, InterfaceTreeItem root)
	{
		for (uint i = 0; i < Classes.GetNum(); i++)
			if (Classes[i]->ClassName == className)
				return Classes[i]->TreeItem;

		ClassList*& list = Classes.Create();
		list = new ClassList();
		list->ClassName = className;
		list->TreeItem = InsertTreeFolder(className.ToChar(), root);//FILEVIEW.InsertItem(className, TV_IT, TV_ITSEL, root);
		return list->TreeItem;
	}





	VirtualVobTree_Portals::VirtualVobTree_Portals() : VirtualVobTree()
	{
		MFCTreeItem = InsertTreeFolder("PORTALS", NULL);//FILEVIEW.InsertItem(_T("PORTALS"), TV_IT, TV_ITSEL);

	}

	void VirtualVobTree_Portals::Insert(GothicTreeNode treeNode)
	{
		zCVob* vob = treeNode->GetData();
		InterfaceTreeItem rootItem = GetClassTreeItem(treeNode, vob->GetClassName_(), MFCTreeItem);
		VirtualVobTree* childVirtualTree = new VirtualVobTree(rootItem, treeNode);
		ChildList.Insert(childVirtualTree);
	}

	void VirtualVobTree_Portals::InsertAsChild(GothicTreeNode treeNode)
	{
		if (TreeNode == treeNode->GetParent())
			return Insert(treeNode);

		VirtualVobTree* node = FindParentVobTreeForVob(treeNode->GetData());
		if (!node)
			return Insert(treeNode);

		zCVob* vob = treeNode->GetData();
		node->InsertAsChild(treeNode);
	}

	void VirtualVobTree_Portals::InsertAsChild(zCVob* vob)
	{
		InsertAsChild(vob->globalVobTreeNode);
	}

	inline void DeletePortalsContentFromVobTree(GothicTreeNode vobTree, Common::string portalName, Common::string className = "")
	{
		GothicTreeNode childs = vobTree->GetFirstChild();
		while (childs)
		{
			zCVob* vob = childs->GetData();
			childs = childs->GetNextChild();
			if (vob->GetSectorNameVobIsIn() && *vob->GetSectorNameVobIsIn() == portalName.ToChar())
				if (className.IsEmpty() || className == vob->GetClassName_().ToChar())
					ogame->GetGameWorld()->RemoveVobSubtree(vob);
		}
	}

	void VirtualVobTree_Portals::DeleteClassInPortalContent(SectorList* portal, ClassList* classObj)
	{
		Common::string portalName = portal->PortalName;
		GothicTreeNode treeNode = &ogame->world->globalVobTree;
		GothicTreeNode childs = treeNode->GetFirstChild();
		Common::string& className = classObj->ClassName;
		while (childs)
		{
			DeletePortalsContentFromVobTree(childs, portalName, className);
			childs = childs->GetNextChild();
		}
	}

	void VirtualVobTree_Portals::DeletePortalContent(SectorList* portal)
	{
		Common::string portalName = portal->PortalName;
		GothicTreeNode treeNode = &ogame->world->globalVobTree;
		GothicTreeNode childs = treeNode->GetFirstChild();
		while (childs)
		{
			DeletePortalsContentFromVobTree(childs, portalName);
			childs = childs->GetNextChild();
		}
	}

	void VirtualVobTree_Portals::ClearCategories(SectorList* portal)
	{
		for (uint i = 0; i < portal->Classes.GetNum(); i++)
			if (portal->Classes[i]->IsPortalEmpty())
				portal->Classes.DeleteAt(i--);
	}

	void VirtualVobTree_Portals::Remove(zCVob* vob)
	{
		VirtualVobTree::Remove(vob);
	}

	void VirtualVobTree_Portals::Remove(InterfaceTreeItem item)
	{
		for (uint i = 0; i < Portals.GetNum(); i++)
		{
			if (Portals[i]->TreeItem == item)
			{
				DeletePortalContent(Portals[i]);
				ClearCategories(Portals[i]);
				if (Portals[i]->IsPortalEmpty())
					Portals.DeleteAt(i);

				return;
			}

			for (uint j = 0; j < Portals[i]->Classes.GetNum(); j++)
			{
				if (Portals[i]->Classes[j]->TreeItem == item)
				{
					DeleteClassInPortalContent(
						Portals[i],
						Portals[i]->Classes[j]
					);

					ClearCategories(Portals[i]);
					if (Portals[i]->IsPortalEmpty())
						Portals.DeleteAt(i);

					return;
				}
			}
		}
	}

	VirtualVobTree* VirtualVobTree_Portals::FindNodeByInterfaceTreeItem(InterfaceTreeItem item)
	{
		for (uint i = 0; i < Portals.GetNum(); i++)
		{
			if (Portals[i]->TreeItem == item)
				return this;

			for (uint j = 0; j < Portals[i]->Classes.GetNum(); j++)
				if (Portals[i]->Classes[j]->TreeItem == item)
					return this;
		}

		return VirtualVobTree::FindNodeByInterfaceTreeItem(item);
	}

	SectorList* VirtualVobTree_Portals::GetPortalList(GothicTreeNode portal, InterfaceTreeItem root)
	{
		const zSTRING& portalName = *portal->GetData()->GetSectorNameVobIsIn();

		for (uint i = 0; i < Portals.GetNum(); i++)
			if (Portals[i]->PortalName == portalName.ToChar())
				return Portals[i];

		SectorList*& list = Portals.Create();
		list = new SectorList();
		list->PortalName = portalName.ToChar();
		list->TreeItem = InsertTreeFolder(portalName.ToChar(), root); // FILEVIEW.InsertItem(portalName.ToChar(), TV_IT, TV_ITSEL, root);
		list->UserData = portal->GetParent()->GetParent();
		return list;
	}

	InterfaceTreeItem VirtualVobTree_Portals::GetClassTreeItem(GothicTreeNode portal, Common::string className, InterfaceTreeItem root)
	{
		SectorList* portalList = GetPortalList(portal, root);
		InterfaceTreeItem treeItem = portalList->GetClassTree(className, portalList->TreeItem);
		return treeItem;
	}

	void VirtualVobTree_Portals::DeleteChilds()
	{
		Portals.DeleteData();
		VirtualVobTree::DeleteChilds();
	}

	VirtualVobTree_Portals& VirtualVobTree_Portals::GetInstance()
	{
		static VirtualVobTree_Portals instance;
		return instance;
	}






	VirtualVobTree_Classes::VirtualVobTree_Classes()
	{

	}

	void VirtualVobTree_Classes::Insert(GothicTreeNode treeNode)
	{
		zCVob* vob = treeNode->GetData();
		zSTRING className = vob->GetClassName_();
		InterfaceTreeItem classlGroup = GetClassTree(className);
		VirtualVobTree::Insert(classlGroup, treeNode);
	}

	InterfaceTreeItem VirtualVobTree_Classes::GetClassTree(Common::string className)
	{
		auto& pair = PortalList[className];
		if (!pair.IsNull())
			return pair;

		InterfaceTreeItem portalGroup = InsertTreeFolder(className.ToChar(), MFCTreeItem);//FILEVIEW.InsertItem(className, TV_IT, TV_ITSEL, MFCTreeItem);
		PortalList.Insert(className, portalGroup);
		return portalGroup;
	}





	VirtualVobTree_Globals::VirtualVobTree_Globals() : VirtualVobTree()
	{
		MFCTreeItem = InsertTreeFolder(_T("GLOBAL"), NULL);//FILEVIEW.InsertItem(_T("GLOBAL"), TV_IT, TV_ITSEL);
	}

	inline GothicTreeNode FindSubMeshBase(GothicTreeNode treeNode) {
		GothicTreeNode globalVobTree = &ogame->world->globalVobTree;
		while (treeNode)
		{
			if (treeNode->GetParent() == globalVobTree)
				return treeNode;

			treeNode = treeNode->GetParent();
		}

		return Null;
	}

	void VirtualVobTree_Globals::Insert(GothicTreeNode treeNode)
	{
		GothicTreeNode sumMeshNode = FindSubMeshBase(treeNode);
		InterfaceTreeItem rootItem = GetClassTreeItem(sumMeshNode, treeNode->GetData()->GetClassName_(), MFCTreeItem);
		VirtualVobTree* childVirtualTree = new VirtualVobTree(rootItem, treeNode);
		ChildList.Insert(childVirtualTree);
	}

	SectorList* VirtualVobTree_Globals::GetSubMeshList(GothicTreeNode subMesh, InterfaceTreeItem root)
	{
		zCVob* vob = subMesh->GetData();
		zCVisual* visual = vob->visual;
		zSTRING visualName = visual ? visual->GetVisualName() : Common::string((int)visual, 16);

		for (uint i = 0; i < SubMeshes.GetNum(); i++)
			if (SubMeshes[i]->PortalName == visualName.ToChar())
				return SubMeshes[i];

		SectorList*& list = SubMeshes.Create();
		list = new SectorList();
		list->PortalName = visualName.ToChar();
		list->TreeItem = InsertTreeFolder(visualName.ToChar(), root);//FILEVIEW.InsertItem(visualName.ToChar(), TV_IT, TV_ITSEL, root);
		list->UserData = subMesh;
		return list;
	}

	InterfaceTreeItem VirtualVobTree_Globals::GetClassTreeItem(GothicTreeNode subMesh, Common::string className, InterfaceTreeItem root)
	{
		SectorList* portalList = GetSubMeshList(subMesh, root);
		InterfaceTreeItem treeItem = portalList->GetClassTree(className, portalList->TreeItem);
		return treeItem;
	}

	void VirtualVobTree_Globals::InsertAsChild(GothicTreeNode treeNode)
	{
		if (TreeNode == treeNode->GetParent())
			return Insert(treeNode);

		VirtualVobTree* node = FindParentVobTreeForVob(treeNode->GetData());
		if (!node)
			return Insert(treeNode);

		zCVob* vob = treeNode->GetData();
		node->InsertAsChild(treeNode);
	}

	void VirtualVobTree_Globals::InsertAsChild(zCVob* vob)
	{
		InsertAsChild(vob->globalVobTreeNode);
	}

	void VirtualVobTree_Globals::DeleteClassInSubMeshContent(SectorList* subMesh, ClassList* classObj)
	{
		GothicTreeNode treeNode = (GothicTreeNode)subMesh->UserData;
		GothicTreeNode childs = treeNode->GetFirstChild();
		Common::string& className = classObj->ClassName;
		while (childs)
		{
			zCVob* vob = childs->GetData();
			childs = childs->GetNextChild();
			if (!vob->GetSectorNameVobIsIn() && vob->GetClassName_() == className.ToChar())
				ogame->GetGameWorld()->RemoveVobSubtree(vob);
		}
	}

	void VirtualVobTree_Globals::DeleteSubMeshContent(SectorList* subMesh)
	{
		GothicTreeNode treeNode = (GothicTreeNode)subMesh->UserData;
		GothicTreeNode childs = treeNode->GetFirstChild();
		while (childs)
		{
			zCVob* vob = childs->GetData();
			childs = childs->GetNextChild();
			if (!vob->GetSectorNameVobIsIn())
				ogame->GetGameWorld()->RemoveVobSubtree(vob);
		}
	}

	void VirtualVobTree_Globals::ClearCategories(SectorList* portal)
	{
		for (uint i = 0; i < portal->Classes.GetNum(); i++)
			if (portal->Classes[i]->IsPortalEmpty())
				portal->Classes.DeleteAt(i--);
	}

	void VirtualVobTree_Globals::Remove(zCVob* vob)
	{
		VirtualVobTree::Remove(vob);
	}

	void VirtualVobTree_Globals::Remove(InterfaceTreeItem item)
	{
		for (uint i = 0; i < SubMeshes.GetNum(); i++)
		{
			if (SubMeshes[i]->TreeItem == item)
			{
				DeleteSubMeshContent(SubMeshes[i]);
				ClearCategories(SubMeshes[i]);
				if (SubMeshes[i]->IsPortalEmpty())
					SubMeshes.DeleteAt(i);

				return;
			}

			for (uint j = 0; j < SubMeshes[i]->Classes.GetNum(); j++)
			{
				if (SubMeshes[i]->Classes[j]->TreeItem == item)
				{
					DeleteClassInSubMeshContent(
						SubMeshes[i],
						SubMeshes[i]->Classes[j]
					);

					ClearCategories(SubMeshes[i]);
					if (SubMeshes[i]->IsPortalEmpty())
						SubMeshes.DeleteAt(i);

					return;
				}
			}
		}
	}

	VirtualVobTree* VirtualVobTree_Globals::FindNodeByInterfaceTreeItem(InterfaceTreeItem item)
	{
		for (uint i = 0; i < SubMeshes.GetNum(); i++)
		{
			if (SubMeshes[i]->TreeItem == item)
				return this;

			for (uint j = 0; j < SubMeshes[i]->Classes.GetNum(); j++)
				if (SubMeshes[i]->Classes[j]->TreeItem == item)
					return this;
		}

		return VirtualVobTree::FindNodeByInterfaceTreeItem(item);
	}

	void VirtualVobTree_Globals::DeleteChilds()
	{
		VirtualVobTree::DeleteChilds();
		SubMeshes.DeleteData();
	}

	VirtualVobTree_Globals& VirtualVobTree_Globals::GetInstance()
	{
		static VirtualVobTree_Globals instance;
		return instance;
	}

}
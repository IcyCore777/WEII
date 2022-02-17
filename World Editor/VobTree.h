// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {


#define FILEVIEW CMainFrame::mainframe->m_wndVobList.m_wndVobList

	typedef HTREEITEM InterfaceTreeItem;
	typedef zCTree<zCVob>* GothicTreeNode;
	typedef Common::Array<class VirtualVobTree*> VirtualTreeList;
	typedef Common::Map<Common::string, InterfaceTreeItem> MapClassList;
	typedef Common::Map<Common::string, MapClassList*> MapPortalList;

	struct VirtualVobTree
	{
		InterfaceTreeItem MFCTreeItem;
		GothicTreeNode TreeNode;
		VirtualTreeList ChildList;
		VirtualVobTree();
		VirtualVobTree(InterfaceTreeItem mfcItem, GothicTreeNode treeNode, Common::string nodeName = "");
		void Insert(InterfaceTreeItem mfcItem, GothicTreeNode treeNode);
		void InsertAsChild(GothicTreeNode treeNode);
		void InsertAsChild(zCVob* vob);
		virtual void Remove(GothicTreeNode treeNode);
		virtual void Remove(InterfaceTreeItem treeNode);
		void Remove(zCVob* vob);
		virtual VirtualVobTree* FindNodeByInterfaceTreeItem(InterfaceTreeItem item);
		VirtualVobTree* FindParentVobTreeForVob(zCVob* vob);
		HTREEITEM FindTreeItemForVob(zCVob*); //мой митад
		bool IsIn(zCVob* vob);
		virtual void DeleteChilds();
		void Clear();
		virtual ~VirtualVobTree();
	};

	struct VirtualVobTree_Classes : public VirtualVobTree
	{
		MapClassList PortalList;

		VirtualVobTree_Classes();
		void Insert(GothicTreeNode treeNode);
		InterfaceTreeItem GetClassTree(Common::string className);
	};





	struct ClassList {
		Common::string ClassName;
		InterfaceTreeItem TreeItem;

		bool IsPortalEmpty()
		{
			return !FILEVIEW.GetChildItem(TreeItem);
		}

		~ClassList()
		{
			FILEVIEW.DeleteItem(TreeItem);
		}
	};

	struct SectorList {
		Common::string PortalName;
		InterfaceTreeItem TreeItem;
		Common::Array<ClassList*> Classes;
		void* UserData;

		InterfaceTreeItem GetClassTree(Common::string className, InterfaceTreeItem root);

		bool IsPortalEmpty()
		{
			return !FILEVIEW.GetChildItem(TreeItem);
		}

		~SectorList()
		{
			Classes.DeleteData();
			FILEVIEW.DeleteItem(TreeItem);
		}
	};

	struct VirtualVobTree_Portals : public VirtualVobTree
	{
		Common::Array<SectorList*> Portals;


		VirtualVobTree_Portals();
		void Insert(GothicTreeNode treeNode);
		void InsertAsChild(GothicTreeNode treeNode);
		void InsertAsChild(zCVob* vob);
		void DeleteClassInPortalContent(SectorList* portal, ClassList* classObj);
		void DeletePortalContent(SectorList* portal);
		void ClearCategories(SectorList* portal);
		virtual void Remove(zCVob* vob);
		virtual void Remove(InterfaceTreeItem item);
		virtual VirtualVobTree* FindNodeByInterfaceTreeItem(InterfaceTreeItem item);
		SectorList* GetPortalList(GothicTreeNode portal, InterfaceTreeItem root);
		InterfaceTreeItem GetClassTreeItem(GothicTreeNode portal, Common::string className, InterfaceTreeItem root);
		virtual void DeleteChilds();
		static VirtualVobTree_Portals& GetInstance();
	};


	struct VirtualVobTree_Globals : public VirtualVobTree
	{
		Common::Array<SectorList*> SubMeshes;

		VirtualVobTree_Globals();
		void Insert(GothicTreeNode treeNode);
		void InsertAsChild(GothicTreeNode treeNode);
		void InsertAsChild(zCVob* vob);
		void DeleteClassInSubMeshContent(SectorList* subMesh, ClassList* classObj);
		void DeleteSubMeshContent(SectorList* subMesh);
		void ClearCategories(SectorList* portal);
		virtual void Remove(zCVob* vob);
		virtual void Remove(InterfaceTreeItem item);
		virtual VirtualVobTree* FindNodeByInterfaceTreeItem(InterfaceTreeItem item);
		SectorList* GetSubMeshList(GothicTreeNode subMesh, InterfaceTreeItem root);
		InterfaceTreeItem GetClassTreeItem(GothicTreeNode subMesh, Common::string className, InterfaceTreeItem root);
		virtual void DeleteChilds();
		static VirtualVobTree_Globals& GetInstance();
	};
}
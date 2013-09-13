#ifndef __TREE_VIEW_MANAGER_H__
#define __TREE_VIEW_MANAGER_H__


class CTreeViewManager
{
public:
	CTreeViewManager();
	~CTreeViewManager();

public:
	void Init(CTreeViewUI* pTreeView);
	void InitUserNode();
	void InitMyConcernNode();
	void InitQuickEntryNode();


private:
	CTreeNodeUI* m_pUserTree;
	CTreeNodeUI* m_pMyConcernTree;
	CTreeNodeUI* m_pQuickEntryTree;
	CTreeViewUI* m_pTreeView;
};




#endif
#include "StdAfx.h"
#include "TreeViewManager.h"


CTreeViewManager::CTreeViewManager(){}
CTreeViewManager::~CTreeViewManager(){}

void CTreeViewManager::InitUserNode()
{
	m_pUserTree = new CTreeNodeUI();
	TCHAR buf[1000];
	::_stprintf(buf, _T("showhtml=\"true\" text=\"  {i Resource/Images/miniLogo.png} 我的Venus\" font=\"1\" selectedbkcolor=\"#FFA7CDF0\" "));
	m_pUserTree->SetAttribute(_T("itemattr"), buf);
	m_pUserTree->SetAttribute(_T("folderattr"), _T("padding=\"0,1,0,0\" width=\"16\" height=\"16\" normalimage=\"file='Resource/Images/treeview_b.png' source='0,0,16,16' \" hotimage=\"file='Resource/Images/treeview_b.png' source='16,0,32,16' \" selectedimage=\"file='Resource/Images/treeview_a.png' source='0,0,16,16' \" selectedhotimage=\"file='Resource/Images/treeview_a.png' source='16,0,32,16' \""));
	
	CTreeNodeUI* pSubNode = new CTreeNodeUI();
	::_stprintf(buf, _T("showhtml=\"true\" text=\"{i Resource/Images/task.png} 流程阿尔法\" font=\"1\" "));
	pSubNode->SetAttribute(_T("itemattr"), buf);
	pSubNode->SetAttribute(_T("folderattr"), _T("enabled=\"false\""));
	m_pUserTree->Add(pSubNode);

	CTreeNodeUI* pSubNode1 = new CTreeNodeUI();
	::_stprintf(buf, _T("showhtml=\"true\" text=\"{i Resource/Images/task.png} 流程贝塔\" font=\"1\" "));
	pSubNode1->SetAttribute(_T("itemattr"), buf);
	pSubNode1->SetAttribute(_T("folderattr"), _T("enabled=\"false\""));
	m_pUserTree->Add(pSubNode1);


	CTreeNodeUI* pSubNode4 = new CTreeNodeUI();
	::_stprintf(buf, _T("showhtml=\"true\" text=\"{i Resource/Images/task.png} 流程伽马\" font=\"1\" "));
	pSubNode4->SetAttribute(_T("itemattr"), buf);
	pSubNode4->SetAttribute(_T("folderattr"), _T("enabled=\"false\""));

	m_pUserTree->Add(pSubNode4);

	CTreeNodeUI* pSubNode5 = new CTreeNodeUI();
	::_stprintf(buf, _T("showhtml=\"true\" text=\"{i Resource/Images/task.png} 流程德尔塔\" font=\"1\" "));
	pSubNode5->SetAttribute(_T("itemattr"), buf);
	pSubNode5->SetAttribute(_T("folderattr"), _T("enabled=\"false\""));

	m_pUserTree->Add(pSubNode5);


	m_pTreeView->Add(m_pUserTree);
}

void CTreeViewManager::Init(CTreeViewUI* pTreeView)
{
	m_pTreeView = pTreeView;
	InitUserNode();
	InitMyConcernNode();
	InitQuickEntryNode();
}

void CTreeViewManager::InitMyConcernNode()
{
	m_pMyConcernTree = new CTreeNodeUI();
	TCHAR buf[1000];
	m_pMyConcernTree->SetAttribute(_T("itemattr"), _T("padding=\"0,0,0,0\" showhtml=\"true\" text=\"  {i Resource/Images/star.png} 我的关注\" font=\"1\" selectedbkcolor=\"#FFA7CDF0\" "));
	m_pMyConcernTree->SetAttribute(_T("folderattr"), _T("padding=\"0,1,0,0\" width=\"16\" height=\"16\" normalimage=\"file='Resource/Images/treeview_b.png' source='0,0,16,16' \" hotimage=\"file='Resource/Images/treeview_b.png' source='16,0,32,16' \" selectedimage=\"file='Resource/Images/treeview_a.png' source='0,0,16,16' \" selectedhotimage=\"file='Resource/Images/treeview_a.png' source='16,0,32,16' \""));
	
	CTreeNodeUI* pSubNode = new CTreeNodeUI();
	::_stprintf(buf, _T("showhtml=\"true\" text=\"{i Resource/Images/task.png} 流程阿尔法\" font=\"1\" "));
	pSubNode->SetAttribute(_T("itemattr"), buf);
	pSubNode->SetAttribute(_T("folderattr"), _T("enabled=\"false\""));
	m_pMyConcernTree->Add(pSubNode);
	
	CTreeNodeUI* pSubNode1 = new CTreeNodeUI();
	::_stprintf(buf, _T("showhtml=\"true\" text=\"{i Resource/Images/task.png} 流程贝塔\" font=\"1\" "));
	pSubNode1->SetAttribute(_T("itemattr"), buf);
	pSubNode1->SetAttribute(_T("folderattr"), _T("enabled=\"false\""));

	m_pMyConcernTree->Add(pSubNode1);

	CTreeNodeUI* pSubNode4 = new CTreeNodeUI();
	::_stprintf(buf, _T("showhtml=\"true\" text=\"{i Resource/Images/task.png} 流程伽马\" font=\"1\" "));
	pSubNode4->SetAttribute(_T("itemattr"), buf);
	pSubNode4->SetAttribute(_T("folderattr"), _T("enabled=\"false\""));

	m_pMyConcernTree->Add(pSubNode4);

	CTreeNodeUI* pSubNode5 = new CTreeNodeUI();
	::_stprintf(buf, _T("showhtml=\"true\" text=\"{i Resource/Images/task.png} 流程德尔塔\" font=\"1\" "));
	pSubNode5->SetAttribute(_T("itemattr"), buf);
	pSubNode5->SetAttribute(_T("folderattr"), _T("enabled=\"false\""));

	m_pMyConcernTree->Add(pSubNode5);
	m_pTreeView->Add(m_pMyConcernTree);
}

void CTreeViewManager::InitQuickEntryNode()
{
	m_pQuickEntryTree = new CTreeNodeUI();

	TCHAR buf[1000];
	m_pQuickEntryTree->SetAttribute(_T("itemattr"), _T("padding=\"0,0,0,0\" showhtml=\"true\" text=\"  {i Resource/Images/quickentry.png} 快速入口\" font=\"1\" selectedbkcolor=\"#FFA7CDF0\" "));
	m_pQuickEntryTree->SetAttribute(_T("folderattr"), _T("padding=\"0,1,0,0\" width=\"16\" height=\"16\" normalimage=\"file='Resource/Images/treeview_b.png' source='0,0,16,16' \" hotimage=\"file='Resource/Images/treeview_b.png' source='16,0,32,16' \" selectedimage=\"file='Resource/Images/treeview_a.png' source='0,0,16,16' \" selectedhotimage=\"file='Resource/Images/treeview_a.png' source='16,0,32,16' \""));


	CTreeNodeUI* pSubNode = new CTreeNodeUI();
	::_stprintf(buf, _T("showhtml=\"true\" text=\"{i Resource/Images/quickcreate.png} 新建阿尔法\" font=\"1\" "));
	pSubNode->SetAttribute(_T("itemattr"), buf);
	pSubNode->SetAttribute(_T("folderattr"), _T("enabled=\"false\""));
	m_pQuickEntryTree->Add(pSubNode);

	CTreeNodeUI* pSubNode1 = new CTreeNodeUI();
	::_stprintf(buf, _T("showhtml=\"true\" text=\"{i Resource/Images/quickcreate.png} 新建贝塔\" font=\"1\" "));
	pSubNode1->SetAttribute(_T("itemattr"), buf);
	pSubNode1->SetAttribute(_T("folderattr"), _T("enabled=\"false\""));
	m_pQuickEntryTree->Add(pSubNode1);

	CTreeNodeUI* pSubNode2 = new CTreeNodeUI();
	::_stprintf(buf, _T("showhtml=\"true\" text=\"{i Resource/Images/quickcreate.png} 新建更新日志\" font=\"1\" "));
	pSubNode2->SetAttribute(_T("itemattr"), buf);
	pSubNode2->SetAttribute(_T("folderattr"), _T("enabled=\"false\""));
	m_pQuickEntryTree->Add(pSubNode2);


	m_pTreeView->Add(m_pQuickEntryTree);
}
#include "StdAfx.h"
#include "ListViewManager.h"

CListViewManager::CListViewManager(){}
CListViewManager::~CListViewManager(){}

void CListViewManager::Init(CListUI* pList)
{
	m_pList = pList;
	m_pList->RemoveAll();

	for(int i = 0; i < 20; i++)
	{
		CListContainerElementUI* pContainer = new CListContainerElementUI();
		pContainer->SetFixedHeight(60);


		CVerticalLayoutUI* pLayout = new CVerticalLayoutUI();
		CHorizontalLayoutUI* pHlayout1 = new CHorizontalLayoutUI();
		CLabelUI* pLabelText = new CLabelUI();
		pLabelText->SetText(_T("  这是标题"));
		pHlayout1->Add(pLabelText);
		pLayout->Add(pHlayout1);

		CHorizontalLayoutUI* pHLayout2 = new CHorizontalLayoutUI();
		CLabelUI* pLabelTime = new CLabelUI();
		pLabelTime->SetText(_T("  2013-9-11 13:40"));
		pLabelTime->SetTextColor(0xFF0000FF);
		pHLayout2->Add(pLabelTime);
		CLabelUI* pLabelDesc = new CLabelUI();
		pLabelDesc->SetText(_T("  这里是详情"));
		pLabelDesc->SetTextColor(0xFF666666);
		pHLayout2->Add(pLabelDesc);

		pLayout->Add(pHLayout2);
		CHorizontalLayoutUI* pHlayout3 = new CHorizontalLayoutUI();
		pLayout->Add(pHlayout3);
		CHorizontalLayoutUI* pHLayout4 = new CHorizontalLayoutUI();
		pHLayout4->SetFixedHeight(1);
		pHLayout4->SetBorderColor(0xFFD9DCDD);
		pHLayout4->SetBorderSize(1);
		pLayout->Add(pHLayout4);
		pContainer->Add(pLayout);
		m_pList->Add(pContainer);
	}
	
}
#ifndef __LIST_VIEW_MANAGER_H__
#define __LIST_VIEW_MANAGER_H__

class CListViewManager
{
public:
	CListViewManager();
	~CListViewManager();

	void Init(CListUI* pList);

private:
	CListUI* m_pList;
};




#endif
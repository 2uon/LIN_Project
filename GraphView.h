#pragma once
#include "StdClass.h"
#include <d2d1.h>

class CGraphView : public CWnd
{
public:
    CGraphView();
    virtual ~CGraphView();

    BOOL Create(CWnd* pParent, const RECT& rect);
    void AddData(float value);

protected:
    afx_msg void OnPaint();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnDestroy();

    DECLARE_MESSAGE_MAP()

private:
    void InitDirect2D();
    void CleanupDirect2D();
    void DrawGraph();

    ID2D1Factory* m_pD2DFactory;
    ID2D1HwndRenderTarget* m_pRenderTarget;
    ID2D1SolidColorBrush* m_pBrush;

    vector<float> m_data;
};

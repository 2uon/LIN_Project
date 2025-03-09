#include "pch.h"
#include "GraphView.h"
#include <algorithm>

#pragma comment(lib, "d2d1.lib")

CGraphView::CGraphView()
    : m_pD2DFactory(nullptr), m_pRenderTarget(nullptr), m_pBrush(nullptr)
{
}

CGraphView::~CGraphView()
{
    CleanupDirect2D();
}

BEGIN_MESSAGE_MAP(CGraphView, CWnd)
    ON_WM_PAINT()
    ON_WM_CREATE()
    ON_WM_TIMER()
    ON_WM_DESTROY()
END_MESSAGE_MAP()

BOOL CGraphView::Create(CWnd* pParent, const RECT& rect)
{
    return CWnd::Create(NULL, _T("GraphView"), WS_CHILD | WS_VISIBLE, rect, pParent, 0);
}

int CGraphView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    InitDirect2D();
    SetTimer(1, 50, NULL); // 50ms 간격으로 타이머 설정 (20FPS)

    return 0;
}

void CGraphView::OnDestroy()
{
    KillTimer(1);
    CleanupDirect2D();
    CWnd::OnDestroy();
}

void CGraphView::InitDirect2D()
{
    D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);

    CRect rc;
    GetClientRect(&rc);

    D2D1_SIZE_U size = D2D1::SizeU(rc.Width(), rc.Height());

    m_pD2DFactory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(m_hWnd, size),
        &m_pRenderTarget
    );

    m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue), &m_pBrush);
}

void CGraphView::CleanupDirect2D()
{
    if (m_pBrush) m_pBrush->Release();
    if (m_pRenderTarget) m_pRenderTarget->Release();
    if (m_pD2DFactory) m_pD2DFactory->Release();
}

void CGraphView::OnPaint()
{
    PAINTSTRUCT ps;
    BeginPaint(&ps);

    if (m_pRenderTarget)
    {
        m_pRenderTarget->BeginDraw();
        m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

        DrawGraph();

        m_pRenderTarget->EndDraw();
    }

    EndPaint(&ps);
}

void CGraphView::OnTimer(UINT_PTR nIDEvent)
{
    if (nIDEvent == 1)
    {
        AddData(static_cast<float>(rand() % 100)); // 랜덤 데이터 추가
        Invalidate(); // 화면 갱신
    }

    CWnd::OnTimer(nIDEvent);
}

void CGraphView::AddData(float value)
{
    if (m_data.size() >= 100)
        m_data.erase(m_data.begin()); // 오래된 데이터 제거

    m_data.push_back(value);
}

void CGraphView::DrawGraph()
{
    if (m_data.empty()) return;

    CRect rc;
    GetClientRect(&rc);

    float width = static_cast<float>(rc.Width());
    float height = static_cast<float>(rc.Height());

    float xStep = width / 100.0f;
    float maxVal = *std::max_element(m_data.begin(), m_data.end());

    D2D1_POINT_2F prevPoint = { 0, height - (m_data[0] / maxVal) * height };

    for (size_t i = 1; i < m_data.size(); ++i)
    {
        float x = i * xStep;
        float y = height - (m_data[i] / maxVal) * height;

        D2D1_POINT_2F currPoint = { x, y };
        m_pRenderTarget->DrawLine(prevPoint, currPoint, m_pBrush, 2.0f);
        prevPoint = currPoint;
    }
}


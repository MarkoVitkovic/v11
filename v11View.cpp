#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "v11.h"
#endif

#include "v11Doc.h"
#include "v11View.h"
#include "mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cv11View

IMPLEMENT_DYNCREATE(Cv11View, CView)

BEGIN_MESSAGE_MAP(Cv11View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &Cv11View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_SHAPE, &Cv11View::shape)
	ON_COMMAND(ID_COLOR, &Cv11View::color)
	ON_WM_LBUTTONDOWN()
	ON_REGISTERED_MESSAGE(AFX_WM_ON_HIGHLIGHT_RIBBON_LIST_ITEM, &Cv11View::OnHighlightRibbonListItem)

END_MESSAGE_MAP()

// Cv11View construction/destruction

Cv11View::Cv11View() {
	
		boja = RGB(0, 0, 0);
		prethodnaB = RGB(0, 0, 0);
		oblik = 0;
		prethodniO = 0;
	
}

Cv11View::~Cv11View()
{
}

BOOL Cv11View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// Cv11View drawing

void Cv11View::OnDraw(CDC* pDC)
{
	CPen olovka;
	olovka.CreatePen(PS_SOLID, 3, boja);
	pDC->SelectObject(olovka);
	POINT point = { 100, 100 };
	switch (oblik)
	{
	case 0:
	{
		pDC->Rectangle(&rect);
		break;
	}
	case 1:
	{
		pDC->Ellipse(&rect);
		break;
	}
	case 2:
	{
		pDC->RoundRect(&rect, point);
		break;
	}
	default:
		break;
	}
	/*if(oblik==0)
		pDC->Rectangle(&rect);
	if(oblik==1)
		pDC->Ellipse(&rect);
	if(oblik==2)
		pDC->RoundRect(&rect, point);*/
	/*switch (oblik)
	{
	case 0:
		pDC->Rectangle(&rect);
		break;
	case 1:
		pDC->Ellipse(&rect);
		break;
	case 2:
		pDC->RoundRect(&rect, p);
		break;
	default:
		break;
	}*/
}


// Cv11View printing


void Cv11View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL Cv11View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void Cv11View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void Cv11View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void Cv11View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void Cv11View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// Cv11View diagnostics

#ifdef _DEBUG
void Cv11View::AssertValid() const
{
	CView::AssertValid();
}

void Cv11View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Cv11Doc* Cv11View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Cv11Doc)));
	return (Cv11Doc*)m_pDocument;
}
#endif //_DEBUG


// Cv11View message handlers


void Cv11View::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRectTracker tracker;

	if (tracker.TrackRubberBand(this, point))
		rect = tracker.m_rect;
	Invalidate();
}
void Cv11View::shape()
{
	CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*> niz;
	((CMainFrame*)AfxGetMainWnd())->m_wndRibbonBar.GetElementsByID(ID_SHAPE, niz);
	CMFCRibbonGallery* pGallery = (CMFCRibbonGallery*)niz.GetAt(0);
	oblik = pGallery->GetSelectedItem();
	prethodniO = oblik;
	Invalidate();
}
void Cv11View::color()
{
	CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*> niz;
	((CMainFrame*)AfxGetMainWnd())->m_wndRibbonBar.GetElementsByID(ID_COLOR, niz);
	CMFCRibbonColorButton* pColorButton = (CMFCRibbonColorButton*)niz.GetAt(0);
	boja = pColorButton->GetColor();
	prethodnaB = boja;
	Invalidate();
}



LRESULT Cv11View::OnHighlightRibbonListItem(WPARAM WW, LPARAM LL)
{
	

	int index = (int)WW;
	CMFCRibbonBaseElement* pElem = (CMFCRibbonBaseElement*)LL;
	CMFCRibbonColorButton* pColor = (CMFCRibbonColorButton*)LL;
	UINT id = pElem->GetID();

	switch (id)
	{
	case ID_COLOR:
		if (index == -1)
			boja = prethodnaB;
		else
			boja = pColor->GetHighlightedColor();
		break;
	case ID_SHAPE:
		if (index == -1)
			oblik=prethodniO;
		else
			oblik = index;
		break;
	}
	Invalidate();
	return 0;
}

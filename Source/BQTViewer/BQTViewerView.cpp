/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface
// (the "Fluent UI") and is provided only as referential material to supplement the
// Microsoft Foundation Classes Reference and related electronic documentation
// included with the MFC C++ library software.
// License terms to copy, use or distribute the Fluent UI are available separately.
// To learn more about our Fluent UI licensing program, please visit
// https://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// BQTViewerView.cpp : implementation of the CBQTViewerView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "BQTViewer.h"
#endif

#include "BQTViewerDoc.h"
#include "BQTViewerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBQTViewerView

IMPLEMENT_DYNCREATE(CBQTViewerView, CListView)

BEGIN_MESSAGE_MAP(CBQTViewerView, CListView)
	ON_WM_STYLECHANGED()
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CBQTViewerView construction/destruction

CBQTViewerView::CBQTViewerView() noexcept
{
	// TODO: add construction code here

}

CBQTViewerView::~CBQTViewerView()
{
}

BOOL CBQTViewerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CListView::PreCreateWindow(cs);
}

void CBQTViewerView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();


	// TODO: You may populate your ListView with items by directly accessing
	//  its list control through a call to GetListCtrl().
}

void CBQTViewerView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CBQTViewerView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CBQTViewerView diagnostics

#ifdef _DEBUG
void CBQTViewerView::AssertValid() const
{
	CListView::AssertValid();
}

void CBQTViewerView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CBQTViewerDoc* CBQTViewerView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBQTViewerDoc)));
	return (CBQTViewerDoc*)m_pDocument;
}
#endif //_DEBUG


// CBQTViewerView message handlers
void CBQTViewerView::OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct)
{
	//TODO: add code to react to the user changing the view style of your window
	CListView::OnStyleChanged(nStyleType,lpStyleStruct);
}

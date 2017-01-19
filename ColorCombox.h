// ColorCombox.h
//this file support a class to show a button combobox
//by means of GPopupColorCtr Class
/////////////////////////////////////////////////////////////////////////////////
//if there have any bugs, pls inform me
//Tel: 021-64397490-564/565
//E-mail:x_qk@netease.com
//////////////////////////////////////////////////////////////////////////////////
#include "PopWnd.h"

// notificiations 
#define CCN_SELCHANGE  1
#define CCBDIM_CXEDGE    2
#define CCBDIM_CYEDGE    2
#define CCBDIM_CYBORDER  3
#define CCBDIM_CXBORDER  2
#define CCBDIM_CXSPACELEFT  4
#define CCBDIM_CXSPACERIGHT 1
#define CCBDIM_CXCOLORRECT  32
#define CCBDIM_CXDIVIDER     2
#define CCBDIM_CXARROW       5

class GColorComboBox : public CWnd
{
   DECLARE_MESSAGE_MAP()

public:
   GColorComboBox();

   BOOL Create(DWORD dwStyle, const RECT& rect,
               CWnd *pParentWnd, UINT nID);
   //set color and get color 
   void SetColor(COLORREF clInit);
   COLORREF GetColor() const;
   //
   virtual void UpdateColor(COLORREF clUpdate);

protected:  
   //overrides the virtual function 
   afx_msg void OnLButtonDown(UINT nFlags, CPoint pt);
   afx_msg int OnCreate(LPCREATESTRUCT lpcs);
   afx_msg BOOL OnEraseBkgnd(CDC *pDC);
   afx_msg void OnPaint();
   //define color variable 
   COLORREF m_cl;

public:
   virtual ~GColorComboBox();
};

#define CCDIM_SQUAREWIDTH   16
#define CCDIM_SQUAREHEIGHT  16
#define CCDIM_CYSPACE       3
#define CCDIM_CXSPACE       3
#define CCDIM_TOTALINDECES  16
//this class is based on GPopupWindow
//it draws color box int the frame of GPopupWindow

class GPopupColorCtrl : public GPopupWindow
{
   DECLARE_MESSAGE_MAP();
   
public:
   GPopupColorCtrl(COLORREF clInit, 
                   GColorComboBox *pBuddyCombo);
   //get selected color
   COLORREF GetSelectedColor();

protected:
   //Overrides the virtual function
   afx_msg void OnPaint();
   afx_msg BOOL OnEraseBkgnd(CDC *pDC);
   afx_msg void OnMouseMove(UINT nFlags, CPoint pt);
   afx_msg void OnLButtonUp(UINT nFlags, CPoint pt);
   afx_msg int OnCreate(LPCREATESTRUCT lpcs);
   //draw color rectangle and frame
   void DrawColorSquare(CDC *pDC, int nIndex, CPoint pt);
   //judge which color the mouse cursor is in
   int CursorToColorIndex(CPoint ptTest);
   //define the user selected color
   COLORREF m_clSelected;
   int m_nActiveSquare; //define current active color area
   int m_nCurHoverSquare;//define mouse move to the color area 
   //
   GColorComboBox *m_pBuddyCombo;

public:
   virtual ~GPopupColorCtrl();
};




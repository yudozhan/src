// PopWnd.h
//this class support a frame of a pop windows
/////////////////////////////////////////////////////////////////////////////////
//add comments by Xia Qiankun
//1999.4.25
//if there have any bugs, pls inform me
//Tel: 021-64397490-564/565
//E-mail:x_qk@netease.com
//////////////////////////////////////////////////////////////////////////////////

class GPopupWindow : public CWnd
{
   DECLARE_MESSAGE_MAP()

public:
   GPopupWindow();
   
protected:
   //Override virtual function
   afx_msg int OnCreate(LPCREATESTRUCT lpcs);
   afx_msg void OnLButtonDown(UINT nFlags, CPoint pt);
   afx_msg void OnCaptureChanged(CWnd *pWnd);

   virtual BOOL OnLButtonDownCheck(UINT nFlags, CPoint pt);
   virtual void PostNcDestroy();
public:
   virtual ~GPopupWindow();
};



 
//this class is designed to manager the memory instead of mfc
/////////////////////////////////////////////////////////////////////////////////
//if there have any bugs, pls inform me
//Tel: 021-64397490-564/565
//E-mail:x_qk@netease.com
//////////////////////////////////////////////////////////////////////////////////
class GMemDC : public CDC
{
public:
   // create a memory dc
   BOOL Create(CDC *pDC,             // device from which to create compatible dc
               const RECT& rClient,  // client rectangle of window receiving paint message 
               CBrush *pBkBrush=NULL,// brush to paint background of memory DC, default is brush of 'pDC'
               const PAINTSTRUCT *pPS=NULL); // paint struct if 'pDC' is actually a CPaintDC
   
   // copy this memory dc to a dc
   BOOL Copy(CDC *pDC,              // device to copy memory dc to
             const RECT& rDraw,     // destination rectangle of 'pDC'
             const POINT *ppt=NULL);// top-left corner of mem dc
                                   
   // release the memory dc
   BOOL Release();                   

   // return the internal paint structure
   const PAINTSTRUCT *GetPaintStruct() const;

public:
   GMemDC();
   ~GMemDC();

private:
   PAINTSTRUCT m_ps;                 // copy of paint dc paint structure

   BOOL bPaintStructInitialized;     // internal paint structure is initialized 
   CBitmap *pOldBitmap;              // bitmap to restore when dc is released
   CBitmap Bitmap;                   // bitmap to select in create

   // privatize compiler gifts
   GMemDC( const GMemDC& );
   GMemDC& operator=( const GMemDC& );
};


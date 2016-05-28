#ifndef _ABOUTDLG_H
#define _ABOUTDLG_H

#include "ACLWinDlg.h"

class AboutDlg :
    public ACLWinDlg
{
public:
    AboutDlg(IN HINSTANCE, IN HWND);
    virtual ~AboutDlg() throw();

    // Message Handlers
    bool OnWMInit
    (
	    IN WPARAM wParam,
	    IN LPARAM lParam
    );
    bool OnDestroy
    (
	    IN WPARAM wParam,
	    IN LPARAM lParam
    );
    bool OnOk
    (
	    IN WPARAM wParam,
	    IN LPARAM lParam
    );
};

#endif // _ABOUTDLG_H

#ifndef _SAMPLEMAINWINDOW_H
#define _SAMPLEMAINWINDOW_H

#include "ACLWinBase.h"
#include "StatusBar.h"

class SampleMainWindow :
    public ACLWinBase
{
public:
    SampleMainWindow(IN HINSTANCE);
    virtual ~SampleMainWindow() throw();

    bool Show(void);	

    // Message Handlers.
    LRESULT OnCreate
    (
	    IN WPARAM wParam,
	    IN LPARAM lParam
    );
    LRESULT OnDestroy
    (
	    IN WPARAM wParam,
	    IN LPARAM lParam
    );
    LRESULT OnAbout
    (
	    IN WPARAM wParam,
	    IN LPARAM lParam
    );
	LRESULT OnStringTest
    (
	    IN WPARAM wParam,
	    IN LPARAM lParam
    );
	LRESULT OnLoadLibTest
    (
	    IN WPARAM wParam,
	    IN LPARAM lParam
    );

    LRESULT OnExit
    (
	    IN WPARAM wParam,
	    IN LPARAM lParam
    );		
    LRESULT OnResize
    (
	    IN WPARAM wParam,
	    IN LPARAM lParam
    );		
    LRESULT OnPaint
    (
	    IN WPARAM wParam,
	    IN LPARAM lParam
    );		

private:
    bool _CreateStatusBar(void);
    void _Cleanup(void);
	void _SetMenuItemCheck
	(
		IN UINT	menuId, 
		IN bool	fCheck
	);
	void _CalculateClientRect(RECT& clientRect);
    void _SetStatusText(char* text);

private:
    StatusBar*                  _statusBar;

};
#endif // _SAMPLEMAINWINDOW_H

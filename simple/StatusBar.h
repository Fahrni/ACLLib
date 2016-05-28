#ifndef _STATUSBAR_H
#define _STATUSBAR_H

#include "ACLWinBase.h"

class StatusBar :
    public ACLWinBase
{
public:
    StatusBar(IN HINSTANCE, IN HWND, IN ACLWinBase*);
    virtual ~StatusBar() throw();

    bool Show(void);	
    void SetPartText
    (
        IN UINT16	index,
        IN char*	text
    );

    // Message Handlers.
    LRESULT OnCreate
    (
	    IN WPARAM wParam,
	    IN LPARAM lParam
    );
    //LRESULT OnResize
    //(
    //    IN WPARAM wParam,
    //    IN LPARAM lParam
    //);
    LRESULT OnDestroy
    (
	    IN WPARAM wParam,
	    IN LPARAM lParam
    );

private: // methods
    void _CreateParts(void);

private:
    ACLWinBase*     _pParentWindow;
};

#endif // _STATUSBAR_H

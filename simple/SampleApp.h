#ifndef _SAMPLEAPP_H
#define _SAMPLEAPP_H

#include "ACLWinApp.h"
#include "SampleMainWindow.h"

class SampleApp :
    public ACLWinApp
{
public:
    SampleApp();
    virtual ~SampleApp() throw();

    virtual bool InitInstance(IN HINSTANCE hInstance, IN int nCmdShow);
    //virtual int  Run(void);            // use default, it's good enough for us.
    virtual int  CleanupInstance(void);

protected:
    bool _RegisterWindowClasses(void);
    bool _CreateMainWindow(void);

private:
    ATOM				_atomMain; 
    SampleMainWindow*	_sampleMainWindow;
};
#endif // _SAMPLEAPP_H

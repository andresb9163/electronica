//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("R_MOD_USB.res");
USEFORM("Main.cpp", MainForm);
USEUNIT("pp2.cpp");
USEUNIT("USB.cpp");
USEUNIT("pp2_comp.cpp");
USEUNIT("grafico.cpp");
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->CreateForm(__classid(TMainForm), &MainForm);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        return 0;
}
//---------------------------------------------------------------------------

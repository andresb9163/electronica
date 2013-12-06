//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("Graficador.res");
USEFORM("formGrafico.cpp", formGraficos);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->CreateForm(__classid(TformGraficos), &formGraficos);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        return 0;
}
//---------------------------------------------------------------------------

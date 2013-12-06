//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("interfazCPP.res");
USEFORM("interfaz.cpp", formPrincipal);
USEUNIT("instruction.cpp");
USEUNIT("instruction_list.cpp");
USEUNIT("phase.cpp");
USEUNIT("phase_list.cpp");
USE("error.h", File);
USEUNIT("error.cpp");
USEUNIT("..\DLPortIO\dlportio\cbuilder\cbuilder.4\DLPortIO.cpp");
USEUNIT("..\DLPortIO\dlportio\cbuilder\cbuilder.4\TDLPortIO.cpp");
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->CreateForm(__classid(TformPrincipal), &formPrincipal);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        return 0;
}
//---------------------------------------------------------------------------

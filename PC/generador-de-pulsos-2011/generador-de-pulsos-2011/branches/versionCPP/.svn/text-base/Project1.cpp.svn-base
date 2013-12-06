//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("Project1.res");
USEUNIT("util.cpp");
USEUNIT("ad.cpp");
USEUNIT("bstrlib.cpp");
USEUNIT("dds.cpp");
USEUNIT("error.cpp");
USEUNIT("f_glist.cpp");
USEFORM("formGrafico.cpp", formGraficos);
USEUNIT("instruction.cpp");
USEUNIT("instruction_list.cpp");
USEUNIT("instruction_listCPP.cpp");
USEUNIT("instruction_sheet.cpp");
USEUNIT("instruction_stack.cpp");
USEUNIT("instructionCPP.cpp");
USEUNIT("interface.cpp");
USEFORM("interfaz.cpp", formPrincipal);
USEUNIT("lexer.cpp");
USEUNIT("lpt_windows.cpp");
USEUNIT("main_functions.cpp");
USEUNIT("parser.cpp");
USEUNIT("phase.cpp");
USEUNIT("phase_listCPP.cpp");
USEUNIT("phaseCPP.cpp");
USEUNIT("pp2.cpp");
USEUNIT("interfaz.cpp");
USELIB("C:\GTK\lib\glib-2.0.lib");
USELIB("glib-2.0.lib");
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

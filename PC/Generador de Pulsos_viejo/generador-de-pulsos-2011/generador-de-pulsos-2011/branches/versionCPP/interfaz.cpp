//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "interfaz.h"
#include "instructionCPP.h"
#include "instruction_listCPP.h"
#include "phase_listCPP.h"
#include "phaseCPP.h"
#include <sstream>
#include <iostream.h>
#include <fstream.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "main_functions.h"


#include "formGrafico.h"

using namespace std;

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TDLPortIO"
#pragma resource "*.dfm"

#define MAX_SAMPLES 100000
#define SYSCALL_MAIN_NAME "rmnController.exe"
#define SYSCALL_PARSE_NAME "mainParsePulseProgram"
#define SYSCALL_EXECUTE_NAME "mainRun"
#define FASES_FILE_PATH "fases.dat"
#define INSTRUCCIONES_FILE_PATH "instrucciones.dat"
#define DEFAULT_CONFIG_FILE "C\\config_conf";

TformPrincipal *formPrincipal;
unsigned int error;
stringstream *hojaPulsosPath;

//---------------------------------------------------------------------------

instruction_list *il;
phase_list *pl;

instruction *parseInstruction(char *src, unsigned int len){
    unsigned int i = 0, j = 0, id = 0, type = 0, duration = 0, number = 0;
    int shift = 0, data = 0;
    instruction *inst = NULL;
    char *aux = (char *)calloc(len, sizeof(char));

    while (src[i] != '\t' && i<len){
        aux[j] = src[i];
        j++;
        i++;
    }
    if(src[i] == '\t'){
        aux[j] = '\0';
        id = atoi(aux);
        j=0;
        i++;
    }else{
        return inst;
    }
    while (src[i] != '\t' && i<len){
        aux[j] = src[i];
        j++;
        i++;
    }
    if(src[i] == '\t'){
        aux[j] = '\0';
        type = atoi(aux);
        j=0;
        i++;
    }else{
        return inst;
    }
    while (src[i] != '\t' && i<len){
        aux[j] = src[i];
        j++;
        i++;
    }
    if(src[i] == '\t'){
        aux[j] = '\0';
        data = atoi(aux);
        j=0;
        i++;
    }else{
        return inst;
    }
    while (src[i] != '\t' && i<len){
        aux[j] = src[i];
        j++;
        i++;
    }
    if(src[i] == '\t'){
        aux[j] = '\0';
        duration = atoi(aux);
        j=0;
        i++;
    }else{
        return inst;
    }
    while (src[i] != '\t' && i<len){
        aux[j] = src[i];
        j++;
        i++;
    }
    if(src[i] == '\t'){
        aux[j] = '\0';
        number = atoi(aux);
        j=0;
        i++;
    }else{
        return inst;
    }
    while (src[i] != '\0' && i<len){
        aux[j] = src[i];
        j++;
        i++;
    }
    if(src[i] == '\0'){
        aux[j] = '\0';
        shift = atoi(aux);
        j=0;
        i++;
    }else{
        return inst;
    }
    inst = new instruction(id, type, data);
    inst->set_number(number);
    inst->set_duration(duration);
    inst->phase_add_shift(shift);
    return inst;
}

phase *parsePhase(char *src, unsigned int len){
        phase *ph = NULL;
        char *aux = (char *)calloc(len, sizeof(char));
        unsigned int i = 0, j = 0, id = 0;
        float value = 0.;

        while (src[i] != '\t' && i<len){
                aux[j] = src[i];
                j++;
                i++;
        }
        if(src[i] == '\t'){
                aux[j] = '\0';
                id = atoi(aux);
                j=0;
                i++;
        }else{
                return ph;
        }
        ph = new phase(id);
        while(src[i] != '\0' && i<len){
            while (src[i] != '\t' && src[i] != '\0' && i<len){
                aux[j] = src[i];
                j++;
                i++;
            }
            if(src[i] == '\t' || src[i] == '\0'){
                aux[j] = '\0';
                value = atof(aux);
                ph->phase_add_value(value);
                j=0;
                i++;
            }else{
                ph = NULL;
            }
        }
        return ph;
}

int checkPulseSheetPath(){
    int res = NO_ERROR;
    if (formPrincipal->campoHojaPulsos->Text == ""){
        res = ERROR_NO_PULSE_SHEET_PATH;
    }
    return res;
}

string getInstruction(instruction *inst){
    stringstream *str = new stringstream();
    bool found = false;
    string res;
    int i = 0; 
    switch(inst->get_type()){
        case LOOP_INST_CODE:{
            *str << "loop "<< inst->get_data()<<" times";
            if(inst->is_phase_shifted()){
                *str << "phase shifted";
            }
            res = str->str().c_str();
            break;
        }
        case ACQUIRE_INST_CODE:{
            *str << "acquire";
            break;
        }
        case PULSE_INST_CODE:{
            *str << "p" << inst->get_id() << "(ph";
            i = 0;
            while(!found && i<pl->length()){
                found = pl->nth_item(i)->phase_id() == inst->get_data();
                i++;
            }
            if(found){
                *str << pl->nth_item(i-1)->phase_id() << ")";
                res = str->str().c_str();
            }
            break;
        }
        case DELAY_INST_CODE:{
            *str << "d";
            *str << inst->get_id();
            res = str->str().c_str();
            break;
        }
        case ENDLOOP_INST_CODE:{
            *str << "endloop";
            res = str->str().c_str();
            break;
        }
        case END_INST_CODE:{
            *str << "end";
            res = str->str().c_str();
            break;
        }
    }
    return res;
}
void parseAllInstructions(char *path){
    ifstream myReadFile;
    myReadFile.open(path);
    char output[100];
    instruction *inst = NULL;

    if (myReadFile.is_open()) {
        while (!myReadFile.eof()) {

            myReadFile.getline(output, 100);
            if(strcmp(output, "\0") != 0){
                inst = parseInstruction(output, 100);
                il->add(inst);
            }
        }
    }
    myReadFile.close();
}

void parseAllPhases(char *path){
    ifstream myReadFile;
    myReadFile.open(path);
    char output[100];
    phase *ph = NULL;
    stringstream *str;
    int j = 0;
    if (myReadFile.is_open()) {
        while (!myReadFile.eof()) {

            myReadFile.getline(output, 100);
            if(strcmp(output, "\0") != 0){
                ph = parsePhase(output, 100);
                  str = new stringstream();
                 *str << "id:"<< ph->phase_id() << " values:";
                for(j=0;j<ph->phase_count_values();j++){
                        *str << ph->phase_nth_value(j) << " ";
                }
                pl->add(ph);
            }
        }
    }
    myReadFile.close();
}

void refreshDelayGrid(TStringGrid *grilla){
    unsigned int f = 1, i = 0;
    while(f<grilla->RowCount && i< il->length()){
        grilla->Cells[1][f] = il->nth_item(i)->get_duration();
        grilla->Cells[0][f] = getInstruction(il->nth_item(i)).c_str();
        i++;
        f++;
    }
}

string uintToString(unsigned int n){
        string s;
        stringstream aux;
        aux << n;
        return aux.str();
}


void refreshCombo(TComboBox *combo){
        TStringList *lst = new TStringList();
        stringstream *aux;
        int i = 0;
        phase *ph;

        for(i=0;i<pl->length();i++){
                aux = new stringstream();
                *aux << "ph";
                ph = pl->nth_item(i);
                *aux << ph->phase_id();
                lst->Add(aux->str().c_str());
                aux->~stringstream();
        }
        combo->Items = lst;
}

__fastcall TformPrincipal::TformPrincipal(TComponent* Owner)
        : TForm(Owner)
{
        StringGridDelays->Cells[0][0] = "Instruccion";
	StringGridDelays->Cells[1][0] = "Delay";
        il = new instruction_list();
        pl = new phase_list();

}
//---------------------------------------------------------------------------
void __fastcall TformPrincipal::botonCargarHojaPulsosClick(TObject *Sender)
{
        dialogoAbrirArchivo->Execute();
        hojaPulsosPath = new stringstream();
        *hojaPulsosPath << dialogoAbrirArchivo->FileName.c_str();
	campoHojaPulsos->Text = dialogoAbrirArchivo->FileName;
}
//---------------------------------------------------------------------------
void __fastcall TformPrincipal::botonParsearHojaClick(TObject *Sender)
{
    /*
    *   *  Llamar a sistema para obtener las fases e intrucciones en archivos pasados por parametro
    *   *  Leer de estos archivos los datos de fase e intruccion y almacenarlos en listas
    *   *  Actualizar la grilla de instrucciones y la lista de fases en la interfaz
    *   *  En caso de error mostrar el cartel correspondiente
    */

    error = checkPulseSheetPath();
    if(error != NO_ERROR){
        formPrincipal->botonEjecutar->Enabled = false;
        formPrincipal->botonGuardarAdquisiciones->Enabled = false;
        formPrincipal->Repeticiones->Enabled = false;
        formPrincipal->CheckBox2->Enabled = false;
        formPrincipal->CheckBox1->Enabled = false;
        formPrincipal->Edit1->Enabled = false;
        formPrincipal->campoDDSFrecuencia1->Enabled = false;
        formPrincipal->campoDDSFrecuencia2->Enabled = false;
        formPrincipal->botonEncenderDDS ->Enabled = false;
        formPrincipal->botonResetearDDS->Enabled = false;
        formPrincipal->botonApagarDDS->Enabled = false;
        formPrincipal->campoMuestrasPorSegundo->Enabled = false;
        formPrincipal->StringGridDelays->Enabled = false;
        formPrincipal->StringGrid1->Enabled = false;
        formPrincipal->ComboBox1->Enabled = false;
        ShowMessage(getMessage(error));
    }else{
        stringstream *str = new stringstream();
        *str << SYSCALL_MAIN_NAME;
        *str << " ";
        *str << SYSCALL_PARSE_NAME;
        *str << " ";
        *str << formPrincipal->campoHojaPulsos->Text.c_str();
        *str << " ";
        *str << FASES_FILE_PATH;
        *str << " ";
        *str << INSTRUCCIONES_FILE_PATH;
//        ShowMessage(str->str().c_str());
        error = system(str->str().c_str()); // AGREGAR A error.h TODAS LAS CONSTANTES DE ERROR QUE PUEDE DEVOLVER system()
        if(error != NO_ERROR){
            formPrincipal->botonEjecutar->Enabled = false;
            formPrincipal->botonGuardarAdquisiciones->Enabled = false;
            formPrincipal->Repeticiones->Enabled = false;
            formPrincipal->CheckBox2->Enabled = false;
            formPrincipal->CheckBox1->Enabled = false;
            formPrincipal->Edit1->Enabled = false;
            formPrincipal->campoDDSFrecuencia1->Enabled = false;
            formPrincipal->campoDDSFrecuencia2->Enabled = false;
            formPrincipal->botonEncenderDDS ->Enabled = false;
            formPrincipal->botonResetearDDS->Enabled = false;
            formPrincipal->botonApagarDDS->Enabled = false;
            formPrincipal->campoMuestrasPorSegundo->Enabled = false;
            formPrincipal->StringGridDelays->Enabled = false;
            formPrincipal->StringGrid1->Enabled = false;
            formPrincipal->ComboBox1->Enabled = false;
            ShowMessage(getMessage(error));
        }else{
            parseAllInstructions(INSTRUCCIONES_FILE_PATH);
            parseAllPhases(FASES_FILE_PATH);

            refreshDelayGrid(StringGridDelays);
            refreshCombo(ComboBox1);
            formPrincipal->botonEjecutar->Enabled = true;
            formPrincipal->botonGuardarAdquisiciones->Enabled = true;
            formPrincipal->Repeticiones->Enabled = true;
            formPrincipal->CheckBox2->Enabled = true;
            formPrincipal->CheckBox1->Enabled = true;
            formPrincipal->Edit1->Enabled = true;
            formPrincipal->campoDDSFrecuencia1->Enabled = true;
            formPrincipal->campoDDSFrecuencia2->Enabled = true;
            formPrincipal->botonEncenderDDS ->Enabled = true;
            formPrincipal->botonResetearDDS->Enabled = true;
            formPrincipal->botonApagarDDS->Enabled = true;
            formPrincipal->campoMuestrasPorSegundo->Enabled = true;
            formPrincipal->StringGridDelays->Enabled = true;
            formPrincipal->StringGrid1->Enabled = true;
            formPrincipal->ComboBox1->Enabled = true;
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TformPrincipal::botonCargarConfiguracionClick(
      TObject *Sender)
{
    dialogoAbrirArchivo->Execute();
    campoCargarConfig->Text = dialogoAbrirArchivo->FileName;
}
//---------------------------------------------------------------------------


void __fastcall TformPrincipal::botonCargarDelaysClick(TObject *Sender)
{
        error = system("C\\Project1.exe cargar_delays config_delays.dat");
        if(error != NO_ERROR){
                ShowMessage(getMessage(error));
        }
}
//---------------------------------------------------------------------------


void __fastcall TformPrincipal::ComboBox1Click(TObject *Sender)
{
       stringstream *st =  new stringstream();
       //*st << ComboBox1->Item
       //ShowMessage(st->str().c_str());
}
//---------------------------------------------------------------------------

int checkNumberOfSamples(){
    int res = NO_ERROR;
    if(formPrincipal->Repeticiones->Text == ""){
        res = ERROR_NO_NUMBER_OF_SAMPLES;
    }else if (!(StrToInt(formPrincipal->Repeticiones->Text) <= MAX_SAMPLES)){
        res = ERROR_TOO_MANY_SAMPLES;
    }
    return res;
}

int checkDDSFreq1(){
    int res = NO_ERROR;
    if (formPrincipal->campoDDSFrecuencia1->Text == ""){
        res = ERROR_DDS_FREQ_1;
    }
    return res;
}

int checkDDSFreq2(){
    int res = NO_ERROR;
    if (formPrincipal->campoDDSFrecuencia2->Text == ""){
        res = ERROR_DDS_FREQ_2;
    }
    return res;
}

int checkADMps(){
    int res = NO_ERROR;
    if (formPrincipal->campoMuestrasPorSegundo->Text == ""){
        res = ERROR_NO_MPS;
    }
    return res;
}

int checkADBufferSize(){
    int res = NO_ERROR;
    if(formPrincipal->grupoTamBuffer->ItemIndex == -1){
        res = ERROR_NO_AD_BUFFER_SIZE;
    }
    return res;
}

int checkADOperationMode(){
    int res = NO_ERROR;
    if(formPrincipal->grupoModoOperacion->ItemIndex == -1){
        res = ERROR_NO_AD_MODE;
    }
     return res;
}

bool stratsWith(char *str, char ch){
    bool res = false;
    if(str != NULL){
        if (strlen(str) > 0){
            res = (str[0] == ch);
        }
    }
    return res;
}

int checkDelays(){
    int res = NO_ERROR;

    int i;
    for(i=0;i<formPrincipal->StringGridDelays->RowCount;i++){
        if(res == NO_ERROR){
            if(formPrincipal->StringGridDelays->Cells[0][i] != ""){
                if(stratsWith(formPrincipal->StringGridDelays->Cells[0][i].c_str(), 'p') || stratsWith(formPrincipal->StringGridDelays->Cells[0][i].c_str(), 'd')){
                    if(atoi(formPrincipal->StringGridDelays->Cells[1][i].c_str()) <= 0){
                        res = ERROR_DELAYS_MISSING;
                    }
                }
            }
        }else{
            break;
        }
    }
    return res;
}

int checkConfig(){
    int res = NO_ERROR;
    if(formPrincipal->campoCargarConfig->Text == ""){
        res = ERROR_NO_CONFIG_PATH;
    }
    return res;
}

int checkSaveFile(){
    int res = NO_ERROR;
    if(formPrincipal->Edit1->Text == ""){
        res = ERROR_NO_SAVE_FILE_PATH;
    }
    return res;
}

void __fastcall TformPrincipal::botonEncenderDDSClick(TObject *Sender)
{
               formGraficos->Show();
}
//---------------------------------------------------------------------------

int checkParamFields(){
   int nSample = checkNumberOfSamples();
   int pulseSheet = checkPulseSheetPath();
   int ddsF1 = checkDDSFreq1();
   int ddsF2 = checkDDSFreq2();
   int adBufSize = checkADBufferSize();
   int adMps = checkADMps();
   int adOpMode = checkADOperationMode();

   if(nSample == NO_ERROR){
       if(pulseSheet == NO_ERROR){
           if(ddsF1 == NO_ERROR){
               if(ddsF2 == NO_ERROR){
                   if(adBufSize == NO_ERROR){
                       if(adMps == NO_ERROR){
                           if(adOpMode == NO_ERROR){
                               return NO_ERROR;
                           }else{
                               return adOpMode;
                           }
                       }else{
                           return adMps;
                       }
                   }else{
                       return adBufSize;
                   }
               }else{
                   return ddsF2;
               }
           }else{
               return ddsF1;
           }
        }else{
            return pulseSheet;
        }
   }else{
       return nSample;
   }
}

int getADBufferSize(){
        switch(formPrincipal->grupoTamBuffer->ItemIndex){
        case 0:{return 1;
                break;
        }
        case 1:{return  2;
                break;
        }
        case 2:{return  4;
                break;
        }
        case 3:{return  8;
                break;
        }
        case 4:{return  16;
                break;
        }
        case 5:{return  32;
                break;
        }
        case 6:{return  64;
                break;
        }
        case 7:{return  128;
                break;
        }
    }
}

int getADOperationMode(){
    switch(formPrincipal->grupoModoOperacion->ItemIndex){
        case 0:{return 0;
                break;
        }
        case 1:{return 1;
                break;
        }
    }
}

char *split(char *src, unsigned int len, char ch){
    unsigned int i = 0, j = 0;
    char * res = (char *)calloc(len, sizeof(char));
    for(i=0;i<len;i++){
        if(src[i] == ch){
            break;
        }else{
            res[j] = src[i];
            j++;
        }
    }
    return res;
}

unsigned int getDelay(TStringGrid *grid, unsigned int inst_type, unsigned int inst_id){
    int i = 0;
    stringstream *str;
    if(inst_type == PULSE_INST_CODE){
        for(i=1;i<grid->RowCount;i++){
            str = new stringstream();
            *str << "p" << inst_id;

            if (strcmp(split(grid->Cells[0][i].c_str(), strlen(grid->Cells[0][i].c_str()), '('), str->str().c_str()) == 0){
                str->~stringstream();
                return StrToInt(grid->Cells[1][i]);
            }
        }
    }else if (inst_type == inst_type == DELAY_INST_CODE){
        for(i=0;i<grid->RowCount;i++){
            str = new stringstream();
            *str << "d" << inst_id;
            if (split(grid->Cells[0][i].c_str(), strlen(grid->Cells[0][i].c_str()), '(') == str->str().c_str()){
                str->~stringstream();
                return StrToInt(grid->Cells[1][i]);
            }
        }
    }

    return 0;
}

char *buildConfigFile(){
    ofstream myOutputFile;
    stringstream *path = new stringstream();
    *path << DEFAULT_CONFIG_FILE;
    myOutputFile.open(path->str().c_str());
    //path->~stringstream();
    stringstream *str;
    instruction *inst;
    int i = 0;
    unsigned int delay = 0;
    if (myOutputFile.is_open()) {
        for(i=0;i<il->length();i++){
            str = new stringstream();
            inst = il->nth_item(i);
            if(inst->get_type() == PULSE_INST_CODE){
                *str << "p" << inst->get_id() << ":" << getDelay(formPrincipal->StringGridDelays, inst->get_type(), inst->get_id()) << "\n";
            }else if(inst->get_type() == DELAY_INST_CODE){
                *str << "d" << inst->get_id() << ":" << inst->get_duration() << "\n";
            }
            myOutputFile << str->str();
        }
        myOutputFile.close();
    }
    return DEFAULT_CONFIG_FILE;
}

int getOpMode(){
    switch(formPrincipal->grupoModoOperacion->ItemIndex){
        case 0:{return 0;
                break;
        }
        case 1:{return 1;
                break;
        }
        default: return -1;
     }
}

int getKPC(){
     switch(formPrincipal->grupoTamBuffer->ItemIndex){
        case 0:{return 1;
                break;
        }
        case 1:{return  2;
                break;
        }
        case 2:{return  4;
                break;
        }
        case 3:{return  8;
                break;
        }
        case 4:{return  16;
                break;
        }
        case 5:{return  32;
                break;
        }
        case 6:{return  64;
                break;
        }
        case 7:{return  128;
                break;
        }
     }
}

void __fastcall TformPrincipal::botonEjecutarClick(TObject *Sender)
{

    int nSamples, mps, DDSfreq1, DDSfreq2, ADbufSize, ADMode;
    string pulseSheetPath, configPath , savePath;

    error = checkParamFields();
    if (error != NO_ERROR){
        ShowMessage(getMessage(error));
    }else{
        nSamples = StrToInt(formPrincipal->Repeticiones->Text);
        pulseSheetPath = formPrincipal->campoHojaPulsos->Text.c_str();
        mps = StrToInt(formPrincipal->campoMuestrasPorSegundo->Text);
        DDSfreq1 = StrToInt(formPrincipal->campoDDSFrecuencia1->Text);
        DDSfreq2 = StrToInt(formPrincipal->campoDDSFrecuencia2->Text);
        ADbufSize = getADBufferSize();
        ADMode = getADOperationMode();
    }
    if((error == NO_ERROR) && (formPrincipal->CheckBox2->Checked)){
        error = checkConfig();
        if(error != NO_ERROR){
            ShowMessage(getMessage(error));
        }else{
            configPath = formPrincipal->campoCargarConfig->Text.c_str();
        }
    }else if (error == NO_ERROR){
        error = checkDelays();
        if(error != NO_ERROR){
            ShowMessage(getMessage(error));
        }else{
            //configPath = buildConfigFile();
        }
    }

    if((error == NO_ERROR) && (formPrincipal->CheckBox1->Checked)){
        error = checkSaveFile();
        if(error != NO_ERROR){
            ShowMessage(getMessage(error));
        }else{
            savePath = formPrincipal->Edit1->Text.c_str();
        }
    }else if (error == NO_ERROR){
        savePath = "_";
    }
    if(error == NO_ERROR){
        stringstream *str = new stringstream();
        *str << SYSCALL_MAIN_NAME;
        *str << " ";
        *str << SYSCALL_EXECUTE_NAME;
        *str << " ";
        *str << "sintaxis_ejemplo";
        *str << " ";
        *str << "config_conf";
        *str << " ";
        *str << formPrincipal->campoMuestrasPorSegundo->Text.c_str();
        *str << " ";
        *str << getKPC();
        *str << " ";
        *str << getOpMode();
        *str << " ";
        *str << formPrincipal->campoDDSFrecuencia1->Text.c_str();
        *str << " ";
        *str << formPrincipal->campoDDSFrecuencia2->Text.c_str();
        *str << " ";
        *str << savePath;
        *str << " ";
        *str << formPrincipal->Repeticiones->Text.c_str();
        //ShowMessage(str->str().c_str());
        main_run("sintaxis_ejemplo", "config_conf", 1000000, 2, 1, 5000000, 5000000, "_", 5);
        if(error != NO_ERROR){
            ShowMessage(getMessage(error));
        }else{
            ShowMessage("OK");
        }

    }
}

void __fastcall TformPrincipal::CheckBox1Click(TObject *Sender)
{

    formPrincipal->Edit1->Enabled = !formPrincipal->Edit1->Enabled;
    formPrincipal->botonGuardarAdquisiciones->Enabled = !formPrincipal->botonGuardarAdquisiciones->Enabled;
}
//---------------------------------------------------------------------------

void __fastcall TformPrincipal::CheckBox2Click(TObject *Sender)
{
    formPrincipal->campoCargarConfig->Enabled = !formPrincipal->campoCargarConfig->Enabled;
    formPrincipal->botonCargarConfiguracion->Enabled = !formPrincipal->botonCargarConfiguracion->Enabled;
}
//---------------------------------------------------------------------------


void __fastcall TformPrincipal::botonGuardarAdquisicionesClick(
      TObject *Sender)
{
    formPrincipal->dialogoAbrirArchivo->Execute();
    formPrincipal->Edit1->Text = dialogoAbrirArchivo->FileName; 
}
//---------------------------------------------------------------------------


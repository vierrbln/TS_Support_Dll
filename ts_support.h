#include <cvidef.h>
#include <ansi_c.h>
#include "tsapicvi.h"
#include <utility.h>
#include <formatio.h>
#include <userint.h>
#include <analysis.h>
#include "ts_support_uir.h"
#include "rspsam.h"
#include "swmgr.h"
#include "hrestim.h" 
#include "inifile.h"


void __stdcall tss_CheckColumnSyntax (CAObjHandle sequenceContext, char input[],
                                     char output[], short *errorOccurred, long *errorCode,
                                     char errorMessage[]);

void __stdcall tss_OpenDBFile (CAObjHandle sequenceContext, char fileName[],
                              short *errorOccurred, long *errorCode,
                              char errorMessage[]);

void __stdcall tss_WriteIntoDBFile (CAObjHandle sequenceContext, char content[],
                                   short *errorOccurred, long *errorCode,
                                   char errorMessage[]);

void __stdcall tss_CloseDBFile (CAObjHandle sequenceContext, short *errorOccurred,
                               long *errorCode, char errorMessage[]);

void __stdcall tss_DeleteDBFile (CAObjHandle sequenceContext, char fileName[],
                                short *errorOccurred, long *errorCode,
                                char errorMessage[]);

void __stdcall tss_SortArray (CAObjHandle sequenceContext, double numericArray[],
                             int arraySize, int direction, short *errorOccurred,
                             long *errorCode, char errorMessage[]);

void __stdcall tss_FileExists (CAObjHandle sequenceContext, char cFileName[],
                              int *fileExits, int *fileSize, short *errorOccurred,
                              long *errorCode, char errorMessage[]);

void __stdcall tss_ShowInitPanel (CAObjHandle sequenceContext,  int AlwaysOnTop,
                                 short *errorOccurred, long *errorCode,
                                 char errorMessage[]) ;

void __stdcall tss_SetTextforPanel (CAObjHandle sequenceContext, char text1[],  char text2[], unsigned int iColour, int AlwaysOnTop,
                                       short *errorOccurred, long *errorCode,
                                       char errorMessage[]) ;


void __stdcall tss_SetTextforInitPanel (CAObjHandle sequenceContext, char text[], int AlwaysOnTop,
                                       short *errorOccurred, long *errorCode,
                                       char errorMessage[])   ;

void __stdcall tss_HideInitPanel (CAObjHandle sequenceContext,
                                 short *errorOccurred, long *errorCode,
                                 char errorMessage[])  ;

void __stdcall tss_ReadFile (CAObjHandle sequenceContext, char *cFileName,
                            char *cContent, int iBufferSize, short *errorOccurred,
                            long *errorCode, char errorMessage[]);


void __stdcall tss_ReadFirstLineOfFile (CAObjHandle sequenceContext, char *cFileName,
                            char *cContent, int iBufferSize, short *errorOccurred,
                            long *errorCode, char errorMessage[]);

void __stdcall tss_WriteFile (CAObjHandle sequenceContext, char *cFileName,
                             char *cContent, short *errorOccurred,
                             long *errorCode, char errorMessage[]);

void __stdcall tss_WriteFileAppend (CAObjHandle sequenceContext, char *cFileName,
                             char *cContent, short *errorOccurred,
                             long *errorCode, char errorMessage[]);

void __stdcall tss_ShowMeasPanel (CAObjHandle sequenceContext, char text[], int alwaysOntop,
                                 short *errorOccurred, long *errorCode,
                                 char errorMessage[]);

void __stdcall tss_SetTextforMeasPanel (CAObjHandle sequenceContext, 
                                       double measurement, double lower,
                                       double upper, int alwaysOntop,
                                       int *pushed, short *errorOccurred,
                                       long *errorCode, char errorMessage[]);

void __stdcall tss_HideMeasPanel (CAObjHandle sequenceContext,
                                 short *errorOccurred, long *errorCode,
                                 char errorMessage[]);

void __stdcall tss_AppendTxtFile (CAObjHandle sequenceContext, char *cFileName,
                                 char *cContent, int iSignsMoveBackFilePointer, short *errorOccurred,
                                 long *errorCode, char errorMessage[]);

void __stdcall tss_AppendTxtToFileSpecial (CAObjHandle sequenceContext, char *cFileName,
                                 char *cContent, int iMaxCols, short *errorOccurred,
                                 long *errorCode, char errorMessage[]);

void __stdcall tss_AppendTxtSimple (CAObjHandle sequenceContext, char *cFileName,
                                   char *cContent, short *errorOccurred, long *errorCode,
                                   char errorMessage[]);

void __stdcall tss_DeleteLastLineTxt (CAObjHandle sequenceContext, char *cFileName,
                                 int iMaxCols, short *errorOccurred,
                                 long *errorCode, char errorMessage[]);

void __stdcall tss_MakeDirectory (CAObjHandle sequenceContext, char directory[],
                                 short *errorOccurred,
                                 long *errorCode, char errorMessage[]);

void __stdcall tss_CreateStationGlobals (CAObjHandle sequenceContext, 
										char nameOfPhyIni[], char nameOfAppIni[],
                                        char benchName[], int countOfPSAM,
                                        int countOfPICT, int countOfPFG,
                                        int countOfPDFT, int countOfPAM,
                                        int countOfPHDT, int countOfPSU,
                                        int countOfPIO2, short *errorOccurred,
                                        long *errorCode, char errorMessage[]);

void __stdcall tss_LoadAppIniFile (CAObjHandle sequenceContext,
                                  short *errorOccurred,
                                  long *errorCode, char errorMessage[]);

void __stdcall tss_LoadChannels (CAObjHandle sequenceContext,
                                  char nameOfAppIni[], short *errorOccurred,
                                  long *errorCode, char errorMessage[]);

void __stdcall tss_DeleteChannels (CAObjHandle sequenceContext,
                                  short *errorOccurred, long *errorCode,
                                  char errorMessage[]);

void __stdcall tss_WriteReport (CAObjHandle sequenceContext, char cPathName[], char cFileName[],
                               char cSerialNumber[], int append, char *cContent,
                               short *errorOccurred, long *errorCode,
                               char errorMessage[]);

void __stdcall tss_AdjustPanel_MeasVoltRMS (CAObjHandle sequenceContext,
                                           int PSAMHandle,
                                           int switchManagerHandle, double range, double Frequency,  
                                           double lowerLimit, double upperLimit,
                                           double lowerScale, double upperScale,
                                           char pinLow[], char pinHigh[],
                                           char busLow[], char busHigh[],
                                           double *lastMeasurement,
                                           int alwaysOntop, char title[],
                                           char text[], short *errorOccurred,
                                           long *errorCode, char errorMessage[]);

void __stdcall tss_PictureMessageBox (CAObjHandle sequenceContext, char cMessageText[], char cButtonText[], int FontSize,
                               char cPathToPicture[], int PictureWith, int PictureHeight,
                               short *errorOccurred, long *errorCode,
                               char errorMessage[]);


void __stdcall tss_FileToArray (CAObjHandle sequenceContext, char cFileAndPathName[], 
                               double dArray[], int iSize, int iGroups,  
                               short *errorOccurred, long *errorCode,
                               char errorMessage[]);

void __stdcall tss_SolveRMS (CAObjHandle sequenceContext,  
                               double InputArray[], int NumberOfElements,  double *RMSValue,
                               short *errorOccurred, long *errorCode,
                               char errorMessage[]);


void __stdcall tss_SolveMean (CAObjHandle sequenceContext, 
                               double InputArray[], int NumberOfElements,  double *MeanValue,
                               short *errorOccurred, long *errorCode,
                               char errorMessage[]);

void __stdcall tss_SolveMedian (CAObjHandle sequenceContext, 
                               double InputArray[], int NumberOfElements,  double *MedianValue,
                               short *errorOccurred, long *errorCode,
                               char errorMessage[]);

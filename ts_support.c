#include "ts_support.h"

int PanelHandle;
int giInitPanelHandle;
int giPSAHandle;

double gdLowerLimit;
double gdUpperLimit;

short gsErrorOccurred;
int giErrorCode;
char gcErrorMessage[1024];

FILE *pDataBaseFile;

void __stdcall tss_CheckColumnSyntax (CAObjHandle sequenceContext, char input[],
                                     char output[], short *errorOccurred, long *errorCode,
                                     char errorMessage[])
{
    char cChar;
	int idx, iLength, iResult;
	
	iLength = strlen(input);
	if (iLength == 0) return;
	
	for (idx = 0; idx < iLength; idx++)
	{
		cChar = input[idx];
		iResult = isalnum (cChar);
		if (iResult == 0)
		{ 
			output[idx] = '_';
		} else {
			output[idx] = cChar;
		}
	}
	output[idx] = '\0'; 
}

void __stdcall tss_OpenDBFile (CAObjHandle sequenceContext, char fileName[],
                              short *errorOccurred, long *errorCode,
                              char errorMessage[])
{
    int iExists, iFileSize;
	
	iExists = FileExists (fileName, &iFileSize);
	if (iExists) DeleteFile(fileName);
	pDataBaseFile = fopen(fileName, "w"); 
	if (!pDataBaseFile) {
		*errorOccurred = 1;
		*errorCode = 1001101;
		sprintf(errorMessage, "Could not open database file");
	}
}

void __stdcall tss_WriteIntoDBFile (CAObjHandle sequenceContext, char content[],
                                   short *errorOccurred, long *errorCode,
                                   char errorMessage[])
{
    int iCharsWritten;
	
	iCharsWritten = fprintf(pDataBaseFile, "%s", content);
	if (!iCharsWritten) {
		*errorOccurred = 1;
		*errorCode = 1001101;
		sprintf(errorMessage, "Could not open database file");
	}
}

void __stdcall tss_CloseDBFile (CAObjHandle sequenceContext, short *errorOccurred,
                               long *errorCode, char errorMessage[])
{
	int iStatus;
    iStatus = fclose (pDataBaseFile);
	if (iStatus) {
		*errorOccurred = 1;
		*errorCode = 1001101;
		sprintf(errorMessage, "Could not close database file");
	}
}

void __stdcall tss_DeleteDBFile (CAObjHandle sequenceContext, char fileName[],
                                short *errorOccurred, long *errorCode,
                                char errorMessage[])
{
    int iSuccess;
	
	iSuccess = DeleteFile(fileName); 
	if (!iSuccess) {
		*errorOccurred = 1;
		*errorCode = 1001101;
		sprintf(errorMessage, "Could not delete database file");
	}
}

void __stdcall tss_SortArray (CAObjHandle sequenceContext, double numericArray[],
                             int arraySize, int direction, short *errorOccurred,
                             long *errorCode, char errorMessage[])
{
    double *TempArray;
	  int	Status, idx;
		
	  TempArray = malloc(sizeof(double)*arraySize);	
		if (!TempArray)
		{
			*errorOccurred = 1;
			*errorCode = -10000212;
			sprintf(errorMessage, "Cannot allocate memory for temp array!");
		}
		Status = Sort (numericArray, arraySize, direction, TempArray);
		if(Status)
		{
			*errorOccurred = 1;
			*errorCode = Status;
			sprintf(errorMessage, "An error occurred while sorting array!");
		}
		for (idx = 0; idx < arraySize; idx++)
			numericArray[idx] = TempArray[idx];
		
		free(TempArray);
		
}

void __stdcall tss_FileExists (CAObjHandle sequenceContext, char cFileName[],
                              int *fileExits, int *fileSize, short *errorOccurred,
                              long *errorCode, char errorMessage[])
{
	  *fileExits = FileExists (cFileName, fileSize);
}

void __stdcall tss_ShowInitPanel (CAObjHandle sequenceContext, int AlwaysOnTop,
                                 short *errorOccurred, long *errorCode,
                                 char errorMessage[])
{
     giInitPanelHandle = LoadPanelEx (0, "ts_support_UIR.uir", PANEL, __CVIUserHInst);
     DisplayPanel (giInitPanelHandle);
     SetPanelAttribute (giInitPanelHandle, ATTR_BACKCOLOR, VAL_YELLOW); 
     if (AlwaysOnTop) SetPanelAttribute (giInitPanelHandle, ATTR_FLOATING, VAL_FLOAT_ALWAYS);
     if (!AlwaysOnTop) SetPanelAttribute (giInitPanelHandle, ATTR_FLOATING, VAL_FLOAT_NEVER);
     SetCtrlVal (giInitPanelHandle, PANEL_TEXTMSG, "");
     SetCtrlAttribute (giInitPanelHandle, PANEL_TEXTMSG, ATTR_TEXT_BGCOLOR, VAL_TRANSPARENT); 
     ProcessDrawEvents ();
}

void __stdcall tss_SetTextforPanel (CAObjHandle sequenceContext, char text1[],  char text2[], unsigned int iColour, int AlwaysOnTop,
                                       short *errorOccurred, long *errorCode,
                                       char errorMessage[])
                                               
{
    char cBuffer[1024];
    
    sprintf(cBuffer, "\n%s\n%s", text1, text2); 
	SetPanelAttribute (giInitPanelHandle, ATTR_BACKCOLOR, iColour);
    SetCtrlVal (giInitPanelHandle, PANEL_TEXTMSG, cBuffer);
    if (AlwaysOnTop) SetPanelAttribute (giInitPanelHandle, ATTR_FLOATING, VAL_FLOAT_ALWAYS);
    if (!AlwaysOnTop) SetPanelAttribute (giInitPanelHandle, ATTR_FLOATING, VAL_FLOAT_NEVER);
    ProcessDrawEvents (); 
    Delay(0.2);
}

void __stdcall tss_SetTextforInitPanel (CAObjHandle sequenceContext, char text[],  int AlwaysOnTop,
                                       short *errorOccurred, long *errorCode,
                                       char errorMessage[])
                                               
{
    char cBuffer[1024];
    
    sprintf(cBuffer, "\nInitialize:\n%s", text); 
    
    SetCtrlVal (giInitPanelHandle, PANEL_TEXTMSG, cBuffer);
    if (AlwaysOnTop) SetPanelAttribute (giInitPanelHandle, ATTR_FLOATING, VAL_FLOAT_ALWAYS);
    if (!AlwaysOnTop) SetPanelAttribute (giInitPanelHandle, ATTR_FLOATING, VAL_FLOAT_NEVER);
    ProcessDrawEvents (); 
    Delay(0.2);
}



void __stdcall tss_HideInitPanel (CAObjHandle sequenceContext,
                                 short *errorOccurred, long *errorCode,
                                 char errorMessage[])
{
    //HidePanel (PanelHandle); 
	ProcessDrawEvents (); 
    ProcessSystemEvents (); 
    Delay(0.2);
	DiscardPanel (giInitPanelHandle);
}

void __stdcall tss_ReadFile (CAObjHandle sequenceContext, char *cFileName,
                            char *cContent, int iBufferSize, short *errorOccurred,
                            long *errorCode, char errorMessage[])
{
	int idx;
	char cChar;
	
	FILE *pFile;
	pFile = fopen (cFileName, "r");
	if(!pFile)
	{
		*errorOccurred = 1;
		*errorCode = -102;
		sprintf(errorMessage,"Could not read file: %s", cFileName); 
		return;
	}
	idx = 0;
	while (!feof(pFile))
	{
		cChar = fgetc(pFile);
		if (!feof(pFile)) 
		{
			cContent[idx] = cChar;
			idx++;
			if (idx >= iBufferSize)
			{
				*errorOccurred = 1;
				*errorCode = -107;
				sprintf(errorMessage,"Buffer size to small"); 
				return;
			}
		}
	}
	cContent[idx] = '\0';
	fclose(pFile);
    
}


void __stdcall tss_ReadFirstLineOfFile (CAObjHandle sequenceContext, char *cFileName,
                            char *cContent, int iBufferSize, short *errorOccurred,
                            long *errorCode, char errorMessage[])
{
	int idx;
	char cChar;
	
	FILE *pFile;
	pFile = fopen (cFileName, "r");
	if(!pFile)
	{
		*errorOccurred = 1;
		*errorCode = -102;
		sprintf(errorMessage,"Could not read file: %s", cFileName); 
		return;
	}
	idx = 0; cChar = 0;
	while (cChar != '\n')
	{
		cChar = fgetc(pFile);
		if (!feof(pFile)) 
		{
			cContent[idx] = cChar;
			idx++;
			if (idx >= iBufferSize)
			{
				*errorOccurred = 1;
				*errorCode = -107;
				sprintf(errorMessage,"Buffer size to small"); 
				return;
			}
		} else {
			break;
		}
	}
	cContent[idx - 1] = '\0';
	fclose(pFile);
    
}

void __stdcall tss_WriteFile (CAObjHandle sequenceContext, char *cFileName,
                             char *cContent, short *errorOccurred,
                             long *errorCode, char errorMessage[])
{
     int idx;
	 int iLength;
	 FILE *pFile;
	 char cErrorMessage[1024];
	 
	 iLength = strlen(cContent);
	 if(iLength == 0)
     {
        *errorOccurred = 1;
        *errorCode = -101;
        sprintf(errorMessage,"Variable Content has no content. Please check your parameters!"); 
        return;
     }
	 
	 pFile = fopen (cFileName, "w");
     if(!pFile)
     {
        *errorOccurred = 1;
        *errorCode = -102;
        sprintf(errorMessage,"Could not write/create file: %s", cFileName); 
        return;
     }
	 
	 for (idx = 0; idx < iLength; idx++)
	 {
		 switch (cContent[idx])
		 {
			case '%': 	fputc('%', pFile);  
						if(errno)
						{
							sprintf(cErrorMessage, "Fehler beim Schreiben vom Prozentzeichen - Index %d", idx);
							MessagePopup("SW-ITS", cErrorMessage);
							idx = iLength + 1;
						}
					  	break;
			case '\\': 	fputc('\\', pFile); 
						if(errno)
						{
							sprintf(cErrorMessage, "Fehler beim Schreiben vom BackSlash - Index %d", idx);
							MessagePopup("SW-ITS", cErrorMessage);
							idx = iLength + 1;
						}
  					  	break;
			default: 	fputc(cContent[idx], pFile); 
						break;  
		 }
	 }
	 
     /*if(errno)
     {
        *errorOccurred = 1;
        *errorCode = -103;
        sprintf(errorMessage,"An error occurred while writing into file: %s", cFileName); 
		fclose(pFile);  
        return;
     }   */
	 
     fclose(pFile);
}

void __stdcall tss_WriteFileAppend (CAObjHandle sequenceContext, char *cFileName,
                             char *cContent, short *errorOccurred,
                             long *errorCode, char errorMessage[])
{
     int idx;
	 int iLength;
	 FILE *pFile;
	 char cErrorMessage[1024];
	 
	 iLength = strlen(cContent);
	 if(iLength == 0)
     {
        *errorOccurred = 1;
        *errorCode = -101;
        sprintf(errorMessage,"Variable Content has no content. Please check your parameters!"); 
        return;
     }
	 
	 pFile = fopen (cFileName, "a");
     if(!pFile)
     {
        *errorOccurred = 1;
        *errorCode = -102;
        sprintf(errorMessage,"Could not write/create file: %s", cFileName); 
        return;
     }
	 
	 for (idx = 0; idx < iLength; idx++)
	 {
		 switch (cContent[idx])
		 {
			case '%': 	fputc('%', pFile);  
						if(errno)
						{
							sprintf(cErrorMessage, "Fehler beim Schreiben vom Prozentzeichen - Index %d", idx);
							MessagePopup("SW-ITS", cErrorMessage);
							idx = iLength + 1;
						}
					  	break;
			case '\\': 	fputc('\\', pFile); 
						if(errno)
						{
							sprintf(cErrorMessage, "Fehler beim Schreiben vom BackSlash - Index %d", idx);
							MessagePopup("SW-ITS", cErrorMessage);
							idx = iLength + 1;
						}
  					  	break;
			default: 	fputc(cContent[idx], pFile); 
						break;  
		 }
	 }
	 
     /*if(errno)
     {
        *errorOccurred = 1;
        *errorCode = -103;
        sprintf(errorMessage,"An error occurred while writing into file: %s", cFileName); 
		fclose(pFile);  
        return;
     }   */
	 
     fclose(pFile);
     
}

void __stdcall tss_ShowMeasPanel (CAObjHandle sequenceContext, char text[], int AlwaysOnTop,
                                 short *errorOccurred, long *errorCode,
                                 char errorMessage[])
{
     PanelHandle = LoadPanelEx (0, "ts_support_UIR.uir", PANEL_2, __CVIUserHInst);
     DisplayPanel (PanelHandle);
     SetPanelAttribute (PanelHandle, ATTR_BACKCOLOR, VAL_RED); 
     if (AlwaysOnTop) SetPanelAttribute (PanelHandle, ATTR_FLOATING, VAL_FLOAT_ALWAYS);
     if (!AlwaysOnTop) SetPanelAttribute (PanelHandle, ATTR_FLOATING, VAL_FLOAT_NEVER);
     SetCtrlVal (PanelHandle, PANEL_TEXTMSG, "");
     SetCtrlAttribute (PanelHandle, PANEL_2_TEXTMSG, ATTR_TEXT_BGCOLOR, VAL_TRANSPARENT); 
     SetCtrlAttribute (PanelHandle, PANEL_2_FORTFAHRE, ATTR_LABEL_BGCOLOR, VAL_TRANSPARENT);
     SetCtrlAttribute (PanelHandle, PANEL_2_FORTFAHRE, ATTR_LABEL_TEXT, text);
     ProcessDrawEvents ();    
}

void __stdcall tss_SetTextforMeasPanel (CAObjHandle sequenceContext,
                                       double measurement, double lower,
                                       double upper, int AlwaysOnTop,
                                       int *pushed, short *errorOccurred,
                                       long *errorCode, char errorMessage[])
{
    int Panel;
    int Item;
    char cBuffer[1024];
    *pushed = 0;
    sprintf(cBuffer, "Aktueller Messwert:\n%.2f V  <   %.2f V  <   %.2f V", lower, measurement, upper); 
    
    SetCtrlVal (PanelHandle, PANEL_2_TEXTMSG, cBuffer);
    if ((lower < measurement) && (measurement < upper))
    {
        SetPanelAttribute (PanelHandle, ATTR_BACKCOLOR, VAL_GREEN);
    }else
    {
        SetPanelAttribute (PanelHandle, ATTR_BACKCOLOR, VAL_RED);
    }
    if (AlwaysOnTop) SetPanelAttribute (PanelHandle, ATTR_FLOATING, VAL_FLOAT_ALWAYS);
    if (!AlwaysOnTop) SetPanelAttribute (PanelHandle, ATTR_FLOATING, VAL_FLOAT_NEVER);
    ProcessDrawEvents (); 
    Delay(0.5); 
    GetUserEvent (0, &Panel, &Item);
    if ((Panel == PanelHandle) && (Item == PANEL_2_FORTFAHRE)) *pushed = 1;
}

void __stdcall tss_HideMeasPanel (CAObjHandle sequenceContext,
                                 short *errorOccurred, long *errorCode,
                                 char errorMessage[])
{
    DiscardPanel (PanelHandle);    
}

void __stdcall tss_AppendTxtFile (CAObjHandle sequenceContext, char *cFileName,
                                 char *cContent, int iSignsMoveBackFilePointer, short *errorOccurred,
                                 long *errorCode, char errorMessage[])
{
	 FILE *pFileSource;
	 FILE *pFileTarget;
	 int iFileSize;
	 char cTempFileName[1024];
	 char cDrive[10];
	 char cDir[1024];
	 char cFile[1024];
	 char cChar;
	 int idx;
	 
	 SplitPath (cFileName, cDrive, cDir, cFile);
	 sprintf(cTempFileName, "%s%sTempFile.tmp", cDrive, cDir);
	 
	 idx = FileExists(cTempFileName, &iFileSize); 
	 if (idx == 1)
	 {
       	idx = DeleteFile(cTempFileName);
		 if (idx < 0)
		 {
	        *errorOccurred = 1;
	        *errorCode = -100;
	        sprintf(errorMessage,"Could not delete temp file before operation file: %s", cTempFileName); 
	        return;
	     } 
     }	 
	  
	 FileExists(cFileName, &iFileSize);
     pFileSource = fopen (cFileName, "r");
     if(!pFileSource)
     {
        *errorOccurred = 1;
        *errorCode = -100;
        sprintf(errorMessage,"Could not open file: %s", cFileName); 
        return;
     }
	 rewind(pFileSource);
	 
	 pFileTarget = fopen(cTempFileName, "w");
	 if(!pFileTarget)
     {
        *errorOccurred = 1;
        *errorCode = -100;
        sprintf(errorMessage,"Could not open temp file: %s", cTempFileName); 
        return;
     }
	 
	 
	 for (idx = 0; idx < (iFileSize - iSignsMoveBackFilePointer); idx++)
	 {
		cChar = fgetc(pFileSource);
		if(errno)
	     {
	        *errorOccurred = 1;
	        *errorCode = -100;
	        sprintf(errorMessage,"An error occurred while reading from source file: %s", cFileName); 
	        return;
	     }
		fputc(cChar, pFileTarget);
		if(errno)
	     {
	        *errorOccurred = 1;
	        *errorCode = -100;
	        sprintf(errorMessage,"An error occurred while writing into temp file: %s", cTempFileName); 
	        return;
	     }
     
	 }
	 
	 fprintf(pFileTarget, cContent);
	 if(errno)
     {
        *errorOccurred = 1;
        *errorCode = -100;
        sprintf(errorMessage,"An error occurred while writing into temp file: %s", cTempFileName); 
        return;
     }
	 
     fclose(pFileSource);
	 fclose(pFileTarget);
	 
	 idx = DeleteFile(cFileName);
	 if (idx < 0)
	 {
        *errorOccurred = 1;
        *errorCode = -100;
        sprintf(errorMessage,"Coudl not delete source file: %s", cFileName); 
        return;
     }
	 
	 idx = RenameFile (cTempFileName, cFileName);
	 if (idx < 0)
	 {
        *errorOccurred = 1;
        *errorCode = -100;
        sprintf(errorMessage,"Coudl not rename temp file: %s", cTempFileName); 
        return;
     }
    
    
}

void __stdcall tss_AppendTxtToFileSpecial (CAObjHandle sequenceContext, char *cFileName,
                                 char *cContent, int iMaxCols, short *errorOccurred,
                                 long *errorCode, char errorMessage[])
{
	 FILE *pFileSource;
	 FILE *pFileTarget;
	 int iFileSize;
	 int iFileHandle;
	 int iBytesRead;
	 char cTempFileName[1024];
	 char cLine[2048];
	 char cCR[2];
	 char cDrive[10];
	 char cDir[1024];
	 char cFile[1024];
	 char cChar;
	 char cLast;
	 
	 char *cBuffer;
	 
	 int idx;
	 int iCountOfLines;
	 int iCountOfCols;
	 
	 SplitPath (cFileName, cDrive, cDir, cFile);
	 sprintf(cTempFileName, "%s%sTempFile.tmp", cDrive, cDir);
	 
	 idx = FileExists(cTempFileName, &iFileSize); 
	 if (idx == 1)
	 {
       	idx = DeleteFile(cTempFileName);
		 if (idx < 0)
		 {
	        *errorOccurred = 1;
	        *errorCode = -100;
	        sprintf(errorMessage,"Could not delete temp file before operation file: %s", cTempFileName); 
	        return;
	     } 
     }	 
	  
	 FileExists(cFileName, &iFileSize);
		 
	 pFileSource = fopen(cFileName, "r");
	 if(!pFileSource)
     {
        *errorOccurred = 1;
        *errorCode = -100;
        sprintf(errorMessage,"Could not open original file: %s", cFileName); 
        return;
     }
	 
	 pFileTarget = fopen(cTempFileName, "w");
   if(!pFileTarget)
   {
      *errorOccurred = 1;
      *errorCode = -100;
      sprintf(errorMessage,"Could not open temp file: %s", cTempFileName); 
      return;
   }
	 
	 idx = 0;
	 iCountOfLines = 0;
	 iCountOfCols = 0;
	 cCR[0] = '\n';
	 cCR[1] = '\0';
	 
	 while (!feof(pFileSource))
	 {
		 cChar = fgetc(pFileSource);
		 if (cChar == '\n')
		 {
			 		cLine[idx] = '\0';  
					//strcat(cBuffer, cLine);
					if ((strlen(cLine) < iMaxCols) && (iCountOfLines > 0))
					{
						cLine[idx] = '\0'; 
						fprintf(pFileTarget, cLine);
						break;
					} else {
						cLine[idx] = '\n';
					    idx++;
					    cLine[idx] = '\0';
						//strcat(cBuffer, cCR);
						fprintf(pFileTarget, cLine);
						//fprintf(pFileTarget, '\n');
						idx = 0;
						iCountOfLines++;
					}
		 } else {
			 cLine[idx] = cChar;
			 idx++;
			 if (cChar == '%')
			 {
				 cLine[idx] = cChar;
			 	 idx++;
			 }
		 }
			 
		 
	 }
	 
	fclose(pFileSource);
		 
	
	
	
	 
	 
	 /*fprintf(pFileTarget, cBuffer);
	 if(errno)
     {
        *errorOccurred = 1;
        *errorCode = -101;
        sprintf(errorMessage,"An error occurred while writing into temp file: %s", cTempFileName); 
        return;
     }			*/


	 
	 fprintf(pFileTarget, cContent);
	 /*if(errno)
     {
        *errorOccurred = 1;
        *errorCode = -102;
        sprintf(errorMessage,"An error occurred while writing into temp file: %s", cTempFileName); 
        return;
     }	*/
	 
     
	 

	 //free(cBuffer);
	 
	 //fclose(pFileSource);
	 fclose(pFileTarget);

	 
	 idx = DeleteFile(cFileName);
	 if (idx < 0)
	 {
        *errorOccurred = 1;
        *errorCode = -100;
        sprintf(errorMessage,"Coudl not delete source file: %s", cFileName); 
        return;
     }
	 
	 idx = RenameFile (cTempFileName, cFileName);
	 if (idx < 0)
	 {
        *errorOccurred = 1;
        *errorCode = -100;
        sprintf(errorMessage,"Coudl not rename temp file: %s", cTempFileName); 
        return;
     }
    
    
}

void __stdcall tss_AppendTxtSimple (CAObjHandle sequenceContext, char *cFileName,
                                   char *cContent, short *errorOccurred, long *errorCode,
                                   char errorMessage[])
{
     FILE *pFileSource; 
	 
	 pFileSource = fopen (cFileName, "a");
	 fprintf(pFileSource, cContent);
	 fclose(pFileSource);
}


void __stdcall tss_DeleteLastLineTxt (CAObjHandle sequenceContext, char *cFileName,
                                 int iCutAtLineBefore, short *errorOccurred,
                                 long *errorCode, char errorMessage[])
{
	 FILE *pFileSource;
	 FILE *pFileTarget;
	 int iFileSize;
	 int iFileHandle;
	 int iBytesRead;
	 char cTempFileName[1024];
	 char cLine[2048];
	 char cCR[2];
	 char cDrive[10];
	 char cDir[1024];
	 char cFile[1024];
	 char cChar;
	 char cLast;
	 
	 char *cBuffer;
	 
	 int idx;
	 int iCountOfLines;
	 int iLineIndex;
	 int iCountOfCols;
	 
	 SplitPath (cFileName, cDrive, cDir, cFile);
	 sprintf(cTempFileName, "%s%sTempFile.tmp", cDrive, cDir);
	 
	 idx = FileExists(cTempFileName, &iFileSize); 
	 if (idx == 1)
	 {
       	idx = DeleteFile(cTempFileName);
		 if (idx < 0)
		 {
	        *errorOccurred = 1;
	        *errorCode = -100;
	        sprintf(errorMessage,"Could not delete temp file before operation file: %s", cTempFileName); 
	        return;
	     } 
     }	 
	  
	 FileExists(cFileName, &iFileSize);
		 
	 pFileSource = fopen(cFileName, "r");
	 if(!pFileSource)
     {
        *errorOccurred = 1;
        *errorCode = -100;
        sprintf(errorMessage,"Could not open original file: %s", cFileName); 
        return;
     }
	 
	 pFileTarget = fopen(cTempFileName, "w");
   if(!pFileTarget)
   {
      *errorOccurred = 1;
      *errorCode = -100;
      sprintf(errorMessage,"Could not open temp file: %s", cTempFileName); 
      return;
   }
	 
	 iCountOfLines = 0;
	 while (!feof(pFileSource))
	 {
		 cChar = fgetc(pFileSource);
		 if (cChar == '\n')
		 {
			 		iCountOfLines++;
		 }
	 }
	 
	 rewind(pFileSource);
	 iLineIndex = iCountOfLines - iCutAtLineBefore; 
	 
	 idx = 0;
	 iCountOfLines = 0;
	 while (!feof(pFileSource))
	 {
		 cChar = fgetc(pFileSource);
		 if (cChar == '\n')
		 {
			 		cLine[idx] = '\0';  
					//strcat(cBuffer, cLine);
					if (iCountOfLines == iLineIndex)
					{
						//cLine[0] = '\n';
						//cLine[1] = '\0'; 
						//fprintf(pFileTarget, cLine);
						break;
					} else {
						cLine[idx] = '\n';
					  idx++;
					  cLine[idx] = '\0';
						//strcat(cBuffer, cCR);
						fprintf(pFileTarget, cLine);
						//fprintf(pFileTarget, '\n');
						idx = 0;
						iCountOfLines++;
					}
		 } else {
			 cLine[idx] = cChar;
			 idx++;
 			 if (cChar == '%') 
			 {
				cLine[idx] = cChar;
			 	idx++;
			 }
				 

		 }
			 
		 
	 }
	 
	fclose(pFileSource);
		 
	
	
	
	 
	 
	 /*fprintf(pFileTarget, cBuffer);
	 if(errno)
     {
        *errorOccurred = 1;
        *errorCode = -101;
        sprintf(errorMessage,"An error occurred while writing into temp file: %s", cTempFileName); 
        return;
     }			*/


	 
	 
	 
     
	 

	 //free(cBuffer);
	 
	 //fclose(pFileSource);
	 fclose(pFileTarget);

	 
	 idx = DeleteFile(cFileName);
	 if (idx < 0)
	 {
        *errorOccurred = 1;
        *errorCode = -100;
        sprintf(errorMessage,"Coudl not delete source file: %s", cFileName); 
        return;
     }
	 
	 idx = RenameFile (cTempFileName, cFileName);
	 if (idx < 0)
	 {
        *errorOccurred = 1;
        *errorCode = -100;
        sprintf(errorMessage,"Coudl not rename temp file: %s", cTempFileName); 
        return;
     }
    
    
}


void __stdcall tss_MakeDirectory (CAObjHandle sequenceContext, char cDirectory[],
                                 short *pErrorOccurred,
                                 long *pErrorCode, char errorMessage[])
{
	int iLength, iStatus;	
	 char cPathName[1024];
		
	  sprintf(cPathName, "%s", cDirectory);
	  iLength = strlen(cPathName);
	  if (cPathName[iLength-1] == '\\') cPathName[iLength-1] = '\0';
	
	  //Check the path name
    iStatus = SetDir (cPathName);
    if (iStatus) 
	  {
		
			iStatus = MakeDir (cPathName);
		    if (iStatus) 
		    {
		        *pErrorOccurred = TRUE;
		        *pErrorCode = iStatus; 
		        sprintf(errorMessage, "Cannot create path for report file!");
		        return;
		    }	  
    }
}

void __stdcall tss_CreateStationGlobals (CAObjHandle sequenceContext,
                                        char nameOfPhyIni[], char nameOfAppIni[],
                                        char benchName[], int countOfPSAM,
                                        int countOfPICT, int countOfPFG,
                                        int countOfPDFT, int countOfPAM,
                                        int countOfPHDT, int countOfPSU,
                                        int countOfPIO2, short *errorOccurred,
                                        long *errorCode, char errorMessage[])
{
    int idx, idy, idz;
    
    char cVarName[1024];
    char cCopyBench[1024];
    
    CAObjHandle dummy;
    HRESULT pHResult;
    ERRORINFO pErrorInfo;
	
    //Check parameters
    if (!strlen(nameOfPhyIni)) 
    {
        *errorOccurred = TRUE;
        *errorCode = -13000;
        sprintf(errorMessage, "You must pass path and filename of the physical-ini-file as first parameter");
        
    }
    if (!strlen(nameOfAppIni)) 
    {
        *errorOccurred = TRUE;
        *errorCode = -13001;
        sprintf(errorMessage, "You must pass path and filename of the application-ini-file as second parameter");
        
    }
    if (!strlen(benchName)) 
    {
        *errorOccurred = TRUE;
        *errorCode = -13002;
        sprintf(errorMessage, "You must pass a valid bench name as third parameter");

    }
    if ((!countOfPSAM) || (countOfPSAM < 0))
    {
        *errorOccurred = TRUE;
        *errorCode = -13002;
        sprintf(errorMessage, "Check parameter countOfPSAM. Value must be greater than zero.");
    }
    if (countOfPICT < 0)
    {
        *errorOccurred = TRUE;
        *errorCode = -13002;
        sprintf(errorMessage, "Check parameter countOfPICT. Value could not be lower than zero.");
    }
    if (countOfPFG < 0)
    {
        *errorOccurred = TRUE;
        *errorCode = -13002;
        sprintf(errorMessage, "Check parameter countOfPICT. Value could not be lower than zero.");
    }
    if (countOfPDFT < 0)
    {
        *errorOccurred = TRUE;
        *errorCode = -13002;
        sprintf(errorMessage, "Check parameter countOfPICT. Value could not be lower than zero.");
    }
    if (countOfPAM < 0)
    {
        *errorOccurred = TRUE;
        *errorCode = -13002;
        sprintf(errorMessage, "Check parameter countOfPICT. Value could not be lower than zero.");
    }
    if (countOfPHDT < 0)
    {
        *errorOccurred = TRUE;
        *errorCode = -13002;
        sprintf(errorMessage, "Check parameter countOfPICT. Value could not be lower than zero.");
    }
    if (countOfPSU < 0)
    {
        *errorOccurred = TRUE;
        *errorCode = -13002;
        sprintf(errorMessage, "Check parameter countOfPICT. Value could not be lower than zero.");
    }
    if (countOfPIO2 < 0)
    {
        *errorOccurred = TRUE;
        *errorCode = -13002;
        sprintf(errorMessage, "Check parameter countOfPICT. Value could not be lower than zero.");
    }
    
    
    
    pHResult = TS_PropertySetValString (sequenceContext, &pErrorInfo, "StationGlobals.TSVP.PhysicalIniFile", TS_PropOption_InsertIfMissing, nameOfPhyIni); 
    if (pHResult)
    {
        *errorOccurred = 1;
        *errorCode = pHResult;
        CA_GetAutomationErrorString (pHResult, errorMessage, 1024);
        return;
    }
    pHResult = TS_PropertySetValString (sequenceContext, &pErrorInfo, "StationGlobals.TSVP.ApplicationIniFile", TS_PropOption_InsertIfMissing, nameOfAppIni); 
    if (pHResult)
    {
        *errorOccurred = 1;
        *errorCode = pHResult;
        CA_GetAutomationErrorString (pHResult, errorMessage, 1024);
        return;
    }
    
    idx = 0;
    idz = 1;
    while (benchName[idx] != '\0')
    {
        idy = 0;
        while ((benchName[idx] != '|') && (benchName[idx] != '\0'))
        {
            cCopyBench[idy] = benchName[idx];   
            idx++;
            idy++;
        }
        cCopyBench[idy] = '\0';
        sprintf(cVarName, "StationGlobals.TSVP.BenchName%i", idz);
        pHResult = TS_PropertySetValString (sequenceContext, &pErrorInfo, cVarName, TS_PropOption_InsertIfMissing, cCopyBench); 
        if (pHResult)
        {
            *errorOccurred = 1;
            *errorCode = pHResult;
            CA_GetAutomationErrorString (pHResult, errorMessage, 1024);
            return;
        }
        if(benchName[idx] == '\0') break;
        idx++;
        idz++;
    }
    pHResult = TS_PropertySetValNumber (sequenceContext, &pErrorInfo, "StationGlobals.TSVP.swmgr_ID", TS_PropOption_InsertIfMissing, 0); 
    if (pHResult)
    {
        *errorOccurred = 1;
        *errorCode = pHResult;
        CA_GetAutomationErrorString (pHResult, errorMessage, 1024);
        return;
    }
    pHResult = TS_PropertySetValNumber (sequenceContext, &pErrorInfo, "StationGlobals.TSVP.dmm_ID", TS_PropOption_InsertIfMissing, 0); 
    if (pHResult)
    {
        *errorOccurred = 1;
        *errorCode = pHResult;
        CA_GetAutomationErrorString (pHResult, errorMessage, 1024);
        return;
    }
    for (idx = 1; idx < countOfPSAM; idx++) 
    {
        sprintf(cVarName, "StationGlobals.TSVP.dmm_%i_ID", idx + 1);
        pHResult = TS_PropertySetValNumber (sequenceContext, &pErrorInfo, cVarName, TS_PropOption_InsertIfMissing, 0); 
        if (pHResult)
        {
            *errorOccurred = 1;
            *errorCode = pHResult;
            CA_GetAutomationErrorString (pHResult, errorMessage, 1024);
            return;
        }   
    }
    if(countOfPICT)
    {
        pHResult = TS_PropertySetValNumber (sequenceContext, &pErrorInfo, "StationGlobals.TSVP.ict_ID", TS_PropOption_InsertIfMissing, 0); 
        if (pHResult)
        {
            *errorOccurred = 1;
            *errorCode = pHResult;
            CA_GetAutomationErrorString (pHResult, errorMessage, 1024);
            return;
        }
        
    }
    if(countOfPFG)
    {
        pHResult = TS_PropertySetValNumber (sequenceContext, &pErrorInfo, "StationGlobals.TSVP.funcgen_ID", TS_PropOption_InsertIfMissing, 0); 
        if (pHResult)
        {
            *errorOccurred = 1;
            *errorCode = pHResult;
            CA_GetAutomationErrorString (pHResult, errorMessage, 1024);
            return;
        }
        for (idx = 1; idx < countOfPFG; idx++) 
        {
            sprintf(cVarName, "StationGlobals.TSVP.funcgen_%i_ID", idx + 1);
            pHResult = TS_PropertySetValNumber (sequenceContext, &pErrorInfo, cVarName, TS_PropOption_InsertIfMissing, 0); 
            if (pHResult)
            {
                *errorOccurred = 1;
                *errorCode = pHResult;
                CA_GetAutomationErrorString (pHResult, errorMessage, 1024);
                return;
            }   
        }
    }
    if(countOfPAM)
    {
        pHResult = TS_PropertySetValNumber (sequenceContext, &pErrorInfo, "StationGlobals.TSVP.siganl_ID", TS_PropOption_InsertIfMissing, 0); 
        if (pHResult)
        {
            *errorOccurred = 1;
            *errorCode = pHResult;
            CA_GetAutomationErrorString (pHResult, errorMessage, 1024);
            return;
        }
        for (idx = 1; idx < countOfPAM; idx++) 
        {
            sprintf(cVarName, "StationGlobals.TSVP.siganl_%i_ID", idx + 1);
            pHResult = TS_PropertySetValNumber (sequenceContext, &pErrorInfo, cVarName, TS_PropOption_InsertIfMissing, 0); 
            if (pHResult)
            {
                *errorOccurred = 1;
                *errorCode = pHResult;
                CA_GetAutomationErrorString (pHResult, errorMessage, 1024);
                return;
            }   
        }
    }
    if(countOfPSU)
    {
        pHResult = TS_PropertySetValNumber (sequenceContext, &pErrorInfo, "StationGlobals.TSVP.dcpwr_ID", TS_PropOption_InsertIfMissing, 0); 
        if (pHResult)
        {
            *errorOccurred = 1;
            *errorCode = pHResult;
            CA_GetAutomationErrorString (pHResult, errorMessage, 1024);
            return;
        }
        
    }
    if(countOfPDFT)
    {
        pHResult = TS_PropertySetValNumber (sequenceContext, &pErrorInfo, "StationGlobals.TSVP.diomgr_ID", TS_PropOption_InsertIfMissing, 0); 
        if (pHResult)
        {
            *errorOccurred = 1;
            *errorCode = pHResult;
            CA_GetAutomationErrorString (pHResult, errorMessage, 1024);
            return;
        }
        
    }
    if(countOfPHDT)
    {
        pHResult = TS_PropertySetValNumber (sequenceContext, &pErrorInfo, "StationGlobals.TSVP.rsphdt_ID", TS_PropOption_InsertIfMissing, 0); 
        if (pHResult)
        {
            *errorOccurred = 1;
            *errorCode = pHResult;
            CA_GetAutomationErrorString (pHResult, errorMessage, 1024);
            return;
        }
        for (idx = 1; idx < countOfPICT; idx++) 
        {
            sprintf(cVarName, "StationGlobals.TSVP.phdt_%i_ID", idx + 1);
            pHResult = TS_PropertySetValNumber (sequenceContext, &pErrorInfo, cVarName, TS_PropOption_InsertIfMissing, 0); 
            if (pHResult)
            {
                *errorOccurred = 1;
                *errorCode = pHResult;
                CA_GetAutomationErrorString (pHResult, errorMessage, 1024);
                return;
            }   
        }
    } 
    if(countOfPIO2)
    {
        pHResult = TS_PropertySetValNumber (sequenceContext, &pErrorInfo, "StationGlobals.TSVP.rspio2_ID", TS_PropOption_InsertIfMissing, 0); 
        if (pHResult)
        {
            *errorOccurred = 1;
            *errorCode = pHResult;
            CA_GetAutomationErrorString (pHResult, errorMessage, 1024);
            return;
        }
        for (idx = 1; idx < countOfPICT; idx++) 
        {
            sprintf(cVarName, "StationGlobals.TSVP.rspio2_%i_ID", idx + 1);
            pHResult = TS_PropertySetValNumber (sequenceContext, &pErrorInfo, cVarName, TS_PropOption_InsertIfMissing, 0); 
            if (pHResult)
            {
                *errorOccurred = 1;
                *errorCode = pHResult;
                CA_GetAutomationErrorString (pHResult, errorMessage, 1024);
                return;
            }   
        }
    } 
    
    pHResult = TS_PropertySetValNumber (sequenceContext, &pErrorInfo, "StationGlobals.TSVP.Panel_ID", TS_PropOption_InsertIfMissing, 0); 
    if (pHResult)
    {
        *errorOccurred = 1;
        *errorCode = pHResult;
        CA_GetAutomationErrorString (pHResult, errorMessage, 1024);
        return;
    }
    pHResult = TS_PropertySetValBoolean (sequenceContext, &pErrorInfo,  "StationGlobals.TSVP.TestStarten",TS_PropOption_InsertIfMissing, FALSE);
    if (pHResult)
    {
        *errorOccurred = 1;
        *errorCode = pHResult;
        CA_GetAutomationErrorString (pHResult, errorMessage, 1024);
        return;
    }
    
    pHResult = TS_PropertySetValBoolean (sequenceContext, &pErrorInfo,  "StationGlobals.TSVP.ProgrammBeenden",TS_PropOption_InsertIfMissing, FALSE);
    if (pHResult)
    {
        *errorOccurred = 1;
        *errorCode = pHResult;
        CA_GetAutomationErrorString (pHResult, errorMessage, 1024);
        return;
    }
    
    pHResult = TS_PropertySetValString (sequenceContext, &pErrorInfo, "StationGlobals.TSVP.SerialNumber", TS_PropOption_InsertIfMissing, ""); 
    if (pHResult)
    {
        *errorOccurred = 1;
        *errorCode = pHResult;
        CA_GetAutomationErrorString (pHResult, errorMessage, 1024);
        return;
    }
    
    pHResult = TS_PropertySetValBoolean (sequenceContext, &pErrorInfo,  "StationGlobals.TSVP.SequenceFileLoadComplete",TS_PropOption_InsertIfMissing, FALSE);
    if (pHResult)
    {
        *errorOccurred = 1;
        *errorCode = pHResult;
        CA_GetAutomationErrorString (pHResult, errorMessage, 1024);
        return;
    }
	
}

void __stdcall tss_LoadAppIniFile (CAObjHandle sequenceContext,
                                  short *errorOccurred,
                                  long *errorCode, char errorMessage[])
{
	HRESULT pHResult;
    ERRORINFO pErrorInfo;
	
	IniText IniHandle;
    int status, idx;
    int NumberOfItems;
	
	int panel = -1;
	int control = -1;
	int pressbutton = 1;
	int benchnotselected = 1;
	int IsChecked = 0;
	int Count;
	
	char *BenchName;
	char cPathName[1024];
	char cVarName[1024];
	
	VBOOL bPropExists;
	
	
	PanelHandle = LoadPanelEx (0, "ts_support_uir.uir", LOADAPP, __CVIUserHInst);
	if (PanelHandle <= 0)
	{
		*errorOccurred = 1;
		*errorCode = PanelHandle;
		errorMessage = GetUILErrorString (PanelHandle);
		return;
	}
	ClearListCtrl (PanelHandle, LOADAPP_BENCHES);
	
	//CA_InitActiveXThreadStyleForCurrentThread (0, COINIT_APARTMENTTHREADED);	   
	status = FileSelectPopup ("", "*.ini", "", "Select application.ini", VAL_LOAD_BUTTON, 0, 1, 1, 0, cPathName);
	//status = FileSelectPopupEx ("", "*.ini", "", "Select application.ini", VAL_LOAD_BUTTON, 0, 1, cPathName);
	if (status < 0)
	{
		 *errorOccurred = TRUE;
		 *errorCode = status;
		 sprintf(errorMessage, "an error occurred with file select popup");
		 return;
	}
    
	IniHandle = Ini_New (0);
	status = Ini_ReadFromFile (IniHandle, cPathName);
	status = Ini_SectionExists (IniHandle, "LogicalNames");
	if (!status) return;
	NumberOfItems = Ini_NumberOfItems (IniHandle, "LogicalNames");
	for (idx = 1; idx <= NumberOfItems; idx++)
	{
		//get benchname
		status = Ini_NthItemName (IniHandle, "LogicalNames", idx, &BenchName);
		InsertListItem (PanelHandle, LOADAPP_BENCHES, -1, BenchName, idx);
	}
	
	InstallPopup (PanelHandle);
	while (benchnotselected)
	{
		pressbutton = 1;
		while (pressbutton)
		{
			GetUserEvent (1, &panel, &control);
			if ((panel == PanelHandle) && ((control == LOADAPP_OK) || (control == LOADAPP_CANCEL))) pressbutton = 0;
		}
		if (control == LOADAPP_CANCEL) 
		{
			RemovePopup(PanelHandle);
			pHResult = TS_PropertySetValString (sequenceContext, &pErrorInfo, "StationGlobals.TSVP.ApplicationIniFile", TS_PropOption_InsertIfMissing, ""); 
		    if (pHResult)
		    {
		        *errorOccurred = 1;
		        *errorCode = pHResult;
		        CA_GetAutomationErrorString (pHResult, errorMessage, 1024);
		        return;
		    }
    		return;
		}
		Count = 0;
		for (idx = 0; idx < NumberOfItems; idx++)
		{	
			IsListItemChecked (PanelHandle, LOADAPP_BENCHES, idx, &IsChecked);
			if (IsChecked) Count++;
		}
		if (Count == 1) 
		{
			benchnotselected = 0;
		} else {
			MessagePopup("Error", "Please select min. one bench!");
		}
	}
	RemovePopup(PanelHandle);
	
	pHResult = TS_PropertySetValString (sequenceContext, &pErrorInfo, "StationGlobals.TSVP.ApplicationIniFile", TS_PropOption_InsertIfMissing, cPathName); 
    if (pHResult)
    {
        *errorOccurred = 1;
        *errorCode = pHResult;
        CA_GetAutomationErrorString (pHResult, errorMessage, 1024);
        return;
    }
    
	for (idx = 0; idx < NumberOfItems; idx++)
	{	
		IsListItemChecked (PanelHandle, LOADAPP_BENCHES, idx, &IsChecked);
		if (IsChecked) 
		{
			 GetLabelFromIndex (PanelHandle, LOADAPP_BENCHES, idx, BenchName);
			 pHResult = TS_PropertySetValString (sequenceContext, &pErrorInfo, "StationGlobals.TSVP.BenchName1", TS_PropOption_InsertIfMissing, BenchName); 
		     if (pHResult)
		     {
		            *errorOccurred = 1;
		            *errorCode = pHResult;
		            CA_GetAutomationErrorString (pHResult, errorMessage, 1024);
		            return;
		     }
        
		}	
	}
		
	pHResult = TS_PropertyExists (sequenceContext, &pErrorInfo, "StationGlobals.TSVP.ReportResults", 0, &bPropExists);
    if (pHResult)
    {
        *errorOccurred = 1;
        *errorCode = pHResult;
        CA_GetAutomationErrorString (pHResult, errorMessage, 1024);
        return;
    }
	if (!bPropExists) 
	{
		pHResult = TS_PropertySetValBoolean (sequenceContext, &pErrorInfo,  "StationGlobals.TSVP.ReportResults",TS_PropOption_InsertIfMissing, FALSE);
	    if (pHResult)
	    {
	        *errorOccurred = 1;
	        *errorCode = pHResult;
	        CA_GetAutomationErrorString (pHResult, errorMessage, 1024);
	        return;
	    }
    }
	
	pHResult = TS_PropertyExists (sequenceContext, &pErrorInfo, "StationGlobals.TSVP.ReportOnlyFail", 0, &bPropExists);
    if (pHResult)
    {
        *errorOccurred = 1;
        *errorCode = pHResult;
        CA_GetAutomationErrorString (pHResult, errorMessage, 1024);
        return;
    }
	if (!bPropExists) 
	{
		pHResult = TS_PropertySetValBoolean (sequenceContext, &pErrorInfo,  "StationGlobals.TSVP.ReportOnlyFail",TS_PropOption_InsertIfMissing, FALSE);
	    if (pHResult)
	    {
	        *errorOccurred = 1;
	        *errorCode = pHResult;
	        CA_GetAutomationErrorString (pHResult, errorMessage, 1024);
	        return;
	    }
	}
	
	pHResult = TS_PropertyExists (sequenceContext, &pErrorInfo, "StationGlobals.TSVP.AppendReport", 0, &bPropExists);
    if (pHResult)
    {
        *errorOccurred = 1;
        *errorCode = pHResult;
        CA_GetAutomationErrorString (pHResult, errorMessage, 1024);
        return;
    }
	if (!bPropExists) 
	{
		pHResult = TS_PropertySetValBoolean (sequenceContext, &pErrorInfo,  "StationGlobals.TSVP.AppendReport",TS_PropOption_InsertIfMissing, FALSE);
	    if (pHResult)
	    {
	        *errorOccurred = 1;
	        *errorCode = pHResult;
	        CA_GetAutomationErrorString (pHResult, errorMessage, 1024);
	        return;
	    }
	}
	
	pHResult = TS_PropertyExists (sequenceContext, &pErrorInfo, "StationGlobals.TSVP.ReportText", 0, &bPropExists);
    if (pHResult)
    {
        *errorOccurred = 1;
        *errorCode = pHResult;
        CA_GetAutomationErrorString (pHResult, errorMessage, 1024);
        return;
    }
	if (!bPropExists) 
	{
		TS_PropertySetValString (sequenceContext, &pErrorInfo, "StationGlobals.TSVP.ReportText", TS_PropOption_InsertIfMissing, "");
	    if (pHResult)
	    {
	        *errorOccurred = 1;
	        *errorCode = pHResult;
	        CA_GetAutomationErrorString (pHResult, errorMessage, 1024);
	        return;
	    }
	}
}

void __stdcall tss_LoadChannels (CAObjHandle sequenceContext,
                                  char nameOfAppIni[], short *errorOccurred,
                                  long *errorCode, char errorMessage[])
{
    HRESULT pHResult;
    ERRORINFO pErrorInfo;
	CAObjHandle VarObject;
	
	IniText IniHandle;
    int status;
    int NumberOfItems;
	int NumberOfChannels;
    int idx, idy, idz;
    
    char *BenchName;
	char *LogicalChannelName;
    char BenchReference[1024];
	char PhysicalChannelName[1024];
    char AppChannelReference[1024];
	char VarLogicalName[1024];
	char VarName[1024];
	
	
    
	IniHandle = Ini_New (0);
	status = Ini_ReadFromFile (IniHandle, nameOfAppIni);
	status = Ini_SectionExists (IniHandle, "LogicalNames");
	if (!status) return;
	NumberOfItems = Ini_NumberOfItems (IniHandle, "LogicalNames");
	for (idx = 1; idx <= NumberOfItems; idx++)
	{
		//get benchname
		status = Ini_NthItemName (IniHandle, "LogicalNames", idx, &BenchName);
		//get bench reference
		status = Ini_GetStringIntoBuffer (IniHandle, "LogicalNames", BenchName, BenchReference, 1024);
		//check bench reference
		status = Ini_SectionExists (IniHandle, BenchReference);
		if (status)
		{
			//Check for item AppChannelTable
			status = Ini_ItemExists (IniHandle, BenchReference, "AppChannelTable");
			if (status)
			{
				//get bench reference
				status = Ini_GetStringIntoBuffer (IniHandle, BenchReference, "AppChannelTable", AppChannelReference, 1024);
				//check bench reference
				status = Ini_SectionExists (IniHandle, AppChannelReference);
				if (status)
				{
					NumberOfChannels = Ini_NumberOfItems (IniHandle, AppChannelReference);
					for (idy = 1; idy <= NumberOfChannels; idy++)
					{
						status = Ini_NthItemName (IniHandle, AppChannelReference, idy, &LogicalChannelName);
						status = Ini_GetStringIntoBuffer (IniHandle, AppChannelReference, LogicalChannelName, PhysicalChannelName, 1024);
						sprintf(VarLogicalName, LogicalChannelName); 
						for (idz = 0; idz < strlen(VarLogicalName); idz++) if (VarLogicalName[idz]=='.') VarLogicalName[idz] = '_';
						sprintf(VarName, "StationGlobals.TSVP.Channels.%s.%s", BenchName, VarLogicalName);
						pHResult = TS_PropertySetValString (sequenceContext, &pErrorInfo, VarName, TS_PropOption_InsertIfMissing, LogicalChannelName);
					    if (pHResult)
					    {
					        *errorOccurred = 1;
					        *errorCode = pHResult;
					        CA_GetAutomationErrorString (pHResult, errorMessage, 1024);
					        return;
					    }
						pHResult = TS_PropertyGetPropertyObject (sequenceContext, &pErrorInfo, VarName, 0, &VarObject);
						if (pHResult)
					    {
					        *errorOccurred = 1;
					        *errorCode = pHResult;
					        CA_GetAutomationErrorString (pHResult, errorMessage, 1024);
					        return;
					    }
						pHResult = TS_PropertySetProperty (VarObject, &pErrorInfo, TS_PropertyComment, CAVT_CSTRING, PhysicalChannelName);
						if (pHResult)
					    {
					        *errorOccurred = 1;
					        *errorCode = pHResult;
					        CA_GetAutomationErrorString (pHResult, errorMessage, 1024);
					        return;
					    }
						pHResult = CA_DiscardObjHandle(VarObject); 
						if (pHResult)
					    {
					        *errorOccurred = 1;
					        *errorCode = pHResult;
					        CA_GetAutomationErrorString (pHResult, errorMessage, 1024);
					        return;
					    }
						
					}
				}
				
			}
			//Check for item DIOChannelTable
			status = Ini_ItemExists (IniHandle, BenchReference, "DIOChannelTable");
			if (status)
			{
				//get bench reference
				status = Ini_GetStringIntoBuffer (IniHandle, BenchReference, "DIOChannelTable", AppChannelReference, 1024);
				//check bench reference
				status = Ini_SectionExists (IniHandle, AppChannelReference);
				if (status)
				{
					NumberOfChannels = Ini_NumberOfItems (IniHandle, AppChannelReference);
					for (idy = 1; idy <= NumberOfChannels; idy++)
					{
						status = Ini_NthItemName (IniHandle, AppChannelReference, idy, &LogicalChannelName);
						status = Ini_GetStringIntoBuffer (IniHandle, AppChannelReference, LogicalChannelName, PhysicalChannelName, 1024);
						sprintf(VarLogicalName, LogicalChannelName); 
						for (idz = 0; idz < strlen(VarLogicalName); idz++) if (VarLogicalName[idz]=='.') VarLogicalName[idz] = '_';
						sprintf(VarName, "StationGlobals.TSVP.Channels.%s.%s", BenchName, VarLogicalName);
						pHResult = TS_PropertySetValString (sequenceContext, &pErrorInfo, VarName, TS_PropOption_InsertIfMissing, LogicalChannelName);
					    if (pHResult)
					    {
					        *errorOccurred = 1;
					        *errorCode = pHResult;
					        CA_GetAutomationErrorString (pHResult, errorMessage, 1024);
					        return;
					    }
						pHResult = TS_PropertyGetPropertyObject (sequenceContext, &pErrorInfo, VarName, 0, &VarObject);
						if (pHResult)
					    {
					        *errorOccurred = 1;
					        *errorCode = pHResult;
					        CA_GetAutomationErrorString (pHResult, errorMessage, 1024);
					        return;
					    }
						pHResult = TS_PropertySetProperty (VarObject, &pErrorInfo, TS_PropertyComment, CAVT_CSTRING, PhysicalChannelName);
						if (pHResult)
					    {
					        *errorOccurred = 1;
					        *errorCode = pHResult;
					        CA_GetAutomationErrorString (pHResult, errorMessage, 1024);
					        return;
					    } 
						pHResult = CA_DiscardObjHandle(VarObject); 
						if (pHResult)
					    {
					        *errorOccurred = 1;
					        *errorCode = pHResult;
					        CA_GetAutomationErrorString (pHResult, errorMessage, 1024);
					        return;
					    }
						
					}
				}
				
			}
		}
		
	}
	
	
	Ini_Dispose (IniHandle);	
}

void __stdcall tss_DeleteChannels (CAObjHandle sequenceContext,
                                  short *errorOccurred, long *errorCode,
                                  char errorMessage[])
{
	HRESULT pHResult;
    ERRORINFO pErrorInfo;
	int iBenches;
	int iVariables;
	char *cName;
	char cBuffer[1024];
	char cDelete[1024];
	CAObjHandle ObjectHandle;
	int idx, idy;
	
	
    pHResult = TS_PropertyGetNumSubProperties (sequenceContext, &pErrorInfo, "StationGlobals.TSVP.Channels", &iBenches);
	if (pHResult)
    {
        *errorOccurred = 1;
        *errorCode = pHResult;
        CA_GetAutomationErrorString (pHResult, errorMessage, 1024);
        return;
    }
	
	 
	
	for (idx = 0; idx < iBenches; idx++)
	{
		//Get Benchname
		pHResult = TS_PropertyGetNthSubPropertyName (sequenceContext, &pErrorInfo, "StationGlobals.TSVP.Channels", 0, 0, &cName);
		if (pHResult)
	    {
	        *errorOccurred = 1;
	        *errorCode = pHResult;
	        CA_GetAutomationErrorString (pHResult, errorMessage, 1024);
	        return;
	    }
		sprintf(cBuffer, "StationGlobals.TSVP.Channels.%s", cName);
		CA_FreeMemory (cName);
		//Get count of variables
		pHResult = TS_PropertyGetNumSubProperties (sequenceContext, &pErrorInfo, cBuffer, &iVariables);
		if (pHResult)
	    {
	        *errorOccurred = 1;
	        *errorCode = pHResult;
	        CA_GetAutomationErrorString (pHResult, errorMessage, 1024);
	        return;
	    }
		for (idy = iVariables; idy > 0; idy--)
		{
			//Get Varname
			pHResult = TS_PropertyGetNthSubPropertyName (sequenceContext, &pErrorInfo, cBuffer, idy - 1, 0, &cName);
			if (pHResult)
		    {
		        *errorOccurred = 1;
		        *errorCode = pHResult;
		        CA_GetAutomationErrorString (pHResult, errorMessage, 1024);
		        return;
		    }
											   
			//Delete var
			sprintf(cDelete, "%s.%s", cBuffer, cName);
			CA_FreeMemory (cName);
			pHResult = TS_PropertyDeleteSubProperty (sequenceContext, &pErrorInfo, cDelete, 0x2);
			if (pHResult)
		    {
		        *errorOccurred = 1;
		        *errorCode = pHResult;
		        CA_GetAutomationErrorString (pHResult, errorMessage, 1024);
		        return;
		    }
		
		}
	    //Delete bench


		pHResult = TS_PropertyDeleteSubProperty (sequenceContext, &pErrorInfo, cBuffer, 0x2);
		if (pHResult)
	    {
	        *errorOccurred = 1;
	        *errorCode = pHResult;
	        CA_GetAutomationErrorString (pHResult, errorMessage, 1024);
	        return;
	    }
		
	  //Delete
	}		   
	

	pHResult = TS_PropertyDeleteSubProperty (sequenceContext, &pErrorInfo, "StationGlobals.TSVP.Channels", 0x2);
	if (pHResult)
    {
        *errorOccurred = 1;
        *errorCode = pHResult;
        CA_GetAutomationErrorString (pHResult, errorMessage, 1024);
        return;
    }
	   	 
}

void __stdcall tss_WriteReport (CAObjHandle sequenceContext, char cPathName[], char cFileName[], 
                               char cSerialNumber[], int append, char *cContent,
                               short *errorOccurred, long *errorCode,
                               char errorMessage[])
{
    int iStatus;
	int iSize;
	int iCount;
	char cAppend[1024];
	char cFileAndPathName[1024];
	char *cDate;
	char *cTime;
		
	FILE *pFile;
	
	iStatus = SetDir (cPathName);
	if (iStatus == -1) iStatus = MakeDir (cPathName);
	if (iStatus) 
	{
		*errorOccurred = TRUE;
        *errorCode = iStatus; 
		sprintf(errorMessage, "Cannot create path for report file!");
        return;	
	}
	if (strlen(cFileName))
	{
		sprintf(cFileAndPathName, "%s\\%s", cPathName, cFileName);    
		if (append)
		{
			pFile = fopen (cFileAndPathName, "a");
			if (errno)
			{
				*errorOccurred = TRUE;
		        *errorCode = iStatus;   
		        sprintf(errorMessage, "Cannot create report file! (%s)", cFileAndPathName);
	        	return;	
			}
			fprintf(pFile, cContent);
			if (errno)
			{
				*errorOccurred = TRUE;
		        *errorCode = iStatus;   
		        sprintf(errorMessage, "Cannot write into report file! (%s)", cFileAndPathName);
	        	return;	
			}
			fclose(pFile);
		} else {
			iCount = 0;
			iStatus = FileExists (cFileAndPathName, &iSize);
			while (iStatus == 1)
			{
				iCount++;
				sprintf (cAppend, "%i_%s", iCount, cFileAndPathName);
				iStatus = FileExists (cAppend, &iSize);
			}
			pFile = fopen (cAppend, "a");
			if (errno)
			{
				*errorOccurred = TRUE;
		        *errorCode = iStatus;   
		        sprintf(errorMessage, "Cannot create report file! (%s)", cFileAndPathName);
	        	return;	
			}
			fprintf(pFile, cContent);
			if (errno)
			{
				*errorOccurred = TRUE;
		        *errorCode = iStatus;   
		        sprintf(errorMessage, "Cannot write into report file! (%s)", cFileAndPathName);
	        	return;	
			}
			fclose(pFile);
		}
	} else {
		if (append)
		{
			sprintf(cFileAndPathName, "%s\\%s.txt", cPathName, cSerialNumber);
			pFile = fopen (cFileAndPathName, "a");
			if (errno)
			{
				*errorOccurred = TRUE;
		        *errorCode = iStatus;   
		        sprintf(errorMessage, "Cannot create report file! (%s)", cFileAndPathName);
	        	return;	
			}
			fprintf(pFile, cContent);
			if (errno)
			{
				*errorOccurred = TRUE;
		        *errorCode = iStatus;   
		        sprintf(errorMessage, "Cannot write into report file! (%s)", cFileAndPathName);
	        	return;	
			}
			fclose(pFile);
		} else {
			cDate = DateStr();
			cDate[2] = '_';
			cDate[5] = '_';
			cTime = TimeStr();
			cTime[2] = '_';
			cTime[5] = '_';
			sprintf(cFileAndPathName, "%s\\%s_%s_%s.txt", cPathName, cSerialNumber, cDate, cTime); 
			pFile = fopen (cFileAndPathName, "a");
			if (errno)
			{
				*errorOccurred = TRUE;
		        *errorCode = iStatus;   
		        sprintf(errorMessage, "Cannot create report file! (%s)", cFileAndPathName);
	        	return;	
			}
			fprintf(pFile, cContent);
			if (errno)
			{
				*errorOccurred = TRUE;
		        *errorCode = iStatus;   
		        sprintf(errorMessage, "Cannot write into report file! (%s)", cFileAndPathName);
	        	return;	
			}
			fclose(pFile);
		}
	}
	
}

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
                                           long *errorCode, char errorMessage[])
{
    
    
    double SampleInterval;
    
    
    //Load panel ....
    PanelHandle = LoadPanelEx (0, "ts_support_UIR.uir", PANEL_3, __CVIUserHInst);
    if (PanelHandle <= 0)
    {
        *errorOccurred = TRUE;
        *errorCode = PanelHandle;   
        errorMessage = GetUILErrorString (PanelHandle);
        return;
    }
    
    //Panel settings
    if (alwaysOntop) SetPanelAttribute (PanelHandle, ATTR_FLOATING, VAL_FLOAT_ALWAYS);
    if (!alwaysOntop) SetPanelAttribute (PanelHandle, ATTR_FLOATING, VAL_FLOAT_NEVER);
    SetPanelAttribute (PanelHandle, ATTR_TITLE, title);
    SetCtrlAttribute (PanelHandle, PANEL_3_DESCRIPTION, ATTR_CTRL_VAL, text);
    SetCtrlAttribute (PanelHandle, PANEL_3_MEASUREMENT, ATTR_MIN_VALUE, lowerScale);
    SetCtrlAttribute (PanelHandle, PANEL_3_MEASUREMENT, ATTR_MAX_VALUE, upperScale);
    SetCtrlAttribute (PanelHandle, PANEL_3_MEASUREMENT, ATTR_FILL_COLOR, VAL_BLACK);
    SetCtrlAttribute (PanelHandle, PANEL_3_MEASUREMENT, ATTR_FILL_HOUSING_COLOR, VAL_WHITE);
    SetCtrlAttribute (PanelHandle, PANEL_3_MEASUREMENT, ATTR_TEXT_BGCOLOR, VAL_TRANSPARENT); 
    SetCtrlAttribute (PanelHandle, PANEL_3_MEASUREMENTTEXT, ATTR_TEXT_BGCOLOR, VAL_TRANSPARENT);
    SetCtrlAttribute (PanelHandle, PANEL_3_DESCRIPTION, ATTR_TEXT_BGCOLOR, VAL_TRANSPARENT);
    
    //Setting measurement method
    *errorCode = rspsam_ConfigureMeasurement (PSAMHandle, RSPSAM_VAL_DC_VOLTS, range, 0.0001);
    if(*errorCode)
    {
        *errorOccurred = TRUE;
        rspsam_error_message (PSAMHandle, *errorCode, errorMessage);
        rspsam_reset (PSAMHandle);
        SWMGR_DisconnectAll (0, switchManagerHandle, errorOccurred, errorCode, errorMessage);
        return;
    }
    
    //solve and set sampling rate
    SampleInterval = 1 / (0.8 * Frequency) / 8192;
    if (SampleInterval < 5E-6) SampleInterval = 5E-6;
    *errorCode = rspsam_ConfigureMultiPoint (PSAMHandle, 1, 8192, RSPSAM_VAL_INTERVAL, SampleInterval);
    if(*errorCode)
    {
        *errorOccurred = TRUE;
        rspsam_error_message (PSAMHandle, *errorCode, errorMessage);
        rspsam_reset (PSAMHandle);
        SWMGR_DisconnectAll (0, switchManagerHandle, errorOccurred, errorCode, errorMessage);
        return;
    }
    
    //solve and set filter
    *errorCode = rspsam_mu_ConfigureLowpassFilter (PSAMHandle, Frequency * 10);
    if(*errorCode)
    {
        *errorOccurred = TRUE;
        rspsam_error_message (PSAMHandle, *errorCode, errorMessage);
        rspsam_reset (PSAMHandle);
        SWMGR_DisconnectAll (0, switchManagerHandle, errorOccurred, errorCode, errorMessage);
        return;
    }
    
    //set standard trigger
    *errorCode = rspsam_ConfigureTrigger (PSAMHandle, RSPSAM_VAL_IMMEDIATE, 0.0);
    if(*errorCode)
    {
        *errorOccurred = TRUE;
        rspsam_error_message (PSAMHandle, *errorCode, errorMessage);
        rspsam_reset (PSAMHandle);
        SWMGR_DisconnectAll (0, switchManagerHandle, errorOccurred, errorCode, errorMessage);
        return;
    }
    
    //Make connextions
    if (stricmp(busLow, "GND")) 
        SWMGR_Connect (0, switchManagerHandle, pinLow, busLow, errorOccurred, errorCode, errorMessage);
    if (*errorOccurred) return;
    
    SWMGR_Connect (0, switchManagerHandle, pinHigh, busHigh, errorOccurred, errorCode, errorMessage);
    if (*errorOccurred) return;
    
    
         if (!stricmp(busLow, "ABa1")) rspsam_cnx_Matrix (PSAMHandle, 5, 0x80, 0x80);
    else if (!stricmp(busLow, "ABb1")) rspsam_cnx_Matrix (PSAMHandle, 5, 0x40, 0x40); 
    else if (!stricmp(busLow, "ABc1")) rspsam_cnx_Matrix (PSAMHandle, 5, 0x20, 0x20);
    else if (!stricmp(busLow, "ABd1")) rspsam_cnx_Matrix (PSAMHandle, 5, 0x10, 0x10);
    else if (!stricmp(busLow, "ABa2")) rspsam_cnx_Matrix (PSAMHandle, 5, 0x8, 0x8);
    else if (!stricmp(busLow, "ABb2")) rspsam_cnx_Matrix (PSAMHandle, 5, 0x4, 0x4);   
    else if (!stricmp(busLow, "ABc2")) rspsam_cnx_Matrix (PSAMHandle, 5, 0x2, 0x2);   
    else if (!stricmp(busLow, "ABd2")) rspsam_cnx_Matrix (PSAMHandle, 5, 0x1, 0x1); 
    else if (!stricmp(busLow, "GND"))  rspsam_cnx_Gnd (PSAMHandle, 5, 1); 
    else 
    {
        *errorOccurred = TRUE;
        *errorCode = 1;
        sprintf (errorMessage, "Es wurde kein gültige Busname übergeben. Bitte überprüfen Sie die Variable busLow!");
        rspsam_reset (PSAMHandle);
        SWMGR_DisconnectAll (0, switchManagerHandle, errorOccurred, errorCode, errorMessage);
        return;
    }
    
         if (!stricmp(busHigh, "ABa1")) rspsam_cnx_Matrix (PSAMHandle, 8, 0x80, 0x80);
    else if (!stricmp(busHigh, "ABb1")) rspsam_cnx_Matrix (PSAMHandle, 8, 0x40, 0x40); 
    else if (!stricmp(busHigh, "ABc1")) rspsam_cnx_Matrix (PSAMHandle, 8, 0x20, 0x20);
    else if (!stricmp(busHigh, "ABd1")) rspsam_cnx_Matrix (PSAMHandle, 8, 0x10, 0x10);
    else if (!stricmp(busHigh, "ABa2")) rspsam_cnx_Matrix (PSAMHandle, 8, 0x8, 0x8);
    else if (!stricmp(busHigh, "ABb2")) rspsam_cnx_Matrix (PSAMHandle, 8, 0x4, 0x4);   
    else if (!stricmp(busHigh, "ABc2")) rspsam_cnx_Matrix (PSAMHandle, 8, 0x2, 0x2);   
    else if (!stricmp(busHigh, "ABd2")) rspsam_cnx_Matrix (PSAMHandle, 8, 0x1, 0x1);
    else 
    {
        *errorOccurred = TRUE;
        *errorCode = 1;
        sprintf (errorMessage, "Es wurde kein gültige Busname übergeben. Bitte überprüfen Sie die Variable busHigh!");
        rspsam_reset (PSAMHandle);
        SWMGR_DisconnectAll (0, switchManagerHandle, errorOccurred, errorCode, errorMessage);
        return;
    }
    
    //Close coupling relays
    rspsam_ConfigureCoupling (PSAMHandle, 0xFF);
    
    //Set global vars
    giPSAHandle = PSAMHandle;
    gdLowerLimit = lowerLimit;
    gdUpperLimit = upperLimit;
    
    //Start UIR
    DisplayPanel (PanelHandle);
    SetActiveCtrl (PanelHandle, PANEL_3_COMMANDBUTTON);
    
    //Start measurement timer
    SetCtrlAttribute (PanelHandle, PANEL_3_TIMER, ATTR_ENABLED, TRUE);
    
    RunUserInterface();
    DiscardPanel (PanelHandle);  
    
    if (giErrorCode) 
    {
        *errorOccurred = TRUE;
        *errorCode = giErrorCode;
        rspsam_error_message (PSAMHandle, *errorCode, errorMessage);
        rspsam_reset (PSAMHandle);
        SWMGR_DisconnectAll (0, switchManagerHandle, errorOccurred, errorCode, errorMessage);
        return;
    }
    
    //open connextions
    rspsam_ConfigureCoupling (PSAMHandle, 0x00); 
    
         if (!stricmp(busLow, "ABa1")) rspsam_cnx_Matrix (PSAMHandle, 5, 0x80, 0x0);
    else if (!stricmp(busLow, "ABb1")) rspsam_cnx_Matrix (PSAMHandle, 5, 0x40, 0x0); 
    else if (!stricmp(busLow, "ABc1")) rspsam_cnx_Matrix (PSAMHandle, 5, 0x20, 0x0);
    else if (!stricmp(busLow, "ABd1")) rspsam_cnx_Matrix (PSAMHandle, 5, 0x10, 0x0);
    else if (!stricmp(busLow, "ABa2")) rspsam_cnx_Matrix (PSAMHandle, 5, 0x8, 0x0);
    else if (!stricmp(busLow, "ABb2")) rspsam_cnx_Matrix (PSAMHandle, 5, 0x4, 0x0);   
    else if (!stricmp(busLow, "ABc2")) rspsam_cnx_Matrix (PSAMHandle, 5, 0x2, 0x0);   
    else if (!stricmp(busLow, "ABd2")) rspsam_cnx_Matrix (PSAMHandle, 5, 0x1, 0x0); 
    else if (!stricmp(busLow, "GND"))  rspsam_cnx_Gnd (PSAMHandle, 5, 0); 
    else 
    {
        *errorOccurred = TRUE;
        *errorCode = 1;
        sprintf (errorMessage, "Es wurde kein gültige Busname übergeben. Bitte überprüfen Sie die Variable busLow!");
        rspsam_reset (PSAMHandle);
        SWMGR_DisconnectAll (0, switchManagerHandle, errorOccurred, errorCode, errorMessage);
        return;
    }
    
         if (!stricmp(busHigh, "ABa1")) rspsam_cnx_Matrix (PSAMHandle, 8, 0x80, 0x0);
    else if (!stricmp(busHigh, "ABb1")) rspsam_cnx_Matrix (PSAMHandle, 8, 0x40, 0x0); 
    else if (!stricmp(busHigh, "ABc1")) rspsam_cnx_Matrix (PSAMHandle, 8, 0x20, 0x0);
    else if (!stricmp(busHigh, "ABd1")) rspsam_cnx_Matrix (PSAMHandle, 8, 0x10, 0x0);
    else if (!stricmp(busHigh, "ABa2")) rspsam_cnx_Matrix (PSAMHandle, 8, 0x8, 0x0);
    else if (!stricmp(busHigh, "ABb2")) rspsam_cnx_Matrix (PSAMHandle, 8, 0x4, 0x0);   
    else if (!stricmp(busHigh, "ABc2")) rspsam_cnx_Matrix (PSAMHandle, 8, 0x2, 0x0);   
    else if (!stricmp(busHigh, "ABd2")) rspsam_cnx_Matrix (PSAMHandle, 8, 0x1, 0x0);
    else 
    {
        *errorOccurred = TRUE;
        *errorCode = 1;
        sprintf (errorMessage, "Es wurde kein gültige Busname übergeben. Bitte überprüfen Sie die Variable busHigh!");
        rspsam_reset (PSAMHandle);
        SWMGR_DisconnectAll (0, switchManagerHandle, errorOccurred, errorCode, errorMessage);
        return;
    }
    
    SWMGR_Disconnect (0, switchManagerHandle, pinLow, busLow, errorOccurred, errorCode, errorMessage);
    SWMGR_Disconnect (0, switchManagerHandle, pinHigh, busHigh, errorOccurred, errorCode, errorMessage);
    
    //Get out of here ...
    
}

int CVICALLBACK Measure (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
    double Array[8192];
    int ActualNumberOfPoints;
    double Reading;
    char cTextbuffer[1024]; 
    switch (event)
        {
        case EVENT_TIMER_TICK:
            //Take measurement
            giErrorCode = rspsam_ReadMultiPoint (giPSAHandle, 5000, 8192, Array, &ActualNumberOfPoints);
            //Solve rms value
            RMS (Array, ActualNumberOfPoints, &Reading);
            //Set numeric slider
            SetCtrlVal (PanelHandle, PANEL_3_MEASUREMENT, Reading);
            //Set user interface
            sprintf(cTextbuffer, "Aktueller Messwert [Vrms]\n%f < %f < %f", gdLowerLimit, Reading, gdUpperLimit);
            SetCtrlVal (PanelHandle, PANEL_3_MEASUREMENTTEXT, cTextbuffer);
            if ((gdLowerLimit < Reading) && (Reading < gdUpperLimit))
            {
                //Measurement is in range
                SetPanelAttribute (PanelHandle, ATTR_BACKCOLOR, VAL_GREEN);
                SetCtrlAttribute (PanelHandle, PANEL_3_COMMANDBUTTON, ATTR_LABEL_TEXT, "Weiter [ENTER]");
                SetCtrlAttribute (PanelHandle, PANEL_3_COMMANDBUTTON, ATTR_SHORTCUT_KEY, VAL_ENTER_VKEY);
            }else
            {
                //Measurement is out of range
                SetPanelAttribute (PanelHandle, ATTR_BACKCOLOR, VAL_RED);
                SetCtrlAttribute (PanelHandle, PANEL_3_COMMANDBUTTON, ATTR_LABEL_TEXT, "Abbruch [ESC]");
                SetCtrlAttribute (PanelHandle, PANEL_3_COMMANDBUTTON, ATTR_SHORTCUT_KEY, VAL_ESC_VKEY);
            }
            ProcessDrawEvents ();
            break;
        }
    return 0;
}

int CVICALLBACK Command (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
    switch (event)
        {
        case EVENT_COMMIT:
            QuitUserInterface(0);
            break;
        }
    return 0;
}


void __stdcall tss_PictureMessageBox (CAObjHandle sequenceContext, char cMessageText[], char cButtonText[], int FontSize,
                               char cPathToPicture[], int PictureWith, int PictureHeight,
                               short *errorOccurred, long *errorCode,
                               char errorMessage[])
{
	
	int iPanelHandle;
	int iScreenHeight, iScreenWidth;
	int iTextHeight;
	int iGetHandle, iGetCtrlID; 
	
	//Load panel ....
    iPanelHandle = LoadPanelEx (0, "ts_support_UIR.uir", PICTUREMSG, __CVIUserHInst);
    if (iPanelHandle <= 0)
    {
        *errorOccurred = TRUE;
        *errorCode = PanelHandle;   
        errorMessage = GetUILErrorString (PanelHandle);
        return;
    }
	
	GetScreenSize (&iScreenHeight, &iScreenWidth);
	SetPanelAttribute (iPanelHandle, ATTR_HEIGHT, iScreenHeight - 30);
	SetPanelAttribute (iPanelHandle, ATTR_WIDTH, iScreenWidth - 10);
	SetPanelPos (iPanelHandle, 30, 5);
	
	SetCtrlVal(iPanelHandle, PICTUREMSG_TEXTMSG, cMessageText);
	SetCtrlAttribute (iPanelHandle, PICTUREMSG_TEXTMSG, ATTR_TEXT_POINT_SIZE, FontSize);
	GetCtrlAttribute (iPanelHandle, PICTUREMSG_TEXTMSG, ATTR_HEIGHT, &iTextHeight);
	
	SetCtrlAttribute (iPanelHandle, PICTUREMSG_PICTURE, ATTR_WIDTH, iScreenWidth - 30);
	SetCtrlAttribute (iPanelHandle, PICTUREMSG_PICTURE, ATTR_HEIGHT, iScreenHeight - 110);
	
	SetCtrlAttribute (iPanelHandle, PICTUREMSG_COMMANDBUTTON, ATTR_TOP, iScreenHeight - 65);
	SetCtrlAttribute (iPanelHandle, PICTUREMSG_COMMANDBUTTON, ATTR_LEFT, iScreenWidth - 150);
	SetCtrlAttribute (iPanelHandle, PICTUREMSG_COMMANDBUTTON, ATTR_LABEL_TEXT, cButtonText);
	
	
	DisplayImageFile (iPanelHandle, PICTUREMSG_PICTURE, cPathToPicture);
	
	ProcessDrawEvents();
	ProcessSystemEvents();
	
	InstallPopup(iPanelHandle);
	
	GetUserEvent (1, &iGetHandle, &iGetCtrlID);
	while ((iGetHandle != PICTUREMSG) && (iGetCtrlID != PICTUREMSG_COMMANDBUTTON) )
	{
		GetUserEvent (1, &iGetHandle, &iGetCtrlID); 	
	}
	
	RemovePopup(iPanelHandle);                     
	
	DiscardPanel (PanelHandle);
}


void __stdcall tss_FileToArray (CAObjHandle sequenceContext, char cFileAndPathName[], 
                               double dArray[], int iSize,  int iGroups,
                               short *errorOccurred, long *errorCode,
                               char errorMessage[])
{
	int iStatus;
	
	iStatus = FileToArray (cFileAndPathName, dArray, VAL_DOUBLE, iSize, iGroups, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
	if (iStatus)
	{
		 *errorOccurred = 1;
		 *errorCode = iStatus;
		 sprintf (errorMessage, "Error while reading values");
	}
}


void __stdcall tss_SolveRMS (CAObjHandle sequenceContext, 
                               double InputArray[], int NumberOfElements,  double *RMSValue,
                               short *errorOccurred, long *errorCode,
                               char errorMessage[])
{
	int iStatus;
	
	iStatus = RMS (InputArray, NumberOfElements, RMSValue);
	if (iStatus)
	{
		 *errorOccurred = 1;
		 *errorCode = iStatus;
		 sprintf (errorMessage, "Error while reading values");
	}
}


void __stdcall tss_SolveMean (CAObjHandle sequenceContext, 
                               double InputArray[], int NumberOfElements,  double *MeanValue,
                               short *errorOccurred, long *errorCode,
                               char errorMessage[])
{
	int iStatus;
	
	iStatus = Mean (InputArray, NumberOfElements, MeanValue);
	if (iStatus)
	{
		 *errorOccurred = 1;
		 *errorCode = iStatus;
		 sprintf (errorMessage, "Error while reading values");
	}
}

void __stdcall tss_SolveMedian (CAObjHandle sequenceContext, 
                               double InputArray[], int NumberOfElements,  double *MedianValue,
                               short *errorOccurred, long *errorCode,
                               char errorMessage[])
{
	int iStatus;
	
	iStatus = Median (InputArray, NumberOfElements, MedianValue);
	if (iStatus)
	{
		 *errorOccurred = 1;
		 *errorCode = iStatus;
		 sprintf (errorMessage, "Error while reading values");
	}
}




int __stdcall DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	int idx;

	switch (fdwReason)
	{
		case DLL_PROCESS_ATTACH:

			/* Needed if linking in external compiler; harmless otherwise 
			if (InitCVIRTE(hinstDLL, 0, 0) == 0)
			{
			   return 0;
			}
			/* out of memory */
			CA_InitActiveXThreadStyleForCurrentThread (0, COINIT_APARTMENTTHREADED);

			//init all test results
			//GetModuleDir(__CVIUserHInst, cDLLPATH);
			break;

		case DLL_PROCESS_DETACH:

			/* Do not call CVI functions if cvirte.dll has already been detached.    */
			/*if (!CVIRTEHasBeenDetached())
			{
			   CloseCVIRTE(); /* Needed if linking in external compiler; harmless otherwise      */
			//}
			break;
	}

	return 1;
}

////////////////////////////////////////////////////////////////////////////////
int __stdcall DllEntryPoint(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID
							lpvReserved)
{
	/* Included for compatibility with Borland    */
	CA_InitActiveXThreadStyleForCurrentThread (0, COINIT_APARTMENTTHREADED); 
	return DllMain(hinstDLL, fdwReason, lpvReserved);
}

					

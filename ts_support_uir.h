/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  LOADAPP                          1
#define  LOADAPP_BENCHES                  2       /* control type: listBox, callback function: (none) */
#define  LOADAPP_CANCEL                   3       /* control type: command, callback function: (none) */
#define  LOADAPP_OK                       4       /* control type: command, callback function: (none) */

#define  PANEL                            2
#define  PANEL_TEXTMSG                    2       /* control type: textMsg, callback function: (none) */

#define  PANEL_2                          3
#define  PANEL_2_TEXTMSG                  2       /* control type: textMsg, callback function: (none) */
#define  PANEL_2_FORTFAHRE                3       /* control type: textButton, callback function: (none) */

#define  PANEL_3                          4
#define  PANEL_3_MEASUREMENT              2       /* control type: scale, callback function: (none) */
#define  PANEL_3_COMMANDBUTTON            3       /* control type: command, callback function: Command */
#define  PANEL_3_MEASUREMENTTEXT          4       /* control type: textMsg, callback function: (none) */
#define  PANEL_3_DESCRIPTION              5       /* control type: textMsg, callback function: (none) */
#define  PANEL_3_TIMER                    6       /* control type: timer, callback function: Measure */

#define  PICTUREMSG                       5
#define  PICTUREMSG_PICTURE               2       /* control type: picture, callback function: (none) */
#define  PICTUREMSG_COMMANDBUTTON         3       /* control type: command, callback function: (none) */
#define  PICTUREMSG_TEXTMSG               4       /* control type: textMsg, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK Command(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Measure(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif

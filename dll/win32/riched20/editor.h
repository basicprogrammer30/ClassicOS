/*
 * RichEdit - prototypes for functions and macro definitions
 *
 * Copyright 2004 by Krzysztof Foltman
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

#pragma once

#include "editstr.h"

struct _RTF_Info;

extern HINSTANCE dll_instance;

#define RUN_IS_HIDDEN(run) ((run)->style->fmt.dwMask & CFM_HIDDEN \
                             && (run)->style->fmt.dwEffects & CFE_HIDDEN)

#define InitFormatEtc(fe, cf, med) \
        {\
        (fe).cfFormat=cf;\
        (fe).dwAspect=DVASPECT_CONTENT;\
        (fe).ptd=NULL;\
        (fe).tymed=med;\
        (fe).lindex=-1;\
        };

static inline WCHAR *get_text( const ME_Run *run, int offset )
{
    return run->para->text->szData + run->nCharOfs + offset;
}

static inline const char *debugstr_run( const ME_Run *run )
{
    return debugstr_wn( get_text( run, 0 ), run->len );
}

/* style.c */
ME_Style *style_get_insert_style( ME_TextEditor *editor, ME_Cursor *cursor );
ME_Style *ME_MakeStyle(CHARFORMAT2W *style);
void ME_AddRefStyle(ME_Style *item);
void ME_DestroyStyle(ME_Style *item);
void ME_ReleaseStyle(ME_Style *item);
ME_Style *ME_ApplyStyle(ME_TextEditor *ed, ME_Style *sSrc, CHARFORMAT2W *style);
void select_style(ME_Context *c, ME_Style *s);
void ME_InitCharFormat2W(CHARFORMAT2W *pFmt);
void ME_SaveTempStyle(ME_TextEditor *editor, ME_Style *style);
void ME_ClearTempStyle(ME_TextEditor *editor);
void ME_DumpStyleToBuf(CHARFORMAT2W *pFmt, char buf[2048]);
void ME_DumpStyle(ME_Style *s);
BOOL cfany_to_cf2w(CHARFORMAT2W *to, const CHARFORMAT2W *from);
BOOL cf2w_to_cfany(CHARFORMAT2W *to, const CHARFORMAT2W *from);
void ME_CopyCharFormat(CHARFORMAT2W *pDest, const CHARFORMAT2W *pSrc); /* only works with 2W structs */
void ME_CharFormatFromLogFont(HDC hDC, const LOGFONTW *lf, CHARFORMAT2W *fmt); /* ditto */

/* list.c */
void ME_InsertBefore(ME_DisplayItem *diWhere, ME_DisplayItem *diWhat);
void ME_Remove(ME_DisplayItem *diWhere);
ME_DisplayItem *ME_FindItemBack(ME_DisplayItem *di, ME_DIType nTypeOrClass);
ME_DisplayItem *ME_FindItemFwd(ME_DisplayItem *di, ME_DIType nTypeOrClass);
ME_DisplayItem *ME_FindItemBackOrHere(ME_DisplayItem *di, ME_DIType nTypeOrClass);
ME_DisplayItem *ME_MakeDI(ME_DIType type);
void ME_DestroyDisplayItem(ME_DisplayItem *item);
void ME_DumpDocument(ME_TextBuffer *buffer);

/* string.c */
ME_String *ME_MakeStringN(LPCWSTR szText, int nMaxChars);
ME_String *ME_MakeStringR(WCHAR cRepeat, int nMaxChars);
ME_String *ME_MakeStringConst(const WCHAR *str, int len);
ME_String *ME_MakeStringEmpty(int len);
void ME_DestroyString(ME_String *s);
BOOL ME_AppendString(ME_String *s, const WCHAR *append, int len);
ME_String *ME_VSplitString(ME_String *orig, int nVPos);
int ME_FindNonWhitespaceV(const ME_String *s, int nVChar);
int ME_CallWordBreakProc(ME_TextEditor *editor, WCHAR *str, INT len, INT start, INT code);
void ME_StrDeleteV(ME_String *s, int nVChar, int nChars);
BOOL ME_InsertString(ME_String *s, int ofs, const WCHAR *insert, int len);

#define CP_UNICODE 1200

/* smart helpers for A<->W conversions, they reserve/free memory and call MultiByte<->WideChar functions */
LPWSTR ME_ToUnicode(LONG codepage, LPVOID psz, INT *len);
void ME_EndToUnicode(LONG codepage, LPVOID psz);

static inline int ME_IsWSpace(WCHAR ch)
{
  return ch > '\0' && ch <= ' ';
}

static inline int ME_CharCompare(WCHAR a, WCHAR b, int caseSensitive)
{
  return caseSensitive ? (a == b) : (towupper(a) == towupper(b));
}

/* note: those two really return the first matching offset (starting from EOS)+1 
 * in other words, an offset of the first trailing white/black */
int ME_ReverseFindNonWhitespaceV(const ME_String *s, int nVChar);
int ME_ReverseFindWhitespaceV(const ME_String *s, int nVChar);

/* row.c */
void row_end_cursor( ME_Row *row, ME_Cursor *cursor, BOOL include_eop );
void row_first_cursor( ME_Row *row, ME_Cursor *cursor );
ME_Run *row_first_run( ME_Row *row );
ME_Row *row_from_cursor( ME_Cursor *cursor );
ME_Row *row_from_row_number( ME_TextEditor *editor, int row_num );
ME_Row *row_next( ME_Row *row );
ME_Row *row_next_all_paras( ME_Row *row );
ME_Run *row_next_run( ME_Row *row, ME_Run *run );
int row_number_from_char_ofs( ME_TextEditor *editor, int ofs );
ME_Paragraph *row_para( ME_Row *row );
ME_Row *row_prev_all_paras( ME_Row *row );
static inline ME_DisplayItem *row_get_di( ME_Row *row )
{
    return (ME_DisplayItem *)((ptrdiff_t)row - offsetof(ME_DisplayItem, member));
}

/* run.c */
void cursor_from_char_ofs( ME_TextEditor *editor, int char_ofs, ME_Cursor *cursor );
BOOL cursor_next_run( ME_Cursor *cursor, BOOL all_para );
BOOL cursor_prev_run( ME_Cursor *cursor, BOOL all_para );
void editor_propagate_char_ofs( ME_TextEditor *editor, ME_Paragraph *para, ME_Run *run, int shift );
int run_char_ofs( ME_Run *run, int ofs );
ME_Run *run_create( ME_Style *s, int nFlags );
ME_Run *run_insert( ME_TextEditor *editor, ME_Cursor *cursor,
                    ME_Style *style, const WCHAR *str, int len, int flags );
void ME_CheckCharOffsets(ME_TextEditor *editor);
/* this one accounts for 1/2 char tolerance */
int ME_CharFromPointContext(ME_Context *c, int cx, ME_Run *run, BOOL closest, BOOL visual_order);
int ME_CharFromPoint(ME_TextEditor *editor, int cx, ME_Run *run, BOOL closest, BOOL visual_order);
int ME_PointFromCharContext(ME_Context *c, ME_Run *pRun, int nOffset, BOOL visual_order);
int ME_PointFromChar(ME_TextEditor *editor, ME_Run *pRun, int nOffset, BOOL visual_order);
BOOL ME_CanJoinRuns(const ME_Run *run1, const ME_Run *run2);
void run_join( ME_TextEditor *editor, ME_Run *run );
ME_Run *run_next( ME_Run *run );
ME_Run *run_next_all_paras( ME_Run *run );
ME_Run *run_prev( ME_Run *run );
ME_Run *run_prev_all_paras( ME_Run *run );
ME_Run *run_split( ME_TextEditor *editor, ME_Cursor *cursor );
void ME_UpdateRunFlags(ME_TextEditor *editor, ME_Run *run);
SIZE ME_GetRunSizeCommon(ME_Context *c, const ME_Paragraph *para, ME_Run *run, int nLen,
                         int startx, int *pAscent, int *pDescent);
void ME_SetCharFormat(ME_TextEditor *editor, ME_Cursor *start, ME_Cursor *end, CHARFORMAT2W *pFmt);
void ME_SetSelectionCharFormat(ME_TextEditor *editor, CHARFORMAT2W *pFmt);
void ME_GetCharFormat(ME_TextEditor *editor, const ME_Cursor *from,
                      const ME_Cursor *to, CHARFORMAT2W *pFmt);
void ME_GetSelectionCharFormat(ME_TextEditor *editor, CHARFORMAT2W *pFmt);
void ME_GetDefaultCharFormat(ME_TextEditor *editor, CHARFORMAT2W *pFmt);
void ME_SetDefaultCharFormat(ME_TextEditor *editor, CHARFORMAT2W *mod);
static inline ME_DisplayItem *run_get_di( ME_Run *run )
{
    return (ME_DisplayItem *)((ptrdiff_t)run - offsetof(ME_DisplayItem, member));
}

/* caret.c */
void cursor_coords( ME_TextEditor *editor, ME_Cursor *cursor, int *x, int *y, int *height );
BOOL cursor_from_coords( ME_TextEditor *editor, int x, int y, ME_Cursor *cursor );
void ME_SetCursorToStart(ME_TextEditor *editor, ME_Cursor *cursor);
int set_selection_cursors(ME_TextEditor *editor, int from, int to);
BOOL ME_MoveCursorWords(ME_TextEditor *editor, ME_Cursor *cursor, int nRelOfs);
void hide_caret(ME_TextEditor *ed);
void show_caret(ME_TextEditor *ed);
void update_caret(ME_TextEditor *ed);
void create_caret(ME_TextEditor *ed);
void ME_LButtonDown(ME_TextEditor *editor, int x, int y, int clickNum);
void ME_MouseMove(ME_TextEditor *editor, int x, int y);
BOOL ME_DeleteTextAtCursor(ME_TextEditor *editor, int nCursor, int nChars);
void ME_InsertTextFromCursor(ME_TextEditor *editor, int nCursor,
                             const WCHAR *str, int len, ME_Style *style);
void ME_InsertEndRowFromCursor(ME_TextEditor *editor, int nCursor);
int ME_MoveCursorChars(ME_TextEditor *editor, ME_Cursor *cursor, int nRelOfs, BOOL final_eop);
BOOL ME_ArrowKey(ME_TextEditor *ed, int nVKey, BOOL extend, BOOL ctrl);

int ME_GetCursorOfs(const ME_Cursor *cursor);
int ME_GetSelectionOfs(ME_TextEditor *editor, LONG *from, LONG *to);
int ME_GetSelection(ME_TextEditor *editor, ME_Cursor **from, ME_Cursor **to);
BOOL ME_IsSelection(ME_TextEditor *editor);
void ME_DeleteSelection(ME_TextEditor *editor);
void ME_SendSelChange(ME_TextEditor *editor);
HRESULT editor_insert_oleobj( ME_TextEditor *editor, const REOBJECT *reo );
BOOL ME_InternalDeleteText(ME_TextEditor *editor, ME_Cursor *start, int nChars, BOOL bForce);
int ME_GetTextLength(ME_TextEditor *editor);
int ME_GetTextLengthEx(ME_TextEditor *editor, const GETTEXTLENGTHEX *how);
ME_Style *ME_GetSelectionInsertStyle(ME_TextEditor *editor);

/* context.c */
void ME_InitContext(ME_Context *c, ME_TextEditor *editor, HDC hDC);
void ME_DestroyContext(ME_Context *c);

/* wrap.c */
BOOL ME_WrapMarkedParagraphs(ME_TextEditor *editor);
void para_range_invalidate( ME_TextEditor *editor, ME_Paragraph *start_para, ME_Paragraph *last_para );
void ME_SendRequestResize(ME_TextEditor *editor, BOOL force);
BOOL wrap_marked_paras_dc( ME_TextEditor *editor, HDC hdc, BOOL invalidate );

/* para.c */
void editor_get_selection_paras(ME_TextEditor *editor, ME_Paragraph **para, ME_Paragraph **para_end );
void editor_get_selection_para_fmt( ME_TextEditor *editor, PARAFORMAT2 *fmt );
void editor_mark_rewrap_all( ME_TextEditor *editor );
void editor_set_default_para_fmt(ME_TextEditor *editor, PARAFORMAT2 *pFmt);
BOOL editor_set_selection_para_fmt( ME_TextEditor *editor, const PARAFORMAT2 *fmt );
void ME_MakeFirstParagraph(ME_TextEditor *editor, HDC hdc);
void ME_DumpParaStyle(ME_Paragraph *s);
void ME_DumpParaStyleToBuf(const PARAFORMAT2 *pFmt, char buf[2048]);
int get_total_width(ME_TextEditor *editor);
ME_Cell *para_cell( ME_Paragraph *para );
void para_destroy( ME_TextEditor *editor, ME_Paragraph *item );
ME_Row *para_end_row( ME_Paragraph *para );
ME_Run *para_end_run( ME_Paragraph *para );
ME_Row *para_first_row( ME_Paragraph *para );
ME_Run *para_first_run( ME_Paragraph *para );
BOOL para_in_table( ME_Paragraph *para );
ME_Paragraph *para_join( ME_TextEditor *editor, ME_Paragraph *para, BOOL use_first_fmt );
void para_mark_add( ME_TextEditor *editor, ME_Paragraph *para );
void para_mark_remove( ME_TextEditor *editor, ME_Paragraph *para );
void para_mark_rewrap( ME_TextEditor *editor, ME_Paragraph *para );
ME_Paragraph *para_next( ME_Paragraph *para );
void para_num_clear( struct para_num *pn );
void para_num_init( ME_Context *c, ME_Paragraph *para );
ME_Paragraph *para_prev( ME_Paragraph *para );
ME_Paragraph *para_split( ME_TextEditor *editor, ME_Run *run, ME_Style *style,
                          const WCHAR *eol_str, int eol_len, int paraFlags );

static inline ME_DisplayItem *para_get_di(ME_Paragraph *para)
{
    return (ME_DisplayItem *)((ptrdiff_t)para - offsetof(ME_DisplayItem, member));
}

/* paint.c */
void editor_draw( ME_TextEditor *editor, HDC hDC, const RECT *update );
void ME_Repaint(ME_TextEditor *editor);
void ME_RewrapRepaint(ME_TextEditor *editor);
void ME_UpdateRepaint(ME_TextEditor *editor, BOOL update_now);
void editor_ensure_visible( ME_TextEditor *editor, ME_Cursor *cursor );
void ME_InvalidateSelection(ME_TextEditor *editor);
BOOL ME_SetZoom(ME_TextEditor *editor, int numerator, int denominator);
int  ME_twips2pointsX(const ME_Context *c, int x);
int  ME_twips2pointsY(const ME_Context *c, int y);

/* scroll functions in paint.c */

void scroll_abs( ME_TextEditor *editor, int x, int y, BOOL notify );
void scroll_h_abs( ME_TextEditor *editor, int x, BOOL notify );
void scroll_v_abs( ME_TextEditor *editor, int y, BOOL notify );
void ME_ScrollUp(ME_TextEditor *editor, int cy);
void ME_ScrollDown(ME_TextEditor *editor, int cy);
void ME_ScrollLeft(ME_TextEditor *editor, int cx);
void ME_ScrollRight(ME_TextEditor *editor, int cx);
void ME_UpdateScrollBar(ME_TextEditor *editor);

/* other functions in paint.c */
int ME_GetParaBorderWidth(const ME_Context *c, int flags);

/* richole.c */
void draw_ole( ME_Context *c, int x, int y, ME_Run* run, BOOL selected );
void ME_GetOLEObjectSize(const ME_Context *c, ME_Run *run, SIZE *pSize);
void ME_CopyReObject(REOBJECT *dst, const REOBJECT *src, DWORD flags);
void ME_DeleteReObject(struct re_object *re_object);
void richole_release_children( struct text_services *services );
extern const IRichEditOleVtbl re_ole_vtbl;
extern const ITextDocument2OldVtbl text_doc2old_vtbl;

/* editor.c */
ME_TextEditor *ME_MakeEditor(ITextHost *texthost, BOOL bEmulateVersion10);
void ME_DestroyEditor(ME_TextEditor *editor);
LRESULT editor_handle_message( ME_TextEditor *editor, UINT msg, WPARAM wParam,
                               LPARAM lParam, HRESULT* phresult );
int ME_GetTextW(ME_TextEditor *editor, WCHAR *buffer, int buflen,
                const ME_Cursor *start, int srcChars, BOOL bCRLF, BOOL bEOP);
void ME_RTFCharAttrHook(struct _RTF_Info *info);
void ME_RTFParAttrHook(struct _RTF_Info *info);
void ME_RTFTblAttrHook(struct _RTF_Info *info);
void ME_RTFSpecialCharHook(struct _RTF_Info *info);
void ME_StreamInFill(ME_InStream *stream);
extern BOOL me_debug;
void ME_ReplaceSel(ME_TextEditor *editor, BOOL can_undo, const WCHAR *str, int len);
int set_selection( ME_TextEditor *editor, int to, int from );
HRESULT editor_copy_or_cut( ME_TextEditor *editor, BOOL cut, ME_Cursor *start, int count,
                            IDataObject **data_out );
ME_Paragraph *editor_end_para( ME_TextEditor *editor );
ME_Paragraph *editor_first_para( ME_TextEditor *editor );
void editor_set_cursor( ME_TextEditor *editor, int x, int y );
void link_notify( ME_TextEditor *editor, UINT msg, WPARAM wParam, LPARAM lParam );

/* table.c */
ME_Cell *cell_create( void );
ME_Paragraph *cell_end_para( ME_Cell *cell );
ME_Paragraph *cell_first_para( ME_Cell *cell );
ME_Cell *cell_next( ME_Cell *cell );
ME_Cell *cell_prev( ME_Cell *cell );
ME_Paragraph *table_append_row( ME_TextEditor *editor, ME_Paragraph *table_row );
void table_handle_tab( ME_TextEditor *editor, BOOL selected_row );
ME_Paragraph *table_insert_cell( ME_TextEditor *editor, ME_Cursor *cursor );
ME_Paragraph *table_insert_row_end( ME_TextEditor *editor, ME_Cursor *cursor );
ME_Paragraph *table_insert_row_start( ME_TextEditor *editor, ME_Cursor *cursor );
ME_Paragraph *table_insert_row_start_at_para( ME_TextEditor *editor, ME_Paragraph *para );
void table_move_from_row_start( ME_TextEditor *editor );
ME_Paragraph *table_outer_para( ME_Paragraph *para );
void table_protect_partial_deletion( ME_TextEditor *editor, ME_Cursor *c, int *num_chars );
ME_Paragraph *table_row_end( ME_Paragraph *para );
ME_Cell *table_row_end_cell( ME_Paragraph *para );
ME_Cell *table_row_first_cell( ME_Paragraph *para );
ME_Paragraph *table_row_start( ME_Paragraph *para );
struct RTFTable *ME_MakeTableDef(ME_TextEditor *editor);
void ME_InitTableDef(ME_TextEditor *editor, struct RTFTable *tableDef);
static inline ME_DisplayItem *cell_get_di(ME_Cell *cell)
{
    return (ME_DisplayItem *)((ptrdiff_t)cell - offsetof(ME_DisplayItem, member));
}


/* txthost.c */
#ifdef __ASM_USE_THISCALL_WRAPPER
extern const struct ITextHost2Vtbl text_host2_stdcall_vtbl;
#define TXTHOST_VTABLE(This) (&text_host2_stdcall_vtbl)
#else
#define TXTHOST_VTABLE(This) (This)->lpVtbl
#endif
 /*** ITextHost methods ***/
#define ITextHost_TxGetDC(This) TXTHOST_VTABLE(This)->TxGetDC(This)
#define ITextHost_TxReleaseDC(This,a) TXTHOST_VTABLE(This)->TxReleaseDC(This,a)
#define ITextHost_TxShowScrollBar(This,a,b) TXTHOST_VTABLE(This)->TxShowScrollBar(This,a,b)
#define ITextHost_TxEnableScrollBar(This,a,b) TXTHOST_VTABLE(This)->TxEnableScrollBar(This,a,b)
#define ITextHost_TxSetScrollRange(This,a,b,c,d) TXTHOST_VTABLE(This)->TxSetScrollRange(This,a,b,c,d)
#define ITextHost_TxSetScrollPos(This,a,b,c) TXTHOST_VTABLE(This)->TxSetScrollPos(This,a,b,c)
#define ITextHost_TxInvalidateRect(This,a,b) TXTHOST_VTABLE(This)->TxInvalidateRect(This,a,b)
#define ITextHost_TxViewChange(This,a) TXTHOST_VTABLE(This)->TxViewChange(This,a)
#define ITextHost_TxCreateCaret(This,a,b,c) TXTHOST_VTABLE(This)->TxCreateCaret(This,a,b,c)
#define ITextHost_TxShowCaret(This,a) TXTHOST_VTABLE(This)->TxShowCaret(This,a)
#define ITextHost_TxSetCaretPos(This,a,b) TXTHOST_VTABLE(This)->TxSetCaretPos(This,a,b)
#define ITextHost_TxSetTimer(This,a,b) TXTHOST_VTABLE(This)->TxSetTimer(This,a,b)
#define ITextHost_TxKillTimer(This,a) TXTHOST_VTABLE(This)->TxKillTimer(This,a)
#define ITextHost_TxScrollWindowEx(This,a,b,c,d,e,f,g) TXTHOST_VTABLE(This)->TxScrollWindowEx(This,a,b,c,d,e,f,g)
#define ITextHost_TxSetCapture(This,a) TXTHOST_VTABLE(This)->TxSetCapture(This,a)
#define ITextHost_TxSetFocus(This) TXTHOST_VTABLE(This)->TxSetFocus(This)
#define ITextHost_TxSetCursor(This,a,b) TXTHOST_VTABLE(This)->TxSetCursor(This,a,b)
#define ITextHost_TxScreenToClient(This,a) TXTHOST_VTABLE(This)->TxScreenToClient(This,a)
#define ITextHost_TxClientToScreen(This,a) TXTHOST_VTABLE(This)->TxClientToScreen(This,a)
#define ITextHost_TxActivate(This,a) TXTHOST_VTABLE(This)->TxActivate(This,a)
#define ITextHost_TxDeactivate(This,a) TXTHOST_VTABLE(This)->TxDeactivate(This,a)
#define ITextHost_TxGetClientRect(This,a) TXTHOST_VTABLE(This)->TxGetClientRect(This,a)
#define ITextHost_TxGetViewInset(This,a) TXTHOST_VTABLE(This)->TxGetViewInset(This,a)
#define ITextHost_TxGetCharFormat(This,a) TXTHOST_VTABLE(This)->TxGetCharFormat(This,a)
#define ITextHost_TxGetParaFormat(This,a) TXTHOST_VTABLE(This)->TxGetParaFormat(This,a)
#define ITextHost_TxGetSysColor(This,a) TXTHOST_VTABLE(This)->TxGetSysColor(This,a)
#define ITextHost_TxGetBackStyle(This,a) TXTHOST_VTABLE(This)->TxGetBackStyle(This,a)
#define ITextHost_TxGetMaxLength(This,a) TXTHOST_VTABLE(This)->TxGetMaxLength(This,a)
#define ITextHost_TxGetScrollBars(This,a) TXTHOST_VTABLE(This)->TxGetScrollBars(This,a)
#define ITextHost_TxGetPasswordChar(This,a) TXTHOST_VTABLE(This)->TxGetPasswordChar(This,a)
#define ITextHost_TxGetAcceleratorPos(This,a) TXTHOST_VTABLE(This)->TxGetAcceleratorPos(This,a)
#define ITextHost_TxGetExtent(This,a) TXTHOST_VTABLE(This)->TxGetExtent(This,a)
#define ITextHost_OnTxCharFormatChange(This,a) TXTHOST_VTABLE(This)->OnTxCharFormatChange(This,a)
#define ITextHost_OnTxParaFormatChange(This,a) TXTHOST_VTABLE(This)->OnTxParaFormatChange(This,a)
#define ITextHost_TxGetPropertyBits(This,a,b) TXTHOST_VTABLE(This)->TxGetPropertyBits(This,a,b)
#define ITextHost_TxNotify(This,a,b) TXTHOST_VTABLE(This)->TxNotify(This,a,b)
#define ITextHost_TxImmGetContext(This) TXTHOST_VTABLE(This)->TxImmGetContext(This)
#define ITextHost_TxImmReleaseContext(This,a) TXTHOST_VTABLE(This)->TxImmReleaseContext(This,a)
#define ITextHost_TxGetSelectionBarWidth(This,a) TXTHOST_VTABLE(This)->TxGetSelectionBarWidth(This,a)
/* ITextHost2 */
#define ITextHost2_TxIsDoubleClickPending(This) TXTHOST_VTABLE(This)->TxIsDoubleClickPending(This)
#define ITextHost2_TxGetWindow(This,a) TXTHOST_VTABLE(This)->TxGetWindow(This,a)
#define ITextHost2_TxSetForegroundWindow(This) TXTHOST_VTABLE(This)->TxSetForegroundWindow(This)
#define ITextHost2_TxGetPalette(This) TXTHOST_VTABLE(This)->TxGetPalette(This)
#define ITextHost2_TxGetEastAsianFlags(This,a) TXTHOST_VTABLE(This)->TxGetEastAsianFlags(This,a)
#define ITextHost2_TxSetCursor2(This,a,b) TXTHOST_VTABLE(This)->TxSetCursor2(This,a,b)
#define ITextHost2_TxFreeTextServicesNotification(This) TXTHOST_VTABLE(This)->TxFreeTextServicesNotification(This)
#define ITextHost2_TxGetEditStyle(This,a,b) TXTHOST_VTABLE(This)->TxGetEditStyle(This,a,b)
#define ITextHost2_TxGetWindowStyles(This,a,b) TXTHOST_VTABLE(This)->TxGetWindowStyles(This,a,b)
#define ITextHost2_TxShowDropCaret(This,a,b,c) TXTHOST_VTABLE(This)->TxShowDropCaret(This,a,b,c)
#define ITextHost2_TxDestroyCaret(This) TXTHOST_VTABLE(This)->TxDestroyCaret(This)
#define ITextHost2_TxGetHorzExtent(This,a) TXTHOST_VTABLE(This)->TxGetHorzExtent(This,a)

/* undo.c */
BOOL add_undo_insert_run( ME_TextEditor *, int pos, const WCHAR *str, int len, int flags, ME_Style *style );
BOOL add_undo_delete_run( ME_TextEditor *, int pos, int len );
BOOL add_undo_set_para_fmt( ME_TextEditor *, const ME_Paragraph *para );
BOOL add_undo_set_char_fmt( ME_TextEditor *, int pos, int len, const CHARFORMAT2W *fmt );
BOOL add_undo_join_paras( ME_TextEditor *, int pos );
BOOL add_undo_split_para( ME_TextEditor *, const ME_Paragraph *para, ME_String *eol_str, const ME_Cell *cell);
void ME_CommitUndo(ME_TextEditor *editor);
void ME_ContinueCoalescingTransaction(ME_TextEditor *editor);
void ME_CommitCoalescingUndo(ME_TextEditor *editor);
BOOL ME_Undo(ME_TextEditor *editor);
BOOL ME_Redo(ME_TextEditor *editor);
void ME_EmptyUndoStack(ME_TextEditor *editor);
void editor_disable_undo(ME_TextEditor *editor);
void editor_enable_undo(ME_TextEditor *editor);

static inline BOOL editor_undo_ignored(ME_TextEditor *editor)
{
    return editor->undo_ctl_state != undoActive || editor->nUndoMode == umIgnore;
}

/* txtsrv.c */
HRESULT create_text_services( IUnknown *outer, ITextHost *text_host, IUnknown **unk, BOOL emulate_10 );
#ifdef __ASM_USE_THISCALL_WRAPPER
extern const struct ITextServicesVtbl text_services_stdcall_vtbl;
#define TXTSERV_VTABLE(This) (&text_services_stdcall_vtbl)
#else
#define TXTSERV_VTABLE(This) (This)->lpVtbl
#endif
#define ITextServices_TxSendMessage(This,a,b,c,d) TXTSERV_VTABLE(This)->TxSendMessage(This,a,b,c,d)
#define ITextServices_TxDraw(This,a,b,c,d,e,f,g,h,i,j,k,l) TXTSERV_VTABLE(This)->TxDraw(This,a,b,c,d,e,f,g,h,i,j,k,l)
#define ITextServices_TxGetHScroll(This,a,b,c,d,e) TXTSERV_VTABLE(This)->TxGetHScroll(This,a,b,c,d,e)
#define ITextServices_TxGetVScroll(This,a,b,c,d,e) TXTSERV_VTABLE(This)->TxGetVScroll(This,a,b,c,d,e)
#define ITextServices_OnTxSetCursor(This,a,b,c,d,e,f,g,h,i) TXTSERV_VTABLE(This)->OnTxSetCursor(This,a,b,c,d,e,f,g,h,i)
#define ITextServices_TxQueryHitPoint(This,a,b,c,d,e,f,g,h,i,j) TXTSERV_VTABLE(This)->TxQueryHitPoint(This,a,b,c,d,e,f,g,h,i,j)
#define ITextServices_OnTxInPlaceActivate(This,a) TXTSERV_VTABLE(This)->OnTxInPlaceActivate(This,a)
#define ITextServices_OnTxInPlaceDeactivate(This) TXTSERV_VTABLE(This)->OnTxInPlaceDeactivate(This)
#define ITextServices_OnTxUIActivate(This) TXTSERV_VTABLE(This)->OnTxUIActivate(This)
#define ITextServices_OnTxUIDeactivate(This) TXTSERV_VTABLE(This)->OnTxUIDeactivate(This)
#define ITextServices_TxGetText(This,a) TXTSERV_VTABLE(This)->TxGetText(This,a)
#define ITextServices_TxSetText(This,a) TXTSERV_VTABLE(This)->TxSetText(This,a)
#define ITextServices_TxGetCurTargetX(This,a) TXTSERV_VTABLE(This)->TxGetCurTargetX(This,a)
#define ITextServices_TxGetBaseLinePos(This,a) TXTSERV_VTABLE(This)->TxGetBaseLinePos(This,a)
#define ITextServices_TxGetNaturalSize(This,a,b,c,d,e,f,g,h) TXTSERV_VTABLE(This)->TxGetNaturalSize(This,a,b,c,d,e,f,g,h)
#define ITextServices_TxGetDropTarget(This,a) TXTSERV_VTABLE(This)->TxGetDropTarget(This,a)
#define ITextServices_OnTxPropertyBitsChange(This,a,b) TXTSERV_VTABLE(This)->OnTxPropertyBitsChange(This,a,b)
#define ITextServices_TxGetCachedSize(This,a,b) TXTSERV_VTABLE(This)->TxGetCachedSize(This,a,b)

/* writer.c */
LRESULT ME_StreamOutRange(ME_TextEditor *editor, DWORD dwFormat, const ME_Cursor *start, int nChars, EDITSTREAM *stream);
LRESULT ME_StreamOut(ME_TextEditor *editor, DWORD dwFormat, EDITSTREAM *stream);

/* clipboard.c */
HRESULT ME_GetDataObject(ME_TextEditor *editor, const ME_Cursor *start, int nChars, LPDATAOBJECT *lplpdataobj);

void release_typelib(void);


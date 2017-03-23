// GridControlAlg.cpp: implementation of the GridControlAlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HANDLER.h"
#include "GridControlAlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GridControlAlg::GridControlAlg()
{

}

GridControlAlg::~GridControlAlg()
{

}

void GridControlAlg::GridCellControlStatic(TSpread *grid, int row, int col)
{	
	SS_CELLTYPE CellType;
	
	grid->SetTypeStaticText(&CellType, SS_TEXT_CENTER | SS_TEXT_VCENTER);
	grid->SetCellType(col,row,&CellType);
}

void GridControlAlg::GridRowHeader(TSpread *grid, BOOL used)
{
	grid->SetBool(SSB_SHOWROWHEADERS, used); // row header 삭제
}

void GridControlAlg::GridColHeader(TSpread *grid, BOOL used)
{
	grid->SetBool(SSB_SHOWCOLHEADERS, used); // col header 삭제
}

void GridControlAlg::GridHorizontal(TSpread *grid, BOOL used)
{
	grid->SetBool(SSB_HORZSCROLLBAR, used); // 좌우 스크롤바
}

void GridControlAlg::GridVertical(TSpread *grid, BOOL used)
{
	grid->SetBool(SSB_VERTSCROLLBAR, used); // 상하 스크롤바
}

void GridControlAlg::GridCellSelectDisplay(TSpread *grid, BOOL used)
{
	grid->SetBool(SSB_EDITMODEPERMANENT, used); // cell선택시 박스 제거
}

void GridControlAlg::GridCellControlDate(TSpread *grid, int row, int col)
{
	SS_CELLTYPE CellType;
	SS_DATEFORMAT dateFormat = { TRUE, '/', IDF_DDMMYY, FALSE };
		
	grid->SetTypeDate(&CellType, 0 , &dateFormat, NULL, NULL);

	grid->SetCellType(col,row,&CellType);
}

void GridControlAlg::GridCellControlEdit(TSpread *grid, int row, int col)
{	
	SS_CELLTYPE CellType;
	
	grid->SetTypeEdit(&CellType,ES_CENTER,60,SS_CHRSET_CHR,SS_CASE_NOCASE);
	
	grid->SetCellType(col,row,&CellType);
}

void GridControlAlg::GridCellControlCombo(TSpread *grid, int row, int col, CString list)
{
	SS_CELLTYPE CellType;
	
	grid->SetTypeComboBox(&CellType, 0, list);
	
	grid->SetCellType(col,row,&CellType);
}

void GridControlAlg::GridCellControlTime(TSpread *grid, int row, int col)
{
	SS_CELLTYPE CellType;
	SS_TIMEFORMAT tmFormat;
	
	tmFormat.b24Hour=TRUE;
	tmFormat.bSpin=TRUE;
	tmFormat.bSeconds=TRUE;
	tmFormat.cSeparator=':';

	grid->SetTypeTime(&CellType, 0, &tmFormat, NULL, NULL);
	
	grid->SetCellType(col,row,&CellType);
}

void GridControlAlg::GridCellControlCheck(TSpread *grid, int row, int col)
{
	SS_CELLTYPE CellType;
	
	grid->SetTypeCheckBox(&CellType,BS_CENTER, "", 
		"CheckUp", BT_BITMAP,
		"CheckDown", BT_BITMAP,
		"CheckFocusUp", BT_BITMAP,
		"CheckFocusDown", BT_BITMAP,
		"CheckDisabledUp", BT_BITMAP,
		"CheckDisabledDown", BT_BITMAP);

	grid->SetCellType(col,row,&CellType);
}

void GridControlAlg::GridCellControlNumber(TSpread *grid, int row, int col, int min, int max, int pos)
{
	SS_CELLTYPE CellType;
	double dpos;

	switch(pos){
		case 0:
			dpos = 1.0;
			break;
		case 1:
			dpos = 1.1;
			break;
		case 2:
			dpos = 1.01;
			break;
		case 3:
			dpos = 1.001;
			break;
		case 4:
			dpos = 1.0001;
			break;
	}
	grid->SetTypeNumberEx(&CellType, 0, pos, min, max,0,".",",",0,0,1,0, dpos);
	
	grid->SetCellType(col,row,&CellType);
}

void GridControlAlg::GridCellColor(TSpread *grid, int row, int col, COLORREF bk, COLORREF tk)
{
	grid->SetColor(col, row, bk, tk);
	grid->SetBackColorStyle(SS_BACKCOLORSTYLE_UNDERGRID);
}

void GridControlAlg::GridCellFont(TSpread *grid, int row, int col, CString fontname, int fontsize)
{
	HFONT font;

	font = CreateFont(fontsize, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 0, 0, fontname);
	grid->SetFont(col, row, font, TRUE);
}

void GridControlAlg::GridCellText(TSpread *grid, int row, int col, CString text)
{
	grid->SetValue(col, row, text);
}

CString GridControlAlg::GridCellText(TSpread *grid, int row, int col)
{
	CString strTmp;
	char chvalue[1000];

	grid->GetValue(col ,row, chvalue);

	strTmp.Format("%s", chvalue);

	return strTmp;
}

void GridControlAlg::GridCellRows(TSpread *grid, int rows)
{
	grid->SetMaxRows(rows);	
}

void GridControlAlg::GridCellCols(TSpread *grid, int cols)
{
	grid->SetMaxCols(cols);
}

void GridControlAlg::GridCellMerge(TSpread *grid, int start_row, int start_col, int count_row, int count_col)
{
	grid->AddCellSpan(start_col, start_row, count_col, count_row);
}

void GridControlAlg::GridFileSave(TSpread *grid, CString filename, CString sheetname)
{
	grid->ExportToExcel(filename, sheetname, "default.log");
}

void GridControlAlg::GridFileOpen(TSpread *grid, CString filename, CString sheetname)
{
	grid->ImportExcelBook(filename, "default.log");
}

void GridControlAlg::GridCellHeight_L(TSpread *grid, int row, double height)
{
	grid->SetRowHeight(row, height);
//	grid->SetRowHeightInPixels(row, height);
}

void GridControlAlg::GridCellWidth_L(TSpread *grid, int col, double width)
{
	grid->SetColWidth(col, width);
//	grid->SetColWidthInPixels(col, width);
}

void GridControlAlg::GridAutoSize(TSpread *grid, BOOL used)
{
	grid->SetBool(SSB_AUTOSIZE, used);
}

void GridControlAlg::GridReset(TSpread *grid)
{
	grid->Reset();
}

void GridControlAlg::GridCellScroll(TSpread *grid, int pos)
{
	SS_CELLCOORD UL, LR;

	UL.Col = 1;
	UL.Row = pos;
	LR.Col = 1;
	LR.Row = pos;

	grid->SetSelectBlock(&UL, &LR);
}

void GridControlAlg::GridCellHeight_P(TSpread *grid, int row, int height)
{
	grid->SetRowHeightInPixels(row, height);
}

void GridControlAlg::GridCellWidth_P(TSpread *grid, int col, int width)
{
	grid->SetColWidthInPixels(col, width);
}

int GridControlAlg::GridCellRow(TSpread *grid)
{
	return grid->GetMaxRows();
}

int GridControlAlg::GridCellCol(TSpread *grid)
{
	return grid->GetMaxCols();
}

int GridControlAlg::GridCellCol_Search(TSpread *grid, int col, int row_s, int row_e, CString str_find)
{
	return grid->SearchCol(col, row_s, row_e, str_find, SS_SEARCHFLAGS_NONE);
}

int GridControlAlg::GridCellRow_Search(TSpread *grid, int row, int col_s, int col_e, CString str_find)
{
	return grid->SearchRow(row, col_s, col_e, str_find, SS_SEARCHFLAGS_NONE);
}

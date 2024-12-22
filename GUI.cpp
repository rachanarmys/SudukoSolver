#include <wx/wx.h>
#include <wx/grid.h>
#include <cstdlib>
#include <cstring>

const int N = 9;

bool isSafe(int board[N][N], int row, int col, int num) {
    for (int i = 0; i < N; i++)
        if (board[row][i] == num || board[i][col] == num)
            return false;

    int boxRowStart = row - row % 3;
    int boxColStart = col - col % 3;

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i + boxRowStart][j + boxColStart] == num)
                return false;

    return true;
}

bool solveSudoku(int board[N][N], int row, int col) {
    if (row == N - 1 && col == N)
        return true;

    if (col == N) {
        row++;
        col = 0;
    }

    if (board[row][col] != 0)
        return solveSudoku(board, row, col + 1);

    for (int num = 1;  num <= 9; num++) {
        if (isSafe(board, row, col, num)) {
            board[row][col] = num;
            if (solveSudoku(board, row, col + 1))
                return true;
            board[row][col] = 0;
        }
    }
    return false;
}

class SudokuFrame : public wxFrame {
public:
    SudokuFrame(const wxString& title);

private:
    wxGrid* grid;
    int board[N][N];

    void OnSolve(wxCommandEvent& event);
    void OnViewSolution(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);

    wxDECLARE_EVENT_TABLE();
};

enum {
    ID_Solve = 1,
    ID_ViewSolution = 2
};

wxBEGIN_EVENT_TABLE(SudokuFrame, wxFrame)
EVT_BUTTON(ID_Solve, SudokuFrame::OnSolve)
EVT_BUTTON(ID_ViewSolution, SudokuFrame::OnViewSolution)
EVT_MENU(wxID_EXIT, SudokuFrame::OnExit)
wxEND_EVENT_TABLE()

class SudokuApp : public wxApp {
public:
    virtual bool OnInit();
};

wxIMPLEMENT_APP(SudokuApp);

bool SudokuApp::OnInit() {
    SudokuFrame* frame = new SudokuFrame("Sudoku Game - wxWidgets");
    frame->Show(true);
    return true;
}

SudokuFrame::SudokuFrame(const wxString& title)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(500, 500)) {
    wxPanel* panel = new wxPanel(this, wxID_ANY);

    grid = new wxGrid(panel, wxID_ANY, wxDefaultPosition, wxSize(450, 450));
    grid->CreateGrid(N, N);
    grid->SetDefaultColSize(50, true);
    grid->SetDefaultRowSize(50, true);

    for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++) {
            grid->SetCellAlignment(row, col, wxALIGN_CENTRE, wxALIGN_CENTRE);
            grid->SetCellValue(row, col, "0");
        }
    }

    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);

    wxButton* solveBtn = new wxButton(panel, ID_Solve, wxT("Solve"));
    wxButton* viewSolutionBtn = new wxButton(panel, ID_ViewSolution, wxT("View Solution"));
    wxButton* exitBtn = new wxButton(panel, wxID_EXIT, wxT("Exit"));

    hbox->Add(solveBtn, 1);
    hbox->Add(viewSolutionBtn, 1, wxLEFT, 5);
    hbox->Add(exitBtn, 1, wxLEFT, 5);

    vbox->Add(grid, 1, wxEXPAND | wxALL, 20);
    vbox->Add(hbox, 0, wxALIGN_CENTER | wxBOTTOM, 10);

    panel->SetSizer(vbox);
}

void SudokuFrame::OnSolve(wxCommandEvent& event) {
    for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++) {
            wxString cellValue = grid->GetCellValue(row, col);
            if (cellValue.IsNumber()) {
                board[row][col] = wxAtoi(cellValue);
            }
            else {
                board[row][col] = 0;
            }
        }
    }

    if (solveSudoku(board, 0, 0)) {
        for (int row = 0; row < N; row++) {
            for (int col = 0; col < N; col++) {
                grid->SetCellValue(row, col, wxString::Format("%d", board[row][col]));
            }
        }
    }
    else {
        wxMessageBox("No solution found!", "Error", wxOK | wxICON_ERROR);
    }
}

void SudokuFrame::OnViewSolution(wxCommandEvent& event) {
    if (solveSudoku(board, 0, 0)) {
        for (int row = 0; row < N; row++) {
            for (int col = 0; col < N; col++) {
                grid->SetCellValue(row, col, wxString::Format("%d", board[row][col]));
            }
        }
    }
    else {
        wxMessageBox("No solution found!", "Error", wxOK | wxICON_ERROR);
    }
}

void SudokuFrame::OnExit(wxCommandEvent& event) {
    Close(true);
}

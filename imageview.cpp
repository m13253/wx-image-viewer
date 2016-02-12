#include <wx/aboutdlg.h>
#include <wx/app.h>
#include <wx/bitmap.h>
#include <wx/dcclient.h>
#include <wx/filedlg.h>
#include <wx/frame.h>
#include <wx/image.h>
#include <wx/menu.h>
#include <wx/msgdlg.h>
#include <wx/panel.h>
#include <wx/sizer.h>

class MyImagePanel : public wxPanel {
public:
    MyImagePanel(wxFrame *parent);
    bool SetImageFile(const wxString &filename);
    void DoPaint();
private:
    wxImage image;
    void OnPaint(wxPaintEvent &event);
    void OnSize(wxSizeEvent &event);
    wxDECLARE_EVENT_TABLE();
};
wxBEGIN_EVENT_TABLE(MyImagePanel, wxPanel)
    EVT_PAINT(MyImagePanel::OnPaint)
    EVT_SIZE(MyImagePanel::OnSize)
wxEND_EVENT_TABLE()

class MyFrame : public wxFrame {
public:
    MyFrame();
private:
    MyImagePanel *image_panel;
    void OnOpen(wxCommandEvent &event);
    void OnExit(wxCommandEvent &event);
    void OnAbout(wxCommandEvent &event);
    wxDECLARE_EVENT_TABLE();
};
wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(wxID_OPEN, MyFrame::OnOpen)
    EVT_MENU(wxID_EXIT, MyFrame::OnExit)
    EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
wxEND_EVENT_TABLE()

class MyApp : public wxApp {
public:
    virtual bool OnInit();
};

MyImagePanel::MyImagePanel(wxFrame *parent) :
    wxPanel(parent) {
    wxInitAllImageHandlers();
}

bool MyImagePanel::SetImageFile(const wxString &filename) {
    if(image.LoadFile(filename)) {
        DoPaint();
        return true;
    } else {
        return false;
    }
}

void MyImagePanel::DoPaint() {
    if(image.IsOk()) {
        wxClientDC dc(this);
#ifdef __WXGTK3__
        double scale_factor = 1; // FIXME: HiDPI scale factor
        // FIXME: A possible bug in GTK+3 backend, omitting this line causes aliasing on HiDPI screens
        dc.SetLogicalScale(1/scale_factor, 1/scale_factor);
#endif
        int width, height;
        dc.GetSize(&width, &height);
#ifdef __WXGTK3__
        width *= scale_factor;
        height *= scale_factor;
#endif
        wxBitmap resized(image.Scale(width, height, wxIMAGE_QUALITY_BICUBIC));
        dc.DrawBitmap(resized, 0, 0);
    }
}

void MyImagePanel::OnPaint(wxPaintEvent &event) {
    DoPaint();
}

void MyImagePanel::OnSize(wxSizeEvent &event) {
    DoPaint();
}

MyFrame::MyFrame() :
    wxFrame(nullptr, wxID_ANY, "Simple Image Viewer"),
    image_panel(new MyImagePanel(this)) {

    auto menuBar = new wxMenuBar;
    auto menuFile = new wxMenu;
    menuFile->Append(wxID_OPEN);
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);
    menuBar->Append(menuFile, "&File");
    auto menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
    menuBar->Append(menuHelp, "&Help");
    SetMenuBar(menuBar);

    auto sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(image_panel, 1, wxEXPAND);
    SetSizer(sizer);
}

void MyFrame::OnOpen(wxCommandEvent &event) {
    wxFileDialog dialog(this, "Choose image file");
    if(dialog.ShowModal() == wxID_CANCEL)
        return;
    image_panel->SetImageFile(dialog.GetPath());
}

void MyFrame::OnExit(wxCommandEvent &event) {
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent &event) {
    wxAboutDialogInfo about_info;
    about_info.SetName("Simple Image Viewer");
    about_info.SetVersion("0.1.0");
    about_info.SetDescription("A simple image viewer application written in wxWidgets, demonstrating its image drawing capabilities.");
    about_info.SetCopyright(L"\u00a9 2016 Star Brilliant <m13253\x40hotmail.com>");
    about_info.SetWebSite("https://github.com/m13253/wx-image-viewer");
    about_info.AddDeveloper("Star Brilliant <m13253\x40hotmail.com>");
    wxAboutBox(about_info, this);
}

bool MyApp::OnInit() {
    auto frame = new MyFrame();
    frame->Show(true);
    return true;
}

wxIMPLEMENT_APP(MyApp);

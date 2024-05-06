#include "main.h"
#include "rc.h"
#include <filesystem>
using namespace Gdiplus;

void main_window::on_paint(HDC hdc) 
{
	Graphics graphics(hdc);
	RECT clientRect;
	GetClientRect(*this, &clientRect);
	if (!selectedImagePath.empty()) {
		
		graphics.DrawImage(image.get(), 0, 0, clientRect.right, clientRect.bottom);

		std::filesystem::path filePath(selectedImagePath);
		std::wstring fileName = filePath.filename().wstring();

		
		Font font(L"Arial", 32, FontStyleBold, UnitPixel);
		RectF textRect;
		graphics.MeasureString(fileName.c_str(), -1, &font, PointF(0, 0), &textRect);
		float textX = (clientRect.right - textRect.Width) / 2.0f;
		float textY = static_cast<float>(clientRect.bottom - 70);
		SolidBrush shadowBrush(Gdiplus::Color::Black);
		graphics.DrawString(fileName.c_str(), -1, &font, PointF(textX + 1, textY + 1), &shadowBrush);
		SolidBrush textBrush(Gdiplus::Color::White);
		graphics.DrawString(fileName.c_str(), -1, &font, PointF(textX, textY), &textBrush);
	}
	
}

void main_window::on_command(int id) 
{
	switch (id) 
	{
		case ID_OPEN:
		{
			TCHAR filename[MAX_PATH];
			filename[0] = '\0';

			OPENFILENAME ofn;
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = *this;
			ofn.lpstrFilter = _T("Image Files\0*.jpeg;*.jpg;*.png;*.bmp;*.gif;*.tif;*.tiff;*.emf\0");
			ofn.lpstrFile = filename;
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

			if (GetOpenFileName(&ofn) == TRUE) {
				selectedImagePath = filename;
				InvalidateRect(*this, nullptr, TRUE);
				const wchar_t* path = selectedImagePath.c_str();
				image = std::make_unique<Gdiplus::Image>(path);
			}
			break;
		}
		case ID_EXIT:
			DestroyWindow(*this);
			break;
	}
}

void main_window::on_destroy() 
{
	::PostQuitMessage(0);
}

int WINAPI _tWinMain(HINSTANCE instance, HINSTANCE, LPTSTR, int)
{
	gdiplus_application app;
	main_window wnd;
	wnd.create(0, WS_OVERLAPPEDWINDOW | WS_VISIBLE, _T("NWP 7"), (int64_t)LoadMenu(instance, MAKEINTRESOURCE(IDM_MAIN)));
	return app.run();
}

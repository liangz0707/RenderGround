#include "RenderGroundEngine.h"
#include "RenderGround.h"
#include "Common.h"

HINSTANCE windowInstance;
HWND window;


LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	case WM_DESTROY:
		DestroyWindow(hWnd);
		PostQuitMessage(0);
		break;
	case WM_PAINT:
		ValidateRect(hWnd, NULL);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
		break;
	}

}


void createWindow(HINSTANCE hInstance) {

	WNDCLASSEX wcex;

	const wchar_t CLASS_NAME[] = L"Sample Window Class";

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = CLASS_NAME;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (!RegisterClassEx(&wcex))
		std::cerr <<"Register Fail"<< std::endl;

	windowInstance = hInstance;

	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	int windowLeft = screenWidth / 2 - WIDTH / 2;
	int windowTop = screenHeight / 2 - HEIGHT / 2;


	window = CreateWindow(
		CLASS_NAME,
		CLASS_NAME,
		WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		0,
		0,
		WIDTH,
		HEIGHT,
		NULL,
		NULL,
		windowInstance,
		NULL);

	if (window == NULL)
	{

		std::cerr << "Create Window Fail" << std::endl;
	}

	ShowWindow(window, SW_SHOW);
	SetForegroundWindow(window);
	SetFocus(window);
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow) 
{
	createWindow(hInstance);

	VulkanRenderGround* app = new VulkanRenderGround();

	try {
		app->init(windowInstance, window);

		MSG message;

		while (GetMessage(&message, NULL, 0, 0)) {
			TranslateMessage(&message);
			DispatchMessage(&message);

			app->drawFrame();
		}

		app->cleanup();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

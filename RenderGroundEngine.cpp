#include "RenderGroundEngine.h"
#include "RenderGround.h"
#include "Common.h"

HINSTANCE windowInstance;
HWND window;
VulkanRenderGround* app;

bool isRun ;
void OnSize(HWND hwnd, UINT flag, int width, int height)
{
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int width = 0;  // Macro to get the low-order word.
	int height = 0;
	switch (msg) {

	case WM_DESTROY:
		isRun = false;
		app->cleanup();
		//DestroyWindow(hWnd);
		PostQuitMessage(0);
		break;

	case WM_SIZE: // Handle window resizing 
		 width = LOWORD(lParam);  // Macro to get the low-order word.
		 height = HIWORD(lParam); // Macro to get the high-order word.
		 if(app)
		 app->ReSize(width, height);
		// Respond to the message:
		OnSize(hWnd, (UINT)wParam, width, height);

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
	isRun = true;
	app = new VulkanRenderGround();
	
	try {
		app->init(windowInstance, window);

		MSG message = { 0 };

		while (WM_QUIT != message.message){
			if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&message);
				DispatchMessage(&message);
			}
			if(isRun && app->GetWidth() != 0 && app->GetHeight())
				app->drawFrame();
		}

	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

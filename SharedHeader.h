#pragma once

// 필요한 모든 헤더들을 모아놓은 헤더. include는 포함한다는 뜻.
#include <Windows.h>
#include <d3d11.h>
#include <wrl.h>
#include <cassert>
#include <d3dcompiler.h>
#include <string>
#include <vector>
//unique를 쓰는 데 필요한 헤더.
#include <memory>
#include "DirectXTK/DirectXTK.h"

//필요한 라이브러리를 링크하는데 씀.
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "DirectXTK.lib")

using namespace DirectX;
using namespace Microsoft::WRL;
using std::string;
using std::wstring;
using std::vector;
using std::unique_ptr;
using std::make_unique;
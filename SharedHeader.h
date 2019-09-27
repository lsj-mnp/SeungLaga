#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <wrl.h>
#include <cassert>
#include <d3dcompiler.h>
#include <string>
#include <vector>
#include "DirectXTK/DirectXTK.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "DirectXTK.lib")

using namespace DirectX;
using namespace Microsoft::WRL;
using std::string;
using std::wstring;
using std::vector;
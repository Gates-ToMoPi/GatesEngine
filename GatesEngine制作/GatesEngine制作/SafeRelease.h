#pragma once
//COMの解放処理を簡単にするためのマクロ
#define SAFE_RELEASE(p){if((p)){(p)->Release();(p) = nullptr;}}

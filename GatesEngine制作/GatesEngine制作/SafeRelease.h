#pragma once
//COM‚Ì‰ð•úˆ—‚ðŠÈ’P‚É‚·‚é‚½‚ß‚Ìƒ}ƒNƒ
#define SAFE_RELEASE(p){if((p)){(p)->Release();(p) = nullptr;}}

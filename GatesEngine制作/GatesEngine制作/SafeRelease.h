#pragma once
//COM�̉���������ȒP�ɂ��邽�߂̃}�N��
#define SAFE_RELEASE(p){if((p)){(p)->Release();(p) = nullptr;}}

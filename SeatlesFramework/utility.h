#pragma once

namespace SeatlesFramework
{

#define SAFE_DELETE(p) { if(p != nullptr) { delete (p); p = nullptr; } }
#define SAFE_DELETES(p) { if(p != nullptr) { delete[] (p); p = nullptr; } }
#define SAFE_RELEASE(p) { if(p != nullptr) { p->Release(); p = nullptr; } }

}

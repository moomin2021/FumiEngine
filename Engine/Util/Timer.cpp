#include "Timer.h"
#include "Util.h"

bool Timer::GetOn()
{
	float elapsed = (Util::GetTimrMSec() - last) / 1000.0f;
	if (elapsed >= coolTime)
	{
		last = Util::GetTimrMSec();
		return true;
	}
	return false;
}
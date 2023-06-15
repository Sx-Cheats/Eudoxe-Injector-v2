#pragma once


#include "../NTapi/NTapi.h"

class RunAs
{
public:

	enum class Acc
	{
		Null,    //   0
		User,    //   1
		Admin,   //   2
		System, //   3 
	};

	static Acc  __stdcall  IsRunningLikeWho();
	static bool  __stdcall StartAs(Acc LikeWho);
	static bool  __stdcall StartProgramAs(std::string ProgramPath, RunAs::Acc LikeWho);
	static std::string __stdcall WhoamI();
};

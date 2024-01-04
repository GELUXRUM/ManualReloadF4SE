#include "hooks.h"

namespace Hooks
{
	typedef int64_t(PlayerControls_DoActionSig)(__int64, int, unsigned int);
	REL::Relocation<PlayerControls_DoActionSig> playerControls_DoAction;

	int64_t HookedPlayerControls_DoAction(__int64, int, unsigned int)
	{
		return 0;
	}

	void Install(F4SE::Trampoline& trampoline)
	{
		REL::Relocation<PlayerControls_DoActionSig> callLocation{ REL::ID(818081), 0x40A };
		playerControls_DoAction = trampoline.write_call<5>(callLocation.address(), &HookedPlayerControls_DoAction);

		REL::Relocation<std::uintptr_t> start{ REL::ID(902833), 0x206 };
		uint8_t bytes[] = { 0x90, 0xE9 }; // replaces near-jump jz `0F 84`
		REL::safe_write<uint8_t>(start.address(), std::span{ bytes });
	}
}

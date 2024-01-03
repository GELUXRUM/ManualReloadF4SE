#include "hooks.h"
#include <xbyak/xbyak.h>

namespace Hooks
{
	typedef int64_t(PlayerControls_DoActionSig)(__int64, int, unsigned int);
	REL::Relocation<PlayerControls_DoActionSig> playerControls_DoAction;

	int64_t HookedPlayerControls_DoAction(__int64, int, unsigned int)
	{
		return 0;
	}

	struct skipAutoReload : public Xbyak::CodeGenerator
	{
		skipAutoReload()
		{
			mov(eax, esi);
			add(rsp, 0xE8);
			pop(rdi);
			pop(rsi);
			pop(rbx);
			pop(rbp);
			ret();
		}
	};
	

	void Install(F4SE::Trampoline& trampoline)
	{
		REL::Relocation<PlayerControls_DoActionSig> callLocation{ REL::ID(818081), 0x40A };
		playerControls_DoAction = trampoline.write_call<5>(callLocation.address(), &HookedPlayerControls_DoAction);
		
		REL::Relocation<std::uintptr_t> start{ REL::ID(902833), 0x1EC };
		skipAutoReload code;
		auto result = trampoline.allocate(code.getSize());
		REL::safe_write(reinterpret_cast<uintptr_t>(result), code.getCode(), code.getSize());
		trampoline.write_branch<5>(start.address(), result);
		
	}
}

#pragma once

namespace vr 
{
	struct CmdContext;
	typedef int (*ConsoleFn)(CmdContext* _context, void* _userData, int _argc, char const* const* _argv);

	///
	void cmdInit();

	///
	void cmdShutdown();

	///
	void cmdAdd(const char* _name, ConsoleFn _fn, void* _userData = NULL);

	///
	void cmdRemove(const char* _name);

	///
	void cmdExec(const char* _format, ...);

} // namespace vr
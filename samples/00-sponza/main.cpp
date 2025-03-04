#include "entry/entry.h"

#include <bx/bx.h>

namespace
{
	class App : public vr::AppI
	{
	public:
		App(const char* _name, const char* _description, const char* _url = "https://bkaradzic.github.io/bgfx/index.html")
			: AppI(_name, _description, _url)
		{
			m_width = 1280;
			m_height = 720;
		}

		~App() override
		{
		}

		void init(int32_t _argc, const char* const* _argv, uint32_t _width, uint32_t _height) override
		{
		}

		int shutdown() override
		{
			return bx::kExitSuccess;
		}

		bool update() override
		{
			if (!vr::processEvents(m_width, m_height, &m_mouseState))
			{
				// ...

				return true;
			}
			return false;
		}

	private:
		uint32_t m_width, m_height;
		vr::MouseState m_mouseState;
	};

} // namespace

ENTRY_IMPLEMENT_MAIN(
	App
	, "00-sponza"
	, "Sponza scene."
	, ""
);
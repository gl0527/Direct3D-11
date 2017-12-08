#pragma once

namespace Egg { namespace Scene
{
	GG_DECL(ControlState)
	GG_DECL(Entity)

	GG_CLASS(ControlLogic)
	protected:
		ControlLogic() {}
	public:
		virtual ControlStateP createState(EntityP entity) = 0;
	GG_ENDCLASS
}}

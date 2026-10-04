#pragma once

#include "RenderAPI.h"

#include <Core/Core.h>

namespace DResources
{
	class IMesh;
	class IMaterial;
}

namespace DRender {

	class IBatch
	{
	public:
		virtual ~IBatch() = default;

		virtual void Submit(Ref<DResources::IMesh> mesh, Ref<DResources::IMaterial> mat) = 0;

		virtual void BuildBatch() = 0;
	};

}

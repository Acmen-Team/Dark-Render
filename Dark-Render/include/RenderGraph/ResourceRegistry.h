#pragma once

#include "RenderAPI.h"

#include <unordered_map>
#include <string>
#include <functional>
#include <any>
#include <stdexcept>

namespace DRender
{

	class DRENDER_API ResourceRegistry {
	public:
		ResourceRegistry() = default;

		struct ResourceEntry {
			std::any handle;
			std::function<void()> destroyFunc;
		};

		// 注册资源：创建和销毁逻辑由外部控制
		void Register(const std::string& name, std::function<std::any()> create, std::function<void()> destroy) {
			auto handle = create();
			resources[name] = ResourceEntry{ handle, destroy };
		}

		// 获取资源时，调用者需指定类型
		template<typename T>
		T Get(const std::string& name) const {
			auto it = resources.find(name);
			if (it == resources.end())
				throw std::runtime_error("Resource not found: " + name);
			return std::any_cast<T>(it->second.handle);
		}

		void DestroyAll() {
			for (auto& [_, entry] : resources) {
				if (entry.destroyFunc) entry.destroyFunc();
			}
			resources.clear();
		}

	private:
		std::unordered_map<std::string, ResourceEntry> resources;
	};

}
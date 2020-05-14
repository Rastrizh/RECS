#ifndef ENGINE_H
#define ENGINE_H

namespace RECS {
	class Engine
	{
	private:
		Engine() = default;

	public:
		static auto instance()->Engine&;
		~Engine() = default;

		auto CreateEntity()->Entity*;
		void KillEntity();
		void KillAllEntities();

		event<Entity*> OnEntityCreated;
		event<Entity*> OnEntityDestroyed;
		event<Entity*> OnEntityChanged;
	};
}

#endif // !ENGINE_H

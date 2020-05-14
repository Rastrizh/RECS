#ifndef ENGINE_H
#define ENGINE_H

namespace RECS {
	class Engine
	{
	public:
		Engine() = default;
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

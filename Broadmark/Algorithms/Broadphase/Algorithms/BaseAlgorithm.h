#pragma once


#include "Configs.h"
#include "Macros.h"
#include "Core/Math/Vec3.h"
#include "Core/Settings.h"
#include "Core/Scene.h"
#include "Broadphase\BroadphaseInterface.h"

#include <type_traits>




template<typename ObjectType, typename CacheType>
class BaseAlgorithm : public BroadphaseInterface {
private:
	static_assert(std::is_base_of<Object, ObjectType>::value, "The ObjectType template argument must derive from Object");

public:
	InflatedSettings m_settings;
	CacheType m_cache;

	StructAlignment(32)
	ObjectType *m_objects;

public:
	virtual ~BaseAlgorithm() = default;


	void Initialize(InflatedSettings settings, const SceneFrame& frameData) override {
		m_settings = settings;

		m_objects = new ObjectType[m_settings.m_numberOfObjects];
		for (size_t i = 0; i < m_settings.m_numberOfObjects; i++) {
			m_objects[i].m_aabb = frameData.m_aabbs[i];
			m_objects[i].m_id = i;
		}
	}

	void UpdateObjects(const SceneFrame& frameData) override {
		for (size_t i = 0; i < m_settings.m_numberOfObjects; i++) {
			m_objects[i].m_aabb = frameData.m_aabbs[i];
		}
	}

	void CleanCache() override {
		m_cache.Clean();
	}

	void SearchOverlaps() override = 0;

	OverlapCache* GetOverlaps() override {
		return &m_cache;
	}
};

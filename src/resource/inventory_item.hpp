#pragma once

#include "godot_cpp/classes/packed_scene.hpp"
#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/classes/resource.hpp"

namespace godot {

// Resource to store item data
class InventoryItem : public Resource {
	GDCLASS(InventoryItem, Resource)

private:
    // Scene of item UI when in inventory
	Ref<PackedScene> item_ui_scene;
	// 2d scene of item when dropped
	Ref<PackedScene> item_2d_scene;

protected:
	static void _bind_methods();

public:
	void set_item_ui_scene(const Ref<PackedScene> &p_scene);
	Ref<PackedScene> get_item_ui_scene() const;

	void set_item_2d_scene(const Ref<PackedScene> &p_scene);
	Ref<PackedScene> get_item_2d_scene() const;
};

} //namespace godot
